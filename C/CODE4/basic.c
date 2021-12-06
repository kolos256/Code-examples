// The implementation of functionality of the Basic interpreter

/* Availiable commands:
LET
IF
THEN
ELSE
GOTO
PRINT 
*/

#include "stdio.h"
#include "math.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"

#define NUM_LAB 100 	// Maximum number of Lables
#define LAB_LEN 10  	// Maximum Lable length
#define PROG_SIZE 10000	// program buffer size

// types of tokens
#define DELIMITER  1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING	  5
#define QUOTE	  6

// token codes
#define PRINT 1
#define IF    2
#define THEN  3
#define ELSE  4
#define GOTO  5
#define EOL   6
#define FINISHED  7
#define LET 8
#define END 9

// main functions definitions
char *find_label(); 
void print(); 
void scan_labels(); 
void find_eol(); 
void exec_goto();
void exec_if();
void exec_else();
void exec_let();
void label_init();
void serror();
void get_exp(); 
void putback();
void level2(); 
void level3(); 
void level4(); 
void level5(); 
void level6(); 
void primitive();
void unary(char o, int *r);
void arith(char o, int *r,int *h);
char load_program(char *p, char *fname);
char isdelim(char c);
char iswhite(char c);
char get_token();
void assignment();
char look_up(char *s);


char *prog;  /* holds expression to be analyzed */

int variables[26]= {    /* 26 user variables,  A-Z */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

struct commands { /* keyword lookup table */
  char command[6];
  char tok;
} table[] = { /* Commands must be entered lowercase */
  "print", PRINT, /* in this table. */
  "if", IF,
  "then", THEN,
  "else", ELSE,
  "goto", GOTO,
  "let", LET,
  "end", END,
  "", END  /* mark end of table */
};

char token[80]; 	//Buffer for token
char token_type, tok;

struct label { 	//Label table
  char name[LAB_LEN];
  char *p;  /* points to place to go in source file*/
};
struct label label_table[NUM_LAB]; 	// Массив меток

void main(int argc, char *argv[])
{
  char in[80];
  int answer;
  char *p_buf;
  char *t;
  /* check that the file name is specified */
  if(argc!=2){
    printf("usage: run <filename>\n");
    exit(1);
  }

  /* allocate memory for the program */
  if(!(p_buf=(char *) malloc(PROG_SIZE))){
    printf("allocation failure");
    exit(2);
  }
  
  /* load the program to execute */
  if(!load_program(p_buf,argv[1])){
  	exit(1);
  }

  prog = p_buf;
  scan_labels(); /* find the labels in the program */

  do{
    token_type = get_token();
    /* check for assignment statement */
    if(token_type==VARIABLE){
      serror(2);	
	}else{ /* if is command then execute code */
	    switch(tok){
	    case PRINT:
		  print();
	  	  break;
	    case GOTO:
		  exec_goto();
		  break;
		case IF:
		  exec_if();
		  break;
		case ELSE:
		  exec_else();
		  break;	
	    case LET:
		  exec_let();
		  break;
		case END:
		  exit(0);
      	}
    }
  }while (tok != FINISHED);
}

/* Load a program. */
char load_program(char *p, char *fname){
  FILE *fp;
  int i=0;
  if(!(fp=fopen(fname, "rb"))){
  	return 0;
  }
  i = 0;
  do{
    *p = getc(fp);
    p++; 
    i++;
  }while((!feof(fp))&&(i<PROG_SIZE));
  *(p-2) = '\0'; /* null terminate the program */
  fclose(fp);
  return 1;
}

/* Assign a variable a value. */
void assignment()
{
  int var, value;
  /* get the variable name */
  get_token();
  if(!isalpha(*token)) {
    serror(4);
    return;
  }
  var = toupper(*token)-'A';
  /* get the equals sign */
  get_token();
  if(*token!='=') {
    serror(3);
    return;
  }
  /* get the value to assign to var */
  get_exp(&value);
  /* assign the value */
  variables[var] = value;
}

/* Execute a simple version of the BASIC PRINT statement */
void print()
{
  int answer;
  int len=0, spaces;
  char last_delim;
  do{
    get_token(); /* get next list item */
    if((tok==EOL)||(tok==FINISHED)){
	  break;
    } 
    if(token_type==QUOTE){ /* is string */
      printf(token);
      len += strlen(token);
      get_token();
    }else{ /* is expression */
      putback();
      get_exp(&answer);
      get_token();
      len += printf("%d", answer);
    }
    last_delim = *token; 

    if(*token==';') {
      /* compute number of spaces to move to next tab */
      spaces = (8 - (len % 8)); 
      len += spaces; /* add in the tabbing position */
      while(spaces){ 
		printf(" ");
        spaces--;
      }
    }else if(*token==','){ /* do nothing */
	}else if((tok!=EOL)&&(tok!=FINISHED)){
	  break;
	} 
  }while((*token==';')||(*token==','));
  
  if((tok==EOL)||(tok==FINISHED)) {
    if((last_delim != ';')&&(last_delim!=',')){
	  printf("\n");
    }
  }else{
  	putback();
  	printf("\n");
  }
}

/* Find all labels in file. */
void scan_labels()
{
  int addr;
  char *temp;

  label_init();  /* zero all labels */
  temp = prog;   /* save pointer to top of program */

  /* if the first token in the file is a label */
  get_token();
  if(token_type==NUMBER){
    strcpy(label_table[0].name,token);
    label_table[0].p=prog;
  }

  find_eol();
  do{     
    get_token();
    if(token_type==NUMBER) {
      addr = get_next_label(token);
      if((addr==-1)||(addr==-2)) {
          (addr==-1) ?serror(5):serror(6);
      }
      strcpy(label_table[addr].name, token);
      label_table[addr].p = prog;  /* current point in program */
    }
    /* if not on a blank line, find next line */
    if(tok!=EOL){
	  find_eol();
	}
  }while(tok!=FINISHED);
  prog = temp;  /* restore to original */
}

/* Find the start of the next line. */
void find_eol(){
  while((*prog!='\n')&& (*prog!='\0')){
  	++prog;
  }
  if(*prog){
  	prog++;
  }
}

/* Return index of next free position in label array. 
   A -1 is returned if the array is full.
   A -2 is returned when duplicate label is found.
*/
int get_next_label(char *s){
  int t;
  for(t=0;t<NUM_LAB;++t) {
    if(label_table[t].name[0]==0){
		return t;
    }
    if(!strcmp(label_table[t].name,s)){
		return -2; /* dup */
    }
  }
  return -1;
}

/* Find location of given label.  A null is returned if
   label is not found; otherwise a pointer to the position
   of the label is returned.
*/
char *find_label(char *s){
  int t;
  for(t=0; t<NUM_LAB; ++t){
    if(!strcmp(label_table[t].name,s)){
		return label_table[t].p;
    }
  }
  return '\0'; /* error condition */
}

/* Execute a GOTO statement. */
void exec_goto()
{
  char *loc;
  get_token(); /* get label to go to */
  /* find the location of the label */
  loc = find_label(token);
  if(loc=='\0'){/* label not defined */
    serror(7);
  }else{ /* start program running at that loc */
    prog=loc;
  }
}

/* Initialize the array that holds the labels. 
   By convention, a null label name indicates that
   array position is unused.
*/
void label_init()
{
  int t;
  for(t=0; t<NUM_LAB; ++t){
  	label_table[t].name[0]='\0';
  }
}

/* Execute an IF statement. */
void exec_if(){
  int x , y, cond;
  char op;
  get_exp(&x); /* get left expression */
  get_token(); /* get the operator */
  if(!strchr("=<>", *token)) {
    serror(0); /* not a legal operator */
    return;
  }
  op=*token;
  get_exp(&y); /* get right expression */

  /* determine the outcome */
  cond = 0;
  switch(op) {
    case '<':
      if(x<y) cond=1;
      break;
    case '>':
      if(x>y) cond=1;
      break;
    case '=':
      if(x==y) cond=1;
      break;
  }
  if(cond) { /* is true so process target of IF */
    get_token();
    if(tok!=THEN) {
      serror(8);
      return;
    }/* else program execution starts on next line */
  }else{ /* search ELSE or start of next line */
    do{
      get_token(); /* get next list item */
    }while((tok!=ELSE)&&(tok!=EOL)&&((tok!=FINISHED)));
  }
}


/* Execute an ELSE statement for some cases. */
void exec_else(){
  find_eol(); /* find start of next line */
}

/* Execute an LET statement. */
void exec_let(){
  token_type = get_token();
  if(token_type!=VARIABLE){
	serror(4);
  }
  putback(); /* return the var to the input stream */
  assignment(); /* must be assignment statement */
}


/* Entry point into parser. */
void get_exp(int *result){
  get_token();
  if(!*token){
    serror(2);
    return;
  }
  level2(result);
  putback(); /* return last token read to input stream */
}


/* display an error message */
void serror(int error){
  static char *e[]= {   
    "syntax error", 
    "unbalanced parentheses", 
    "no expression present",
    "equals sign expected",
    "not a variable",
    "Label table full",
    "duplicate label",
    "undefined label",
    "THEN expected"
  }; 
  printf("%s\n", e[error]);
  exit(3);
}

/* Get a token. */
char get_token(){
  char *temp;
  token_type=0; tok=0;
  temp=token;

  if(*prog=='\0') { /* end of file */
    *token=0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }

  while(iswhite(*prog)){ /* skip over white space */
  	++prog;
  }  

  if(*prog=='\r') { /* crlf */
    ++prog; ++prog;
    tok = EOL;
	*token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),><", *prog)){ /* delimiter */
    *temp=*prog;
    prog++; /* advance to next position */
    temp++;
    *temp=0; 
    return (token_type=DELIMITER);
  }
    
  if(*prog=='"') { /* quoted string */
    prog++;
    while((*prog!='"')&&(*prog!='\r')){
		*temp++=*prog++;
	}
    if(*prog=='\r'){
		serror(1);
	}
    prog++;
	*temp=0;
    return(token_type=QUOTE);
  }
  
  if(isdigit(*prog)){ /* number */
    while(!isdelim(*prog)){
	  *temp++=*prog++;
    }
    *temp = '\0';
    return(token_type = NUMBER);
  }

  if(isalpha(*prog)) { /* var or command */
    while(!isdelim(*prog)){
	  *temp++=*prog++;
    }
    token_type=STRING;
  }

  *temp = '\0';

  /* see if a string is a command or a variable */
  if(token_type==STRING) {
    tok=look_up(token); /* convert to internal rep */
    if(!tok){
      token_type = VARIABLE;
    }else{ 
      token_type = COMMAND; /* is a command */
    }
  }
  return token_type;
}



/* Return a token to input stream. */
void putback(){
  char *t; 
  t = token; 
  for(;*t;t++){
  	prog--;
  } 
}

/* Look up a a token's internal representation in the
   token table.
*/
char look_up(char *s){
  int i,j;
  char *p;
  /* convert to lowercase */
  p = s;
  while(*p){ 
  	*p = tolower(*p); 
	p++; 
  }

  /* see if token is in table */
  for(i=0; *table[i].command; i++){
	if(!strcmp(table[i].command, s)){
		return table[i].tok;
	}
  }
  return 0; /* unknown command */
}

/* Return true if c is a delimiter. */
char isdelim(char c){
  return (strchr(" ;,+-<>/*%^=()", c)||(c==9)||(c=='\r')||(c==0));
}

/* Return true if c is space or tab. */
char iswhite(char c){
  return ((c==' ')||(c=='\t'));
}


/*  Add or subtract two terms. */
void level2(int *result){
  char  op; 
  int hold; 
  level3(result); 
  while(((op = *token) == '+')||(op == '-')){
    get_token(); 
    level3(&hold); 
    arith(op, result, &hold);
  }
}

/* Multiply or divide two factors. */
void level3(int *result){
  char  op; 
  int hold;
  level4(result); 
  while(((op = *token) == '*')||(op == '/')||(op == '%')){
    get_token(); 
    level4(&hold); 
    arith(op, result, &hold); 
  }
}

/* Process integer exponent. */
void level4(int *result){
  int hold; 
  level5(result); 
  if(*token== '^'){
    get_token(); 
    level4(&hold); 
    arith('^', result, &hold); 
  }
}

/* Is a unary + or -. */
void level5(int *result){
  char  op; 
  op = 0; 
  if((token_type==DELIMITER) && (*token=='+') || (*token=='-')){
    op = *token; 
    get_token(); 
  }
  level6(result); 
  if(op){
  	unary(op, result);
  }   
}

/* Process parenthesized expression. */
void level6(int *result){
  if((*token == '(') && (token_type == DELIMITER)){
    get_token(); 
    level2(result); 
    if(*token != ')'){
      serror(1);
    }
    get_token(); 
  }else{
  	primitive(result);
  }
}

/* Find value of number or variable. */
void primitive(int *result){
  switch(token_type) {
  case VARIABLE:
    *result = find_var(token);
    get_token(); 
    return; 
  case NUMBER:
    *result = atoi(token);
    get_token();
    return;
  default:
    serror(0);
  }
}

/* Perform the specified arithmetic. */
void arith(char o, int *r,int *h)
{
  int t, ex;
  switch(o) {
    case '-':
      *r = (*r)-(*h); 
      break; 
    case '+':
      *r = (*r)+(*h); 
      break; 
    case '*':  
      *r = (*r)*(*h); 
      break; 
    case '/':
      *r = (*r)/(*h);
      break; 
    case '%':
      t = (*r)/(*h); 
      *r = *r-(t*(*h)); 
      break; 
    case '^':
      ex = *r; 
      if(*h==0) {
        *r = 1; 
        break; 
      }
      for(t=*h-1; t>0; --t){
	  	*r = (*r) * ex;
	  }
      break;       
  }
}

/* Reverse the sign. */
void unary(char o, int *r){
  if(o=='-'){
  	*r = -(*r);
  }
}

/* Find the value of a variable. */
int find_var(char *s){
  if(!isalpha(*s)){
    serror(4); /* not a variable */
    return 0;
  }
  return variables[toupper(*token)-'A'];
}
