#pragma once
#include <netinet/in.h>

#include <string>

#include "MarketEvents.hpp"

class UdpPublisher {
   public:
    UdpPublisher(const std::string& group, uint16_t port, int ttl = 1);
    ~UdpPublisher();
    void publishTrade(const TradeEvent& t);
    void publishBook(const BookDelta& d);

   private:
    int sock_ = -1;
    sockaddr_in addr_{};
};
