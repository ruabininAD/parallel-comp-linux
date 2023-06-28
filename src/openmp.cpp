#include <iostream>
#include <omp.h>

int main() {
    // Установка количества потоков
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        // Получение идентификатора потока
        int thread_id = omp_get_thread_num();

        // Вывод сообщения с использованием идентификатора потока
        std::cout << "Hello from thread " << thread_id << std::endl;

        // Параллельный цикл
        #pragma omp for
        for (int i = 0; i < 10; ++i) {
            std::cout << "Thread " << thread_id << ": Iteration " << i << std::endl;
        }
    }

    return 0;
}
