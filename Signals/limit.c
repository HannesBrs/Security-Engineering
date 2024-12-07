#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>

// Signalhandler-Funktion
void signal_handler(int signum) {
    switch (signum)
    {
    case SIGXCPU:
        printf("CPU-Zeit-Limit überschritten! (Signal %d)\n", signum);
        break;

    case SIGXFSZ:
        printf("Dateigrößen-Limit überschritten! (Signal %d)\n", signum);
        break;

    case SIGSEGV:
        printf("Stapelgrößen-Limit überschritten! (Signal %d)\n", signum);
        break;

    default:
        printf("Signal %d empfangen.\n", signum);
        break;
    }
    exit(1);
}

// Funktion, die das CPU-Zeit-Limit überschreitet
void exceed_cpu_limit() {
    while (1);
}

// Funktion, die das Stack-Limit überschreitet
void exceed_stack_limit() {
    exceed_stack_limit();
}

// Funktion, die das Dateigrößen-Limit überschreitet
void exceed_file_size_limit() {
    FILE *f = fopen("file.txt", "w");
    while (1) {
        fputc('A', f);
    }
    fclose(f);
}

int main() {
    struct rlimit limit;

    // Setzen der CPU-Zeit auf 1 Sekunde
    limit.rlim_cur = 1;
    limit.rlim_max = 1;
    setrlimit(RLIMIT_CPU, &limit);

    // Setzen der Stack-Größe auf 1 MB
    limit.rlim_cur = 1024 * 1024;
    limit.rlim_max = 1024 * 1024;
    setrlimit(RLIMIT_STACK, &limit);

    // Setzen der Dateigröße auf 1 MB
    limit.rlim_cur = 1024 * 1024;
    limit.rlim_max = 1024 * 1024;
    setrlimit(RLIMIT_FSIZE, &limit);

    // Registrieren des Signalhandlers für SIGXCPU, SIGXFSZ und SIGSEGV
    signal(SIGXCPU, signal_handler);
    signal(SIGXFSZ, signal_handler);
    signal(SIGSEGV, signal_handler);
    
    //exceed_cpu_limit();
    exceed_file_size_limit();
    //exceed_stack_limit();

    return 0;
}
