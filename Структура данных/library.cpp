#include <iostream>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "Автор: Хилобок Андрей Викторович\n";
    cout << "Группа: РПИа-025\n\n";
    int N;
    cin >> N;
    int r = 0;
    string answer = "";
    for (int i = 1; i <= N; i++) {
        r = (r * 10 + 1) % N;
        answer.push_back('1');

        if (r == 0) {
            cout << answer << "\n";
            return 0;
        }
    }
    cout << "NO\n";
    return 0;
}
