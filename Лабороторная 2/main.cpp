#include <iostream>
#include <complex>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>
#include <algorithm>
#include <random>

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
    mt19937 gen(42);
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < n * n; ++i) {
        mat[i] = Complex(dist(gen), dist(gen));
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

// Вариант 3: Блочное умножение + OpenMP
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

// Замер времени
template<typename Func>
double measure_time(Func func,
    const vector<Complex>& A,
    const vector<Complex>& B,
    vector<Complex>& C,
    int n,
    const string& name) {

    auto start = high_resolution_clock::now();
    func(A, B, C, n);
    auto end = high_resolution_clock::now();

    double t = duration<double>(end - start).count();
    double c = 2.0 * n * n * n;   // без pow
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

    omp_set_num_threads(omp_get_max_threads());

    const int n = 2048;

    cout << "Размер матриц: " << n << "x" << n << "\n";
    cout << "Память на матрицу: "
        << (n * n * sizeof(Complex)) / (1024 * 1024)
        << " MB\n\n";

    cout << "Генерация матриц...\n";
    auto A = generate_matrix(n);
    auto B = generate_matrix(n);

    vector<Complex> C1(n * n), C2(n * n), C3(n * n);

    cout << "Старт тестов...\n";

    // 1. Классический
    double mflops1 = measure_time(mul_classic, A, B, C1, n,
        "1. Классический алгоритм");

    // 2. MKL
    cout << "\n2. cblas_zgemm (MKL):\n";

    Complex alpha(1.0, 0.0);
    Complex beta(0.0, 0.0);

    auto start = high_resolution_clock::now();

    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        n, n, n,
        &alpha,
        A.data(), n,
        B.data(), n,
        &beta,
        C2.data(), n);

    auto end = high_resolution_clock::now();

    double t_mkl = duration<double>(end - start).count();
    double mflops_mkl = (2.0 * n * n * n) / t_mkl * 1e-6;

    cout << "  Время = " << t_mkl << " сек\n";
    cout << "  MFlops = " << mflops_mkl << "\n";

    // 3. Оптимизированный
    double mflops3 = measure_time(mul_blocked_omp, A, B, C3, n,
        "3. Блочный + OpenMP");

    // Сравнение
    cout << "\nСРАВНЕНИЕ (MKL = 100%):\n";
    cout << "Классический: " << (mflops1 / mflops_mkl) * 100 << "%\n";
    cout << "Оптимизированный: " << (mflops3 / mflops_mkl) * 100 << "%\n";

    // Полная проверка
    cout << "\nПРОВЕРКА КОРРЕКТНОСТИ:\n";

    double max_error = 0;

    for (int i = 0; i < n * n; ++i) {
        double err = abs(C1[i] - C2[i]);
        if (err > max_error) max_error = err;
    }

    cout << "Максимальная ошибка: " << max_error << "\n";

    if (max_error < 1e-9)
        cout << "✓ Результаты совпадают\n";
    else
        cout << "✗ Есть расхождения!\n";

    return 0;
}
