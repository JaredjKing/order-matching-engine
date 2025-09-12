#pragma once
#include <atomic>
#include <functional>
#include <string>

struct TradeEvent {
    std::string symbol;
    int makerId;  // resting order id
    int takerId;  // aggressing order id
    int qty;
    double px;
};

struct BookDelta {
    std::string symbol;
    char side;  // 'B' or 'S'
    double px;
    int qty;  // new aggregate qty at price (0 == remove level)
};

// Simple global callbacks (kept tiny for now).
namespace MarketEvents {
extern std::function<void(const TradeEvent&)> onTrade;
extern std::function<void(const BookDelta&)> onBookDelta;
}  // namespace MarketEvents
