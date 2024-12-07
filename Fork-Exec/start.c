#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

    if (argc < 3) {
        printf("Illegal amount of arguments! Usage: ./start [prog] [arg1] [arg2] [arg3] ...\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch (pid) {
	case -1: //error
	    perror("fork\n");
            exit(EXIT_FAILURE);
	    break;
        case 0: //child
            setpriority(PRIO_PROCESS, 0, 19);
            execvp(argv[1], argv + 1);

            perror("execvp\n");
            exit(EXIT_FAILURE);
            break;
        default: //parent

            //kill(pid, SIGINT);
            //kill(pid, SIGTERM);
            //kill(pid, SIGKILL);

            printf("(from parent) PID of %s (child): %d\n", argv[1], pid);

            int status;
            wait(&status);
            pid_t exit_code = WEXITSTATUS(status);

            printf("(from parent) Return Code of %s: %d\n", argv[1], exit_code);

            if (WIFSIGNALED(status)) {
                int signal_num = WTERMSIG(status);
                printf("(from parent) Child process terminated by signal: %d\n", signal_num);
                psignal(signal_num, "(from parent) Signal received: ");
            } else {
                printf("(from parent) Child process exited normally\n");
            }
            break;
    }

    return 0;
}
