//Exemple creacio de processos
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i, pid, status;
    for (i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) { // fill
            printf("Soc el fill %d, amb pid %d\n", i, getpid());
            exit(0);
        }
    }
    for (i = 0; i < 5; i++) { // pare
        pid = wait(&pid);
        printf("Soc el pare, he acabat de esperar al fill %d\n", pid);
    }
    return 0;
}

// Recordeu que per a executar el programa l'heu de compilar amb el següent comandament:
// gcc -o creacio_processos creacio_processos.c
// Reviseu el funcionament de wait() i fork() aqui: https://www.geeksforgeeks.org/wait-system-call-c/
