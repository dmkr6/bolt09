#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>

int Daemon(void);
char* getTime();
int writeLog(char msg[256]);
char* getCommand(char command[128]);

char* getTime() { //return date and time
    time_t now;
    struct tm *ptr;
    static char tbuf[64];
    bzero(tbuf,64);
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf,64, "%Y-%m-%e %H:%M:%S", ptr);
    return tbuf;
}

char* getCommand(char command[128]) { //return command's execution result
    FILE *pCom;
    static char comText[256];
    bzero(comText, 256);
    char  buf[64];
    pCom = popen(command, "r"); //execute
    if(pCom == NULL) {
        writeLog("Error Command");
        return "";
    }
    strcpy(comText, "");
    while(fgets(buf, 64, pCom) != NULL) { //read result
        strcat(comText, buf);
    }
    pclose(pCom);
    return comText;
}

int writeLog(char msg[256]) {
    FILE * pLog;
    pLog = fopen("/var/log/my_daemon.log", "a");
    if(pLog == NULL) {
        return 1;
    }
    char str[312];
    bzero(str, 312);
    strcpy(str, getTime());
    strcat(str, " ==========================\n");
    strcat(str, msg);
    strcat(str, "\n");
    fputs(str, pLog);
    //fwrite(msg, 1, sizeof(msg), pLog);
    fclose(pLog);
    return 0;
}

int main(int argc, char* argv[]) {
    writeLog("Daemon Start");

    pid_t parpid, sid;
    
    parpid = fork(); //create child process
    if(parpid < 0) {
        exit(1);
    } else if(parpid != 0) {
        exit(0);
    } 
    umask(0);//allow permission to work with filesystem
    sid = setsid();//generate sid
    if(sid < 0) {
        exit(1);
    }
    if((chdir("/")) < 0) {//go to root directory
        exit(1);
    }
    close(STDIN_FILENO);//deny permission to STD...
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    return Daemon();
}

int Daemon(void) { //daemon infinite loop
    char *log;
    while(1) {
        log = getCommand("arp -i eth0");
        if(strlen(log) > 5) { //if not empty
          writeLog(log);
        }
        sleep(60);//wait 1 minute
    }
    return 0;
}
