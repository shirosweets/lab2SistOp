#ifndef RANDOM_H
#define RANDOM_H

/* Semilla por defecto, se va modificando cada ves que se genera un número aleatorio
 * Modificar para reseetar semilla
 */
int global_seed = 0;

/* Obtiene un número aleatorio a partir de x
 * Pasarle el mismo x produse el mismo resultado, pero un pequeño cambio en x
 * cambia completamente el resultado
 */
int next_random(int x);

/* Obtiene un nuevo número aleatorio a partir de la semilla y modifica la semilla
 */
int new_random(void);

/* Obtiene en número aleatorio entre -n y n (incluidos)
 * Lo hace con la semilla, y la modifica
 *
 * POS: -abs(n) <= res && res <= abs(n)
 */
int new_random_less_than(int n);

#include "random.c"
#endif // RANDOM_H
