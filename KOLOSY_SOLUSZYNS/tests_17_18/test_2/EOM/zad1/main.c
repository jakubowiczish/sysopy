#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define MAX 20
#define KEY "./path"

int main() {
    int shm_id, i;
    int* buf;

    // Wygeneruj klucz dla kolejki na podstawie KEYi znaku 'a'

    int key = ftok(KEY, 'a');

    // Utwórz segment pamięci współdzielonej shmid o prawach dostępu 600,
    // rozmiarze MAX jeśli segment już istnieje, zwróć błąd, jeśli utworzenie
    // pamięci się nie powiedzie zwróć błąd i wartość 1

    shm_id = shmget(key, MAX, IPC_CREAT | 0600);
    if (shm_id == -1) {
        printf("shmget error\n");
        return 1;
    }

    // Przyłącz segment pamięci współdzielonej do procesu do buf, obsłuż błędy i
    // zwróć 1

    buf = shmat(shm_id, NULL, 0);
    if (buf == (void*)-1) {
        printf("shmat error\n");
        return 1;
    }

    for (i = 0; i < MAX; i++) {
        buf[i] = i * i;
        printf("Wartość: %d \n", buf[i]);
    }
    
    printf("Memory written\n");

    // odłącz i usuń segment pamięci współdzielonej

    shmdt(buf);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
