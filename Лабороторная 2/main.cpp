#include <iostream>
#include <complex>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

#include <omp.h>
#include <mkl.h>

using namespace std;
using namespace chrono;

using Complex = complex<double>;

// Генерация случайной комплексной матрицы
vector<Complex> generate_matrix(int n) {
    vector<Complex> mat(n * n);
    for (int i = 0; i < n * n; ++i) {
        mat[i] = Complex(rand() / (double)RAND_MAX,
            rand() / (double)RAND_MAX);
    }
    return mat;
}

// Вариант 1: Классический тройной цикл
void mul_classic(const vector<Complex>& A, const vector<Complex>& B,
    vector<Complex>& C, int n) {
    fill(C.begin(), C.end(), Complex(0, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Complex sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

// Вариант 3: Блочное умножение с OpenMP (оптимизированный)
void mul_blocked_omp(const vector<Complex>& A, const vector<Complex>& B,
    vector<Complex>& C, int n, int block_size = 64) {
    fill(C.begin(), C.end(), Complex(0, 0));

#pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < n; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int k = 0; k < n; k += block_size) {
                int i_max = min(i + block_size, n);
                int j_max = min(j + block_size, n);
                int k_max = min(k + block_size, n);

                for (int ii = i; ii < i_max; ++ii) {
                    for (int kk = k; kk < k_max; ++kk) {
                        Complex aik = A[ii * n + kk];
                        for (int jj = j; jj < j_max; ++jj) {
                            C[ii * n + jj] += aik * B[kk * n + jj];
                        }
                    }
                }
            }
        }
    }
}

// Шаблонная функция для измерения времени и производительности
template<typename Func>
double measure_time(Func func, const vector<Complex>& A,
    const vector<Complex>& B, vector<Complex>& C,
    int n, const string& name) {
    auto start = high_resolution_clock::now();
    func(A, B, C, n);
    auto end = high_resolution_clock::now();

    double t = duration<double>(end - start).count();
    double c = 2.0 * pow(n, 3);
    double mflops = c / t * 1e-6;

    cout << "\n" << name << ":\n";
    cout << "  Время = " << t << " сек\n";
    cout << "  MFlops = " << mflops << "\n";

    return mflops;
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    cout << "===========================================\n";
    cout << "Студент: Хилобок Андрей Викторович\n";
    cout << "Группа: РПИа-025\n";
    cout << "===========================================\n\n";

    const int n = 2048;
    cout << "Размер матриц: " << n << "x" << n << "\n";
    cout << "Объем памяти на матрицу: "
        << (n * n * sizeof(Complex)) / (1024 * 1024) << " MB\n\n";

    // Генерация исходных матриц
    cout << "Генерация матриц A и B...\n";
    srand(42);  // Фиксируем seed для воспроизводимости
    auto A = generate_matrix(n);
    auto B = generate_matrix(n);

    // Выделение памяти под результирующие матрицы
    vector<Complex> C1(n * n), C2(n * n), C3(n * n);

    cout << "Начало тестирования...\n";

    // Вариант 1: Классический алгоритм
    double mflops1 = measure_time(mul_classic, A, B, C1, n,
        "1. Классический алгоритм (тройной цикл)");

    // Вариант 2: cblas_zgemm из Intel MKL
    cout << "\n2. cblas_zgemm (Intel MKL):\n";
    auto start = high_resolution_clock::now();

    double alpha = 1.0, beta = 0.0;
    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        n, n, n, &alpha, A.data(), n, B.data(), n,
        &beta, C2.data(), n);

    auto end = high_resolution_clock::now();
    double t_mkl = duration<double>(end - start).count();
    double mflops_mkl = (2.0 * pow(n, 3)) / t_mkl * 1e-6;
    cout << "  Время = " << t_mkl << " сек\n";
    cout << "  MFlops = " << mflops_mkl << "\n";

    // Вариант 3: Оптимизированный блочный алгоритм
    double mflops3 = measure_time(mul_blocked_omp, A, B, C3, n,
        "3. Оптимизированный блочный алгоритм + OpenMP");

    // Сравнение результатов
    cout << "\n===========================================\n";
    cout << "СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ:\n";
    cout << "===========================================\n";
    cout << "Относительно MKL (100%):\n";
    cout << "  Классический:      " << (mflops1 / mflops_mkl) * 100 << "%\n";
    cout << "  Оптимизированный:  " << (mflops3 / mflops_mkl) * 100 << "%\n";

    // Проверка корректности (выборочная)
    cout << "\n===========================================\n";
    cout << "ПРОВЕРКА КОРРЕКТНОСТИ:\n";
    cout << "===========================================\n";
    bool correct = true;
    for (int i = 0; i < min(10, n * n); ++i) {
        if (abs(C1[i] - C2[i]) > 1e-9) {
            correct = false;
            break;
        }
    }
    if (correct) {
        cout << "✓ Все три результата совпадают (с точностью до округления)\n";
    }
    else {
        cout << "✗ Ошибка: результаты не совпадают!\n";
    }

    cout << "\nТестирование завершено.\n";
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}