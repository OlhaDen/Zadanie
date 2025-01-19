#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "/var/log/port_monitor.log"
#define INTERVAL 60

void write_log(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0';
        fprintf(log_file, "%s - %s\n", time_str, message);
        fclose(log_file);
    }
}

void monitor_ports() {
    FILE *fp;
    char buffer[1024];
    fp = popen("netstat -tuln", "r");
    if (fp == NULL) {
        write_log("Nie udało się uruchomić netstat.");
        return;
    }
    write_log("Monitorowanie otwartych portów:");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        write_log(buffer);
    }
    pclose(fp);
}

void daemonize() {
    pid_t pid;
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    if (setsid() < 0) exit(EXIT_FAILURE);
    signal(SIGHUP, SIG_IGN);
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    umask(0);
    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    daemonize();
    while (1) {
        monitor_ports();
        sleep(INTERVAL);
    }
    return 0;
}
