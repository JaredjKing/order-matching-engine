#include "MarketEvents.hpp"

std::function<void(const TradeEvent&)> MarketEvents::onTrade = nullptr;
std::function<void(const BookDelta&)> MarketEvents::onBookDelta = nullptr;
