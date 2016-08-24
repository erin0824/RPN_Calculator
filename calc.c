#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// define constant for maximum arg length
#define MAX_ARG_LENGTH 100

// create stack structure
struct struct_stack {
  double value;
  struct struct_stack *next;
};

struct struct_stack *head; // pointer to head of linked list
double popped; // variable for popped elmt
bool err; // boolean to check if operation valid

// define stack operations: pop, peek, push

// return element at top of stack
void pop() {
  struct struct_stack *curr;
  if (head == NULL) {
    err = true; // mark err if invalid
    printf("STACK EMPTY\n");
  } else {
    popped = head -> value; // store variable obtained from pop
    curr = head -> next;
    free(head); // free memory of popped elmt
    head = curr; // set new head
    err = false;
    // printf("here\n");
  }
}

// print element at top of stack
void peek() {
  // double peeked;
  if (head == NULL) {
    return;
  } else {
    // peeked = head -> value; // store top elmt in temp variable
    printf("%lf\n", head -> value); // print top elmt
  }
}

void push(double pushed) {
  // allocate memory
  struct struct_stack *curr = malloc(sizeof(struct struct_stack));
  if (curr == NULL) { // invalid return from malloc
    fprintf(stderr, "OUT OF MEMORY\n");
    exit(1);
  }
  if (head == NULL) { // empty linked list
    curr -> value = pushed;
    curr -> next = NULL;
    head = curr; // make elmt head
  } else { // non-empty linked list
    curr -> value = pushed; // store input elmt
    curr -> next = head; // push down existing top elmt
    head = curr; // set new head
  }
}

// create queue structure
struct struct_queue {
  char val[MAX_ARG_LENGTH];
  struct struct_queue *next;
};

struct struct_queue *begin; // pointer to beginning of queue
struct struct_queue *end; // pointer to end of queue
char removed[MAX_ARG_LENGTH]; // elmt from deque
bool fail; // boolean to check if queue operation is valid

// define queue operations: enqueue, dequeue, peek
void enqueue(char *added) {
  // allocate memory for queue node
  struct struct_queue *now = malloc(sizeof(struct struct_queue));

  if (now == NULL) {
    fprintf(stderr, "OUT OF MEMORY\n");
    exit(1);
  }

  now -> next = NULL; // for next ptr
  strcpy(now -> val, added); // store text of queue node in another string

  if (begin == NULL) { // empty queue
    begin = now;
    end = now;
  } else { // non-empty queue
    end -> next = now;
    end = now;
  }
}

void dequeue() {
  struct struct_queue* now;

  if (begin == NULL) { // error bc nothing to dequeue
    fprintf(stderr, "QUEUE EMPTY\n");
    return;
  }

  strcpy(removed, begin -> val);

  if (begin == end) {
    // queue empty after dequeing only elmt
    free(begin);
    begin = NULL;
    end = NULL;
  } else { // dequeue head elmt in queue
    now = begin -> next;
    free(begin);
    begin = now;
  }
}

void free_queue(struct struct_queue* queue) { // helper for free_entire_queue
  if (queue == NULL) {
    fprintf(stderr, "QUEUE EMPTY\n");
    return;
  }
  free(queue -> next);
  free(queue);
}

void free_entire_queue() { // empties entire queue
  free_queue(end);
}

void look() { // displays elmt at head of queue
  if (begin == NULL) {
    return;
  } else {
    printf("%s\n", begin -> val);
  }
}

int main() {
  char arg[MAX_ARG_LENGTH];
  char args[MAX_ARG_LENGTH];
  char temp[MAX_ARG_LENGTH];
  char output[MAX_ARG_LENGTH];
  char call[MAX_ARG_LENGTH];
  bool func_exists = false;
  int i;
  double value, first, second;
  struct struct_stack *nn;
  struct struct_queue *np;
  char *p;
  char *invalid;
  bool init;

  while (1) {
    printf("Type into calculator\n"); // displayed on screen when running calc
    fgets(args, MAX_ARG_LENGTH, stdin); // read input as string
    // capitalize all char in input string to make not case-sensitive
    for (i = 0; i < strlen(args); i++) {
      args[i] = toupper(args[i]);
    }
    // make a copy of the original input for strtok to keep an unmodified copy
    strcpy(temp, args);
    // if (init) {
    //   strcpy(args, call);
    //   printf("called\n");
    // }
    for (char *p = strtok(args, " "); p != NULL; p = strtok(NULL, " ")) {
      sscanf(p, "%s", arg);
      if(strcmp (arg, "DEF") == 0) {
        for (char *p = strtok(temp, " "); p != NULL; p = strtok(NULL, " ")) {
          // printf("this is temp:%s\n", p);
          sscanf(p, "%s", output);
          // if (func_exists) { // if function already exists, empty queue
          //   free_entire_queue();
          // }
          // enqueue all args into stack other than keyword "DEF"
          if (strcmp(output, "DEF") != 0) {
            enqueue(output);
          }
        }
        func_exists = true; // function has been stored in queue
      } else if(strcmp(arg, "APPEND_FUNC") == 0) {
        for (char *p = strtok(temp, " "); p != NULL; p = strtok(NULL, " ")) {
          if (func_exists) { // function must exist for arg to be valid
            sscanf(p, "%s", arg);
            // enqueue inputs after keyword "APPEND_FUNC"
            if (strcmp(arg, "APPEND_FUNC") != 0) {
              enqueue(arg);
            }
          } else { // otherwise nothing to enqueue
            fprintf(stderr, "NO FUNCTION DEFINED\n");
          }
        }
      } else if (strcmp(arg, "REMOVE_ARG") == 0) {
        // if function exists and queue is not empty
        if (func_exists) {
          dequeue();
        } else { // otherwise nothing to remove
          fprintf(stderr, "NO FUNCTION DEFINED\n");
        }
      } else if(isdigit(*arg) != 0 || strncmp(arg, "-0", 2) == 0 ||
      strncmp(arg, "-1", 2) == 0 || strncmp(arg, "-2", 2) == 0 ||
      strncmp(arg, "-3", 2) == 0 ||strncmp(arg, "-4", 2) == 0 ||
      strncmp(arg, "-5", 2) == 0 || strncmp(arg, "-6", 2) == 0 ||
      strncmp(arg, "-7", 2) == 0 || strncmp(arg, "-8", 2) == 0 ||
      strncmp(arg, "-9", 2) == 0 ) {
        // push into stack only if input is a number
        sscanf(arg, "%lf%s", &value, invalid);
        // indicates input is negative
        if (strncmp(arg, "-", 1) == 0) {
          push(0 - value); // push in negative number
        }
        push(value); // push value into stack
        peek(); // display number at top of stack
      } else if (strcmp(arg, "PRINT_FUNC") == 0) {
        if (!func_exists) { // error msg if no function exists
          fprintf(stderr, "NO FUNCTION DEFINED\n");
        } else {
          np = begin;
          while (np != NULL) { // traverse while printing until null reached
            printf("%s\n", np -> val);
            np = np -> next;
          }
        }
        arg[0] = '\0'; // reset arg
      }
      else if (strcmp(arg, "CALL_FUNC") == 0) {
        if (!func_exists) {
          fprintf(stderr, "NO FUNCTION DEFINED\n");
          exit(3);
        }
        np = begin;
        for (np = begin; np != NULL; ) {
          for (i = 0; i < MAX_ARG_LENGTH; i++) {
            call[i] = (np -> val)[i];
          } np = np -> next;
        }
        init = true;
        // otherwise, match string with following operations:
      } else if (strcmp(arg, "SIN") == 0) { // perform sin on input
        pop();
        if (!err) {
          push(sin(popped));
          peek();
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "COS") == 0) { // perform cos on input
        pop();
        if (!err) {
          push(cos(popped));
          peek();
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "TAN") == 0) { //perform tan on input
        pop();
        if (!err) {
          push(tan(popped));
          peek();
        }
      } else if (strcmp(arg, "SQRT") == 0) {
        pop();
        if (!err) {
          if (popped > 0 ) { // divide top entry by second
            push(sqrt(popped));
            peek();
          } else {
            fprintf(stderr, "STACK ERROR\n");
          }
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "POW") == 0) {
        pop();
        if(!err) {
          first = popped;
          pop();
          if (!err) {
            second = popped;
            push(pow(second, first)); //  second^first
            peek();
          } else {
            push(first);
          }
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "DUP") == 0) {
        pop();
        if (!err) {
          push(popped); // push elmt in twice to create copy
          push(popped);
          peek();
        }
      } else if (strcmp(arg, "SWAP") == 0) {
        pop();
        if (!err) {
          first = popped;
          pop();
          if(!err) {
            second = popped;
            push(first); // push in elmnts
            push(second); // in reverse order from previously
            peek();
          } else {
            push(first);
          }
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "POP") == 0) {
        pop();
        if (!err) {
          peek();
        }
      } else if (strcmp(arg, "PRINT") == 0) {
        nn = head; // set temp node to head
        // traverse through linked list until null
        while (nn != NULL) {
          printf("%lf\n", nn -> value); // print all nodes
          nn = nn -> next;
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "QUIT") == 0) {
        nn = head;// set temp node to head
        // empty stack
        while(nn != NULL) {
          free(nn);
          head = head -> next;
          nn = head;
        }
        // empty entire queue
        free_entire_queue();
        return 0;
      } else if (strcmp(arg, "+") == 0) {
        pop();
        if (!err) {
          first = popped; // store first elmt, if exists
          pop();
          if (!err) {
            second = popped; // store second elmt, if exists
            push(first + second); // push in sum of two elmts
            peek();
          } else {
            push(first);
          }
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "-") == 0) {
        pop();
        if (!err) {
          first = popped; // store first elmt, if exists
          pop();
          if (!err) {
            second = popped; // store second elmt, if exists
            push(first - second); // push in diff of two elmts
            peek();
          } else {
            push (first);
          }
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "*") == 0) {
        pop();
        if (!err) {
          first = popped; // store first elmt, if exists
          pop();
          if (!err) {
            second = popped; // store second elmt, if exists
            push(second * first); // push in product of two elmts
            peek();
          } else {
            push (first);
          }
        }
        arg[0] = '\0'; // reset arg
      } else if (strcmp(arg, "/") == 0) {
        pop();
        if (!err) {
          first = popped; // store first elmt, if exists
          pop();
          if (!err) {
            second = popped; // store second elmt, if exists
            if (second != 0) {
              push(first / second); // push in quotient of two elmts
              peek();
            } else { // cannot divide by 0, so quit
              fprintf(stderr, "STACK ERROR\n");
            }
          } else {
            push (first);
          }
        }
        arg[0] = '\0'; // reset arg
      } else {
        fprintf(stderr, "INVALID COMMAND\n");
      }
    }
  }
}
