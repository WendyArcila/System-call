

#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[]) {

     // Verifica que se proporcionen suficientes argumentos
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <number> <username>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Input values
    char *endptr;
    int number = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0'){
        fprintf(stderr, "Uso: %s <number> <username>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *username = argv[2];
    size_t username_length = strlen(username) + 1;

    char dest_buffer[55];  // Ajusta el tamaño según tus necesidades
    size_t dest_buffer_length = sizeof(dest_buffer);

    // Call to the system call
    long result = syscall(451, number, username, username_length, dest_buffer, dest_buffer_length);

    // Check for errors
    if (result < 0) {
        perror("System call failed");
        return EXIT_FAILURE;
    }

    // Print the result
    printf("%s\n", dest_buffer);

    return EXIT_SUCCESS;
}