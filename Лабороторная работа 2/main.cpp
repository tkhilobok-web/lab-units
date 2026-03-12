#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <Accelerate/Accelerate.h>

using namespace std;

typedef complex<double> cd;

const int N = 2048;
const int BLOCK = 64;

vector<cd> A(N*N);
vector<cd> B(N*N);
vector<cd> C(N*N);

void generate_matrix(vector<cd>& M)
{
    for(int i = 0; i < N*N; i++)
        M[i] = cd(rand()%10, rand()%10);
}

void multiply_naive()
{
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
        {
            cd sum = 0;
            for(int k = 0; k < N; k++)
                sum += A[i*N + k] * B[k*N + j];

            C[i*N + j] = sum;
        }
}

void multiply_block()
{
    for(int ii = 0; ii < N; ii += BLOCK)
        for(int jj = 0; jj < N; jj += BLOCK)
            for(int kk = 0; kk < N; kk += BLOCK)

                for(int i = ii; i < min(ii + BLOCK, N); i++)
                    for(int j = jj; j < min(jj + BLOCK, N); j++)
                    {
                        cd sum = C[i*N + j];

                        for(int k = kk; k < min(kk + BLOCK, N); k++)
                            sum += A[i*N + k] * B[k*N + j];

                        C[i*N + j] = sum;
                    }
}

void multiply_blas()
{
    cd alpha = 1.0;
    cd beta = 0.0;

    cblas_zgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        N,
        N,
        N,
        &alpha,
        A.data(),
        N,
        B.data(),
        N,
        &beta,
        C.data(),
        N
    );
}

double measure(void (*func)())
{
    auto start = chrono::high_resolution_clock::now();

    func();

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;

    return diff.count();
}

int main()
{
    cout<<"Автор: Хилобок Андрей Викторович\n";
    cout<<"Группа: РПИа-025\n\n";

    generate_matrix(A);
    generate_matrix(B);

    fill(C.begin(), C.end(), cd(0,0));

    double t1 = measure(multiply_naive);

    fill(C.begin(), C.end(), cd(0,0));

    double t2 = measure(multiply_blas);

    fill(C.begin(), C.end(), cd(0,0));

    double t3 = measure(multiply_block);

    double c = 2.0 * N * N * N;

    double p1 = c / t1 * 1e-6;
    double p2 = c / t2 * 1e-6;
    double p3 = c / t3 * 1e-6;

    cout<<"Naive multiplication\n";
    cout<<"Time: "<<t1<<" sec\n";
    cout<<"MFLOPS: "<<p1<<"\n\n";

    cout<<"BLAS multiplication\n";
    cout<<"Time: "<<t2<<" sec\n";
    cout<<"MFLOPS: "<<p2<<"\n\n";

    cout<<"Blocked multiplication\n";
    cout<<"Time: "<<t3<<" sec\n";
    cout<<"MFLOPS: "<<p3<<"\n";

}