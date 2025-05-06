#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <ranges>
#include <iomanip>
#include <cstdint>

using std::string;
using std::vector;

struct Event {
    string time;
    int16_t id = -1;
    string body;
    int64_t table_number = -1;
};

struct Table {
    int64_t number = -1;
    uint32_t profit = 0;
    uint32_t occupied_time = 0;
    bool isBusy = false;
    string client_name;
    string start_time;
};

class App {
    static inline uint32_t number_of_tables;
    static inline string opening_time;
    static inline uint32_t opening_time_min;
    static inline string closing_time;
    static inline uint32_t closing_time_min;
    static inline uint32_t cost_per_hour;

    static inline vector<Event> incoming_events;
    static inline vector<Event> outgoing_events;

    static inline vector<Table> tables;
    static inline std::unordered_set<string> club_clients;
    static inline std::queue<string> clients_queue;
    static inline std::unordered_map<int64_t, string> table_to_client;
    static inline std::map<string, int64_t> client_to_table;

    App() = default;
    ~App() = default;

    [[nodiscard]] static uint32_t str_to_min(const string&);
    [[nodiscard]] static string min_to_str(uint32_t);
    [[nodiscard]] static uint32_t calculate_profit(const string&, const string&);
    static void clear_table(const string&, const string&);
    static void read_file(const char*);
    static void generate_outgoing_events();
    static void handle_first_id(const Event&);
    static void handle_second_id(const Event&);
    static void handle_third_id(const Event&);
    static void handle_fourth_id(const Event&);
    static void print_output();

public:
    App(const App& other) = delete;
    App& operator=(const App& other) = delete;
    App(App&& other) = delete;
    App& operator=(App&& other) = delete;

    static void start(const char*);
};