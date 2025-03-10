#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "OrderBook.hpp"  // Include your OrderBook implementation

void processOrdersFromCSV(const std::string& filename, OrderBook& orderBook) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);  // Skip header

    auto total_order_processing_time = std::chrono::nanoseconds(0);

    while (true) {
        int batch_count = 0;
        while (batch_count < 30'000 && std::getline(file, line)) {
            std::stringstream ss(line);
            std::string order_id, timestamp, symbol, order_type, order_side, price, quantity;

            std::getline(ss, order_id, ',');
            std::getline(ss, timestamp, ',');
            std::getline(ss, symbol, ',');
            std::getline(ss, order_type, ',');
            std::getline(ss, order_side, ',');
            std::getline(ss, price, ',');
            std::getline(ss, quantity, ',');

            int id = std::stoi(order_id);
            double order_price = std::stod(price);
            int order_quantity = std::stoi(quantity);
            OrderSide side = (order_side == "BUY") ? OrderSide::BUY : OrderSide::SELL;
            OrderType type = OrderType::LIMIT;

            auto start_time = std::chrono::high_resolution_clock::now();
            orderBook.addLimitOrder(id, order_price, order_quantity, type, side);
            auto end_time = std::chrono::high_resolution_clock::now();

            total_order_processing_time += (end_time - start_time);
            batch_count++;
        }
        if (batch_count == 0) break;  // No more orders to process
    }

    file.close();
    double total_time_ms =
        std::chrono::duration<double, std::milli>(total_order_processing_time).count();

    std::cout << "✅ Finished processing all orders from " << filename << std::endl;
    std::cout << "⏱️ Total order processing time: " << total_time_ms << " ms" << std::endl;
}

int main() {
    OrderBook orderBook;
    std::string filename = "../aapl_limit_orders.csv";  // Ensure this file is present

    std::cout << "Processing orders in real-time from CSV...\n";
    processOrdersFromCSV(filename, orderBook);

    // Final summary of order book
    // std::cout << "\n========= FINAL ORDER BOOK =========\n";
    // orderBook.printSubmittedOrders();
    // orderBook.printBuyList();
    // orderBook.printSellList();

    return 0;
}
