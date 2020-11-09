//----------USER define------------
#define DEBUG       1    // print auxiliary text or not
#define TEXT_SIZE 64     // size of input text in bytes
#define SPILTOR   " "    // a character that spilts input array, for example "build 1+3i 3-2i"
//------END USER define------------

#define CMD_BUILD     "build"
#define CMD_IS_EMPTY  "isempty"
#define CMD_INSERT    "insert"
#define CMD_SEARCH    "search"
#define CMD_DELETE    "rm"
#define CMD_PRINT     "print"
#define CMD_COUNT     "count"
#define CMD_MID       "middle"
#define CMD_EXIT      "exit"
#define CMD_HELP      "help"
#define HELP_MSG " \
  Usage:[OPTION] [COMPLEX]... \n\
  \n\
  [COMPLEX]...\n\
  One or more than one complex number with the format \"a+bi\", \"a\" or \"bi\". \n\
  A is the real part and b is the imaginary part.\n\
  Multiple complex numbers should be spilted with space. Here is an example:\n\
    10+i i -3.2i -3.5+9.7i 87\n\
  \n\
  [OPTION]\n\
  help                  Show this help. \n\
  build [COMPLEX]...    Build a doubly linked list storing comlex numbers, which are [COMPLEX]... \n\
                        Build an empty list if [COMPLEX]... is not supplied.\n\
  isempty               Print \"The list is empty.\" or \"The list is not empty.\". \n\
  insert [COMPLEX]...   Insert one or multiple complex numbers into the end of the doubly list. \n\
  print                 Print the whole list with indexing starting from zero. \n\
                        Each complex number is printed with its index and newline. \n\
"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Structure of doubly linked list of complex number
typedef struct dlistComplex{
  float real;      // real part of complexnumber
  float img;       // imaginary part of complexnumber
  struct dlistComplex *L; // left  link, pointing to prevous node
  struct dlistComplex *R; // right link, pointing to next node
} dlistComplex;

// Private functions
void trim_end(char *text);
char** str_to_str_array(char* str, char* spiltor, unsigned int counts);
int str2complex(const char *str, float *real, float *img);
dlistComplex *insert_complex(dlistComplex *trail, float real, float img);
dlistComplex *insert_str_arr(dlistComplex *trail, char **arr_str, int counts);


int main(){
  char raw_input[TEXT_SIZE];      // string read from input
  char **arr_str;                 // string array, pointer of char*
  int counts = 1;                 // counts of how many command inputs, ie. "build 1+5i 9+2i" for counts=3
  dlistComplex *head = NULL;      // head node, just an entry. head->L has the 1st data.
  dlistComplex *current = NULL;   // current node

  printf("Welcome message blablabla\n");
  printf(HELP_MSG);
  
  // keep reading input and execute until command "exit".
  while(1){
    // Read user input
    printf(">> "); fflush(stdout);
    fgets(raw_input, TEXT_SIZE-1, stdin);
    trim_end(raw_input);

    // Count for how many spilted command
    counts = 1;
    for(unsigned int i=0; i<TEXT_SIZE-1; i++){
      if     (raw_input[i]==SPILTOR[0])  counts++;
      else if(raw_input[i]=='\0')        break;
    }
    if(DEBUG) printf("Item counts:%d\n", counts);

    // Convert user input to string array
    arr_str = str_to_str_array(raw_input, SPILTOR, counts);
    if(DEBUG){
      for(int i=0; i<counts; i++)
        printf("Input item %d: %s\n", i, arr_str[i]);
    }

    // Command implementation
    if     (!strcmp(arr_str[0], CMD_BUILD)){
      // Create a list if it's empty
      if(!head){
        head = insert_complex(head, 0, 0);  // real=img=0 cuz head contains nothing.
        current = head;
        // Insert the complex numbers if there is any
        // arr_str[0] is command, complex numbers start from arr_str[1]
        // counts-1 cuz the steps from arr_str[1] to end is counts-1
        current = insert_str_arr(current, arr_str+1, counts-1);
      }
      else printf("The list already created.\n");
    }
    else if(!strcmp(arr_str[0], CMD_IS_EMPTY)){
      if(!head)   printf("The list is empty.\n");
      else        printf("The list is not empty.\n");
    }
    else if(!strcmp(arr_str[0], CMD_INSERT)){
      if(!head)   printf("The list is empty. Type \"build\" to build a list.\n");
      else        current = insert_str_arr(current, arr_str+1, counts-1);
    }
    else if(!strcmp(arr_str[0], CMD_SEARCH)){       // TODO
      printf("%s not implemented yet.\n", arr_str[0]);
    }
    else if(!strcmp(arr_str[0], CMD_DELETE)){       // TODO
      printf("%s not implemented yet.\n", arr_str[0]);
    }
    else if(!strcmp(arr_str[0], CMD_PRINT)){
      if(!head)   printf("The list is empty.\n");
      else{       // non empty list
        dlistComplex *temp = head->R;
        int i=0;
        for(temp=temp; temp!=head; temp=temp->R){
          i++;
          printf("Node %d:%g%+gi\n", i, temp->real, temp->img);
        }
      }
    }
    else if(!strcmp(arr_str[0], CMD_COUNT)){        // TODO
      printf("%s not implemented yet.\n", arr_str[0]);
    }
    else if(!strcmp(arr_str[0], CMD_MID)){          // TODO
      printf("%s not implemented yet.\n", arr_str[0]);
    }
    else if(!strcmp(arr_str[0], CMD_HELP))  printf(HELP_MSG);
    else if(!strcmp(arr_str[0], CMD_EXIT))  break;
    else                                    printf("Unkonw command \"%s\", try \"help\" to get more information.\n", arr_str[0]);
  }


  // free the memory and leave the program
  for(unsigned int i=0; i<counts; i++)
    free(arr_str[i]);
  free(arr_str);
  printf("\n\nProgram ends here.\n");
}


/* Insert the string array as nodes to trail and return the new trail */
dlistComplex *insert_str_arr(dlistComplex *trail, char **arr_str, int counts){
  float real=0, img=0;
  for(int i = 0; i<counts; i++){
    real = 0;
    img = 0;
    if(str2complex(arr_str[i], &real, &img)!=0)
      printf("@line%d Wrong format of complex number at index of %d:%s. 0+0i inserted.\n", __LINE__, i, arr_str[i]);
    trail = insert_complex(trail, real, img);
    if(DEBUG)   printf("Complex %d: %2.2f, %2.2fi\n", i, trail->real, trail->img);
  }
  return trail;
}

/* Create a node, insert it to after trail and return the new trail pointer.
   */
dlistComplex *insert_complex(dlistComplex *trail, float real, float img){
  dlistComplex *temp = malloc(sizeof(dlistComplex));
  temp->real = real;
  temp->img  = img;
  if(!trail){
    trail = temp;
    trail->L = trail;
    trail->R = trail;
  }

  // original: (trail)<=>(trail->R)
  // inserted: (trail)<=>(temp)<=>(trail->R)
  else{
    // two links(in/out) at right side of temp
    temp->R       = trail->R;
    (trail->R)->L = temp;
    
    // two links(in/out) at left side of temp
    temp->L       = trail;
    trail->R      = temp;
  }

  return temp;
}

/* Convert string to complex
 @return 0 on success, -1 on string format error*/
int str2complex(const char *str, float *real, float *img){
  int i;                  // iterater of for
  int cnt = 0;            // count for specific char
  const int len = strlen(str);
  *real = 0;
  *img = 0;

  // Check for length of str
  if(len<1){
    if(DEBUG)   printf("@line%d Wrong format, empty string.\n", __LINE__);
    return -1;
  }

  // Check for character if it's not one of '0'~'9', 'i', '+', '-' nor '.'
  for(i=0; i<len; i++){
    if(!('0'<=str[i] && str[i]<='9') && str[i]!='i' &&  str[i]!='+' &&  str[i]!='-' &&  str[i]!='.'){
      if(DEBUG)   printf("@line%d Wrong format, character is not 0~9 nor \'i\'.\n", __LINE__);
      return -1;
    }
  }

  // 'x+yi', '[+-]x[+-]yi'
  // sscanf()==2 means real part and img part are converted from the input string
  if(sscanf(str, " %f %f", real, img)==2){
    return 0;
  }

  // 'i', '+i' or '-i'
  if(str[0]=='i'){
    *real = 0;
    *img = 1;
    return 0;
  }
  else if( (str[0]=='+'||str[0]=='-') && str[1]=='i'){
    *real = 0;
    *img = str[0]=='+' ? 1:-1;
    return 0;
  }

  // '+10i', '-4i' or '3i'
  // i=1 to skip the first possible sign(+-)
  cnt = 0; // count for [+-]
  for(i=1; i<len; i++){
    if(str[i]=='+' || str[i]=='-'){
      cnt++;
      break;
    }
  }
  if(!cnt && str[len-1]=='i'){
    *real = 0;
    sscanf(str, " %f", img);
    return 0;
  }

  // '+10', '3', '-5' real part only (no 'i')
  for(i=0; i<len && str[i]!='i'; i++);
  if(i==strlen(str)){
    *img = 0;
    sscanf(str, " %f", real);
    return 0;
  }
  
  // '10+i', '-1-i'. abs(img)==1
  cnt = 0;
  for(i=1; i<len; i++){
    if(str[i]=='+' || str[i]=='-'){
      sscanf(str, " %f", real);
      *img = str[i]=='+' ? 1:-1;
      return 0;
    }
  }

  *real = 0;
  *img = 0;
  if(DEBUG)   printf("@line%d Wrong format, no format match.\n", __LINE__);
  return -1; // no format match, means str has incorrect format
}

/* Return an string(char*) array, whoes elements are parsed from str
  @str: a string containing elements spilted with the @spiltor
  @spiltor: a string which seperates the elements in @str
  @counts: how many elements are there in @str

  example:
    arr=str_to_str_array("hello 1 2 3 world", " ", 5);
    arr: {"hello", "1", "2", "3", "world"};

  note: remember to free each elements and arr itself
*/
char** str_to_str_array(char *str, char* spiltor, unsigned int counts){
  char** str_arr;
  char* item;     // an item in the input array
  str_arr=calloc(counts, sizeof(char*));
  item=strtok(str, (char*)spiltor);  //get the first item in text
  for(unsigned int i=0; i<counts; i++){
    // strlen(item)+1, +1 is due to '\0'
    str_arr[i]=calloc(strlen(item)+1, sizeof(char));
    strcpy(str_arr[i], item);
    item=strtok(NULL, (char*)spiltor);
  }
  return str_arr;
}

/* Remove the ending space characters of a string.
  Replacing the ending spaces, newlines with \0
*/
void trim_end(char *text){
  int i = 0;
  for(i = strlen(text)-1; text[i] == ' '|text[i] == '\n'|text[i] == '\r'; i--);
  text[i+1] = '\0';
}


