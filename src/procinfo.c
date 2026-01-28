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
  
  fnextline(file_pointer_stat);
  fnextline(file_pointer_stat);
  printf("%s", fgets(result, 100, file_pointer_stat));
  fnextline(file_pointer_stat);
  fnextline(file_pointer_stat);
  fnextline(file_pointer_stat);
  printf("%s", fgets(result, 100, file_pointer_stat));
  printf("%s\n", fgets(result, 100, file_pointer_cmd));
  for (int i=0; i<15; i++){
    fnextline(file_pointer_stat);
  }
  printf("%s", fgets(result, 100, file_pointer_stat));
  for (int i=0; i<15; i++){
    fnextitem(file_pointer_cpu);
  }
  char curchar = '\0';
  char usertime[20] = "";
  printf("cpu time (user): ");
  while (curchar != ' '){
    printf("%c", curchar = fgetc(file_pointer_cpu));
    //strcat(usertime, curchar);
  }
  printf("\n");
  curchar = '\0';
  char systemtime[20] = "";
  printf("cpu time (system): ");
  while (curchar != ' '){
    printf("%c", curchar = fgetc(file_pointer_cpu));
    //strcat(systemtime, curchar);
  }
  printf("\n");

  
  
  fclose(file_pointer_stat);
  fclose(file_pointer_cmd);
  fclose(file_pointer_cpu);
  return 0;
}
