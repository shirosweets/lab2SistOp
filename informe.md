# Informe lab 2

- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**

---

Encargado del informe: Iván Renison

# Índice

Esquema del informe (para borrar):

Desarrollo inicial

    Parte 1

        Explicar que se podría haber hecho de forma muy simple, pero nosotros lo modularizamos un poco

        Explicar que primero lo hicimos en `console.c`, pero que después lo movimos a `VGA_reg.c`

    Parte 2

        Explicar como lo logramos al principio, y que si bien en ese momento lo hicimos en el `console.c`, después lo sacamos de ahí

    Parte 3

        Explicar como lo habíamos hecho al principio, y que después lo cambiamos

        Explicar que decidimos hacer una llamada al sistema `VGA_plot_screen`

    Parte 4

        Explicación breve de como empezamos

Extras en el kernel

    `stdin_read` (motivación y explicación)

    Uso de todos los modos de VGA

        Explicación de VGA

        Explicación de como usamos el `modes.c`

    Uso de la paleta completa

El kernel final y como usarlo

El flappy bird

    Cómo usarlo

    Cómo esta hecho

        Modularización

            Módulos mas "generales" (`random` y `VGA_graphics`)

            Módulos del flappy

Estilo del código

Nuestra forma de trabajar

# Desarrollo inicial

## Parte 1

    En esta parte el enunciado pide hacer una función que muestre un pie de pantalla que diga `SO2021` (el enunciado dice `SO2020`, pero suponemos que quedo así del año pasado) y llamarla al inicio del sistema.

    Para hacer esto se da la siguiente información:

    Se explica que VGA trabaja con buffers, en los cuales está guardada la información que se está mostrando por la pantalla. Que cuando VGA está configurado en modo texto de 80×25 caracteres (que es como viene configurado cuando se inicia el sistema) el buffer está en la dirección `0xB0000`. Y que cada elemento del buffer se compone de 2 bytes, uno para el código ASCII del caracter, y otro para los atributos, ósea, el color del texto, el color del fondo y la fuente que se usa.

    Se da esta imagen que explica mejor los bits de los elementos del buffer:

![bits_text_buffer.png](./Imagenes_informe/bits_text_buffer.png)

    Se da un ejemplo en el cuál se guardan caracteres en el buffer con esta forma:

```c
*(int *)P2V(0xB8000) = 0x4348;
```

    Nosotros decidimos modularizarlo un poco, haciendo 2 funciones auxiliares, `VGA_text_plot_letter` que pone una letra con sus atributos en una coordenada de la pantalla y `VGA_text_put_string` que pone un string en una coordenada de la pantalla dada. Esto nos pareció mejor que hacer como en el ejemplo, ya que así no hay que buscar el código ASCII de los caracteres que se ponen, y todo es menos repetitivo.

    Utilizando esas dos funciones hicimos la función `vgainit` para que muestre un el pie de pantalla así:

![pie_de_pantalla_vgainit.png](./Imagenes_informe/pie_de_pantalla_vgainit.png)

    Por último, al prototipo de la función `vgainit` lo agregamos a `defs.h` para que pueda ser usada desde cualquier lugar del kernel y la llamamos al comienzo de `main` en `main.c`.

    

    A esas funciones en ese momento las hicimos en `console.c`, pero después las movimos a `VGA_reg.c`, que es donde se pueden ver ahora, y también las modificamos un poco (ver explicación en "Extras en el kernel" (poner link)). Las versiones originales eran así:

```c
typedef struct{char s_ASCII_code; char s_atributes;} VGA_char;

#define VGA_text_array P2V(0xB8000)
#define VGA_text_array_pos(x, y) (VGA_char*)(VGA_text_array + (sizeof(VGA_char))*(x + y * 80))
#define VGA_text_width 80
#define VGA_text_height 25

static void
VGA_text_plot_letter(int x, int y, char letter, char atributes)
{
  // Solo se imprime si está dentro de la pantalla
  if(0 <= x && x < VGA_text_width && 0 <= y && y < VGA_text_height) {
    *VGA_text_array_pos(x, y) = (VGA_char){letter, atributes};
  }
}

static void
VGA_text_put_string(int x, int y, char* str, char atributes)
{
  if(str != NULL) {
    for(uint i = 0u; str[i] != '\0'; i++) {
      VGA_text_plot_letter(x, y, str[i], atributes);
      x++;
    }
  }
}

void
vgainit(void)
{
  for (int x = 0; x < VGA_text_width; x++) {
    VGA_text_plot_letter(x, 0, ' ', 0x2f);
  }
  VGA_text_put_string(37, 0, "SO2021", 0x2f);
}
```



## Parte 2
 
    Para lograr el cambio de modo nos guíamos del código que se encuentra en la página https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c, básicamente lo que se hace es escribir en los registros correspondientes al arreglo del modo al que se quiere cambiar, inicialmente este código se encontraba en el archivo console.c, pero luego de la modularización se colocaron todos las funciones relacionadas con VGA en el archivo VGA_regs.c
     
    En el archivo VGA_regs.c se encuentra la declaración de los arreglos del modo gráfico y el modo texto, la función write_regs se encarga de escribir los registros necesarios para realizar el cambio al modo deseado, además, si bien se puede cambiar entre modos no es algo que el usuario pueda hacer libremente, solo sucede en el caso de que algún programa de usuario que ejecute necesite cambiar de modo, por ejemplo al ejecutar el programa flappy desde la consola se cambia a modo gráfico, y al salir del programa se cambia de vuelta a modo texto, es un cambio que realiza el programa automaticamente.

    Cuando se realiza un cambio del modo gráfico al modo texto, se ejecuta la funcion write_fonts, la cual se encuentra en modes.c y se encarga de recuperar las fuentes para que al volver al modo texto se pueda visualizar la consola correctamente y no queden datos que se escribieron al estar en modo gráfico.

## Parte 3

        Explicar como lo habíamos hecho al principio, y que después lo cambiamos

        Explicar que decidimos hacer una llamada al sistema `VGA_plot_screen`

## Parte 4

        Explicación breve de como empezamos

# Extras en el kernel

## `stdin_read`

## Uso de todos los modos de VGA

        Explicación de VGA

        Explicación de como usamos el `modes.c`

## Uso de la paleta completa

# El kernel final y como usarlo

# El flappy bird

## Cómo usarlo

## Cómo esta hecho

### Modularización

            Módulos mas "generales" (`random` y `VGA_graphics`)

            Módulos del flappy

# Estilo del código

# Nuestra forma de trabajar

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**