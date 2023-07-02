#include <stdio.h>
#include <omp.h>
#include <string.h>

//gcc -fopenmp hash.c -o hash

#define NUM_THREADS 4
#define MAX_PASSWORD_LENGTH 100

void compute_hash(const char* password, unsigned int* hash) {
    int sum = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        sum += password[i];
    }
    *hash = (unsigned char)sum;
}

int main() {
    const char* passwords[] = { "password1", "password2", "password3", "password4" };
    int num_passwords = sizeof(passwords) / sizeof(passwords[0]);

    // Устанавливаем количество потоков
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        // Вычисляем хэш для каждого пароля
        #pragma omp for
        for (int i = 0; i < num_passwords; i++) {
            unsigned int hash;
            compute_hash(passwords[i], &hash);

            // Выводим результат
            printf("Thread %d: Password '%s' - Hash: ", tid, passwords[i]);
            
            printf("%d", hash);
            
            printf("\n");
        }
    }

    return 0;
}
