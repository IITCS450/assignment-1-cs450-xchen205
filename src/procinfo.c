#include "common.h"
#include <ctype.h> //char checker functions
#include <stdio.h> //file-related functions
#include <unistd.h>

static void usage(const char *a){
  fprintf(stderr, "Usage: %s <pid>\n", a); 
  exit(1);
}

static int isnum(const char*s){
  for(; *s; s++) {
    if( !isdigit(*s) ) {
      return 0;  
    }
  }
  return 1;
}

//custom functions

void fnextline(FILE *file_pointer){
  char curchar = '\0';
  while (curchar != '\n'){
    curchar = fgetc(file_pointer);
  }
}

void fnextitem(FILE *file_pointer){
  char curchar = '\0';
  while (curchar != ' '){
    curchar = fgetc(file_pointer);
  }
}

//main

int main(int c, char**v){ 
  //the command line takes int, which is the number of arguments (the command line itself counting as one) and the args themselves
  char result[100] = "";
  if( c!=2 || !isnum( v[1] ) ) { //check if pid is a number
    usage(v[0]);
  }
  char statname[50] = "/proc/";
  strcat(statname, v[1]);
  //set up and check file pointer
  FILE *file_pointer_stat = fopen(strcat(statname, "/status"), "r");
  if(!file_pointer_stat){
    switch errno {
      case 2:
        printf("the pid asked for doesn't exist\n");
        break;
      case 13:
        printf("permission denied\n");
        break;
      default:
        printf("unknown error: %d\n", errno);
        break;
    }
    return 1;
  }
  char cmdname[50] = "/proc/";
  strcat(cmdname, v[1]);
  FILE *file_pointer_cmd = fopen(strcat(cmdname, "/cmdline"), "r");
  char cpuname[50] = "/proc/";
  strcat(cpuname, v[1]);
  FILE *file_pointer_cpu = fopen(strcat(cpuname, "/stat"), "r");
  //get stats
  fnextline(file_pointer_stat);
  fnextline(file_pointer_stat);
  printf("%s", fgets(result, 100, file_pointer_stat));
  fnextline(file_pointer_stat);
  fnextline(file_pointer_stat);
  fnextline(file_pointer_stat);
  printf("%s", fgets(result, 100, file_pointer_stat));
  printf("command line: %s\n", fgets(result, 100, file_pointer_cmd));
  for (int i=0; i<15; i++){
    fnextline(file_pointer_stat);
  }
  printf("%s", fgets(result, 100, file_pointer_stat));
  for (int i=0; i<13; i++){
    fnextitem(file_pointer_cpu);
  }
  int total = 0;
  char curchar = '\0';
  int charcount = 0;
  int numhold = 0;
  int multiplier = 1;
  //get ustat
  while (curchar != ' '){ //count chars
    curchar = fgetc(file_pointer_cpu);
    charcount++;
  }
  fseek(file_pointer_cpu, -charcount, SEEK_CUR);
  charcount -= 2;
  curchar = '\0';
  while (curchar != ' '){ //multiply input
    curchar = fgetc(file_pointer_cpu);
    multiplier = 1;
    for(int i=1; i<=charcount; i++){
      multiplier *= 10;
    }
    numhold += (((int)curchar) - 48) * multiplier;
    charcount--;
  }
  total += numhold + 16;
  curchar = '\0';
  charcount = 0;
  numhold = 0;
  multiplier = 1;
  //get sstat
  while (curchar != ' '){ 
    curchar = fgetc(file_pointer_cpu);
    charcount++;
  }
  fseek(file_pointer_cpu, -charcount, SEEK_CUR);
  charcount -= 2;
  curchar = '\0';
  while (curchar != ' '){ //multiply input
    curchar = fgetc(file_pointer_cpu);
    multiplier = 1;
    for(int i=1; i<=charcount; i++){
      multiplier *= 10;
    }
    numhold += (((int)curchar) - 48) * multiplier;
    charcount--;
  }
  total += numhold + 16;
  printf("cpu time (user+system): %.2fs\n", ((double)total)/sysconf(_SC_CLK_TCK));
  
  fclose(file_pointer_stat);
  fclose(file_pointer_cmd);
  fclose(file_pointer_cpu);
  return 0;
}
