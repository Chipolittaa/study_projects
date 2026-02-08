#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

void print_binary(unsigned long long num) {
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
        printf("%lld", (num >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

unsigned long long reverse_bits(unsigned long long num) {
    unsigned long long reversed = 0;
    int num_bits = sizeof(num) * 8;

    for (int i = 0; i < num_bits; i++) {
        reversed = (reversed << 1) | (num & 1);
        num >>= 1;
    }

    return reversed;
}

int main(int argc, char *argv[]) {
    unsigned long long num;
    char *endptr;

    if (argc > 1) {
        errno = 0;
        num = strtoull(argv[1], &endptr, 10);
        if (*endptr != '\0' || errno != 0) {
            fprintf(stderr, "Ошибка: '%s' не является числом.\n", argv[1]);
            return 1;
        }
    } else {
        srand((unsigned) time(NULL));
        num = rand();
    }

    printf("Исходное число в двоичном формате:\n");
    print_binary(num);

    num = reverse_bits(num);

    printf("Число после разворота битов в двоичном формате:\n");
    print_binary(num);

    return 0;
}
