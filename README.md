# Homeworks of Datastructure, NTUST-ECE, 109
## Doubly linked list with complex number
  - Features  
  From the PDF: Consider a complex number system(ie. *x+iy*). For complex number data sets, create a user-friendly, menu-driven program that implements a doubly **linked list**, including the following operations. Please discuss the complexity for each operation.  
    - (a) Build a list by reading a sequence of complex numbers from input.  
    - (b) Determine if a list is empty.  
    - (c) Search for a complex number.  
    - (d) Search for a complex number.  
    - (e) Delete a complex number.  
    - (f) Print out a list.  
    - (g) Count the number of times a given complex number occurs in a list.  
    - (h) Find the middle element of a list.(If there are even nodes, then there would be two middle numbers.)  

### Usage
Compile and execute the code, and then it shows you how to perfome above features with specific commands.
```cmd
$ .\HW01_doubly_list_complex_num.exe
Welcome message blablabla
   Usage:[OPTION] [COMPLEX]... 
  
  [COMPLEX]
  Single complex.
  
  [COMPLEX]...
  One or more than one complex number with the format "a+bi", "a" or "bi". 
  a is the real part and b is the imaginary part.
  Multiple complex numbers should be spilted with space. Here is an example:
    10+i i -3.2i -3.5+9.7i 87
  
  [OPTION]
  help                  Show this help. 
  build [COMPLEX]...    Build a doubly linked list storing comlex numbers, which are [COMPLEX]...        
                        Build an empty list if [COMPLEX]... is not supplied.
  isempty               Print "The list is empty." or "The list is not empty.". 
  insert [COMPLEX]...   Insert one or multiple complex numbers into the end of the doubly list. 
  search [COMPLEX]      Search for a complex number. Return the mathed complex numbers with their indexs 
                        and the total matched count 
  rm [COMPLEX]          Delete a complex number in the doubly linked list.
  print                 Print the whole list with indexing starting from zero.
                        Each complex number is printed with its index and newline.
  count [COMPLEX]       Work like "search [COMPLEX]".
  middle [COMPLEX]      Find the middle element of a list.
                        (If there are even nodes, then there would be two middle numbers.)
>> 
```

### Discussion  
  - Flow  
  Briefly explanation of executing procedures of this code.
    > Blcok diagram
    > <img src="docs/pics/hw01_diagram.png" alt="drawing" width=""/>
    - **Initail**: The entry of the program, including declartions and initializing of local varibles. Then print the welcome and help messages.
    - **Read Input**: Wait fot and capture user input from `stdin` with `fgets()`.
    - **Command Parse**: Spilt the command into `[OPTION]` and `[COMPLEX]...`
    - **Exit**: If `[OPTION]` is `exit`, then free the dynamic allocated memories and exit.
    - **Command Execute**: If `[OPTION]` is not `exit`, execute a corresponding feature *((a)~(h) from above)* .

  - Time Complexity of custom functions  
    These are the functoins invoked by implementation of the features. So I will talk about the timecomplexity of these functions before we get to the time complexity of features. Here is the table of symbols and representations that will show up in the following discussion.  
    |Symbol|Represntation|
    |-     |-            |
    | O( ) | big-oh|
    | N    | Node counts in existing list |
    | n    | Input complex number counts. (counts of `[COMPLEX]...`)|
    | len  | Character counts of each `[COMPLEX]`|
    - `function`: big oh
    - `compare_complex`: O(1)
      No loop statement envoled.
      ```c
      /** Compare two complex number, return 0 if a==b
       *  @param a A complex number struct pointer
       *  @param b A complex number struct pointer
       *  @return Absolute difference between a and b.
       *          which is 0 if complex number a equals to complex number b
       * */
      unsigned int compare_complex(dlistComplex *a, dlistComplex *b){
        int real_diff = a->real - b->real;
        int  img_diff = a->img  - b->img;
        
        // limit the difference at positive or zero
        real_diff = real_diff>=0 ? real_diff : -1*real_diff;
        img_diff  =  img_diff>=0 ?  img_diff : -1*img_diff;

        return real_diff+img_diff;
      }
      ```
    - `str2complex`: O(len)
      `len` is the character counts of a input complex number `[COMPLEX]`. For example, for `3.4+i`, `len` is 5. There are several `for` loops with execution times of `len`, which leads to O(`len`).
      ```c
      /* Convert string to complex
        @return 0 on success, -1 on string format error*/
      int str2complex(const char *str, float *real, float *img);
      ```
      ```c 322
        // Check for character if it's not one of '0'~'9', 'i', '+', '-' nor '.'
        for(i=0; i<len; i++){
          if(!('0'<=str[i] && str[i]<='9') && str[i]!='i' &&  str[i]!='+' &&  str[i]!='-' &&  str[i]!='.'){
            if(DEBUG)   printf("@line%d Wrong format, character is not 0~9 nor \'i\'.\n", __LINE__);
            return -1;
          }
        }
      ```
    - `insert_complex`: O(1)
      No loop statement envoled.
      ```c
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
      ```
    - `ins_str_arr`: O(n)
      The for loop executes `counts` times, which is n, the input complex number counts. And each loop executes `insert_complex` once, results in O(n\*1) = O(n).
      ```c
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
      ```
    
    
    
    
    - Time Complexity of features  
      - (a) **Build**
      - (b) **Is Empty**
      - (c) **Insert**
      - (d) **Search**
      - (e) **Delete**
      - (f) **Print**
      - (g) **Count**
      - (h) **Middle**
