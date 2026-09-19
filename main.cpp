#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <climits>
#include <iomanip>
#include <termios.h>
#include <unistd.h>
using namespace std;


// ======================================================
// EDGE STRUCTURE
// ======================================================

struct Edge
{
    int u, v;
    int length;
    int cost;
    int capacity = 100;      // Maximum bandwidth (Mbps)
    int currentLoad = 0;     // Current used bandwidth (Mbps)
};

// ======================================================
// NODE NAMES
// ======================================================

vector<string> nodeName =
{
    "",
    "Engineering Building",
    "YKSG-2",
    "RASG-1",
    "Food Court",
    "Auditorium",
    "AB-4",
    "Annex Building",
    "Admission Building",
    "Hall Accommodation",
    "ID Card Section",
    "Gate-4",
    "Gate-3",
    "Gate-2",
    "AB-1",
    "Teachers Home",
    "Gate-8",
    "Mosque",
    "AB-3",
    "Green Garden",
    "Gate-1",
    "Transport Building"
};


// ======================================================
// INITIAL CABLE CONNECTIONS
// ======================================================

vector<Edge> edges =
{
    {6, 7, 120, 12000, 100, 25},    // AB-4 -> Annex
    {6, 5, 80, 8000, 100, 45},      // AB-4 -> Auditorium
    {5, 4, 60, 6000, 100, 60},      // Auditorium -> Food Court
    {4, 3, 90, 9000, 100, 30},      // Food Court -> RASG-1
    {3, 2, 70, 7000, 100, 70},      // RASG-1 -> YKSG-2
    {2, 1, 100, 10000, 100, 80},    // YKSG-2 -> Engineering Building

    {3, 14, 110, 11000, 100, 35},   // RASG-1 -> AB-1
    {6, 14, 75, 7500, 100, 20},     // AB-4 -> AB-1
    {6, 11, 50, 5000, 100, 75},     // AB-4 -> Gate-4
    {6, 8, 95, 9500, 100, 40},      // AB-4 -> Admission Building

    {9, 8, 65, 6500, 100, 25},      // Hall Accommodation -> Admission
    {9, 12, 85, 8500, 100, 55},     // Hall Accommodation -> Gate-3
    {8, 12, 55, 5500, 100, 65},     // Admission -> Gate-3
    {8, 10, 45, 4500, 100, 30},     // Admission -> ID Card Section
    {10, 13, 70, 7000, 100, 50},    // ID Card Section -> Gate-2

    {14, 15, 90, 9000, 100, 45},    // AB-1 -> Teachers Home
    {15, 16, 60, 6000, 100, 70},    // Teachers Home -> Gate-8
    {14, 17, 50, 5000, 100, 35},    // AB-1 -> Mosque
    {17, 18, 65, 6500, 100, 50},    // Mosque -> AB-3
    {18, 3, 80, 8000, 100, 25},     // AB-3 -> RASG-1

    {3, 19, 75, 7500, 100, 40},     // RASG-1 -> Green Garden
    {3, 20, 100, 10000, 100, 65},    // RASG-1 -> Gate-1
    {14, 21, 120, 12000, 100, 30}   // AB-1 -> Transport Building
};


void showConnectionDetails(const Edge &e)
{
    int available = e.capacity - e.currentLoad;

    cout << "Connection : "
         << nodeName[e.u]
         << " -> "
         << nodeName[e.v]
         << endl;

    cout << "Length     : "
         << e.length
         << " meter"
         << endl;

    cout << "Cost       : Tk. "
         << e.cost
         << endl;

    cout << "Capacity   : "
         << e.capacity
         << " Mbps"
         << endl;

    cout << "Current Load : "
         << e.currentLoad
         << " Mbps"
         << endl;

    cout << "Available  : "
         << available
         << " Mbps"
         << endl;
}


// ======================================================
// FIND NODE BY NAME
// ======================================================

int findNode(string name)
{
    for (int i = 1; i < nodeName.size(); i++)
    {
        if (nodeName[i] == name)
        {
            return i;
        }
    }

    return -1;
}


// ======================================================
// SHOW ALL NODES
// ======================================================

void showAllNodes()
{
    cout << endl;

    cout << "==================================================" << endl;
    cout << "                 CAMPUS NODES                     " << endl;
    cout << "==================================================" << endl;

    for (int i = 1; i < nodeName.size(); i++)
    {
        cout << i << ". " << nodeName[i] << endl;
    }
}


// ======================================================
// SHOW ALL CONNECTIONS
// ======================================================

void showAllConnections()
{
    cout << endl;

    cout << "==================================================" << endl;
    cout << "              ALL CONNECTIONS                     " << endl;
    cout << "==================================================" << endl;

    for (int i = 0; i < edges.size(); i++)
    {
        cout << i + 1 << ". "
             << nodeName[edges[i].u]
             << " -> "
             << nodeName[edges[i].v]
             << endl;

        cout << "   Length        : "
             << edges[i].length
             << " meter" << endl;

        cout << "   Cost          : Tk. "
             << edges[i].cost << endl;

        cout << "   Capacity      : "
             << edges[i].capacity
             << " Mbps" << endl;

        cout << "   Current Load  : "
             << edges[i].currentLoad
             << " Mbps" << endl;

        cout << "   Available     : "
             << edges[i].capacity - edges[i].currentLoad
             << " Mbps" << endl;

        cout << endl;
    }
}


// ======================================================
// SEARCH CONNECTIONS BY NODE NAME
// ======================================================

vector<int> searchConnections(string name)
{
    vector<int> result;

    for (int i = 0; i < edges.size(); i++)
    {
        if (nodeName[edges[i].u] == name ||
            nodeName[edges[i].v] == name)
        {
            result.push_back(i);
        }
    }

    return result;
}


// ======================================================
// SEARCH & MODIFY CONNECTION
// ======================================================

void searchAndModify()
{
    string name;

    cout << endl;
    cout << "Enter building/node name: ";

    cin.ignore();
    getline(cin, name);

    int node = findNode(name);

    if (node == -1)
    {
        cout << endl;
        cout << "Node not found!" << endl;
        return;
    }

    vector<int> result = searchConnections(name);

    if (result.empty())
    {
        cout << endl;
        cout << "No connection found for this node." << endl;
        return;
    }

    cout << endl;
    cout << "==================================================" << endl;
    cout << "             SEARCH RESULTS" << endl;
    cout << "==================================================" << endl;

    for (int i = 0; i < result.size(); i++)
    {
        int index = result[i];

        cout << i + 1 << ". "
             << nodeName[edges[index].u]
             << " -> "
             << nodeName[edges[index].v]
             << endl;

        cout << "   Length        : "
             << edges[index].length
             << " meter" << endl;

        cout << "   Cost          : Tk. "
             << edges[index].cost << endl;

        cout << "   Capacity      : "
             << edges[index].capacity
             << " Mbps" << endl;

        cout << "   Current Load  : "
             << edges[index].currentLoad
             << " Mbps" << endl;

        cout << "   Available     : "
             << edges[index].capacity
             - edges[index].currentLoad
             << " Mbps" << endl;

        cout << endl;
    }

    int choice;

    cout << "Select connection to modify: ";
    cin >> choice;

    if (choice < 1 || choice > result.size())
    {
        cout << "Invalid selection!" << endl;
        return;
    }

    int index = result[choice - 1];

    cout << endl;
    cout << "==================================================" << endl;
    cout << "             MODIFY CONNECTION" << endl;
    cout << "==================================================" << endl;

    cout << "Connection : "
         << nodeName[edges[index].u]
         << " -> "
         << nodeName[edges[index].v]
         << endl;

    cout << endl;

    cout << "1. Change Length" << endl;
    cout << "2. Change Cost" << endl;
    cout << "3. Change Capacity" << endl;
    cout << "4. Change Current Load" << endl;
    cout << "5. Change All" << endl;
    cout << "6. Cancel" << endl;

    cout << endl;
    cout << "Enter choice: ";

    int option;
    cin >> option;

    if (option == 1)
    {
        cout << "Enter new length: ";
        cin >> edges[index].length;

        cout << endl;
        cout << "Length updated successfully!" << endl;
    }

    else if (option == 2)
    {
        cout << "Enter new cost: ";
        cin >> edges[index].cost;

        cout << endl;
        cout << "Cost updated successfully!" << endl;
    }

    else if (option == 3)
    {
        cout << "Enter new capacity: ";
        cin >> edges[index].capacity;

        if (edges[index].currentLoad > edges[index].capacity)
        {
            cout << endl;
            cout << "Warning: Current load is greater than new capacity!" << endl;
        }
        else
        {
            cout << endl;
            cout << "Capacity updated successfully!" << endl;
        }
    }

    else if (option == 4)
    {
        int newLoad;

        cout << "Enter new current load: ";
        cin >> newLoad;

        if (newLoad > edges[index].capacity)
        {
            cout << endl;
            cout << "Invalid! Load cannot be greater than capacity." << endl;
        }
        else
        {
            edges[index].currentLoad = newLoad;

            cout << endl;
            cout << "Current load updated successfully!" << endl;
        }
    }

    else if (option == 5)
    {
        int newLength;
        int newCost;
        int newCapacity;
        int newLoad;

        cout << "Enter new length: ";
        cin >> newLength;

        cout << "Enter new cost: ";
        cin >> newCost;

        cout << "Enter new capacity: ";
        cin >> newCapacity;

        cout << "Enter new current load: ";
        cin >> newLoad;

        if (newLoad > newCapacity)
        {
            cout << endl;
            cout << "Invalid! Current load cannot be greater than capacity." << endl;
            return;
        }

        edges[index].length = newLength;
        edges[index].cost = newCost;
        edges[index].capacity = newCapacity;
        edges[index].currentLoad = newLoad;

        cout << endl;
        cout << "All connection data updated successfully!" << endl;
    }

    else if (option == 6)
    {
        cout << "Modification cancelled." << endl;
    }

    else
    {
        cout << "Invalid choice!" << endl;
    }
}


// ======================================================
// ADD NEW CONNECTION
// ======================================================

void addNewConnection()
{
    string firstName;
    string secondName;

    cout << endl;

    cout << "Enter first building/node name: ";

    cin.ignore();
    getline(cin, firstName);

    int u = findNode(firstName);

    if (u == -1)
    {
        cout << "First node not found!" << endl;
        return;
    }

    cout << "Enter second building/node name: ";

    getline(cin, secondName);

    int v = findNode(secondName);

    if (v == -1)
    {
        cout << "Second node not found!" << endl;
        return;
    }

    int length;
    int cost;
    int capacity;
    int currentLoad;

    cout << "Enter cable length: ";
    cin >> length;

    cout << "Enter cable cost: ";
    cin >> cost;

    cout << "Enter cable capacity (Mbps): ";
    cin >> capacity;

    cout << "Enter current load (Mbps): ";
    cin >> currentLoad;

    if (currentLoad > capacity)
    {
        cout << endl;
        cout << "Invalid! Current load cannot be greater than capacity." << endl;
        return;
    }

    edges.push_back(
        {u, v, length, cost, capacity, currentLoad}
    );

    cout << endl;

    cout << "==================================================" << endl;
    cout << "        CONNECTION ADDED SUCCESSFULLY" << endl;
    cout << "==================================================" << endl;

    cout << "Connection   : "
         << nodeName[u]
         << " -> "
         << nodeName[v]
         << endl;

    cout << "Length       : "
         << length
         << " meter" << endl;

    cout << "Cost         : Tk. "
         << cost << endl;

    cout << "Capacity     : "
         << capacity
         << " Mbps" << endl;

    cout << "Current Load : "
         << currentLoad
         << " Mbps" << endl;

    cout << "Available    : "
         << capacity - currentLoad
         << " Mbps" << endl;
}

// ======================================================
// ADD NEW NODE + CONNECTIONS
// ======================================================

void addNewNode()
{
    string newName;

    cout << endl;

    cout << "Enter new building/node name: ";

    cin.ignore();
    getline(cin, newName);

    if (findNode(newName) != -1)
    {
        cout << endl;
        cout << "This node already exists!" << endl;
        return;
    }

    nodeName.push_back(newName);

    int newID = nodeName.size() - 1;

    cout << endl;

    cout << "New node added successfully!" << endl;

    cout << "Node ID   : "
         << newID << endl;

    cout << "Node Name : "
         << newName << endl;

    int numberOfConnections;

    cout << endl;

    cout << "How many existing nodes do you want to connect "
         << "with this node? ";

    cin >> numberOfConnections;

    for (int i = 0; i < numberOfConnections; i++)
    {
        string existingName;

        cout << endl;

        cout << "Enter existing node name "
             << i + 1 << ": ";

        cin.ignore();
        getline(cin, existingName);

        int existingID = findNode(existingName);

        if (existingID == -1)
        {
            cout << "Node not found! Try again." << endl;

            i--;
            continue;
        }

        int length;
        int cost;
        int capacity;
        int currentLoad;

        cout << "Enter cable length: ";
        cin >> length;

        cout << "Enter cable cost: ";
        cin >> cost;

        cout << "Enter cable capacity (Mbps): ";
        cin >> capacity;

        cout << "Enter current load (Mbps): ";
        cin >> currentLoad;

        if (currentLoad > capacity)
        {
            cout << endl;
            cout << "Invalid! Current load cannot be greater "
                 << "than capacity." << endl;

            i--;
            continue;
        }

        edges.push_back(
            {
                newID,
                existingID,
                length,
                cost,
                capacity,
                currentLoad
            }
        );

        cout << endl;

        cout << "Connection added: "
             << newName
             << " -> "
             << existingName
             << endl;

        cout << "Length       : "
             << length
             << " meter" << endl;

        cout << "Cost         : Tk. "
             << cost << endl;

        cout << "Capacity     : "
             << capacity
             << " Mbps" << endl;

        cout << "Current Load : "
             << currentLoad
             << " Mbps" << endl;

        cout << "Available    : "
             << capacity - currentLoad
             << " Mbps" << endl;
    }

    cout << endl;

    cout << "==================================================" << endl;
    cout << "          NEW NODE SETUP COMPLETE" << endl;
    cout << "==================================================" << endl;
}


// ======================================================
// MODIFY DATA PANEL
// ======================================================

void ModifyData()
{
    while (true)
    {
        cout << endl;

        cout << "==================================================" << endl;
        cout << "              MODIFY CABLE DATA                  " << endl;
        cout << "==================================================" << endl;

        cout << endl;

        cout << "1. Search & Modify Connection" << endl;
        cout << "2. Add New Connection" << endl;
        cout << "3. Add New Building / Node" << endl;
        cout << "4. Show All Nodes" << endl;
        cout << "5. Show All Connections" << endl;
        cout << "6. Back to Main Panel" << endl;

        cout << endl;

        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:
                searchAndModify();
                break;

            case 2:
                addNewConnection();
                break;

            case 3:
                addNewNode();
                break;

            case 4:
                showAllNodes();
                break;

            case 5:
                showAllConnections();
                break;

            case 6:
                return;

            default:
                cout << endl;
                cout << "Invalid choice!" << endl;
        }
    }
}


// ======================================================
// BINARY SEARCH
// ======================================================

int binarySearch(int target)
{
    int left = 0;
    int right = edges.size() - 1;

    while (left <= right)
    {
        int mid =
            (left + right) / 2;

        int serial =
            mid + 1;

        if (serial == target)
        {
            return mid;
        }

        else if (serial < target)
        {
            left = mid + 1;
        }

        else
        {
            right = mid - 1;
        }
    }

    return -1;
}


// ======================================================
// PROBLEM 1 - CABLE SEARCH
// ======================================================

void Problem1()
{
    while (true)
    {
        cout << endl;

        cout << "==================================================" << endl;
        cout << "          PROBLEM 1 - CABLE SEARCH               " << endl;
        cout << "==================================================" << endl;

        cout << endl;

        cout << "1. Show All Connections" << endl;
        cout << "2. Search all Connection by Name" << endl;
        cout << "3. Back to Main Panel" << endl;

        cout << endl;

        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            showAllConnections();
        }

        else if (choice == 2)
        {
            string buildingName;

            cout << endl;
            cout << "Enter building name: ";

            cin.ignore();
            getline(cin, buildingName);

            int node = findNode(buildingName);

            if (node == -1)
            {
                cout << endl;
                cout << "Building not found!" << endl;
                continue;
            }

            vector<int> result = searchConnections(buildingName);

            if (result.empty())
            {
                cout << endl;
                cout << "No connection found for this building!" << endl;
                continue;
            }

            cout << endl;

            cout << "==================================================" << endl;
            cout << "           CONNECTIONS OF " << buildingName << endl;
            cout << "==================================================" << endl;

            cout << endl;

            for (int i = 0; i < result.size(); i++)
            {
                int index = result[i];

                cout << i + 1 << ". ";

                showConnectionDetails(edges[index]);

                cout << endl;
            }
        }

        else if (choice == 3)
        {
            return;
        }

        else
        {
            cout << endl;
            cout << "Invalid choice!" << endl;
        }
    }
}

int shortestDistance(int start, int destination)
{
    int n = nodeName.size() - 1;

    const int INF = 1000000000;

    vector<int> distance(n + 1, INF);
    vector<bool> visited(n + 1, false);

    distance[start] = 0;

    for (int count = 1; count <= n; count++)
    {
        int current = -1;

        for (int i = 1; i <= n; i++)
        {
            if (!visited[i] && distance[i] != INF)
            {
                if (current == -1 ||
                    distance[i] < distance[current])
                {
                    current = i;
                }
            }
        }

        if (current == -1)
            break;

        visited[current] = true;

        for (int i = 0; i < edges.size(); i++)
        {
            int next = -1;

            if (edges[i].u == current)
                next = edges[i].v;

            else if (edges[i].v == current)
                next = edges[i].u;

            if (next != -1)
            {
                int length = edges[i].length;

                if (distance[current] + length <
                    distance[next])
                {
                    distance[next] =
                        distance[current] + length;
                }
            }
        }
    }

    if (distance[destination] == INF)
        return -1;

    return distance[destination];
}


bool dijkstraShortestPath(
    int source,
    int destination,
    vector<int> &path,
    int &totalLength,
    int &totalCost
)
{
    int n = nodeName.size() - 1;
    const int INF = 1000000000;

    vector<int> dist(n + 1, INF);
    vector<int> parent(n + 1, -1);
    vector<bool> visited(n + 1, false);

    dist[source] = 0;

    // Dijkstra
    for (int count = 1; count <= n; count++)
    {
        int u = -1;

        for (int i = 1; i <= n; i++)
        {
            if (!visited[i] &&
                (u == -1 || dist[i] < dist[u]))
            {
                u = i;
            }
        }

        if (u == -1 || dist[u] == INF)
            break;

        visited[u] = true;

        for (int i = 0; i < edges.size(); i++)
        {
            int v = -1;

            if (edges[i].u == u)
                v = edges[i].v;
            else if (edges[i].v == u)
                v = edges[i].u;

            if (v != -1)
            {
                if (dist[u] + edges[i].length < dist[v])
                {
                    dist[v] =
                        dist[u] + edges[i].length;

                    parent[v] = u;
                }
            }
        }
    }

    // No path
    if (dist[destination] == INF)
    {
        return false;
    }

    // Build path
    int current = destination;

    while (current != -1)
    {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    totalLength = dist[destination];

    // Calculate total cost
    totalCost = 0;

    for (int i = 0; i < path.size() - 1; i++)
    {
        int u = path[i];
        int v = path[i + 1];

        for (int j = 0; j < edges.size(); j++)
        {
            if (
                (edges[j].u == u &&
                 edges[j].v == v)

                ||

                (edges[j].u == v &&
                 edges[j].v == u)
            )
            {
                totalCost += edges[j].cost;
                break;
            }
        }
    }

    return true;
}


// ======================================================
// PROBLEM 2 - SHORTEST PATH
// ======================================================

void Problem2()
{
    while (true)
    {
        cout << endl;

        cout << "==================================================" << endl;
        cout << "          PROBLEM 2 - SHORTEST PATH              " << endl;
        cout << "==================================================" << endl;

        cout << endl;

        cout << "1. Find Shortest Path" << endl;
        cout << "2. Back to Main Panel" << endl;

        cout << endl;

        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            string startName;
            string endName;

            cout << endl;

            cout << "Enter starting point: ";

            cin.ignore();
            getline(cin, startName);

            cout << "Enter ending point: ";
            getline(cin, endName);

            int start =
                findNode(startName);

            int destination =
                findNode(endName);

            if (start == -1)
            {
                cout << endl;
                cout << "Starting point not found!" << endl;
                continue;
            }

            if (destination == -1)
            {
                cout << endl;
                cout << "Ending point not found!" << endl;
                continue;
            }

            int n =
                nodeName.size() - 1;

            vector<vector<pair<int, pair<int, int>>>> graph(
                n + 1
            );

            for (int i = 0; i < edges.size(); i++)
            {
                int u = edges[i].u;
                int v = edges[i].v;
                int length = edges[i].length;
                int cost = edges[i].cost;

                graph[u].push_back(
                    {v, {length, cost}}
                );

                graph[v].push_back(
                    {u, {length, cost}}
                );
            }

            const int INF =
                1000000000;

            vector<int> distance(
                n + 1,
                INF
            );

            vector<int> parentNode(
                n + 1,
                -1
            );

            vector<bool> visited(
                n + 1,
                false
            );

            distance[start] = 0;

            for (int count = 1;
                 count <= n;
                 count++)
            {
                int current = -1;

                for (int i = 1;
                     i <= n;
                     i++)
                {
                    if (!visited[i] &&
                        distance[i] != INF)
                    {
                        if (current == -1 ||
                            distance[i] <
                            distance[current])
                        {
                            current = i;
                        }
                    }
                }

                if (current == -1)
                {
                    break;
                }

                visited[current] = true;

                for (int j = 0;
                     j < graph[current].size();
                     j++)
                {
                    int next =
                        graph[current][j].first;

                    int length =
                        graph[current][j].second.first;

                    if (distance[current] + length <
                        distance[next])
                    {
                        distance[next] =
                            distance[current] + length;

                        parentNode[next] =
                            current;
                    }
                }
            }

            if (distance[destination] == INF)
            {
                cout << endl;

                cout << "==================================================" << endl;
                cout << "             NO PATH AVAILABLE                   " << endl;
                cout << "==================================================" << endl;

                continue;
            }

            vector<int> path;

            int current =
                destination;

            while (current != -1)
            {
                path.push_back(current);

                current =
                    parentNode[current];
            }

            reverse(
                path.begin(),
                path.end()
            );

            int totalCost = 0;

            for (int i = 0;
                 i < path.size() - 1;
                 i++)
            {
                int u = path[i];
                int v = path[i + 1];

                for (int j = 0;
                     j < edges.size();
                     j++)
                {
                    if (
                        (edges[j].u == u &&
                         edges[j].v == v)
                        ||
                        (edges[j].u == v &&
                         edges[j].v == u)
                    )
                    {
                        totalCost +=
                            edges[j].cost;

                        break;
                    }
                }
            }

            cout << endl;

            cout << "==================================================" << endl;
            cout << "             SHORTEST PATH RESULT                " << endl;
            cout << "==================================================" << endl;

            cout << endl;

            cout << "Starting Point : "
                 << nodeName[start]
                 << endl;

            cout << "Ending Point   : "
                 << nodeName[destination]
                 << endl;

            cout << endl;

            cout << "--------------------------------------------------" << endl;
            cout << "                  SHORTEST PATH                  " << endl;
            cout << "--------------------------------------------------" << endl;

            cout << endl;

            cout << "Path : ";

            for (int i = 0;
                 i < path.size();
                 i++)
            {
                cout << nodeName[path[i]];

                if (i != path.size() - 1)
                {
                    cout << " -> ";
                }
            }

            cout << endl;

            cout << endl;

            cout << "--------------------------------------------------" << endl;
            cout << "              CONNECTION DETAILS                 " << endl;
            cout << "--------------------------------------------------" << endl;

            for (int i = 0;
                 i < path.size() - 1;
                 i++)
            {
                int u = path[i];
                int v = path[i + 1];

                for (int j = 0;
                     j < edges.size();
                     j++)
                {
                    if (
                        (edges[j].u == u &&
                         edges[j].v == v)
                        ||
                        (edges[j].u == v &&
                         edges[j].v == u)
                    )
                    {
                        cout << endl;

                        cout << nodeName[u]
                             << " -> "
                             << nodeName[v]
                             << endl;

                        cout << "Length : "
                             << edges[j].length
                             << " meter"
                             << endl;

                        cout << "Cost   : Tk. "
                             << edges[j].cost
                             << endl;

                        break;
                    }
                }
            }

            cout << endl;

            cout << "==================================================" << endl;
            cout << "                 FINAL RESULT                    " << endl;
            cout << "==================================================" << endl;

            cout << endl;

            cout << "Total Shortest Length : "
                 << distance[destination]
                 << " meter"
                 << endl;

            cout << "Total Path Cost       : Tk. "
                 << totalCost
                 << endl;

            cout << "Total Nodes in Path   : "
                 << path.size()
                 << endl;
        }

        else if (choice == 2)
        {
            return;
        }

        else
        {
            cout << endl;
            cout << "Invalid choice!" << endl;
        }
    }
}


// ======================================================
// DFS FOR CONNECTIVITY
// ======================================================

void DFS(
    int node,
    vector<vector<int>> &graph,
    vector<bool> &visited
)
{
    visited[node] = true;

    for (int i = 0;
         i < graph[node].size();
         i++)
    {
        int next =
            graph[node][i];

        if (!visited[next])
        {
            DFS(
                next,
                graph,
                visited
            );
        }
    }
}

// ======================================================
// CHECK NETWORK CONNECTIVITY
// ======================================================

bool isNetworkConnected(
    int failedEdge
)
{
    int n =
        nodeName.size() - 1;

    vector<vector<int>> graph(
        n + 1
    );

    for (int i = 0;
         i < edges.size();
         i++)
    {
        if (i == failedEdge)
        {
            continue;
        }

        int u =
            edges[i].u;

        int v =
            edges[i].v;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<bool> visited(
        n + 1,
        false
    );

    DFS(
        1,
        graph,
        visited
    );

    for (int i = 1;
         i <= n;
         i++)
    {
        if (!visited[i])
        {
            return false;
        }
    }

    return true;
}


// ======================================================
// BACKUP PATH USING DIJKSTRA
// ======================================================

bool findBackupPath(
    int start,
    int destination,
    int failedEdge,
    vector<int> &path,
    int &totalLength,
    int &totalCost
)
{
    int n =
        nodeName.size() - 1;

    vector<vector<pair<int, pair<int, int>>>> graph(
        n + 1
    );


    // ----------------------------------------------
    // Create graph without failed connection
    // ----------------------------------------------

    for (int i = 0;
         i < edges.size();
         i++)
    {
        if (i == failedEdge)
        {
            continue;
        }

        int u =
            edges[i].u;

        int v =
            edges[i].v;

        int length =
            edges[i].length;

        int cost =
            edges[i].cost;


        graph[u].push_back(
            {v, {length, cost}}
        );

        graph[v].push_back(
            {u, {length, cost}}
        );
    }


    const int INF =
        1000000000;


    vector<int> distance(
        n + 1,
        INF
    );

    vector<int> parentNode(
        n + 1,
        -1
    );

    vector<bool> visited(
        n + 1,
        false
    );


    distance[start] = 0;


    // ----------------------------------------------
    // Dijkstra
    // ----------------------------------------------

    for (int count = 1;
         count <= n;
         count++)
    {
        int current = -1;


        for (int i = 1;
             i <= n;
             i++)
        {
            if (!visited[i] &&
                distance[i] != INF)
            {
                if (current == -1 ||
                    distance[i] <
                    distance[current])
                {
                    current = i;
                }
            }
        }


        if (current == -1)
        {
            break;
        }


        visited[current] = true;


        for (int j = 0;
             j < graph[current].size();
             j++)
        {
            int next =
                graph[current][j].first;

            int length =
                graph[current][j].second.first;


            if (distance[current] + length <
                distance[next])
            {
                distance[next] =
                    distance[current] + length;

                parentNode[next] =
                    current;
            }
        }
    }


    if (distance[destination] == INF)
    {
        return false;
    }


    // ----------------------------------------------
    // Build Path
    // ----------------------------------------------

    int current =
        destination;


    while (current != -1)
    {
        path.push_back(current);

        current =
            parentNode[current];
    }


    reverse(
        path.begin(),
        path.end()
    );


    totalLength =
        distance[destination];


    totalCost = 0;


    // ----------------------------------------------
    // Calculate Backup Cost
    // ----------------------------------------------

    for (int i = 0;
         i < path.size() - 1;
         i++)
    {
        int u =
            path[i];

        int v =
            path[i + 1];


        for (int j = 0;
             j < edges.size();
             j++)
        {
            if (j == failedEdge)
            {
                continue;
            }


            if (
                (edges[j].u == u &&
                 edges[j].v == v)
                ||
                (edges[j].u == v &&
                 edges[j].v == u)
            )
            {
                totalCost +=
                    edges[j].cost;

                break;
            }
        }
    }


    return true;
}


// ======================================================
// PROBLEM 3 - CABLE FAILURE & BACKUP PLANNER
// ======================================================



int getComponentSize(int start, int failedEdge)
{
    int n = nodeName.size() - 1;
    vector<bool> visited(n + 1, false);

    queue<int> q;

    q.push(start);
    visited[start] = true;

    int count = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        count++;

        for (int i = 0; i < edges.size(); i++)
        {
            if (i == failedEdge)
                continue;

            int v = -1;

            if (edges[i].u == u)
                v = edges[i].v;

            else if (edges[i].v == u)
                v = edges[i].u;

            if (v != -1 && !visited[v])
            {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return count;
}





void Problem3()
{
    while (true)
    {
        cout << endl;

        cout << "==================================================" << endl;
        cout << "          PROBLEM 3 - CABLE FAILURE ANALYZER     " << endl;
        cout << "==================================================" << endl;

        cout << endl;

        cout << "1. Simulate Cable Failure" << endl;
        cout << "2. Back to Main Panel" << endl;

        cout << endl;

        cout << "Enter your choice: ";

        int choice;
        cin >> choice;


        // ==================================================
        // OPTION 1 : SIMULATE CABLE FAILURE
        // ==================================================

        if (choice == 1)
        {
            cout << endl;

            cout << "==================================================" << endl;
            cout << "             AVAILABLE CONNECTIONS               " << endl;
            cout << "==================================================" << endl;

            cout << endl;

            for (int i = 0; i < edges.size(); i++)
            {
                cout << i + 1 << ". "
                     << nodeName[edges[i].u]
                     << " -> "
                     << nodeName[edges[i].v]
                     << endl;

                cout << endl;
            }


            // ----------------------------------------------
            // SELECT CABLE
            // ----------------------------------------------

            int failedConnection;

            cout << "Enter connection number to simulate failure: ";
            cin >> failedConnection;


            if (failedConnection < 1 ||
                failedConnection > edges.size())
            {
                cout << endl;
                cout << "Invalid connection number!" << endl;
                continue;
            }


            int failedEdge = failedConnection - 1;

            int u = edges[failedEdge].u;
            int v = edges[failedEdge].v;


            // ----------------------------------------------
            // FAILURE INFORMATION
            // ----------------------------------------------

            cout << endl;

            cout << "==================================================" << endl;
            cout << "              CABLE FAILURE                      " << endl;
            cout << "==================================================" << endl;

            cout << endl;

            cout << "Failed Connection : "
                 << nodeName[u]
                 << " -> "
                 << nodeName[v]
                 << endl;

            cout << "Cable Length      : "
                 << edges[failedEdge].length
                 << " meter" << endl;

            cout << "Cable Cost        : Tk. "
                 << edges[failedEdge].cost
                 << endl;


            // ----------------------------------------------
            // CHECK COMPONENTS
            // ----------------------------------------------

            int componentU =
                getComponentSize(u, failedEdge);

            int componentV =
                getComponentSize(v, failedEdge);
            
            int n = nodeName.size() - 1;

            // ==================================================
            // STATUS 1 : SAFE
            // ==================================================

            if (componentU == n &&
                componentV == n)
            {
                cout << endl;

                cout << "==================================================" << endl;
                cout << "                 STATUS : SAFE                    " << endl;
                cout << "==================================================" << endl;

                cout << endl;

                cout << "The failed cable has an alternative route."
                     << endl;

                cout << endl;

                // ----------------------------------------------
                // FIND ALTERNATIVE / BACKUP PATH
                // ----------------------------------------------

                vector<int> backupPath;

                int backupLength = 0;
                int backupCost = 0;

                bool backupFound =
                    findBackupPath(
                        u,
                        v,
                        failedEdge,
                        backupPath,
                        backupLength,
                        backupCost
                    );

                if (backupFound)
                {
                    cout << endl;

                    cout << "--------------------------------------------------" << endl;
                    cout << "              ALTERNATIVE PATH                   " << endl;
                    cout << "--------------------------------------------------" << endl;

                    cout << endl;

                    cout << "Backup Path : ";

                    for (int i = 0;
                         i < backupPath.size();
                         i++)
                    {
                        cout << nodeName[backupPath[i]];

                        if (i != backupPath.size() - 1)
                        {
                            cout << " -> ";
                        }
                    }

                    cout << endl;

                    cout << endl;

                    cout << "Backup Length : "
                         << backupLength
                         << " meter"
                         << endl;

                    cout << "Backup Cost   : Tk. "
                         << backupCost
                         << endl;
                }
                else
                {
                    cout << endl;

                    cout << "Unable to find an alternative path."
                         << endl;
                }

                cout << endl;

                cout << "Network Status:" << endl;

                cout << "   All campus locations are still connected."
                     << endl;

                cout << endl;

                cout << "The failed connection can be bypassed"
                     << " through another path." << endl;

                cout << endl;

                cout << "Result:" << endl;

                cout << "   No major network problem detected."
                     << endl;

                cout << "   Cable repair is not immediately required."
                     << endl;
            }


            // ==================================================
            // STATUS 2 : WARNING
            // ==================================================

            else if (componentU == 1 ||
                     componentV == 1)
            {
                cout << endl;

                cout << "==================================================" << endl;
                cout << "               STATUS : WARNING                   " << endl;
                cout << "==================================================" << endl;

                cout << endl;

                cout << "No alternative route is available"
                     << " for this connection." << endl;

                cout << endl;

                cout << "Affected Location:" << endl;

                if (componentU == 1)
                {
                    cout << "   "
                         << nodeName[u]
                         << endl;
                }
                else
                {
                    cout << "   "
                         << nodeName[v]
                         << endl;
                }

                cout << endl;

                cout << "Network Status:" << endl;

                cout << "   The main campus network remains connected."
                     << endl;

                cout << "   Only the affected location is isolated."
                     << endl;

                cout << endl;

                cout << "Result:" << endl;

                cout << "   Local network problem detected."
                     << endl;

                cout << "   Cable repair is recommended."
                     << endl;
            }


            // ==================================================
            // STATUS 3 : CRITICAL
            // ==================================================

            else
            {
                cout << endl;

                cout << "==================================================" << endl;
                cout << "              STATUS : CRITICAL                   " << endl;
                cout << "==================================================" << endl;

                cout << endl;

                cout << "NO ALTERNATIVE CONNECTION IS AVAILABLE."
                     << endl;

                cout << endl;

                cout << "The failed cable separates"
                     << " the campus network." << endl;

                cout << endl;

                cout << "Network Section Information:" << endl;

                cout << "   Section 1 : "
                     << componentU
                     << " locations" << endl;

                cout << "   Section 2 : "
                     << componentV
                     << " locations" << endl;

                cout << endl;

                cout << "Affected Connection:" << endl;

                cout << "   "
                     << nodeName[u]
                     << " -> "
                     << nodeName[v]
                     << endl;

                cout << endl;

                cout << "Network Status:" << endl;

                cout << "   Part of the campus is disconnected."
                     << endl;

                cout << "   Major communication problem detected."
                     << endl;

                cout << endl;

                cout << "Result:" << endl;

                cout << "   IMMEDIATE CABLE REPAIR REQUIRED."
                     << endl;
            }


            cout << endl;

            cout << "==================================================" << endl;
        }

        // ==================================================
        // OPTION 3 : BACK
        // ==================================================

        else if (choice == 2)
        {
            return;
        }


        // ==================================================
        // INVALID
        // ==================================================

        else
        {
            cout << endl;
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// ======================================================
// PROBLEM 4
// ======================================================

void Problem4()
{
    int technicianCount;
    int faultCount;

    cout << endl;
    cout << "==================================================" << endl;
    cout << "       PROBLEM 4 - IT TECHNICIAN DISPATCH" << endl;
    cout << "==================================================" << endl;

    // ============================================
    // REGISTER TECHNICIANS
    // ============================================

    cout << endl;
    cout << "--------------- REGISTER TECHNICIANS ------------" << endl;

    cout << "Enter number of technicians: ";
    cin >> technicianCount;

    vector<string> technicianName(technicianCount);
    vector<int> technicianLocation(technicianCount);

    for(int i = 0; i < technicianCount; i++)
    {
        technicianName[i] =
            "Technician-" + to_string(i + 1);

        cout << endl;
        cout << technicianName[i] << endl;

        cout << "Available Locations:" << endl;

        for(int j = 1; j <= 21; j++)
        {
            cout << j << ". "
                 << nodeName[j] << endl;
        }

        cout << "Enter current location number: ";
        cin >> technicianLocation[i];

        while(technicianLocation[i] < 1 ||
              technicianLocation[i] > 21)
        {
            cout << "Invalid location!" << endl;
            cout << "Enter again: ";
            cin >> technicianLocation[i];
        }
    }

    // ============================================
    // REPORT CABLE FAULTS
    // ============================================

    cout << endl;
    cout << "--------------- REPORT CABLE FAULTS ------------" << endl;

    cout << "Enter number of faults: ";
    cin >> faultCount;

    vector<int> faultLocation(faultCount);

    for(int i = 0; i < faultCount; i++)
    {
        cout << endl;

        cout << "Fault-" << i + 1 << endl;

        cout << "Available Locations:" << endl;

        for(int j = 1; j <= 21; j++)
        {
            cout << j << ". "
                 << nodeName[j] << endl;
        }

        cout << "Enter fault location number: ";
        cin >> faultLocation[i];

        while(faultLocation[i] < 1 ||
              faultLocation[i] > 21)
        {
            cout << "Invalid location!" << endl;
            cout << "Enter again: ";
            cin >> faultLocation[i];
        }
    }

    // ============================================
    // CREATE DISTANCE MATRIX
    // ============================================

    vector<vector<int>> distanceMatrix(
        technicianCount,
        vector<int>(faultCount)
    );

    for(int i = 0; i < technicianCount; i++)
    {
        for(int j = 0; j < faultCount; j++)
        {
            distanceMatrix[i][j] =
                shortestDistance(
                    technicianLocation[i],
                    faultLocation[j]
                );
        }
    }

    // ============================================
    // SHOW DISTANCE MATRIX
    // ============================================

    cout << endl;
    cout << "==================================================" << endl;
    cout << "              DISTANCE MATRIX" << endl;
    cout << "==================================================" << endl;

    cout << left << setw(18) << "Technician";

    for(int j = 0; j < faultCount; j++)
    {
        string fault =
            "Fault-" + to_string(j + 1);

        cout << setw(12) << fault;
    }

    cout << endl;

    for(int i = 0; i < technicianCount; i++)
    {
        cout << left
             << setw(18)
             << technicianName[i];

        for(int j = 0; j < faultCount; j++)
        {
            cout << setw(12)
                 << distanceMatrix[i][j];
        }

        cout << endl;
    }

    // ============================================
    // GREEDY ASSIGNMENT
    // ============================================

    vector<bool> technicianUsed(
        technicianCount,
        false
    );

    vector<bool> faultAssigned(
        faultCount,
        false
    );

    vector<int> assignedFault(
        technicianCount,
        -1
    );

    vector<int> assignedDistance(
        technicianCount,
        0
    );

    int totalDistance = 0;

    int maxAssignments =
        min(technicianCount, faultCount);

    for(int k = 0; k < maxAssignments; k++)
    {
        int bestTechnician = -1;
        int bestFault = -1;
        int bestDistance = INT_MAX;

        for(int i = 0; i < technicianCount; i++)
        {
            if(technicianUsed[i])
                continue;

            for(int j = 0; j < faultCount; j++)
            {
                if(faultAssigned[j])
                    continue;

                if(distanceMatrix[i][j] < bestDistance)
                {
                    bestDistance =
                        distanceMatrix[i][j];

                    bestTechnician = i;
                    bestFault = j;
                }
            }
        }

        if(bestTechnician == -1)
            break;

        technicianUsed[bestTechnician] = true;
        faultAssigned[bestFault] = true;

        assignedFault[bestTechnician] =
            bestFault;

        assignedDistance[bestTechnician] =
            bestDistance;

        totalDistance += bestDistance;
    }

    // ============================================
    // DISPATCH REPORT
    // ============================================

    cout << endl;
    cout << "==================================================" << endl;
    cout << "              DISPATCH REPORT" << endl;
    cout << "==================================================" << endl;

    for(int i = 0; i < technicianCount; i++)
    {
        cout << endl;

        cout << technicianName[i] << endl;

        cout << "Current Location : "
             << nodeName[technicianLocation[i]]
             << endl;

        if(assignedFault[i] != -1)
        {
            int fault =
                assignedFault[i];

            int distance =
                assignedDistance[i];

            double eta =
                distance / 100.0;

            cout << "Assigned Fault   : Fault-"
                 << fault + 1
                 << endl;

            cout << "Fault Location   : "
                 << nodeName[faultLocation[fault]]
                 << endl;

            cout << "Travel Distance  : "
                 << distance
                 << " meter"
                 << endl;

            cout << fixed
                 << setprecision(2);

            cout << "Estimated ETA    : "
                 << eta
                 << " minutes"
                 << endl;

            cout << "Status            : DISPATCHED"
                 << endl;
        }
        else
        {
            cout << "Assigned Fault   : None"
                 << endl;

            cout << "Status           : AVAILABLE"
                 << endl;
        }

        cout << "--------------------------------------------------"
             << endl;
    }

    // ============================================
    // PENDING FAULTS
    // ============================================

    cout << endl;
    cout << "==================================================" << endl;
    cout << "              PENDING FAULTS" << endl;
    cout << "==================================================" << endl;

    int pendingCount = 0;

    for(int j = 0; j < faultCount; j++)
    {
        if(!faultAssigned[j])
        {
            cout << "Fault-" << j + 1
                 << " : "
                 << nodeName[faultLocation[j]]
                 << " -> PENDING"
                 << endl;

            pendingCount++;
        }
    }

    if(pendingCount == 0)
    {
        cout << "No pending faults." << endl;
    }

    // ============================================
    // FINAL RESULT
    // ============================================

    cout << endl;
    cout << "==================================================" << endl;
    cout << "               FINAL RESULT" << endl;
    cout << "==================================================" << endl;

    cout << "Total Technicians : "
         << technicianCount << endl;

    cout << "Total Faults      : "
         << faultCount << endl;

    cout << "Assigned Faults   : "
         << min(technicianCount, faultCount)
         << endl;

    cout << "Pending Faults    : "
         << pendingCount << endl;

    cout << "Total Travel      : "
         << totalDistance
         << " meter"
         << endl;

    cout << "==================================================" << endl;

    if(pendingCount == 0)
    {
        cout << "STATUS : ALL FAULTS DISPATCHED"
             << endl;
    }
    else
    {
        cout << "STATUS : SOME FAULTS ARE PENDING"
             << endl;
    }

    cout << "==================================================" << endl;
}


// ======================================================
// PROBLEM 5
// ======================================================

// ======================================================
// PROBLEM 5 - CAMPUS CABLE TRAFFIC CONTROLLER
// ======================================================

void Problem5()
{
    while (true)
    {
        cout << endl;

        cout << "==================================================" << endl;
        cout << "       CAMPUS CABLE TRAFFIC CONTROLLER            " << endl;
        cout << "==================================================" << endl;

        cout << endl;

        cout << "1. New Traffic Request" << endl;
        cout << "2. Show Cable Traffic Status" << endl;
        cout << "3. Back to Main Panel" << endl;

        cout << endl;

        cout << "Enter your choice: ";

        int choice;
        cin >> choice;


        // ==================================================
        // OPTION 1 - NEW TRAFFIC REQUEST
        // ==================================================

        if (choice == 1)
        {
            string sourceName;
            string destinationName;
            
            int requiredBandwidth;
            
            
            cout << endl;
            
            cout << "==================================================" << endl;
            cout << "             NEW TRAFFIC REQUEST                  " << endl;
            cout << "==================================================" << endl;
            
            cout << endl;
            
            cout << "Enter source building: ";
            
            cin.ignore();
            getline(cin, sourceName);
            
            
            cout << "Enter destination building: ";
            getline(cin, destinationName);
            
            
            int source =
            findNode(sourceName);
            
            int destination =
            findNode(destinationName);
            
            
            if (source == -1)
            {
                cout << endl;
                cout << "Source building not found!" << endl;
                continue;
            }
            
            
            if (destination == -1)
            {
                cout << endl;
                cout << "Destination building not found!" << endl;
                continue;
            }
            
            
            cout << "Enter required bandwidth (Mbps): ";
            cin >> requiredBandwidth;
            
            
            if (requiredBandwidth <= 0)
            {
                cout << endl;
                cout << "Invalid bandwidth!" << endl;
                continue;
            }
            
            
            // ==================================================
            // FIRST FIND NORMAL SHORTEST PATH
            // ==================================================
            
            vector<int> normalPath;
            
            int normalLength = 0;
            int normalCost = 0;
            
            
            bool normalFound =
            dijkstraShortestPath(
                                 source,
                                 destination,
                                 normalPath,
                                 normalLength,
                                 normalCost
                                 );
            
            
            // ==================================================
            // TRAFFIC-AWARE DIJKSTRA
            // ==================================================
            
            int n =
            nodeName.size() - 1;
            
            const int INF =
            1000000000;
            
            
            vector<vector<pair<int, int>>> graph(n + 1);
            
            
            // --------------------------------------------------
            // ONLY ADD CONNECTIONS WHICH HAVE ENOUGH CAPACITY
            // --------------------------------------------------
            
            for (int i = 0; i < edges.size(); i++)
            {
                int availableCapacity =
                edges[i].capacity -
                edges[i].currentLoad;
                
                
                if (availableCapacity >=
                    requiredBandwidth)
                {
                    graph[edges[i].u].push_back(
                                                {
                                                    edges[i].v,
                                                    edges[i].length
                                                }
                                                );
                    
                    graph[edges[i].v].push_back(
                                                {
                                                    edges[i].u,
                                                    edges[i].length
                                                }
                                                );
                }
            }
            
            
            // ==================================================
            // DIJKSTRA
            // ==================================================
            
            vector<int> distance(
                                 n + 1,
                                 INF
                                 );
            
            vector<int> parentNode(
                                   n + 1,
                                   -1
                                   );
            
            vector<bool> visited(
                                 n + 1,
                                 false
                                 );
            
            
            distance[source] = 0;
            
            
            for (int count = 1;
                 count <= n;
                 count++)
            {
                int current = -1;
                
                
                for (int i = 1;
                     i <= n;
                     i++)
                {
                    if (
                        !visited[i] &&
                        distance[i] != INF
                        )
                    {
                        if (
                            current == -1 ||
                            distance[i] <
                            distance[current]
                            )
                        {
                            current = i;
                        }
                    }
                }
                
                
                if (current == -1)
                {
                    break;
                }
                
                
                visited[current] = true;
                
                
                for (
                     int j = 0;
                     j < graph[current].size();
                     j++
                     )
                {
                    int next =
                    graph[current][j].first;
                    
                    int length =
                    graph[current][j].second;
                    
                    
                    if (
                        distance[current] + length
                        <
                        distance[next]
                        )
                    {
                        distance[next] =
                        distance[current] + length;
                        
                        parentNode[next] =
                        current;
                    }
                }
            }
            
            
            // ==================================================
            // NO AVAILABLE ROUTE
            // ==================================================
            
            if (distance[destination] == INF)
            {
                cout << endl;
                
                cout << "==================================================" << endl;
                cout << "              TRAFFIC RESULT                     " << endl;
                cout << "==================================================" << endl;
                
                cout << endl;
                
                cout << "Source      : "
                << nodeName[source]
                << endl;
                
                cout << "Destination : "
                << nodeName[destination]
                << endl;
                
                cout << "Required    : "
                << requiredBandwidth
                << " Mbps"
                << endl;
                
                cout << endl;
                
                cout << "STATUS : BLOCKED" << endl;
                
                cout << endl;
                
                cout << "No available route has enough bandwidth."
                << endl;
                
                if (normalFound)
                {
                    cout << endl;
                    
                    cout << "Normal shortest route exists, "
                    << "but it cannot handle the required traffic."
                    << endl;
                }
                
                cout << endl;
                
                cout << "==================================================" << endl;
                
                continue;
            }
            
            
            // ==================================================
            // BUILD TRAFFIC-AWARE PATH
            // ==================================================
            
            vector<int> path;
            
            int current =
            destination;
            
            
            while (current != -1)
            {
                path.push_back(current);
                
                current =
                parentNode[current];
            }
            
            
            reverse(
                    path.begin(),
                    path.end()
                    );
            
            
            // ==================================================
            // TOTAL COST
            // ==================================================
            
            int totalLength =
            distance[destination];
            
            int totalCost = 0;
            
            int minimumAvailableCapacity =
            INT_MAX;
            
            
            for (
                 int i = 0;
                 i < path.size() - 1;
                 i++
                 )
            {
                int u =
                path[i];
                
                int v =
                path[i + 1];
                
                
                for (
                     int j = 0;
                     j < edges.size();
                     j++
                     )
                {
                    if (
                        (edges[j].u == u &&
                         edges[j].v == v)
                        
                        ||
                        
                        (edges[j].u == v &&
                         edges[j].v == u)
                        )
                    {
                        totalCost +=
                        edges[j].cost;
                        
                        
                        int available =
                        edges[j].capacity -
                        edges[j].currentLoad;
                        
                        
                        if (
                            available <
                            minimumAvailableCapacity
                            )
                        {
                            minimumAvailableCapacity =
                            available;
                        }
                        
                        
                        break;
                    }
                }
            }
            
            // ==================================================
                        // DETERMINE STATUS
                        // ==================================================

                        double usagePercentage =
                            (
                                (double)requiredBandwidth /
                                minimumAvailableCapacity
                            ) * 100;


                        string status;


                        if (usagePercentage <= 50)
                        {
                            status = "NORMAL";
                        }

                        else
                        {
                            status = "CONGESTED";
                        }


                        // ==================================================
                        // FINAL TRAFFIC RESULT
                        // ==================================================

                        cout << endl;

                        cout << "==================================================" << endl;
                        cout << "              TRAFFIC RESULT                     " << endl;
                        cout << "==================================================" << endl;

                        cout << endl;

                        cout << "Source      : "
                             << nodeName[source]
                             << endl;

                        cout << "Destination : "
                             << nodeName[destination]
                             << endl;

                        cout << "Required    : "
                             << requiredBandwidth
                             << " Mbps"
                             << endl;


                        // --------------------------------------------------
                        // NORMAL SHORTEST ROUTE
                        // --------------------------------------------------

                        if (normalFound)
                        {
                            bool sameRoute =
                                (normalPath.size() ==
                                 path.size());


                            if (sameRoute)
                            {
                                for (
                                    int i = 0;
                                    i < path.size();
                                    i++
                                )
                                {
                                    if (
                                        normalPath[i] !=
                                        path[i]
                                    )
                                    {
                                        sameRoute = false;
                                        break;
                                    }
                                }
                            }


                            if (!sameRoute)
                            {
                                cout << endl;

                                cout << "WARNING:" << endl;

                                cout << "The normal shortest route "
                                     << "cannot handle this traffic."
                                     << endl;

                                cout << "An alternative available route "
                                     << "has been selected."
                                     << endl;
                            }
                        }


                        cout << endl;

                        cout << "--------------------------------------------------" << endl;
                        cout << "             SELECTED ROUTE                      " << endl;
                        cout << "--------------------------------------------------" << endl;

                        cout << endl;


                        for (
                            int i = 0;
                            i < path.size();
                            i++
                        )
                        {
                            cout << nodeName[path[i]];


                            if (
                                i != path.size() - 1
                            )
                            {
                                cout << " -> ";
                            }
                        }


                        cout << endl;

                        cout << endl;

                        cout << "Total Distance : "
                             << totalLength
                             << " meter"
                             << endl;

                        cout << "Total Cost     : Tk. "
                             << totalCost
                             << endl;

                        cout << "Available Bandwidth : "
                             << minimumAvailableCapacity
                             << " Mbps"
                             << endl;

                        cout << "Required Bandwidth  : "
                             << requiredBandwidth
                             << " Mbps"
                             << endl;


                        cout << endl;

                        cout << "STATUS : "
                             << status
                             << endl;


                        if (status == "CONGESTED")
                        {
                            cout << endl;

                            cout << "WARNING: Selected route is "
                                 << "currently highly loaded."
                                 << endl;
                        }

                        else
                        {
                            cout << endl;

                            cout << "The selected route has sufficient "
                                 << "bandwidth."
                                 << endl;
                        }


                        cout << endl;

                        cout << "==================================================" << endl;
                    }


                    // ==================================================
                    // OPTION 2 - SHOW TRAFFIC STATUS
                    // ==================================================

                    else if (choice == 2)
                    {
                        cout << endl;

                        cout << "==================================================" << endl;
                        cout << "             CABLE TRAFFIC STATUS                " << endl;
                        cout << "==================================================" << endl;

                        cout << endl;


                        for (
                            int i = 0;
                            i < edges.size();
                            i++
                        )
                        {
                            int available =
                                edges[i].capacity -
                                edges[i].currentLoad;


                            double usage =
                                (
                                    (double)edges[i].currentLoad /
                                    edges[i].capacity
                                ) * 100;


                            string status;


                            if (usage >= 80)
                            {
                                status = "CONGESTED";
                            }

                            else if (usage >= 50)
                            {
                                status = "MODERATE";
                            }

                            else
                            {
                                status = "NORMAL";
                            }


                            cout << i + 1 << ". "
                                 << nodeName[edges[i].u]
                                 << " -> "
                                 << nodeName[edges[i].v]
                                 << endl;

                            cout << "   Capacity : "
                                 << edges[i].capacity
                                 << " Mbps"
                                 << endl;

                            cout << "   Load     : "
                                 << edges[i].currentLoad
                                 << " Mbps"
                                 << endl;

                            cout << "   Available: "
                                 << available
                                 << " Mbps"
                                 << endl;

                            cout << fixed
                                 << setprecision(1);

                            cout << "   Usage    : "
                                 << usage
                                 << "%"
                                 << endl;

                            cout << "   Status   : "
                                 << status
                                 << endl;

                            cout << endl;
                        }
                    }


                    // ==================================================
                    // OPTION 3 - BACK
                    // ==================================================

                    else if (choice == 3)
                    {
                        return;
                    }


                    // ==================================================
                    // INVALID
                    // ==================================================

                    else
                    {
                        cout << endl;

                        cout << "Invalid choice!"
                             << endl;
                    }
                }
            }


            // ======================================================
            // CHANGE USERNAME
            // ======================================================

            void ChangeUsername(string &username)
            {
                string oldUsername;
                string newUsername;


                cout << endl;

                cout << "Enter old username: ";
                cin >> oldUsername;


                if (oldUsername == username)
                {
                    cout << "Enter new username: ";
                    cin >> newUsername;


                    username =
                        newUsername;


                    cout << endl;
                    cout << "Username changed successfully!" << endl;
                }


                else
                {
                    cout << endl;
                    cout << "Incorrect old username!" << endl;
                }
            }


            // ======================================================
            // CHANGE PASSWORD
            // ======================================================

            void ChangePassword(string &password)
            {
                string oldPassword;
                string newPassword;


                cout << endl;

                cout << "Enter old password: ";
                cin >> oldPassword;


                if (oldPassword == password)
                {
                    cout << "Enter new password: ";
                    cin >> newPassword;


                    password =
                        newPassword;


                    cout << endl;
                    cout << "Password changed successfully!" << endl;
                }


                else
                {
                    cout << endl;
                    cout << "Incorrect old password!" << endl;
                }
            }


            // ======================================================
            // SETTINGS
            // ======================================================



            bool Settings(
                string &username,
                string &password
            )
            {
                while (true)
                {
                    cout << endl;

                    cout << "==================================================" << endl;
                    cout << "                    SETTINGS                      " << endl;
                    cout << "==================================================" << endl;

                    cout << endl;

                    cout << "1. Change Username" << endl;
                    cout << "2. Change Password" << endl;
                    cout << "3. Back to Main Panel" << endl;

                    cout << endl;

                    cout << "Enter your choice: ";

                    int choice;
                    cin >> choice;


                    if (choice == 1)
                    {
                        ChangeUsername(username);

                        return true;
                    }

             
                    else if (choice == 2)
                    {
                        ChangePassword(password);

                        return true;
                    }


                    else if (choice == 3)
                    {
                        return false;
                    }


                    else
                    {
                        cout << endl;
                        cout << "Invalid choice!" << endl;
                    }
                }
            }


            // ======================================================
            // LOGIN
            // ======================================================

            bool Login(
                string &username,
                string &password
            )
            {
                string usernames[4] =
                {
                    "sadid",
                    "sabbir",
                    "bayezid",
                    "jayed"
                };

                string passwords[4] =
                {
                    "252-15-880",
                    "252-15-884",
                    "252-15-892",
                    "252-15-896"
                    
                };


                string inputUser;
                string inputPass;


                for (int i = 0; i < 3; i++)
                {
                    cout << endl;

                    cout << "==================================================" << endl;
                    cout << "                  LOGIN PANEL                    " << endl;
                    cout << "==================================================" << endl;

                    cout << endl;

                    cout << "Username : ";
                    cin >> inputUser;

                    cout << "Password : ";
                    cin >> inputPass;


                    bool valid = false;


                    for (int j = 0; j < 4; j++)
                    {
                        if (
                            inputUser == usernames[j] &&
                            inputPass == passwords[j]
                        )
                        {
                            valid = true;

                            username = inputUser;
                            password = inputPass;

                            break;
                        }
                    }


                    if (valid)
                    {
                        cout << endl;

                        cout << "Login Successful!" << endl;

                        cout << "Welcome to Campus Cable Planner."
                             << endl;

                        return true;
                    }


                    cout << endl;

                    cout << "Invalid username or password!"
                         << endl;


                    if (i < 2)
                    {
                        cout << "Attempts remaining: "
                             << 2 - i
                             << endl;
                    }
                }


                cout << endl;

                cout << "==================================================" << endl;
                cout << "WARNING: Maximum attempts crossed!" << endl;
                cout << "Program locked automatically!" << endl;
                cout << "==================================================" << endl;


                return false;
            }

            // ======================================================
            // MAIN PROJECT PANEL
            // ======================================================

            void MainProjectPanel(
                string &username,
                string &password
            )
            {
                while (true)
                {
                    cout << endl;

                    cout << "==================================================" << endl;
                    cout << "             CAMPUS CABLE PLANNER                " << endl;
                    cout << "==================================================" << endl;

                    cout << endl;

                    cout << "                  PROJECT MENU                   " << endl;

                    cout << endl;

                    cout << "1. Problem 1 - Cable Search System." << endl;
                    cout << "2. Problem 2 - Shortest Path Finder." << endl;
                    cout << "3. Problem 3 - Cable Failure & Backup Planner." << endl;
                    cout << "4. Problem 4 - IT Technician Dispatch System." << endl;
                    cout << "5. Problem 5 - Campus Cable Traffic Controller." << endl;

                    cout << endl;

                    cout << "6. Modify Data" << endl;
                    cout << "7. Settings" << endl;
                    cout << "8. Exit Program" << endl;

                    cout << endl;

                    cout << "Enter your choice: ";

                    int choice;
                    cin >> choice;


                    switch (choice)
                    {
                        case 1:

                            Problem1();

                            break;


                        case 2:

                            Problem2();

                            break;


                        case 3:

                            Problem3();

                            break;


                        case 4:

                            Problem4();

                            break;


                        case 5:

                            Problem5();

                            break;


                        case 6:

                            ModifyData();

                            break;


                        case 7:
                        {
                            bool changed =
                                Settings(
                                    username,
                                    password
                                );


                            if (changed)
                            {
                                cout << endl;

                                cout << "==================================================" << endl;
                                cout << "       Account information changed!              " << endl;
                                cout << "       Please login again.                        " << endl;
                                cout << "==================================================" << endl;


                                if (!Login(
                                        username,
                                        password
                                    ))
                                {
                                    return;
                                }
                            }

                            break;
                        }


                        case 8:

                            cout << endl;

                            cout << "==================================================" << endl;
                            cout << "      Thank you for using Campus Cable Planner!  " << endl;
                            cout << "==================================================" << endl;

                            return;


                        default:

                            cout << endl;

                            cout << "Invalid choice! Please try again."
                                 << endl;
                    }
                }
            }
            // ======================================================
            // MAIN FUNCTION
            // ======================================================

            int main()
            {
                string username;
                string password;


                // --------------------------------------------------
                // LOGIN
                // --------------------------------------------------

                bool loggedIn =
                    Login(
                        username,
                        password
                    );


                if (!loggedIn)
                {
                    return 0;
                }


                // --------------------------------------------------
                // MAIN PROJECT PANEL
                // --------------------------------------------------

                MainProjectPanel(
                    username,
                    password
                );


                return 0;
            }
