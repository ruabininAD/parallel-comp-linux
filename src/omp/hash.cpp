#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

// Вспомогательная функция для вычисления хэша пароля
std::string computeHash(const std::string& password)
{
    // Пример простого алгоритма хэширования
    // В данном случае используется простое суммирование кодов символов

    // Инициализация суммы хэша
    int hash = 0;

    // Проход по символам пароля и суммирование их кодов
    for (char c : password) {
        hash += static_cast<int>(c);
    }

    // Преобразование суммы хэша в строку и возвращение результата
    return std::to_string(hash);
}

int main()
{
    // Входные пароли
    std::vector<std::string> passwords = {"password1", "password2", "password3", "password4"};

    // Количество потоков, которые будут использоваться для вычисления хэша
    int numThreads = 4;

    // Установка количества потоков для OpenMP
    omp_set_num_threads(numThreads);

    // Вектор для хранения хэшей паролей
    std::vector<std::string> hashes(passwords.size());

    // Параллельный цикл для вычисления хэшей паролей
    #pragma omp parallel for
    for (int i = 0; i < passwords.size(); ++i) {
        // Получение идентификатора текущего потока
        int threadId = omp_get_thread_num();

        // Вычисление хэша пароля и сохранение результата в векторе хэшей
        hashes[i] = computeHash(passwords[i]);

        // Вывод результата для каждого потока
        #pragma omp critical
        {
            std::cout << "Thread " << threadId << " processed password: " << passwords[i] << std::endl;
        }
    }

    // Вывод хэшей паролей
    std::cout << "Hashes:" << std::endl;
    for (const auto& hash : hashes) {
        std::cout << hash << std::endl;
    }

    return 0;
}
