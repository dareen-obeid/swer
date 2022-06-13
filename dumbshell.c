
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <stdlib.h>
#include <signal.h>

#define maxarg 20
#define FORMAT "%5s %s\t%8s %s\n"
#define      DEBUG        1
#define      MAXLINELEN   4096
#define      MAXARGS             128
#define      END_OF_LINE  0
#define      SEQ_OP       ';'
#define      SEQUENCE     1

struct cmd {
    struct cmd   *next;
    int          terminator;
    char         *exe_path;
    int          nargs;
    char         *arg[MAXARGS];
};

void   *ck_malloc(size_t   size){
    void   *ret = malloc(size);
    if (!ret) {
        perror("dumbshell:ck_malloc");
        exit(1);
    }
    return ret;
}

char   *skip_to_non_ws(char       *p){
    int    ch;
    while (ch = *p) {
        if (ch != ' ' && ch != '\t' && ch != '\n') return p;
        ++p;
        }
    return 0;
}

char   *skip_to_ws_or_sep(char    *p){
    int    ch;
    while (ch = *p) {
        if (ch == ' ' || ch == '\t' || ch == '\n') return p;
        if (ch == SEQ_OP) return p;
        ++p;
    }
    return 0;
}

struct cmd *parse_commands(char  *line){
    char         *ptr; int          ix; struct cmd   *cur;

    ptr = skip_to_non_ws(line);
    if (!ptr) return 0;
    cur = ck_malloc(sizeof *cur);
    cur->next = 0;
    cur->exe_path = ptr;
    cur->arg[0] = ptr;
    cur->terminator = END_OF_LINE;
    ix = 1;
    for (;;) {
        ptr = skip_to_ws_or_sep(ptr);
        if (!ptr) {
            break;
        }
        if (*ptr == SEQ_OP) {
            *ptr = 0;
            cur->next = parse_commands(ptr+1);
            if (cur->next) {
                cur->terminator = SEQUENCE;
            }
            break;
        }
        *ptr = 0;
        ptr = skip_to_non_ws(ptr+1);
        if (!ptr) {
             break;
        }
            if (*ptr == SEQ_OP) {
            /* found a sequence operator */
            cur->next = parse_commands(ptr+1);
            if (cur->next) {
                cur->terminator = SEQUENCE;
            }
            break;
        }
        cur->arg[ix] = ptr;
        ++ix;
    }
    cur->arg[ix] = 0; cur->nargs = ix; return cur;
}

void pwd(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s \n", cwd);

}

void ps(char** args)
{
DIR *dir;
  struct dirent *ent;
  int i, fd_self, fd;
  unsigned long time, stime;
  char flag, *tty;
  char cmd[256], tty_self[256], path[256], time_s[256];
  FILE* file;

  dir = opendir("/proc");
  fd_self = open("/proc/self/fd/0", O_RDONLY);
  sprintf(tty_self, "%s", ttyname(fd_self));
  printf(FORMAT, "PID", "TTY", "TIME", "CMD");

  while ((ent = readdir(dir)) != NULL)
  {
  flag = 1;
  for (i = 0; ent->d_name[i]; i++)
  if (!isdigit(ent->d_name[i]))
  { 
   flag = 0;
   break;
  }

  if (flag)
  {
  sprintf(path, "/proc/%s/fd/0", ent->d_name);
  fd = open(path, O_RDONLY);
  tty = ttyname(fd);

  if (tty && strcmp(tty, tty_self) == 0)
  {

   sprintf(path, "/proc/%s/stat", ent->d_name);
   file = fopen(path, "r");
   fscanf(file, "%d%s%c%c%c", &i, cmd, &flag, &flag, &flag);
   cmd[strlen(cmd) - 1] = '\0';

  for (i = 0; i < 11; i++)
  fscanf(file, "%lu", &time);
  fscanf(file, "%lu", &stime);
  time = (int)((double)(time + stime) / sysconf(_SC_CLK_TCK));
  sprintf(time_s, "%02lu:%02lu:%02lu",
  (time / 3600) % 3600, (time / 60) % 60, time % 60);
 
  printf(FORMAT, ent->d_name, tty + 5, time_s, cmd + 1);
  fclose(file);
  }
 close(fd);
}
}
close(fd_self);
}


int psA(){
    DIR *dirPtr;
    dirPtr = opendir("/proc");
    int openDisc;
    openDisc = open("/dev/fd/0", O_RDONLY);
    char teleTYpwriterString[350];
    snprintf(teleTYpwriterString, 400, "%s", ttyname(openDisc));
    printf("%7s %5s\t%10s %5s\n", "PID", "TTY", "TIME", "CMD");
    struct dirent *entryPtr;

    while ((entryPtr = readdir(dirPtr)) != NULL){
        int flag;
        flag = 1;
        for (int j = 0; entryPtr->d_name[j]; j++){
            if (!isdigit(entryPtr->d_name[j])){
                flag = 0;
                break;
            }
        }
        if (flag){
            char entryPath[350];
            snprintf(entryPath, 400, "/proc/%s/fd/0", entryPtr->d_name);
            int fileD;
            fileD = open(entryPath, O_RDONLY);
            char *ttyPtr;
            snprintf(entryPath, 400, "/proc/%s/stat", entryPtr->d_name);
            FILE *filePtr;
            filePtr = fopen(entryPath, "r");
            int intNum;
            char comLine[500];
            char readF;
            fscanf(filePtr, "%i%s%c%c%c", &intNum, comLine, &readF, &readF, &readF);
            comLine[strlen(comLine) - 1] = '\0';
            int userModeT;
            int startT;
            int finalTime;

            for (int j = 0; j < 11; j++)
            fscanf(filePtr, "%i", &userModeT);
            fscanf(filePtr, "%i", &startT);

            finalTime = ((userModeT + startT) / sysconf(_SC_CLK_TCK));

            char time_s[400];

            sprintf(time_s, "%02i:%02i:%02i", (finalTime / 3600) % 3600, (finalTime / 60) % 60, finalTime % 60);

            ttyPtr = ttyname(fileD);

            if (ttyPtr) {
                printf("%7s %5s\t%10s %5s\n", entryPtr->d_name, ttyPtr + 5, time_s, comLine + 1);
            }
            else {
                printf("%7s %5c\t%10s %5s\n", entryPtr->d_name, '?', time_s, comLine + 1);
            }

            fclose(filePtr);

            close(fileD);
        }
    }

    close(openDisc);

    return 0;
}

void   execute(struct cmd  *clist){
    
    char cwd[1024];
    //cd 
    if( strcmp(clist->exe_path,"cd")==0){
        if (clist->nargs == 1) {
           chdir(getenv("HOME"));
           printf("%s\n", getcwd(cwd, sizeof(cwd)));
        } 
        else {
            char* dir = clist->arg[1];

            if (!strcmp(dir, " ") || !strcmp(dir, "")) {       //!strcmp(dir, "~") ||
                char* home = getenv("HOME");
                chdir(home);
                printf("%s\n", getcwd(cwd, sizeof(cwd)));
            } 
            else {
                chdir(clist->arg[1]);
                printf("%s\n", getcwd(cwd, sizeof(cwd)));
            }  
        }
    }

if( strcmp(clist->exe_path,"ps -A")==0){

DIR *directory;
struct dirent *ent;
  int i, fd_self, fd;
  unsigned long time, stime;
  char flag, *tty;
  char cmd[300], tty_self[300], path[300], time_s[300];
  FILE* file;
  

  directory = opendir("/proc");
  fd_self = open("/proc/self/fd/0", O_RDONLY);
  sprintf(tty_self, "%s", ttyname(fd_self));
  

  while ((ent = readdir(directory)) != NULL)
  {
  flag = 1;
  for (i = 0; ent->d_name[i]; i++)
  if (!isdigit(ent->d_name[i]))
  { 
   flag = 0;
   break;
  }

  if (flag)
  {
  
  fd = open(path, O_RDONLY);//remove the if statment to show all the processes in system

   sprintf(path, "/proc/%s/stat", ent->d_name);
   file = fopen(path, "r");
   fscanf(file, "%d%s%c%c%c", &i, cmd, &flag, &flag, &flag);
   cmd[strlen(cmd) - 1] = '\0';

  for (i = 0; i < 11; i++)
  fscanf(file, "%lu", &time);
  fscanf(file, "%lu", &stime);
  time = (int)((double)(time + stime) / sysconf(_SC_CLK_TCK));
  sprintf(time_s, "%02lu:%02lu:%02lu",
  (time / 3600) % 3600, (time / 60) % 60, time % 60);
 
  printf(FORMAT, ent->d_name, time_s, cmd + 1);
  fclose(file);
 close(fd);
}
}
close(fd_self);
}


    
int    pid, npid, stat;

pid = fork();
if (pid == -1) {
perror("dumbshell:fork");
exit(1);
}
if (!pid) {
/* child */
// execvp(clist->exe_path,clist->arg);
// fprintf(stderr,"No such command: %s\n",clist->exe_path);
exit(1); 
}
do {
npid = wait(&stat);
// printf("Process %d exited with status %d\n",npid,stat); 
} while (npid != pid);
switch (clist->terminator) {
case SEQUENCE:
execute(clist->next);
}

    if(!strcmp(clist->exe_path,"pwd")){
        pwd();
    }

    if(!strcmp(clist->exe_path,"ps")){
        ps(clist->arg);
    }

    if(!strcmp(clist->exe_path,"kill")){
        killCommand(clist->arg);
    }

    if (strcmp("ps", clist->exe_path) == 0 && strcmp("-A", clist->arg[1]) == 0){

                if (psA() != 0)
                    perror("error invoking ps");

                exit(0);
            }

}

void   free_commands(struct cmd   *clist)
{
struct cmd   *nxt;

do {
nxt = clist->next;
free(clist);
clist = nxt;
} while (clist);
}

char	*get_command(char   *buf, int	size, FILE	*in)
{
if (in == stdin) {
fputs("@ ",stdout); /* prompt */
}
return fgets(buf,size,in);
}



int main(void){
    char         linebuf[MAXLINELEN];
    struct cmd   *commands;



    while (get_command(linebuf,MAXLINELEN,stdin) != NULL) {


        commands = parse_commands(linebuf);
        if (commands) {
            execute(commands);
            free_commands(commands);
        }
    }


}


