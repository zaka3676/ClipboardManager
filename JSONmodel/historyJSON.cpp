#include "historyJSON.hpp"
#include "json.hpp"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iterator>
#include <sstream>

using json = nlohmann::json;

// HistoryEntry Class

HistoryEntry::HistoryEntry(const std::string& date, const std::string& content)
    : date(date), content(content) {}

std::string HistoryEntry::getDate() const {
    return date;
}

std::string HistoryEntry::getContent() const {
    return content;
}


// HistoryJSON Class
void HistoryJSON::addHistoryEntry(const std::string& content) {
    // Get current time using the system clock
    std::time_t currentTime = std::time(0);
    
    // Convert the current time to a tm struct
    std::tm* now = std::localtime(&currentTime);

    int year = now->tm_year + 1900; // years since 1900
    int month = now->tm_mon + 1;    // months since January (1-12)
    int day = now->tm_mday;         // day of the month (1-31)
    int hours = now->tm_hour;   // hours since midnight (0-23)
    int minutes = now->tm_min;  // minutes past the hour (0-59)
    int seconds = now->tm_sec;  // seconds past the minute (0-59)
    
    // Format the date and time with leading zeros
    std::stringstream dateStream;
    dateStream << std::setw(4) << std::setfill('0') << year << "-"
               << std::setw(2) << std::setfill('0') << month << "-"
               << std::setw(2) << std::setfill('0') << day << " "
               << std::setw(2) << std::setfill('0') << hours << ":"
               << std::setw(2) << std::setfill('0') << minutes << ":"
               << std::setw(2) << std::setfill('0') << seconds;
    
    std::string date = dateStream.str();
    History.emplace_back(date, content);
}

void HistoryJSON::removeHistoryEntry(const std::string& date) {
  for (int i = 0; i < History.size(); ++i) {
    if (History[i].getDate() == date) {
      auto index = std::next(History.begin(), i);
      History.erase(index);
    }
  }
}

void HistoryJSON::saveHistory() const {
    json jsonData;
    for (const HistoryEntry& history_entry : History) {
        json historyJson;
        historyJson["date"] = history_entry.getDate();
        historyJson["content"] = history_entry.getContent();
        jsonData.push_back(historyJson);
    }

    std::ofstream output("history.json");
    if (output.is_open()) {
        output << std::setw(4) << jsonData << std::endl;
        output.close();
    } else {
        std::cerr << "Error creating file." << std::endl;
    }
}

std::vector<HistoryEntry> HistoryJSON::loadHistory() {
    std::ifstream input("history.json");
    json jsonData;

    // Check if the file was opened successfully
    if (input.is_open()) {
        input >> jsonData;
        input.close();
    } else {
        std::cerr << "Error opening file." << std::endl;
    }

    History.clear(); // clear loaded history
    for (const auto& historyJson : jsonData) {
        std::string date = historyJson["date"];
        std::string content = historyJson["content"];
        History.emplace_back(date, content);
    }
    return History;
}
