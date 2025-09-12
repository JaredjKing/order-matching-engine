#pragma once
#include <atomic>
#include <cstdint>
#include <thread>

class OrderBook;  // fwd-declare

class TcpServer {
   public:
    TcpServer(OrderBook& ob, uint16_t port = 5000);
    ~TcpServer();
    void start();
    void stop();

   private:
    void acceptLoop();
    static void handleClient(int fd, OrderBook* ob);

    int srv_ = -1;
    std::atomic<bool> running_{false};
    std::thread th_;
    OrderBook* ob_;  // store pointer to engine
    uint16_t port_;
};
