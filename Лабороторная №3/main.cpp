#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;

map<char, vector<char>> graph;
map<char, int> indegree;
set<char> nodes;

bool hasCycleUtil(char v, map<char, bool>& visited, map<char, bool>& recStack) {
    visited[v] = true;
    recStack[v] = true;

    for (char u : graph[v]) {
        if (!visited[u] && hasCycleUtil(u, visited, recStack))
            return true;
        else if (recStack[u])
            return true;
    }

    recStack[v] = false;
    return false;
}

bool hasCycle() {
    map<char, bool> visited, recStack;

    for (char node : nodes) {
        if (!visited[node]) {
            if (hasCycleUtil(node, visited, recStack))
                return true;
        }
    }
    return false;
}

bool isFullOrder() {
    queue<char> q;
    map<char, int> tempIndegree = indegree;
    vector<char> result;

    for (char node : nodes) {
        if (tempIndegree[node] == 0)
            q.push(node);
    }

    while (!q.empty()) {
        char v = q.front();
        q.pop();
        result.push_back(v);

        for (char u : graph[v]) {
            if (--tempIndegree[u] == 0)
                q.push(u);
        }
    }

    return result.size() == nodes.size();
}

int main() {
    cout << "Автор: Хилобок Андрей Викторович, РПИА-025\n\n";

    int n;
    cout << "Введите количество пар: ";
    cin >> n;

    cout << "Введите пары (например: a b):\n";

    for (int i = 0; i < n; i++) {
        char a, b;
        cin >> a >> b;

        graph[a].push_back(b);
        indegree[b]++;
        nodes.insert(a);
        nodes.insert(b);

        if (!indegree.count(a))
            indegree[a] = 0;
    }

    if (hasCycle()) {
        cout << "Последовательность ПРОТИВОРЕЧИВАЯ\n";
    } else if (isFullOrder()) {
        cout << "Последовательность ПОЛНАЯ\n";
    } else {
        cout << "Последовательность НЕПОЛНАЯ\n";
    }

    return 0;
}