#include "connection.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

ConnectionManager::ConnectionManager() { load(); }
bool ConnectionManager::contains(const vector<int> &v, int id) const
{
    return find(v.begin(), v.end(), id) != v.end();
}
void ConnectionManager::load()
{
    ifstream file("connections.txt");
    if (!file)
        return;
    connections.clear();
    pending.clear();
    char type;
    int a, b;
    while (file >> type >> a >> b)
    {
        if (type == 'C')
        {
            if (!contains(connections[a], b))
                connections[a].push_back(b);
            if (!contains(connections[b], a))
                connections[b].push_back(a);
        }
        else if (type == 'P')
        {
            if (!contains(pending[b], a))
                pending[b].push_back(a);
        }
    }
}
void ConnectionManager::save() const
{
    ofstream file("connections.txt");
    for (const auto &entry : connections)
        for (int id : entry.second)
            if (entry.first < id)
                file << "C " << entry.first << ' ' << id << '\n';
    for (const auto &entry : pending)
        for (int sender : entry.second)
            file << "P " << sender << ' ' << entry.first << '\n';
}
void ConnectionManager::sendRequest(int fromId, int toId)
{
    if (fromId == toId)
    {
        cout << "You cannot connect with yourself.\n";
        return;
    }
    if (contains(connections[fromId], toId))
    {
        cout << "You are already connected.\n";
        return;
    }
    if (contains(pending[toId], fromId))
    {
        cout << "Request already sent.\n";
        return;
    }
    if (contains(pending[fromId], toId))
    {
        cout << "This user has already sent you a request. Accept it from View Requests.\n";
        return;
    }
    pending[toId].push_back(fromId);
    save();
    cout << "Connection request sent.\n";
}
void ConnectionManager::viewRequests(int userId, const vector<User> &users)
{
    vector<int> &requests = pending[userId];
    if (requests.empty())
    {
        cout << "No pending connection requests.\n";
        return;
    }
    for (size_t i = 0; i < requests.size();)
    {
        int sender = requests[i];
        string name = "Unknown user";
        for (const User &u : users)
            if (u.id == sender)
                name = u.fullName;
        cout << "\nRequest from " << name << " (ID " << sender << ")\n1. Accept\n2. Reject\n3. Skip\nChoice: ";
        int choice;
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            return;
        }
        if (choice == 1)
        {
            connections[userId].push_back(sender);
            connections[sender].push_back(userId);
            requests.erase(requests.begin() + i);
            cout << "Request accepted.\n";
        }
        else if (choice == 2)
        {
            requests.erase(requests.begin() + i);
            cout << "Request rejected.\n";
        }
        else
            ++i;
    }
    save();
}
void ConnectionManager::viewConnections(int userId, const vector<User> &users) const
{
    auto it = connections.find(userId);
    if (it == connections.end() || it->second.empty())
    {
        cout << "You have no connections yet.\n";
        return;
    }
    cout << "\n--- Your Connections ---\n";
    for (int id : it->second)
    {
        for (const User &u : users)
            if (u.id == id)
                cout << u.id << ". " << u.fullName << " (" << u.role << ", " << u.department << ")\n";
    }
}
