#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/*
 * These are defined for each TIP program in the compiled code.
 */
int64_t _tip_main();
extern int64_t _tip_num_inputs;
extern int64_t _tip_input_array[]; 

/* 
 * runtime library functions for TIP IO expressions and statements
 *    x = input;
 *    output y;
 *    error y;
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

/*
 * If the compiled program has no "main" function then one is created
 * that calls this function.
 */
void _tip_main_undefined() {
  printf("Error: missing main function\n"); 
  exit(-1);
}

/*
 * Set up the arguments to be read by the TIP "main" function.
 * The number of arguments is defined by the compiled TIP code
 * and read here to perform error checking.  All arguments to
 * the TIP "main" are passed through the "_tip_input_array".
 * Finally, the TIP "main" is renamed during compilation to "_tip_main",
 * its arguments are removed, and code to read them from "_tip_input_array"
 * is generated.
 */
int main(int argc, char *argv[]) {
  // Throw an error if the wrong number of arguments are passed
  if (argc != _tip_num_inputs + 1) {
     printf("expected %" PRId64 " integer arguments\n", _tip_num_inputs);
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

