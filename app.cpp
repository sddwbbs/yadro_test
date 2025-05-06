#include "app.hpp"

[[nodiscard]] uint32_t App::str_to_min(const string& str) {
    uint32_t hours = 0;
    uint32_t minutes = 0;
    char delim;
    std::istringstream iss(str);
    iss >> hours >> delim >> minutes;
    return (hours * 60 + minutes);
}

[[nodiscard]] string App::min_to_str(const uint32_t total_minutes) {
    uint32_t hours = total_minutes / 60;
    uint32_t minutes = total_minutes % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;

    return oss.str();
}

[[nodiscard]] uint32_t App::calculate_profit(const string& start, const string& end) {
    uint32_t start_min = str_to_min(start);
    uint32_t end_min = str_to_min(end);
    uint32_t hours = (end_min - start_min + 59) / 60;
    return hours * cost_per_hour;
}

void App::clear_table(const string& time, const string& client_name) {
    int64_t table_number = client_to_table[client_name];
    Table& table = tables[table_number];
    table.profit += calculate_profit(table.start_time, time);
    table.occupied_time += str_to_min(time) - str_to_min(table.start_time);
    table.client_name = "";
    table.start_time = "";
    table.isBusy = false;
    table_to_client.erase(table_number);
    client_to_table.erase(client_name);
}

void App::read_file(const char* file_name) {
    std::ifstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("Ошибка при открытии файла");
    }

    string line;

    if (!std::getline(file, line) || !(std::istringstream(line) >> number_of_tables))
        throw std::runtime_error("Неверные входные данные в строке: " + line);
    if (!std::getline(file, line) || !(std::istringstream(line) >> opening_time >> closing_time))
        throw std::runtime_error("Неверные входные данные в строке: " + line);
    if (!std::getline(file, line) || !(std::istringstream(line) >> cost_per_hour))
        throw std::runtime_error("Неверные входные данные в строке: " + line);

    opening_time_min = str_to_min(opening_time);
    closing_time_min = str_to_min(closing_time);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Event ev;
        if (!(iss >> ev.time >> ev.id >> ev.body))
            throw std::runtime_error("Неверные входные данные в строке: " + line);
        if (!iss.eof() && !(iss >> ev.table_number))
            throw std::runtime_error("Неверные входные данные в строке: " + line);

        incoming_events.emplace_back(ev);
    }

    file.close();
}

void App::generate_outgoing_events() {
    tables.resize(number_of_tables);
    for (int64_t i = 0; i < number_of_tables; ++i) {
        tables[i].number = i + 1;
    }

    for (const auto& event : incoming_events) {
        outgoing_events.emplace_back(event);

        if (event.id == 1) {
            handle_first_id(event);
        }
        else if (event.id == 2) {
            handle_second_id(event);
        }
        else if (event.id == 3) {
            handle_third_id(event);
        }
        else if (event.id == 4) {
            handle_fourth_id(event);
        }
    }

    vector<string> client_names;
    for (const auto& key : client_to_table | std::views::keys) {
        client_names.push_back(key);
    }

    for (const auto& client : client_names) {
        clear_table(closing_time, client);
        outgoing_events.emplace_back(closing_time, 11, client, -1);
    }
}

void App::handle_first_id(const Event& event) {
    const string& client_name = event.body;

    if (club_clients.contains(client_name)) {
        outgoing_events.emplace_back(event.time, 13, "YouShallNotPass", -1);
        return;
    }

    if (uint32_t event_time = str_to_min(event.time);
        event_time < opening_time_min || event_time >= closing_time_min) {
        outgoing_events.emplace_back(event.time, 13, "NotOpenYet", -1);
        return;
    }

    club_clients.insert(client_name);
}

void App::handle_second_id(const Event& event) {
    const string& client_name = event.body;

    if (!club_clients.contains(client_name)) {
        outgoing_events.emplace_back(event.time, 13, "ClientUnknown", -1);
        return;
    }
    if (tables[event.table_number - 1].isBusy) {
        outgoing_events.emplace_back(event.time, 13, "PlaceIsBusy", -1);
        return;
    }

    if (client_to_table.contains(client_name)) {
        clear_table(event.time, client_name);
    }

    Table& cur_table = tables[event.table_number - 1];
    cur_table.client_name = client_name;
    cur_table.start_time = event.time;
    cur_table.isBusy = true;
    client_to_table[client_name] = event.table_number - 1;
    table_to_client[event.table_number - 1] = client_name;
}

void App::handle_third_id(const Event& event) {
    const string& client_name = event.body;

    bool free_tables_exist = false;
    for (const auto& table : tables) {
        if (!table.isBusy) free_tables_exist = true;
    }

    if (free_tables_exist) {
        outgoing_events.emplace_back(event.time, 13, "ICanWaitNoLonger", -1);
        return;
    }
    if (clients_queue.size() > number_of_tables) {
        outgoing_events.emplace_back(event.time, 11, client_name, -1);
        return;
    }

    clients_queue.push(event.body);
}

void App::handle_fourth_id(const Event& event) {
    const string& client_name = event.body;

    if (!club_clients.contains(client_name)) {
        outgoing_events.emplace_back(event.time, 13, "ClientUnknown", -1);
    }

    uint32_t table_number = client_to_table[client_name];
    clear_table(event.time, client_name);

    if (!clients_queue.empty()) {
        string client_from_queue = clients_queue.front();
        clients_queue.pop();

        Table& cur_table = tables[table_number];
        cur_table.client_name = client_from_queue;
        cur_table.start_time = event.time;
        cur_table.isBusy = true;
        client_to_table[client_from_queue] = table_number;
        table_to_client[table_number] = client_from_queue;

        outgoing_events.emplace_back(event.time, 12, client_from_queue, table_number + 1);
    }
}

void App::print_output() {
    std::cout << opening_time << std::endl;
    for (const auto& event : outgoing_events) {
        std::cout << event.time << ' '
        << event.id << ' '
        << event.body << ' '
        << (event.table_number == -1 ? "" : std::to_string(event.table_number)) << std::endl;
    }
    std::cout << closing_time << std::endl;

    for (const auto& table : tables) {
        std::cout << table.number << ' '
        << table.profit << ' '
        << min_to_str(table.occupied_time) << std::endl;
    }
}

void App::start(const char* file_name) {
    read_file(file_name);
    generate_outgoing_events();
    print_output();
}