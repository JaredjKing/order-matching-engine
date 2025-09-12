#include "TcpServer.hpp"

#include <arpa/inet.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

#include "OrderBook.hpp"

static std::unordered_map<std::string, std::string> parseKV(const std::string& s) {
    std::unordered_map<std::string, std::string> kv;
    std::stringstream ss(s);
    std::string tok;
    std::string key, val;
    size_t start = 0;
    while (true) {
        size_t pos = s.find('|', start);
        std::string token =
            s.substr(start, pos == std::string::npos ? std::string::npos : pos - start);
        auto eq = token.find('=');
        if (eq != std::string::npos) kv[token.substr(0, eq)] = token.substr(eq + 1);
        if (pos == std::string::npos) break;
        start = pos + 1;
    }
    return kv;
}

TcpServer::TcpServer(OrderBook& ob, uint16_t port) : ob_(&ob), port_(port) {
    srv_ = ::socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    ::setsockopt(srv_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(port_);
    a.sin_addr.s_addr = INADDR_ANY;
    ::bind(srv_, (sockaddr*)&a, sizeof(a));
    ::listen(srv_, 64);
}

TcpServer::~TcpServer() {
    stop();
    if (srv_ >= 0) ::close(srv_);
}

void TcpServer::start() {
    running_ = true;
    th_ = std::thread(&TcpServer::acceptLoop, this);
}
void TcpServer::stop() {
    running_ = false;
    if (th_.joinable()) th_.join();
}

void TcpServer::acceptLoop() {
    while (running_) {
        int fd = ::accept(srv_, nullptr, nullptr);
        if (fd < 0) continue;
        std::thread([this, fd]() { TcpServer::handleClient(fd, ob_); }).detach();
    }
}

// Wire format:
// NEW|id=1|sym=SYM|side=B|px=100.25|qty=50
// CANCEL|id=1
void TcpServer::handleClient(int fd, OrderBook* ob) {
    std::string buf;
    char tmp[1024];
    while (true) {
        ssize_t n = ::read(fd, tmp, sizeof(tmp));
        if (n <= 0) break;
        buf.append(tmp, tmp + n);
        size_t pos;
        while ((pos = buf.find('\n')) != std::string::npos) {
            std::string line = buf.substr(0, pos);
            buf.erase(0, pos + 1);

            if (line.rfind("NEW|", 0) == 0) {
                auto kv = parseKV(line.substr(4));
                try {
                    int id = std::stoi(kv.at("id"));
                    double px = std::stod(kv.at("px"));
                    int qty = std::stoi(kv.at("qty"));
                    char side = kv.at("side").empty() ? 'B' : kv.at("side")[0];
                    Order o{id, px, qty, OrderType::LIMIT,
                            side == 'B' ? OrderSide::BUY : OrderSide::SELL};
                    ob->addLimitOrder(std::move(o));
                    std::string ack = "ACK|id=" + std::to_string(id) + "\r\n";
                    ::send(fd, ack.c_str(), ack.size(), 0);
                } catch (...) {
                    std::string rej = "REJECT|id=?|reason=bad_fields\r\n";
                    ::send(fd, rej.c_str(), rej.size(), 0);
                }
            } else if (line.rfind("CANCEL|", 0) == 0) {
                auto kv = parseKV(line.substr(7));
                std::string id = kv.count("id") ? kv["id"] : "?";
                std::string rej = "REJECT|id=" + id + "|reason=cancel_unimplemented\r\n";
                ::send(fd, rej.c_str(), rej.size(), 0);
            } else {
                std::string rej = "REJECT|id=?|reason=bad_message\r\n";
                ::send(fd, rej.c_str(), rej.size(), 0);
            }
        }
    }
    ::close(fd);
}
