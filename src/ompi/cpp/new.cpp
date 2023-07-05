
#include <iostream>
#include <cmath>
#include <mpi.h>

// Глобальная переменная, доступная всем процессам
double globalVariable = 0.0;

// Функция, которая будет выполняться параллельно
void computeFunction(int functionId, int rank) {
    // Вычисление функции на каждом процессе
    double result = 0.0;
    switch (functionId) {
        case 1:
            result = std::sin(globalVariable);
            break;
        case 2:
            result = std::cos(globalVariable);
            break;
        case 3:
            result = std::tan(globalVariable);
            break;
    }

    // Вывод результата и ранга процесса
    std::cout << "Function " << functionId << " on process " << rank << " computed: " << result << std::endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int numProcesses, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Каждый процесс получает свою уникальную глобальную переменную
    globalVariable = 1.0 + rank;

    // Вычисление функций на каждом процессе
    for (int functionId = 1; functionId <= 3; ++functionId) {
        computeFunction(functionId, rank);
    }

    MPI_Finalize();
    return 0;
}