#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>


#define N_DATA 2000000
#define N_SHARED 2000

//Methode zur Initalisierung der Semaphoren
void sem_init(int sem_set, int sem_num, int op) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = op;
    sb.sem_flg = 0;
    /**
    * semop-Funktion wird aufgerufen um operationen auf den Semaphoren auszuführen.
    * sem_set: ID des Semaphore-Sets, auf dem die Operationen ausgeführt werden sollen.
	* &sb: Zeiger auf Array von Strukturen vom Typ sembuf, der die Operationen spezifiziert, die auf die Semaphore angewendet werden sollen
	* 1: Die Anzahl der Operationen, die ausgeführt werden sollen
    */
    if (semop(sem_set, &sb, 1) < 0) {
        perror("semop init");
        exit(EXIT_FAILURE);
    }
}

/**
* P-Operation verringert den Semaphore-Wert um 1.
* wird verwendet, um einen kritischen Abschnitt zu betreten und Ressourcen zu reservieren.
* wenn Semaphore-Wert vor der Operation 0 ist, blockiert die P-Operation den Prozess oder Thread,
* bis der Wert größer als 0 wird, um Ressourcenkonflikte zu vermeiden
* wenn Semaphore-Wert 1 ist, wird Wert auf 0 gesetzt und der kritische Abschnitt wird betreten
*/
void sem_p_operation(int sem_set, int sem_num) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = -1;
    sb.sem_flg = 0;

    if (semop(sem_set, &sb, 1) < 0) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

/**
* V-Operation erhöht den Semaphore-Wert um 1.
* Sie wird verwendet, um einen kritischen Abschnitt zu verlassen und reservierte Ressourcen freizugeben.
*/
void sem_v_operation(int sem_set, int sem_num) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = 1;
    sb.sem_flg = 0;

    if (semop(sem_set, &sb, 1) < 0) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int shm_id;
    int sem_set;
    int *shm_ptr;
    int data[N_DATA];

    /**
     * shmget liefert einen identifier für ein Shared Memory Segment.
     * IPC_Private: Schlüssel, der dafür sorgt, dass das Segment eindeutig und nur für diesen prozess ist.
     * N_SHARED * sizeof(int): für die benötigte Größe des Segments (* sizeof(int),
     * da die größe in Bytes angegeben werden muss).
     * IPC_CREATE: Sorgt dafür, dass ein neues Shared Memory ersellt wird.
     * | 0666: Sorgt dafür, dass die rwx rechte auf 0666 gesetzt werden (lesbar und beschreibbar für alle)
    */
    shm_id = shmget(IPC_PRIVATE, N_SHARED * sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        return 1;
    }

    /**
     * Anbindung (Attach) des Shared Memory Segments
     * shm_id: ID des Shared Memory Segments, dass angebunden werden soll
     * NULL: Zeiger auf eine Speicheradresse, an der das System den Segment anbindet (Null -> automatische Auswahl durch das System)
     * 0: Flags (hier keine speziellen Flags gesetzt)
    */
    shm_ptr = (int *)shmat(shm_id, NULL, 0);
    if (shm_ptr == NULL) {
        perror("shmat");
        return 1;
    }

    /**
     * Semaphoren erstellen
     * IPC_Private: Schlüssel, der dafür sorgt, dass das der Semaphor eindeutig und nur für diesen prozess ist.
     * 2: Anzahl der Semaphoren, die erstellt werden.
     * IPC_CREATE: Sorgt dafür, dass ein neuer ersellt wird.
     * | 0666: Sorgt dafür, dass die rwx rechte auf 0666 gesetzt werden (lesbar und beschreibbar für alle)
    */
    sem_set = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    if (sem_set < 0) {
        perror("semget");
    }

    // Initialisierung der Semaphoren
    sem_init(sem_set, 0, 1); //S1 freigegeben
    sem_init(sem_set, 1, 0); //S2 blockiert

    /**
     * Daten mit int-Werten im Vaterprozess erzeugen
     * srand48() übergibt Startwert für lrand48() (aktueller Zeitstempel)
     * NULL -> Wert wird direkt zurück gegeben (nicht als time Variable)
     * Zufällige zahlen werden mit hilfe von lrand48 erstellt
     */
    srand48(time(NULL));
    for (int i = 0; i < N_DATA; i++) {
        data[i] = lrand48();
    }

    pid_t pid = fork();

    switch(pid) {
        case -1:
            perror("fork\n");
            exit(EXIT_FAILURE);
            break;
        case 0:
            // Sohnprozess P2 (Verbraucher)
            for (int i = 0; i < N_DATA; i += N_SHARED) {
                sem_p_operation(sem_set, 1); // Warten auf Signal vom Erzeuger (S2)

                // Daten aus dem Shared Memory lesen
                for (int j = 0; j < N_SHARED && (i + j) < N_DATA; j++) {
                    printf("int read from shm: %d\n", shm_ptr[j]);
                }

                sem_v_operation(sem_set, 0); // Signal an den Erzeuger senden (S1)
            }
            printf("child process finished\n");
            shmdt(shm_ptr);
            exit(0);
            break;
        default:
            // Vaterprozess P1 (Erzeuger)
            for (int i = 0; i < N_DATA; i += N_SHARED) {
                sem_p_operation(sem_set, 0); // Warten auf Signal vom Erzeuger (S1)

                // Daten in den Shared Memory schreiben
                for (int j = 0; j < N_SHARED && (i + j) < N_DATA; j++) {
                    shm_ptr[j] = data[i + j];
                }

                sem_v_operation(sem_set, 1); // Signal an den Verbraucher senden (S2)
            }

            wait(NULL); // Wenn Vaterprozess beendet, auf den Sohnprozess warten
            shmdt(shm_ptr); // lösen des Prozesses von Shared Memory Segment
            shmctl(shm_id, IPC_RMID, NULL); // Shared Memory Segment löschen
            semctl(sem_set, 0, IPC_RMID); // Semaphoren löschen        
            break;
    
    }

    return 0;
}