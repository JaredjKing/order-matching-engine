#include "UdpPublisher.hpp"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

UdpPublisher::UdpPublisher(const std::string& group, uint16_t port, int ttl) {
    sock_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    int enable = 1;
    ::setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
    ::setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    ::inet_pton(AF_INET, group.c_str(), &addr_.sin_addr);
}

UdpPublisher::~UdpPublisher() {
    if (sock_ >= 0) ::close(sock_);
}

static void sendLine(int s, const sockaddr_in& a, const std::string& line) {
    ::sendto(s, line.c_str(), line.size(), 0, (const sockaddr*)&a, sizeof(a));
}

// Line-delimited JSON-ish for readability;  ca to FlatBuffers later if needed later.
void UdpPublisher::publishTrade(const TradeEvent& t) {
    char buf[256];
    std::snprintf(buf, sizeof(buf), "TRADE|sym=%s|px=%.10g|qty=%d|maker=%d|taker=%d\n",
                  t.symbol.c_str(), t.px, t.qty, t.makerId, t.takerId);
    sendLine(sock_, addr_, buf);
}

void UdpPublisher::publishBook(const BookDelta& d) {
    char buf[256];
    std::snprintf(buf, sizeof(buf), "BOOK|sym=%s|side=%c|px=%.10g|qty=%d\n", d.symbol.c_str(),
                  d.side, d.px, d.qty);
    sendLine(sock_, addr_, buf);
}
