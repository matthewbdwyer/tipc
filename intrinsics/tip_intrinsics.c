#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/*
 * These are defined for each TIP program.
 */
int64_t _tip_main();
extern int64_t _tip_num_inputs;
extern int64_t _tip_input_array[]; 

/* 
 * intrinsic functions for TIP I/O/E
 *    x = input;
 *    output y;
 *    error y;
 *
 * Note that these are reserved words in TIP, so there will be no
 * naming conflicts.
 */
int64_t _tip_input() {
  int64_t x;
  printf("Enter input: ");
  scanf("%" SCNd64, &x);
  return x;
}

void _tip_output(int64_t x) {
  printf("Program output: %" PRId64 "\n", x); 
}  

void _tip_error(int64_t x) {
  printf("[error] Error: Execution error, code: %" PRId64 "\n", x); 
  exit(-1);
}

void _tip_main_undefined() {
  printf("Error: missing main function\n"); 
  exit(-1);
}

int main(int argc, char *argv[]) {
  // Throw an error if the wrong number of arguments are passed
  if (argc != _tip_num_inputs + 1) {
     printf("expected %ld integer arguments\n", _tip_num_inputs);
     exit(-1);
  }

  // required by strtoll, but discarded
  char *eptr;

  for (size_t i=0; i < _tip_num_inputs; i++) {
    _tip_input_array[i] = strtoll(argv[i+1], &eptr, 10);
  }
  
  printf("Program output: %" PRId64 "\n", _tip_main());

  return 0;
}

