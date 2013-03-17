/* Justyn Harriman */
/* using modified code from CS152 lab 8 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUF_SZ 256

/* c_rev : (char *) -> void */
/* destructive reverses a character array */
void c_rev(char *in)
{
  int len = strlen(in);
  len--;
  int i;
  for(i = 0; i < len; i++){
    char tmp;
    tmp = in[len];
    in[len] = in[i];
    in[i] = tmp;
    len--;
  }
  return;
}

/* bsint: int -> (char *) */
/* creates a bitstring of an integer (only an int, not a long) */
char *bsint(int b)
{
  int i;
  char *bs = (char *)malloc(sizeof(int) * 8 + 1);
  bs[32] = '\0';
  for(i = 31; i >= 0; i--){
    bs[i] = b & 1 ? '1':'0';
    b >>= 1;
  }
  return bs;
}

/* itohexs: int -> (char *) */
/* creates a hexadecimal string of an int */
char *itohexs(int b)
{
  char *new =(char *)malloc(sizeof(char)*1024);
  sprintf(new, "%x", b);
  return new;
}

/* itoocts: int -> (char *)*/
/* creates a octal string of an int */ 
char *itoocts(int b)
{
  char *new = (char *)malloc(sizeof(char)*1024);
  int i = 0;
  while(b){
    new[i] = (char)(((int)'0')+(b%8));
    b >>=3;
    i++;
  }
  new[i] = '\0';
  c_rev(new);
  return new;
}

/* trun_binary_int : (char *) -> (char *) */
/* trucates an integer bitstring to exactly the necessary length without leading zeros */
char *trun_binary_int(char *inb)
{
  int i = 0;
  int stop = 1;
  while(stop){
    if(inb[i] == '1')
      stop = 0;
    else
      i++;
  }
  return inb += i;
}

/* trim_newline: (char *) -> (char *) */
/* trims the newline character off the end of a string recevied from fgets */
/* from lab8 */
char *trim_newline(char *s)
{
  char *t;
  int n = strlen(s);
  int must_change = (s[n-1]=='\n');
  if (must_change)
    s[n-1] = '\0';
  t = strdup(s);
  if (must_change)
    s[n-1] = '\n'; /* tidy up */
  return t;
}

/* dec_to_etc: (char *) (char *) int -> void */
/* writes the prompt that actually drills the user */
void dec_to_etc(char *ans, char *type, int dec_val)
{
  char new0[1024];
  while(1){
    fprintf(stdout, "What is the %s representation of %d?\n", type, dec_val);
    fprintf(stdout, "Answer: ");
    fgets(new0, 1024, stdin);
    char *new = trim_newline(new0);
    if(!strcmp("give", new)){
      fprintf(stdout, "\nThe correct response was: %s\n", ans);
      fprintf(stdout, "Press enter to continue.");
      fgetc(stdin);
      putchar('\n');
      break;
    }
    if(!strcmp(new, ans)){
      fprintf(stdout, "Correct!\n");
      putchar('\n');
      break;
    }
    else{
      fprintf(stdout, "Incorrect. Please try again or type 'give' to give up\n\n");
    }
  }
  return;
}

/******************************************************************************/
/* Code after this point is modified from the main.c file from lab8 */
/******************************************************************************/
char *extract_arg(char *cmd, int *parse_succeeded)
{
  int i;
  char cmdbuf[2];
  char argbuf[100];
  for (i=0; i<100; i++)
    argbuf[i] = '\0';
  sscanf(cmd,"%s %s",cmdbuf,argbuf);
  *parse_succeeded = strlen(argbuf)>0;
  return strdup(argbuf);
}

void set_cmd_vars(char *cmd, int *parse_succeeded, int *num)
{
  char *optnum = extract_arg(cmd, parse_succeeded);
  if (*parse_succeeded) {
    *num = atoi(optnum);
  }
  else{
    srand(time(NULL));
    *num = rand()%100;
  }
    
}

int process_cmd(char *cmd, int *quit)
{
  int parse_succeeded;
  char *ans;
  int num = 0;
  switch (cmd[0]) {
  case 'q':
    if (strcmp(cmd,"q")==0) {
      *quit = 1;
      return 1;
    }
    return 0;
  case 'b':
    set_cmd_vars(cmd, &parse_succeeded, &num);
    ans = trun_binary_int(bsint(num));
    dec_to_etc(ans, "binary", num);
    return 1;
  case 'h':
    set_cmd_vars(cmd, &parse_succeeded, &num);
    ans = itohexs(num);
    dec_to_etc(ans, "hexadecimal", num);
    return 1;
  case 'o':
    set_cmd_vars(cmd, &parse_succeeded, &num);
    ans = itoocts(num);
    dec_to_etc(ans, "octal", num);
    return 1;
  case '?':
    fprintf(stdout, "-\tEntering b, h, or o asks you to write a decimal number ");
    fprintf(stdout, "in binary, hexadecimal, and octal respectively\n");
    fprintf(stdout, "-\tWriting a number next to an option will replace ");
    fprintf(stdout, "a random number with the number you specify.\n\n");
    return 1;
  default:
    return 0;
  }
}

void prompt()
{
  char *p =
    "Enter b [opt#] (binary), h [opt#] (hex), o [opt#] (octal), ? (help), q (quit):\n> ";
  fprintf(stdout,"%s",p);
  fflush(stdout);
  return;
}

int main(int argc, char *argv[])
{
  int quit = 0;
  char buf[256];
  /* interactive i/o loop */
  while (1) {
    char *cmd;
    prompt();
    cmd = fgets(buf, BUF_SZ, stdin);
    cmd = trim_newline(cmd);
    /* printf("cmd=%s\n", cmd); */
    putchar('\n');
    if (!process_cmd(cmd, &quit))
      fprintf(stdout, "unknown command: \"%s\"\n", cmd);
    else
      if (quit){
        free(cmd);
        break;
      }
    free(cmd);
  }
  return 0;
}

