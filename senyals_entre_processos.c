#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define MATRICULA_LEN 8

void processar_matricula(char *matricula, int pid, int p);

int main(int argc, char *argv[]) {
    char resultat[10];
    int p[2], pid, nbytes;

    // Rebem per parametre el nom del fitxer que volem llegir:
    // ./pipes fitxer.txt
    // Si no rebem cap parametre, sortim del programa:
    if (argc != 2) {
        printf("Error: ./control_processos fitxer.txt\n");
        exit(1);
    }
    // Obrim el fitxer en mode lectura:
    FILE *f = fopen(argv[1], "r");
    // Si no podem obrir el fitxer, sortim del programa:
    if (f == NULL) {
        printf("Error: no s'ha pogut obrir el fitxer %s\n", argv[1]);
        exit(1);
    }

    // Creem una canonada:
    pipe(p);

    // Creem un buffer per llegir linia a linia:
    char buffer[100];
    // Creem un array per a guardar els pid dels processos fills:
    int pid_array[100];
    int count = 0;
    // Llegim linia a linia, cada línea conté una matrícula, cada matricula es processa en un proces amb la funció fork():
    while (fgets(buffer, 100, f) != NULL) {
        // Creem un proces amb fork():
        int pid = fork();
        // Si pid == 0, som al fill:
        if (pid == 0) {
            // Executem la funció que processa la matricula:
            processar_matricula(buffer, pid, p[1]);
        } else {
            // Guardem el pid del proces fill:
            pid_array[count] = pid;
            count++;
        }
    }

    // Tanquem el fitxer:
    fclose(f);
    sleep(1);
    // Enviem un senyal a tots els processos fills per a que acabin:
    for (int i = 0; i < count; i++) {
        kill(pid_array[i], SIGUSR1);
    }

    // Mentres hi hagi dades a llegir de la canonada, les llegim:
    while ((nbytes = read(p[0], resultat, 11)) > 0) {
        printf("El resultat de la matricula es %s \n", resultat);
    }

}

void signal_handler(int signum) {
    printf("El fill %d ha rebut el senyal %d\n", getpid(), signum);
}

void processar_matricula(char *matricula, int pid, int p) {
    signal(SIGUSR1, signal_handler);
    pause();

    // Simulem un proces que triga entre 1 i 5 segons en processar la matricula:
    srand(time(NULL) + getpid());
    sleep(rand() % 5 + 1);
    // Generem un valor aleatori entre 0 i 10 per a simular el resultat de la matricula:
    int resultat = rand() % 10;

    // Eliminem el salt de linia de la matricula:
    matricula[strlen(matricula) - 1] = '\0';
    
    // Creem un string amb la matricula i el resultat:
    char str[100];
    sprintf(str, "%s %d", matricula, resultat);

    printf("S'ha processat la matricula %s amb pid %d\n", str, getpid());
    // Escribim el resultat a la canonada:
    write(p, &str, strlen(str) + 1);
    // Mostrem per pantalla que s'ha processat la matricula:
    exit(0);
}
