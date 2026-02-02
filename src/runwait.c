#include "common.h"
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
static void usage(const char *a){
  fprintf(stderr,"Usage: %s <cmd> [args]\n",a); 
  exit(1);
}

static double d(struct timespec a, struct timespec b){
  return (b.tv_sec-a.tv_sec)+(b.tv_nsec-a.tv_nsec)/1e9;
  }
  
int main(int c,char**v){
  //struct timespec time_start, time_end;
  struct timespec time_start, time_end;
  pid_t cpid;
  cpid = fork();
  int exitstat = -1;
  
  if(!cpid){
    execvp(v[1], v+2);
    exit(0);
  }
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  timespec_get(&time_start, TIME_UTC);
  waitpid(cpid, 0, 0);
  exitstat = errno;
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  timespec_get(&time_end, TIME_UTC);
  printf("child pid: %d, exit: %d, time: %fs\n", cpid, exitstat, d(time_start, time_end));
  return 0;
}
