#ifndef CONNECTION_H
#define CONNECTION_H
#include <map>
#include <vector>
#include <string>
#include "auth.h"

class ConnectionManager {
private:
    std::map<int, std::vector<int> > connections;
    std::map<int, std::vector<int> > pending;
    void save() const;
    bool contains(const std::vector<int>& values, int id) const;
public:
    ConnectionManager();
    void load();
    void sendRequest(int fromId, int toId);
    void viewRequests(int userId, const std::vector<User>& users);
    void viewConnections(int userId, const std::vector<User>& users) const;
};
#endif
