#ifndef HISTORYJSON_H
#define HISTORYJSON_H

#include <string>
#include <vector>

class HistoryEntry {
public:
    HistoryEntry(const std::string& date, const std::string& content);

    std::string getDate() const;
    std::string getContent() const;

private:
    std::string date;
    std::string content;
};

class HistoryJSON {
public:
    void addHistoryEntry(const std::string& content);
    void removeHistoryEntry(const std::string& date);
    void saveHistory() const;
    std::vector<HistoryEntry> loadHistory();
    void clear();

private:
    std::vector<HistoryEntry> History;
};

#endif // HISTORYJSON_H
