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

    int order_count = 0;
    auto total_order_processing_time = std::chrono::nanoseconds(0);  // Track total processing time

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string order_id, timestamp, symbol, order_type, order_side, price, quantity;

        std::getline(ss, order_id, ',');
        std::getline(ss, timestamp, ',');  // Ignore timestamp for now
        std::getline(ss, symbol, ',');
        std::getline(ss, order_type, ',');
        std::getline(ss, order_side, ',');
        std::getline(ss, price, ',');
        std::getline(ss, quantity, ',');

        // Convert string values to appropriate data types
        int id = std::stoi(order_id);
        double order_price = std::stod(price);
        int order_quantity = std::stoi(quantity);
        OrderSide side = (order_side == "BUY") ? OrderSide::BUY : OrderSide::SELL;
        OrderType type = OrderType::LIMIT;

        // Create order
        Order order(id, order_price, order_quantity, type, side);

        // Time the order matching & insertion
        auto start_time = std::chrono::high_resolution_clock::now();
        orderBook.addLimitOrder(std::move(order));
        auto end_time = std::chrono::high_resolution_clock::now();

        // Accumulate processing time
        total_order_processing_time += (end_time - start_time);

        // Print progress every 100,000 orders
        if (++order_count % 100000 == 0) {
            std::cout << "Processed " << order_count << " orders...\n";
        }
    }

    file.close();

    double total_time_ms =
        std::chrono::duration<double, std::milli>(total_order_processing_time).count();

    std::cout << "✅ Finished processing all orders from " << filename << std::endl;
    std::cout << "⏱️ Total order processing time: " << total_time_ms << " ms" << std::endl;
    std::cout << "⏱️ Average time per order: " << (total_time_ms / order_count) << " ms"
              << std::endl;
}

int main() {
    OrderBook orderBook;
    std::string filename = "aapl_limit_orders.csv";  // Ensure this file is present

    std::cout << "Processing orders in real-time from CSV...\n";
    processOrdersFromCSV(filename, orderBook);

    // Final summary of order book
    // std::cout << "\n========= FINAL ORDER BOOK =========\n";
    // orderBook.printSubmittedOrders();
    // orderBook.printBuyList();
    // orderBook.printSellList();

    return 0;
}
