#include "../types.h"

#include "random.h"

#define min(a, b) (a < b ? a : b)
#define abs(x) (0 < x ? x : (-x))

// Algoritmo usado: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
int
next_random(int x)
{
  // Casteo a uint, por qud necesito que el >> agregue siempre 0 a la izquierda
  uint a = (uint)(x == 0 ? 1 : x);
  a = (a >> 1) | (((a >> 0) ^ (a >> 2) ^ (a >> 3) ^ (a >> 5)) << 15);

  return((int)a);
}

int
new_random(void)
{
  global_seed = next_random(global_seed);
  return(global_seed);
}

/* Obtiene el menor x positivo tal que 2^x >= abs(n)
 */
static int
next_power_of_2(int n)
{
  n = abs(n);
  int res = 0;
  while((1 << res) < n){ // 1 << res es lo mismo que 2^res (<< es un LSL)
    res++;
  }
  return(res);
}

int
new_random_less_than(int n)
{
  int shift = 8*sizeof(int) - next_power_of_2(n) - 1;
  int res;
  do{
    /* Se crea un nuevo número aletaorio positivo, y se le hace un shift para
       que quede entre 0 y next_power_of_2(n). Se hace eso las veces haga falta
       para que quede entre 0 y n */
    res = new_random();
    res = res >> shift;
  } while(abs(res) > abs(n));
  return(res);
}
