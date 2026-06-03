#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
void printAuthor()
{
    cout << "ФИО: Хилобок Андрей Викторович" << endl;
    cout << "Группа: РПИа-о25" << endl;
    cout << endl;
}
void arrayImplementation()
{
    cout << "===== РЕАЛИЗАЦИЯ ЧЕРЕЗ МАССИВ =====" << endl;
    int n;
    cout << "Введите количество пар: ";
    cin >> n;
    int matrix[26][26] = {0};
    vector<char> symbols;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        char a, b;
        cout << "Введите пару: ";
        cin >> a >> b;
        matrix[a - 'a'][b - 'a'] = 1;
        bool foundA = false;
        bool foundB = false;
        for (char c : symbols)
        {
            if (c == a)
                foundA = true;

            if (c == b)
                foundB = true;
        }
        if (!foundA)
            symbols.push_back(a);

        if (!foundB)
            symbols.push_back(b);
    }
    // Транзитивность
    for (int k = 0; k < 26; k++)
    {
        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (matrix[i][k] && matrix[k][j])
                {
                    matrix[i][j] = 1;
                }
            }
        }
    }
    bool contradiction = false;
    for (int i = 0; i < 26; i++)
    {
        if (matrix[i][i])
        {
            contradiction = true;
        }
        for (int j = 0; j < 26; j++)
        {
            if (matrix[i][j] && matrix[j][i] && i != j)
            {
                contradiction = true;
            }
        }
    }
    bool complete = true;
    for (int i = 0; i < symbols.size(); i++)
    {
        for (int j = i + 1; j < symbols.size(); j++)
        {
            int a = symbols[i] - 'a';
            int b = symbols[j] - 'a';

            if (!matrix[a][b] && !matrix[b][a])
            {
                complete = false;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    if (contradiction)
    {
        cout << "Последовательность противоречива" << endl;
    }
    else
    {
        cout << "Противоречий нет" << endl;
    }
    if (complete)
    {
        cout << "Последовательность полная" << endl;
    }
    else
    {
        cout << "Последовательность не полная" << endl;
    }
    cout << "Время работы: " << duration.count() << " мкс" << endl;
    cout << endl;
}

struct Node
{
    char from;
    char to;
    Node* next;
};
void linkedListImplementation()
{
    cout << "===== РЕАЛИЗАЦИЯ ЧЕРЕЗ СВЯЗАННЫЙ СПИСОК =====" << endl;
    int n;
    cout << "Введите количество пар: ";
    cin >> n;
    Node* head = nullptr;
    vector<char> symbols;
    int matrix[26][26] = {0};
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        char a, b;
        cout << "Введите пару: ";
        cin >> a >> b;
        Node* newNode = new Node;
        newNode->from = a;
        newNode->to = b;
        newNode->next = head;
        head = newNode;
        matrix[a - 'a'][b - 'a'] = 1;
        bool foundA = false;
        bool foundB = false;
        for (char c : symbols)
        {
            if (c == a)
                foundA = true;

            if (c == b)
                foundB = true;
        }
        if (!foundA)
            symbols.push_back(a);

        if (!foundB)
            symbols.push_back(b);
    }
    for (int k = 0; k < 26; k++)
    {
        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (matrix[i][k] && matrix[k][j])
                {
                    matrix[i][j] = 1;
                }
            }
        }
    }
    bool contradiction = false;
    for (int i = 0; i < 26; i++)
    {
        if (matrix[i][i])
        {
            contradiction = true;
        }
        for (int j = 0; j < 26; j++)
        {
            if (matrix[i][j] && matrix[j][i] && i != j)
            {
                contradiction = true;
            }
        }
    }
    bool complete = true;
    for (int i = 0; i < symbols.size(); i++)
    {
        for (int j = i + 1; j < symbols.size(); j++)
        {
            int a = symbols[i] - 'a';
            int b = symbols[j] - 'a';

            if (!matrix[a][b] && !matrix[b][a])
            {
                complete = false;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    if (contradiction)
    {
        cout << "Последовательность противоречива" << endl;
    }
    else
    {
        cout << "Противоречий нет" << endl;
    }
    if (complete)
    {
        cout << "Последовательность полная" << endl;
    }
    else
    {
        cout << "Последовательность не полная" << endl;
    }
    cout << "Время работы: " << duration.count() << " мкс" << endl;
    cout << endl;
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void stlImplementation()
{
    cout << "===== РЕАЛИЗАЦИЯ ЧЕРЕЗ STL =====" << endl;
    int n;
    cout << "Введите количество пар: ";
    cin >> n;
    vector<pair<char, char>> pairs;
    int matrix[26][26] = {0};
    vector<char> symbols;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        char a, b;
        cout << "Введите пару: ";
        cin >> a >> b;
        pairs.push_back(make_pair(a, b));
        matrix[a - 'a'][b - 'a'] = 1;
        bool foundA = false;
        bool foundB = false;
        for (char c : symbols)
        {
            if (c == a)
                foundA = true;

            if (c == b)
                foundB = true;
        }
        if (!foundA)
            symbols.push_back(a);

        if (!foundB)
            symbols.push_back(b);
    }
    for (int k = 0; k < 26; k++)
    {
        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (matrix[i][k] && matrix[k][j])
                {
                    matrix[i][j] = 1;
                }
            }
        }
    }
    bool contradiction = false;
    for (int i = 0; i < 26; i++)
    {
        if (matrix[i][i])
        {
            contradiction = true;
        }

        for (int j = 0; j < 26; j++)
        {
            if (matrix[i][j] && matrix[j][i] && i != j)
            {
                contradiction = true;
            }
        }
    }
    bool complete = true;
    for (int i = 0; i < symbols.size(); i++)
    {
        for (int j = i + 1; j < symbols.size(); j++)
        {
            int a = symbols[i] - 'a';
            int b = symbols[j] - 'a';

            if (!matrix[a][b] && !matrix[b][a])
            {
                complete = false;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    if (contradiction)
    {
        cout << "Последовательность противоречива" << endl;
    }
    else
    {
        cout << "Противоречий нет" << endl;
    }
    if (complete)
    {
        cout << "Последовательность полная" << endl;
    }
    else
    {
        cout << "Последовательность не полная" << endl;
    }
    cout << "Время работы: " << duration.count() << " мкс" << endl;
    cout << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    printAuthor();
    arrayImplementation();
    linkedListImplementation();
    stlImplementation();
    return 0;
}