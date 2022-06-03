/*
 Запрограмувати обидві реалізації 23 варіанту (а, б) та 22 варіанту (зі включеним та відключеним CPU cache) і
 порівняти час виконання в обох випадках.

22.  Потоки. Паралелізм. Помножити матриці A[n x m] та B[m x k].
 Для обчислення створити (динамічно, автоматично) n*k [однотипних] потоків для обчислень (множення векторів).
 Передбачити введення матриці з клавіатури або авто-генерування та виведення результату обчислень “по ходу обчислень”.
 Продемонструвати непослідовність обчислень елементів матриці-добутку.



23.  Взаємодія потоків, критичний сегмент.
 Промоделювати паралельну роботу двох потоків (threads) з загальною коміркою пам’яті:
 а) з використанням критичного сегменту,
 б) без використання критичного сегменту.
 Продемонструвати різницю. (Наприклад, збільшувати значення спільної комірки на 1 1000 разів в кожному потоці.)

 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <fstream>

uint64_t a = 0;
bool useCrit = false;
std::mutex a_mutex;

using Matrix = std::vector<std::vector<int>>;

Matrix multiply(int n, int m, int k, const Matrix &matA, const Matrix &matB);

void initMatrix(int N, int M, Matrix &matrix);

void printMatrix(int N, int M, Matrix matrix);


void task1() {
    for (int i = 0; i < 50000000; ++i) {
        if (useCrit) {
            std::lock_guard<std::mutex> guard(a_mutex);
            a++;
        } else {
            a++;
        }
    }
}


int main(int argc, char **argv) {
    if (argc > 1) {
        useCrit = true;
    }

//    auto start = std::chrono::steady_clock::now();
//    std::thread thread_1(task1);
//    std::thread thread_2(task1);
//
//    // do other stuff
//    thread_2.join();
//    thread_1.join();
//    auto end = std::chrono::steady_clock::now();
//
//    printf("a=%lu\n", a);
//    printf("elapsed time= %ld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());


    int n = 100, m = 100, k = 100;
    std::vector<std::vector<int>> mA, mB;
    initMatrix(n, m, mA);
    initMatrix(m, k, mB);

    std::ofstream out("out.txt");
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    Matrix mul = multiply(n, m, k, mA, mB);
    return 0;
}

void initMatrix(int n, int m, Matrix &matrix) {
    matrix.resize(n);
    for (int i = 0; i < n; ++i) {
        matrix[i].resize(m);
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = i + j;
        }
    }
}

void printMatrix(int N, int M, Matrix matrix) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void threadJob(Matrix &result, const Matrix &matA, const Matrix &matB,
               int i, int m, int j) {
    for (int l = 0; l < m; ++l) {
        result[i][j] += matA[i][l] * matB[l][j];
    }
}

Matrix multiply(int n, int m, int k, const Matrix &matA, const Matrix &matB) {
    Matrix result;
    result.resize(n);
    for (int i = 0; i < k; ++i) {
        result[i].resize(k);
    }

    std::vector<std::thread> threads;
    threads.reserve(n * k);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            threads.emplace_back(threadJob, ::std::ref(result), matA, matB, i, m, j);
        }
        printMatrix(n, m, result);
    }

    for (auto &thr : threads) {
        thr.join();
    }

    return result;
}

