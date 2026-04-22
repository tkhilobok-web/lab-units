#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <chrono>

using namespace std;
using namespace chrono;

// ====================== ОБЩИЕ ФУНКЦИИ ======================

bool hasCycleDFS(char v, map<char, bool>& visited, map<char, bool>& recStack,
                 map<char, vector<char>>& graph) {
    visited[v] = true;
    recStack[v] = true;

    for (char u : graph[v]) {
        if (!visited[u] && hasCycleDFS(u, visited, recStack, graph))
            return true;
        else if (recStack[u])
            return true;
    }

    recStack[v] = false;
    return false;
}

bool hasCycle(map<char, vector<char>>& graph, set<char>& nodes) {
    map<char, bool> visited, recStack;

    for (char node : nodes) {
        if (!visited[node]) {
            if (hasCycleDFS(node, visited, recStack, graph))
                return true;
        }
    }
    return false;
}

bool isFullOrder(map<char, vector<char>>& graph, map<char, int>& indegree, set<char>& nodes) {
    queue<char> q;
    map<char, int> temp = indegree;
    vector<char> result;

    for (char node : nodes) {
        if (temp[node] == 0)
            q.push(node);
    }

    while (!q.empty()) {
        char v = q.front();
        q.pop();
        result.push_back(v);

        for (char u : graph[v]) {
            if (--temp[u] == 0)
                q.push(u);
        }
    }

    return result.size() == nodes.size();
}

// ====================== 1. STL ======================

void solveSTL(vector<pair<char,char>> input) {
    map<char, vector<char>> graph;
    map<char, int> indegree;
    set<char> nodes;

    for (auto p : input) {
        graph[p.first].push_back(p.second);
        indegree[p.second]++;
        nodes.insert(p.first);
        nodes.insert(p.second);

        if (!indegree.count(p.first))
            indegree[p.first] = 0;
    }

    if (hasCycle(graph, nodes))
        cout << "[STL] ПРОТИВОРЕЧИВАЯ\n";
    else if (isFullOrder(graph, indegree, nodes))
        cout << "[STL] ПОЛНАЯ\n";
    else
        cout << "[STL] НЕПОЛНАЯ\n";
}

// ====================== 2. МАССИВ ======================

void solveArray(vector<pair<char,char>> input) {
    const int MAX = 256;
    int matrix[MAX][MAX] = {0};
    bool used[MAX] = {false};

    for (auto p : input) {
        matrix[p.first][p.second] = 1;
        used[p.first] = used[p.second] = true;
    }

    // Флойд для транзитивности
    for (int k = 0; k < MAX; k++)
        for (int i = 0; i < MAX; i++)
            for (int j = 0; j < MAX; j++)
                if (matrix[i][k] && matrix[k][j])
                    matrix[i][j] = 1;

    // проверка цикла
    for (int i = 0; i < MAX; i++) {
        if (matrix[i][i]) {
            cout << "[ARRAY] ПРОТИВОРЕЧИВАЯ\n";
            return;
        }
    }

    // проверка полноты
    vector<int> nodes;
    for (int i = 0; i < MAX; i++)
        if (used[i]) nodes.push_back(i);

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            int a = nodes[i], b = nodes[j];
            if (!matrix[a][b] && !matrix[b][a]) {
                cout << "[ARRAY] НЕПОЛНАЯ\n";
                return;
            }
        }
    }

    cout << "[ARRAY] ПОЛНАЯ\n";
}

// ====================== 3. СПИСОК ======================

void solveList(vector<pair<char,char>> input) {
    map<char, list<char>> graph;
    map<char, int> indegree;
    set<char> nodes;

    for (auto p : input) {
        graph[p.first].push_back(p.second);
        indegree[p.second]++;
        nodes.insert(p.first);
        nodes.insert(p.second);

        if (!indegree.count(p.first))
            indegree[p.first] = 0;
    }

    // преобразуем в vector для использования DFS
    map<char, vector<char>> tempGraph;
    for (auto& g : graph) {
        tempGraph[g.first] = vector<char>(g.second.begin(), g.second.end());
    }

    if (hasCycle(tempGraph, nodes))
        cout << "[LIST] ПРОТИВОРЕЧИВАЯ\n";
    else if (isFullOrder(tempGraph, indegree, nodes))
        cout << "[LIST] ПОЛНАЯ\n";
    else
        cout << "[LIST] НЕПОЛНАЯ\n";
}

// ====================== MAIN ======================

int main() {
    cout << "Автор: Хилобок Андрей Викторович, РПИА-025\n\n";

    int n;
    cout << "Введите количество пар: ";
    cin >> n;

    vector<pair<char,char>> input;

    cout << "Введите пары:\n";
    for (int i = 0; i < n; i++) {
        char a, b;
        cin >> a >> b;
        input.push_back({a, b});
    }

    auto start = high_resolution_clock::now();
    solveSTL(input);
    auto end = high_resolution_clock::now();
    cout << "Время STL: "
         << duration_cast<microseconds>(end - start).count()
         << " мкс\n\n";

    start = high_resolution_clock::now();
    solveArray(input);
    end = high_resolution_clock::now();
    cout << "Время ARRAY: "
         << duration_cast<microseconds>(end - start).count()
         << " мкс\n\n";

    start = high_resolution_clock::now();
    solveList(input);
    end = high_resolution_clock::now();
    cout << "Время LIST: "
         << duration_cast<microseconds>(end - start).count()
         << " мкс\n";

    return 0;
}