# Informe lab 2

- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**

---

# Índice

- [Instalación](#instalación)
- [¿Cómo correrlo?](#¿cómo-correrlo?)
- [Desarrollo inicial](#desarrollo-inicial)
  - [Parte 1](#parte-1)
  - [Parte 2](#parte-2)
  - [Parte 3](#parte-3)
    - [`VGA_mode_switch`](#vga_mode_switch)
    - [`VGA_plot_pixel`](#vga_plot_pixel)
    - [`VGA_plot_screen`](#vga_plot_screen)
  - [Parte 4](#parte-4)
- [Extras en el kernel](#extras-en-el-kernel)
  - [`stdin_read`](#stdin_read)
  - [Uso de la paleta completa](#uso-de-la-paleta-completa)
  - [Recuperar las fuentes al volver a modo texto](#recuperar-las-fuentes-al-volver-a-modo-texto)
- [El flappy bird](#el-flappy-bird)
  - [Funcionamiento](#funcionamiento)
    - [Modularización](#modularización)
      - [`random`](#random)
      - [`VGA_graphics`](#vga_graphics)
      - [`flappy_bird_TAD`](#flappy_bird_tad)
      - [`flappy_bird_graphics`](#flappy_bird_graphics)
      - [`flappy_bird_logic`](#flappy_bird_logic)
      - [`flappy` (main)](#flappy-main)
      - [`flappy bird: score`](#flappy-bird:-score)
- [Nuestra forma de trabajar](#nuestra-forma-de-trabajar)
  - [Estilo del código](#estilo-del-código)
  - [Modularización de xv6](#modularización-de-xv6)

# Instalación
## QEMU

```bash
sudo apt-get update -y
```

```bash
sudo apt-get install -y qemu-system-i386
```

# ¿Cómo correrlo?

```bash
cd xv6-public ; make clean && make qemu
```

Aparecerá una ventana donde deberemos escribir la palabra `flappy` y presionar `<Enter>`, luego nos aparecerá el juego que podremos jugar presionando cualquier tecla para hacer que el flappy salte:

![Menú](./assets/how_to_run.png)

# Desarrollo inicial

## Parte 1

    En esta parte el enunciado pide hacer una función que muestre un pie de pantalla que diga `SO2021` (el enunciado dice `SO2020`, pero suponemos que quedo así del año pasado) y llamarla al inicio del sistema.

    Para hacer esto se da la siguiente información:

    Se explica que VGA trabaja con buffers, en los cuáles está guardada la información que se está mostrando por la pantalla. Cuando VGA está configurado en modo texto de `80×25` caracteres (que es como viene configurado cuando se inicia el sistema) el buffer está en la dirección `0xB0000`. Y que cada elemento del buffer se compone de 2 bytes, uno para el código ASCII del carácter, y otro para los atributos, o sea, el color del texto, el color del fondo y la fuente que se usa.

    Se da esta imagen que explica mejor los bits de los elementos del buffer:

![bits_text_buffer.png](./assets/bits_text_buffer.png)

    Se da un ejemplo en el cuál se guardan caracteres en el buffer con esta forma:

```c
*(int *)P2V(0xB8000) = 0x4348;
```

    Nosotros decidimos *modularizarlo* un poco, haciendo *2 funciones auxiliares*, `VGA_text_plot_letter` que pone una letra con sus atributos en una coordenada de la pantalla y `VGA_text_put_string` que pone un string en una coordenada de la pantalla dada. Esto nos pareció mejor que hacer como en el ejemplo, ya que así no hay que buscar el código ASCII de los caracteres que se ponen, y todo es menos repetitivo.

    Utilizando esas dos funciones hicimos la función `vgainit` para que muestre un el pie de pantalla así:

![pie_de_pantalla_vgainit.png](./assets/pie_de_pantalla_vgainit.png)

    Por último, al prototipo de la función `vgainit` lo agregamos a `defs.h` para que pueda ser usada desde cualquier lugar del **kernel** y la llamamos al comienzo de `main` en `main.c`.

    A esas funciones en ese momento las hicimos en `console.c`, pero después las movimos a `VGA.c`, que es donde se pueden ver ahora, y también las modificamos un poco (ver explicación en [Extras en el kernel](#extras-en-el-kernel)). Las versiones originales eran así:

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

    En esta parte se pide hacer funciones para cambiar entre modo gráfico y modo texto en el kernel (es decir, para ser ejecutadas en modo kernel). Para lograr eso se da como ayuda el código [modes.c](https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c).

    En ese código hay varios arreglos que tienen los registros de los distintos modos, y también hay una función `write_regs` que escribe los registros.

    Entonces, básicamente lo que se hace es escribir los registros correspondientes al arreglo del modo al que se quiere cambiar, o sea, se evalúa el arreglo del modo al que se quiere cambiar en la función `write_regs`.

    Y eso fue exactamente lo que hicimos: copiamos la función `write_regs` a `console.c`, cambiando las llamadas a `outportb` e `inportb` por llamadas a `outb` e `inb`, que son los equivalentes a esas funciones en xv6. Copiamos los arreglos `g_80x25_text` y `g_320x200x256`, que son los correspondientes a los modos que pide el enunciado, e hicimos en `console.c` las siguientes funciones:

```c
// Cambia a modo gráfico (320 x 200), si ya estaba en ese modo no hace nada
void
VGA_to_mode_graphic(void)
{
  write_regs(g_320x200x256);
}

// Cambia a modo gráfico (80 x 25), si ya estaba en ese modo no hace nada
void
VGA_to_mode_text(void)
{
  write_regs(g_80x25_text);
}
```

    Inicialmente este código se encontraba en el archivo `console.c`, pero luego de la modularización se colocaron todos las funciones relacionadas con VGA en el archivo `VGA.c`, y también se le agregaron más cosas, como la recuperación de las fuentes (ver explicaciones en [Extras en el kernel](#extras-en-el-kernel)).

## Parte 3

    Todo el código de la [parte 2](#parte-2) es código para ser ejecutado en el kernel, pero no puede ser ejecutado en modo usuario, y por ende, en la parte 3 se pide implementar una llamada al sistema para que los programas de usuario puedan cambiar de modo, y también una llamada al sistema para pintar en la pantalla, ya que para pintar en la pantalla hay que guardar los valores de los colores en ciertas direcciones de memoria, lo cuál no se puede hacer en modo usuario.

    En xv6 las llamadas al sistema se pueden hacer desde un programa de usuario usando la función cuyo prototipo está en `user.h`. Cuando se llama a una de esas funciones lo que pasa es que se ejecuta una función definida en `usys.S` a través de un macro, la cuál produce el trap de llamada al sistema que entra en modo kernel.

    Para que el kernel pueda saber cuál llamada al sistema ejecutar, cada llamada al sistema tiene un número asignado en `syscall.h`, el cuál es puesto en un registro por el macro de `usys.S` antes de hacer el trap. Cuando el trap se produce, se ejecuta la función `trap`, la cuál ve que es un trap por llamada al sistema y llama a la función `syscall` de `syscall.c`.

    En el archivo `syscall.c` hay un arreglo que tiene las direcciones de memoria en las cuales está el código de cada una de las llamadas al sistema, entonces, la función `syscall` lo que hace es llamar a la función correspondiente de esa llamada, usando el número de esa llamada para saber cuál ejecutar.

    La función en la cuál está la dirección de memoria en el arreglo de `syscall.c` se llama `sys_nombreDeLaLlamada` (y no `nombreDeLaLlamada` solo), y se tiene que encargar de, además de ejecutar la llamada al sistema en sí, obtener los parámetros de la llamada, ya que `syscall` desconoce cuáles son los parámetros, y tiene que ejecutar una función que toma `void`.

    Para obtener los parámetros desde `sys_nombreDeLaLlamada` se pueden usar unas funciones que están definidas en `syscall.c` y que se encargan de hacerlo tomando el número de parámetro. Muchas de las llamadas al sistema en la función `sys_nombreDeLaLlamada` se encargan de obtener los parámetros y luego llamar a una función `nombreDeLaLlamada` que se encargue de ejecutar la función. En las nuestras hicimos exactamente eso.

### `VGA_mode_switch`

    En esta llamada al sistema (que en el enunciado se pide como `modeswitch`, pero le cambiamos el nombre para que quede más acorde al resto de las funciones que estábamos haciendo), lo que primero hicimos fue ver a que modo se quería cambiar y hacer mas o menos lo mismo que en el punto 2.

    Después, cuando íbamos a hacer otra llamada al sistema, nos dimos cuenta que hace falta poder saber cuál es el modo actual, así que agregamos una variable global que guarda el modo actual, y que se actualiza en cada cambio de modo. Con eso, el código nos quedó así:

```c
typedef int VGA_mode;
#define VGA_mode_text 0
#define VGA_mode_graphic 1

VGA_mode actual_mode = VGA_mode_text;

void
VGA_mode_switch(VGA_mode mode)
{
  if(mode == VGA_mode_graphic){
    write_regs(g_320x200x256);
    actual_mode = VGA_mode_graphic;
  }
  else if(mode == VGA_mode_text){
    write_regs(g_80x25_text);
    actual_mode = VGA_mode_text;
  }
}
```

### `VGA_plot_pixel`

    Para que los programas de usuario pueden dibujar cosas en la pantalla, en el enunciado se pide hacer una llamada el sistema que tome las coordenadas y el color, y pinte un pixel en la pantalla.

    Ya que no se especificaba nada acerca de qué tiene que pasar si se trata de pintar un pixel de fuera de la pantalla, nosotros decidimos hacer que simplemente no se pinte, ya que esto nos permite que si se está dibujando algo grande que se sale de la pantalla, se pueda dibujar con normalidad sin tener que hacer verificaciones extras.

    Otra cosa que no se aclara es qué tiene que pasar si se trata de pintar un pixel estando en modo texto. Nosotros decidimos también hacer que simplemente no se haga nada.

    Con eso, el código nos quedó así:

```c
#define VGA_graphic_width 320
#define VGA_graphic_height 200
#define VGA_graphic_array ((uchar*)P2V(0x000A0000))
#define VGA_graphic_pos(x, y) ((uchar*)(VGA_graphic_array + y*VGA_graphic_width + x))

void
VGA_plot_pixel(int x, int y, uchar color)
{
  if(0 <= x && x < VGA_graphic_width && 0 <= y && y < VGA_graphic_height
    && actual_mode == VGA_mode_graphic){
    *VGA_graphic_pos(x, y) = color;
  }
}
```

### `VGA_plot_screen`

    Si los programas de usuario a todo lo que dibujan lo tienen que poner en la pantalla pixel por pixel con `VGA_plot_pixel` se demoran mucho, ya que cada llamada al sistema es lenta. Para mitigar eso, en el enunciado se propone hacer una llamada al sistema `plotrectangle` que dibuje un rectángulo todo del mismo color. Esto ayuda un poco, pero no mucho, ya que solo sirve cuando el programa quiere dibujar un cuadrado todo del mismo color.

    Lo que nosotros decidimos hacer es hacer una llamada al sistema `VGA_plot_screen` que toma un arreglo que tiene los colores de todos los pixeles y los pone a todos en la pantalla. Esto permite al programa dibujar todo de forma rápida en un arreglo propio, y después ponerlo al todo al mismo tiempo en la pantalla.

    Esa llamada al sistema fue simplemente un `for` que copie todo de un arreglo al otro:

```c
void
VGA_plot_screen(uchar* buffer)
{
  if(actual_mode == VGA_mode_graphic){
    for(int j = 0; j < VGA_graphic_width*VGA_graphic_height; j++){
      VGA_graphic_array[j] = buffer[j];
    }
  }
}
```

    A todas esas funciones al principio las habíamos hecho en el archivo `proc.c`, porque vimos que ahí estaban implementadas algunas de las otras llamadas al sistema, pero luego las movimos a `VGA.c` y las modificamos un poco por distintos motivos. Por eso, es que pusimos acá el código, para que se pueda ver como eran en el momento en el que las hicimos.

## Parte 4

    Para la parte 4 el enunciado hay que hacer algún programa de usuario que use el **VGA**. Nosotros decidimos hacer un juego de Flappy Bird ([Wikipedia - Flappy Bird](https://es.wikipedia.org/wiki/Flappy_Bird) para información sobre el original). Empezamos creando una carpeta para el juego y haciendo algunas cosas, pero nos dimos cuenta de que necesitamos implementar algunas cosas extra en el kernel antes de poder implementar completamente el juego.

# Extras en el kernel

    Nosotros decidimos hacer varias cosas más en el kernel, algunas por sugerencia del enunciado y otras porque las necesitamos para hacer el Flappy Bird. Las cosas que hicimos fueron:

  • Una llamada al sistema `stdin_read` que dice si hay caracteres disponibles en el stdin, y si los hay los saca y los devuelve.

  • Usar 256 colores en el modo gráfico, ya que por defecto se pueden usar solo 64.

  • Recuperar las fuentes al volver a modo texto, ya que si no se hace nada, no se puede leer lo que se escribe en la pantalla.

    A continuación están explicadas todas:

## `stdin_read`

    En **xv6** la única llamada al sistema que permite leer caracteres de la entrada estándar es `read`. El funcionamiento de `read` es que toma el descriptor de archivo (0 en el caso de la entrada estándar), una dirección de memoria en la que guardar los caracteres y el máximo de caracteres que guardar.

    Cuando se lee de la entrada estándar con `read` los caracteres no son leídos si no hay un salto de línea, es decir, si se llama a `read` cuando la entrada estándar está vacía, no se retorna hasta que se ingresa un salto de línea.

    Para hacer el Flappy Bird nosotros necesitamos que cuando se presiona una tecla el flappy salte, y cuando no se presiona una tecla que caiga (en ambos casos el juego tiene que seguir ejecutándose). Por eso, no podíamos usar la función `read`, que no retorna hasta que no se presiona la tecla enter.

    Entonces, para poder hacer el juego, decidimos agregar una nueva llamada al sistema `stdin_read`, la cuál toma una dirección de memoria en la que guardar un carácter, y mira si hay caracteres en la entrada estándar. En el caso de que no haya retorna `false`, y en el caso de que si haya retorna `true`, guarda el primer carácter en la dirección de memoria, y lo saca de la entrada estándar.

    Para poder hacer está función, vimos que cuando se produce un **trap por interrupción del teclado**, el carácter se guarda en `struct input` que está en `console.c`, que es como un buffer, y cuando se lee un carácter de la entrada estándar, se lo saca de ahí. Por lo cuál en la función `stdin_read` simplemente miramos si hay algún carácter en ese buffer, y si lo hay lo sacamos.

## Uso de la paleta completa

    Al cambiar a modo gráfico, si no se hace ninguno cambio adicional sólo se tienen disponibles 64 *colores*, la idea era modificar la paleta para poder tener el uso de los 256 colores y poder graficar de mejor manera el Flappy Bird.

    Para poder extender la paleta hay que asignar cada color a través de los puertos de VGA, ya que no hay ningún lugar en la memoria donde se encuentren los colores. La idea para escribir la función que cambia la paleta se sacó de [sam46/xv6](https://github.com/sam46/xv6) y de [cGraphicsPalette](https://www.oocities.org/siliconvalley/park/7113/OldPages/cGraphicsPalette.html).

    Básicamente lo primero que hicimos fue **declarar un arreglo con 256 colores básicos de RGB en formato hexadecimal**, ya sabemos que el modelo RGB combina los colores primarios (rojo, verde y azul) para generar distintos colores, al expresarse en hexadecimal los primeros 8 bits corresponden a la intensidad del color rojo, los siguientes 8 al color verde y los últimos 8 al color azul.

Ejemplo: el color `0xFF0000`, `0xFF` en hexadecimal es 255 y representa la cantidad de color rojo que hay, como 255 es el máximo quiere decir que el color tiene la mayor cantidad de rojo posible, como los siguientes bytes están en 0, quiere decir que no hay verde ni azul, por lo que el `0xFF0000` representa el rojo.

    Lo que se debe hacer es escribir al puerto `0x3C8` en VGA, que es el que maneja las paletas, e indicarle cual es índice de la paleta que se quiere modificar, y luego escribir el color en si en el puerto `0x3C9`, en este último puerto se debe escribir tres veces consecutivas (una para cada color primario), según las referencias que usamos, si no se escribe tres veces consecutivas al puerto `0x3C9` se puede tener comportamiento indefinido.

    La función `VGA_set_palette_color` es la que se encarga de recibir el índice del color de la paleta que se quiere modificar y el valor de los colores primarios, luego el `VGA_set_palette` se encarga de recorrer cada color en el arreglo `VGA_palette_256` que contiene los colores en formato hexadecimal y de llamar a la función `VGA_set_palette_color` para escribir los valores en los puertos.

    Es importante recalcar que los puertos de VGA sólo reciben 6 bits, y un color RGB en formato hexadecimal tiene 24 bits, 8 para cada color primario, por esto para poder almacenar los bits correctamente primero lo que se hace es un shift del valor en hexadecimal de tal forma que los últimos 6 bits sean los 6 bits más significativos de cada color primario, y luego se hace un **and bit a bit** (*ver mini explicación abajo*) con el número `0x3F`, que representa el `0b00111111`, de esta forma sólo obtenemos 6 bits para cada color primario y los podemos escribir correctamente en los puertos.

Mini explicación **and bit a bit**:

```c
a     = 0b10110010
b     = 0b00111111
a & b = 0b00110010
```

    Al final, para poder cambiar correctamente la paleta lo que se hace es que al cambiar al modo gráfico luego de escribir sobre los registros necesarios se hace una llamada a `VGA_set_palette_color`. Para conocer como acceder a cada color utilizamos la paleta de `8-bit-mode` (que es la que tiene 256 colores) publicada en la página [vga_color_palettes](https://www.fountainware.com/EXPL/vga_color_palettes.htm).

## Recuperar las fuentes al volver a modo texto

    En modo texto cada carácter tiene (en el caso del `80×25`) 8×16 pixeles que pueden ser del color del carácter o del color del fondo. La información de como va cada pixel VGA la guarda como 128 bits consecutivos en memoria, en el que cada 8 bits se representa una fila de las 16 filas. Si se ven los 128 bits como un arreglo de 16 `uchar`s, se puede ver un poco la letra. Por ejemplo, esta sería la letra `B`:

```c
0b00000000
0b00000000
0b11111100
0b01100110
0b01100110
0b01100110
0b01111100
0b01100110
0b01100110
0b01100110
0b01100110
0b11111100
0b00000000
0b00000000
0b00000000
0b00000000
```

    A toda la información de cada uno de los 256 caracteres cuando se está en modo texto, VGA la tiene guardada en su propia memoria, pero cuando se pasa a modo gráfico la borra, y si después se vuelve a modo texto, hay basura en esa parte de la memoria y no se puede leer nada de lo que se imprime en la pantalla:

![Fuentes_ilegibles.png](assets/Fuentes_ilegibles.png)

    VGA tiene una opción para pasarla la fuente que uno quiera.

    Hay dos elementos importantes que participan en el cambio de modos: el espacio de memoria `0xA0000-0xBFFFF` donde se **mapea la información mostrada en la pantalla**, y los registros de VGA donde se configura cómo es interpretada esa sección de la memoria.

    Como el **`modo gráfico`** **comparte** una **sección** de la **memoria** con el **`modo texto`**, al dibujar pixeles en el primero se sobrescribe información codificada para el segundo, y al cambiar se sobrescriben los registros. Ambas cosas deben ser recuperadas al estado anterior para que el modo texto funcione correctamente.

    Inicialmente, por simplicidad, implementamos la conservación de las fuentes **copiando toda la sección gráfica de la memoria** en un arreglo estático. Este se copiaba en el array `VGA_graphic_array` (que actualmente está eliminado) e intercambiarlo con el buffer.

```c
/* Intercambia lo que está desde 0xA0000 hasta 0xBFFFF
 * con lo que esta en el arreglo buffer_mode */
static void VGA_switch_buffers(void);  // Utiliza VGA_graphic_array

/* Utiliza VGA_switch_buffers() dependiendo el modo actual */
void VGA_mode_switch(VGA_mode mode)
```

    Cuando el modo actual es el gráfico, el arreglo conservaba la memoria del texto y viceversa.

    Eventualmente esta idea fue descartada ya que no funcionaba correctamente y se optó por replicar el código de la *super ayuda* adaptado a nuestra arquitectura de xv6, utilizando la función `write_font`, cambiando los `outportb` e `inportb` por `outb` e `inb` como en la parte 2 y cambiando la llamada a `vmemwr` por un `for`, ya que la función `vmemwr()` es *"Virtual Memory Write"* y esencialmente escribe en el espacio de Kernel traduciendo una *dirección física a la virtual correspondiente*.
    La parte del `vmemwr` en el original era así:

```c
    for(i = 0; i < 256; i++)
    {
        vmemwr(16384u * 0 + i * 32, buf, font_height);
        buf += font_height;
    }
```

La adaptación en `xv6` fue usar la escritura normal de memoria pero aplicando la traducción provista P2V (*Physical to Virtual*):

```c
#define VGA_font_array P2V(0xB8000)

  for(uint i = 0; i < 256; i++){
    for (uint j = 0; j < font_height; j++){
      // VGA_MEMBASE
      ((uchar*)VGA_font_array)[32*i + j] = *buf;
      buf++;
    }
  }
```

    Para efectivamente recuperar las fuentes, en la función `VGA_mode_switch` cuando se pasa a modo texto agregamos una llamada a `write_font` con `g_8x16_font` así:

```c
    // Se ponen las fuentes
    write_font(g_8x16_font, 16);
```

**Referencias importantes a considerar:**

* http://www.osdever.net/FreeVGA/vga/vga.htm

* http://www.osdever.net/FreeVGA/vga/vgareg.htm

  * http://www.osdever.net/FreeVGA/vga/vgareg.htm#intro

* http://www.techhelpmanual.com/70-video_graphics_array__vga_.html

* http://www.techhelpmanual.com/900-video_graphics_array_i_o_ports.html

* http://www.techhelpmanual.com/89-video_memory_layouts.html

# El flappy bird

    Como mencionamos en [Parte 4](#parte-4) nosotros hicimos un Flappy Bird, a continuación una imagen de cómo se ve:

![Flappy Bird](./assets/Flappy_Bird.png)

    Para jugar al flappy simplemente hay que iniciar xv6 (`make qemu` desde la carpeta `xv6-public`) y ejecutar `flappy`. Con eso ya se inicia el juego con una semilla aleatoria (la cuál determina las alturas de los huecos por los que hay que pasar). Si se desea especificar la semilla se puede hacer pasando algo como segundo parámetro, y los valores numéricos de los primeros 4 caracteres del segundo parámetro serán usados para generar la semilla.

    Cuando se choca contra algo, se vuelve a la consola, imprimiéndose el puntaje. Para volver a jugar simplemente hay que presionar cualquier tecla. Para salir del juego y cerrar el programa hay que presionar la tecla escape, o hacer `ctrl + d` (lo cual manda un final de archivo).

## Funcionamiento

    El juego funciona con una estructura en la que se guardan los datos dinámicos del juego (los que cambian), y un buffer en el que dibuja lo que va en la pantalla en cada fotograma antes de pasárselo al kernel para que lo muestre por la pantalla.

    La parte principal del programa, que es lo que ejecuta el juego en sí, tiene un ciclo que se ejecuta 1 vez por cada fotograma y cada vez mira si se presionó una tecla (con nuestra llamada al sistema `stdin_read`), actualiza los datos internos, dibuja todo en el buffer secundario, llama al kernel para que lo ponga en la pantalla y mide el tiempo de ejecución del cuerpo del ciclo (para poder hacer que las velocidades de los objetos sean iguales sin importar que tan rápido está ejecutándose el juego).

    Para hacer todo eso, el código está dividido en varios módulos. Eso hace que el `Makefile` que viene en `xv6` no lo compile bien, ya que esta hecho para compilar a los programas de usuario (los `UPROG` en el `Makefile`) como si estuvieran todos en un solo archivo. Para poder compilar el flappy teniéndolo dividido en varios archivos lo que hicimos fue poner todos los archivos del flappy en una carpeta y agregar una regla especial para compilar nuestro programa:

```makefile
_flappy: $(ULIB)
    cd flappy_bird ; $(CC) $(CFLAGS) -c *.c
    $(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ flappy_bird/*.o $^
```

    Como se puede ver, esta regla no tiene como dependencia los archivos `.c` y `.h` del programa, lo cuál causa que si se los modifica no los vuelva a compilar, teniendo que hacer `make clean` cada vez que se modifica algo del flappy.

    El motivo por el cuál está así, es que no logramos poner a los archivos como dependencia. Intentamos poner como dependencia `flappy_bird/flappy.c` y `./flappy_bird/flappy.c`, pero da error:

```
flappy_bird/flappy.c: file not recognized: file format not recognized
make: *** [Makefile:162: _flappy] Error 1
```

    También probamos poner `flappy_bird` como dependencia, como para que dependa de la carpeta entera, pero por mas que modifiquemos archivos de adentro de la carpeta, no re-compila.

### Modularización

A continuación una pequeña explicación de que hace cada módulo y cómo funciona:

#### `random`

    En el flappy las alturas de los huecos por los que tiene que pasar el flappy se van generando pseudo-aleatoriamente. La altura de cada hueco (que se mide en pixeles) se determina según la altura del hueco anterior y un número aleatorio que dice la diferencia con la altura del hueco anterior. Obviamente ese número que determina la diferencia no puede ser cualquier `int`, porque eso permitiría que la diferencia entre un tubo y otro sea demasiado grande como para que se pueda pasar y también podría que pasar el hueco queda fuera de la pantalla.

    Para evitar que la diferencia de alturas sea demasiado grande se usa un número aleatorio que esté en el rango `[-max_diff_hight_tubes, max_diff_hight_tubes]` \(donde `max_diff_hight_tubes` es un define de `50`).

    Para evitar que que se salga de la pantalla cada vez que se genera la altura de un nuevo tubo se mira que esté en el rango `[width_hole_tube/2 + min_distance_to_border, ground_height - width_hole_tube/2 - min_distance_to_border]` (donde esas variables también son defines) y si no está se vuelve a generar una altura hasta que se genere alguna que si esté.

    En C ya existen librerías para trabajar con números aleatorios, pero como nosotros estamos trabajando en xv6, no las podemos usar (o no es tan simple usarlas), así que decidimos hacer todas las funciones por nosotros mismos en el módulo `random`.

    El módulo funciona guardando una semilla en una variable global y usando esa semilla y actualizandola cada vez que se necesita un número aleatorio. Para obtener los números aleatorios tiene 3 funciones públicas (en el `.h`), pero la importante es `new_random_less_than` que obtiene un número entre `-n` y `n`, y que es la que usamos en el flappy.

    Las explicaciones de que hace cada función se pueden ver en `random.h` y las de como funcionan `random.c`.

#### `VGA_graphics`

    En este archivo se encuentran funciones auxiliares para dibujar cosas en pantalla haciendo uso de la función `draw_pixel`. Decidimos crear esta función nueva para dibujar cosas en un buffer, ya que la función `VGA_plot_pixel` hace una llamada a sistema para dibujar un pixel en la pantalla y es más eficiente dibujar todo en un buffer y luego usando `VGA_plot_screen` representarlo por la pantalla.

    Las funciones creadas en este archivo incluyen `draw_horizontal_line` que dibuja una línea horizontal, `draw_vertical_line` que dibuja una línea vertical, `draw_rectangle` que dibuja un rectángulo y `draw_circle` que dibuja un círculo. Todas estas funciones se utilizaron para realizar la parte gráfica del flappy. Todas las funciones toman como parámetro la dirección de memoria del buffer, las coordenadas donde se va a pintar la figura respectiva y el color que se va a usar.

#### `flappy_bird_TAD`

    Acá se encuentran las definiciones de varios macros que ayudan a la legibilidad del código, además de que hace que sea más fácil modificar los parámetros del juego, por ejemplo la variable `flappy_radius` que cambia el tamaño del flappy, o la variable `offset_tubes` que determina la distancia que hay entre cada tubo.

    Otra cosa que también se encuentra en este archivo es el struct de `game_status` y la variable global `game` que es de este tipo y es donde se almacenan los datos que se deben actualizar al dibujar el juego en cada iteración del ciclo.

#### `flappy_bird_graphics`

    Acá se encuentra la parte gráfica del juego, están definidas las funciones que se encargan de dibujar el fondo, el flappy y los tubos, y también se encuentran definidos varios macros que hacen mas fácil la modificación de los parámetros, como por ejemplo el color de los tubos, el color del flappy, o la altura a la que se dibujan las nubes.

    La función `draw_background` se encarga de pintar el fondo, haciendo llamadas a las funciones `draw_sky` que dibuja el cielo, `draw_clouds` que dibuja las nubes, `draw_bush` que dibuja la vegetación y `draw_ground` que dibuja el suelo.

    Luego está las función `draw_tubes` que se encarga de dibujar los tubos, la función `draw_flappy` que se encarga de dibujar el flappy y por último la función `draw_game` que se encarga de hacer llamadas a las funciones mencionadas anteriormente para de esta manera dibujar todo el juego en el buffer, esta es la función que se utiliza en el ciclo principal del juego para dibujar en cada iteración.

#### `flappy_bird_logic`

    Este es el módulo que se encarga de actualizar el estado interno del juego. Tiene 2 funciones públicas, `init_game` que inicializa el `game_status game` y se la llama al comienzo del juego, y `update_game` que toma la cantidad de tiempo que paso desde la última llamada a `update_game`, si tiene que saltar o no y actualiza el estado interno del juego, moviendo el flappy y los tubos (agregando y eliminando tubos cuando hace falta).

    Internamente se divide en tres partes:

    Por un lado se actualizan las posiciones del flappy y del tubo, y la velocidad del flappy.

    Por otro lado se eliminan los tubos que ya no están visibles, y se agregan tubos cuando hace falta.

    Y por otro lado se chequea si el flappy se chocó contra algo o no.

#### `flappy` (main)

    Este es el archivo principal del programa, y el que se encarga de ejecutar el ciclo principal del juego.

    Como cuando el juego en si finaliza el programa no se cierra, si no que se va al modo texto y se imprime el puntaje y un mensaje para volver a jugar o no, la función `main` cuenta con un ciclo en el cuál se ejecuta el juego en si (la función `game_loop`) y luego se imprime el mensaje y se espera a que se presione una tecla.

    Para detectar si se presiono una tecla se utiliza este ciclo:

```c
while(!stdin_ready(&c)){
  sleep(30);
}
```

    Entendiendo como funciona `stdin_ready` es claro que hacer eso es muy ineficiente ya que se está todo el tiempo preguntándole al sistema si hay algún carácter. El motivo por el cuál lo hicimos así es porque la única otra llamada al sistema que hay para leer caracteres es `read`, pero `read` no lee de la entrada estándar hasta que no se presiona enter, por lo cuál si usábamos `read` iba a ser menos interactivo.



**Observación**: A veces ocurre un bug en el cual el juego detecta que se perdió aunque no es así, no sabemos a que se debe este error pero probando, descubrimos que el juego funciona correctamente y sin bugs compilando sin `-O2`, por lo que pensamos que debe ser un problema de la optimización.

### `flappy bird: score`

```c
// Configuración de cada número para dibujar en una casilla
struct _digit_cell{
  /* Cada celda representa una linea del gráfico de cada
   * de cada casilla de dígito. Distintas configuraciones
   * 'iluminan' cada celda, determinando distintos números
  */
  bool cells[7];
};
```

Existen 10 casillas de dígitos y cada una contiene una celda de dígito. A continuación la configuración de cada celda respecto a la estructura anteriormente mencionada:

![Celdas de dígitos](./assets/flappy_bird_score.jpg)

Si quisiéramos dibujar por ejemplo un `0` (cero/zero) deberíamos setear el struc de la siguiente manera:

```c
digit_cell zero;

zero.cells[0] = true;  // ROJO
zero.cells[1] = true;  // VERDE
zero.cells[2] = true;  // MORADO
zero.cells[3] = false;  // ROSA (no visible para el cero)
zero.cells[4] = true;  // NARANJA
zero.cells[5] = true;  // CIAN
zero.cells[6] = true;  // BLANCO
```

En la siguiente imagen se puede observar cada línea de la celda diferenciadas por colores:

![Celdas de dígitos v1](./assets/flappy_bird_score_v1.png)

```c
/*
 * Dibuja en pantalla el score actual hasta 10
 * casillas. Las casillas están enumeradas desde
 * 0 hasta 9. La casilla 0 es la casilla correspondiente
 * a la unidad (primer casilla de la derecha).
*/
static void draw_score(uchar* buffer);

/*
* Llama a la función correspondiente al dígito
* para renderizarlo en pantalla en la casilla
* señalada para ese mismo.
*/
static void draw_check_digit(uchar* buffer, uint digit, uint position);

/*
 * Toma la configuración de qué celdas pintar de la variable number
 * y cuál casilla pintar de la variable position.
 *
 * Pinta las líneas respecto al dígito que deba ser.
*/
static void draw_digit(uchar* buffer, uint position, digit_cell number);
```

Versión final del score:

![Celdas de dígitos v2](./assets/flappy_bird_score_v2.png)

Al perder se nos mostrará la puntuación final y la cantidad de tubos superados:
![Score final](./assets/flappy_bird_score_game_over.png)

- Video en Youtube (*hacer click en la imagen*):

[![Video Youtube](https://img.youtube.com/vi/9HXrArUGIjY/0.jpg)](https://www.youtube.com/watch?v=9HXrArUGIjY)

Dato curioso: el orden de las líneas de la celda fueron definidas en el orden que se realiza el trazado al dibujar un cisne

![Celdas de dígitos: dato curioso - celda](./assets/score_celda.jpg)
![Celdas de dígitos: dato curioso - cisne](./assets/score_cisne.jpg)

# Nuestra forma de trabajar

    En el archivo [todo.md](todo.md) se encuentran las diferentes consignas y tareas que realizábamos (no siempre es commiteado).

    Pair programming con la extensión de [Live Share](https://visualstudio.microsoft.com/services/live-share/) de Visual Studio Code.

    Comunicación: [Telegram](https://web.telegram.org/) y [Discord](https://discord.com/).

## Estilo del código

    En xv6 no hay ninguna política completamente definida sobre el estilo del código, e incluso algunas cosas no son iguales en todos los archivos, así que nosotros tratamos de seguirlo en las cosas en las que era igual en todos lados:

    Indentación de 2 espacios.

    El tipo de las funciones arriba del nombre, y la llave que abre el cuerpo de la función abajo.

    Usar los tipos acortados de `types.h` (`uint`, `ushort`, etc.).

    Los `.h` del kernel de xv6 no tienen `ifndef` `define`, eso dentro del kernel lo mantuvimos, pero en nuestro programa de usuario si usamos los `ifndef` `define`.

## Modularización de xv6

    También, de forma aparte al laboratorio modificamos xv6 para separar en distintos archivos los de xv6 en carpetas para el kernel, los programas de usuario y el `mkfs`. Eso se encuentra en [GitHub - IvanRenison/xv6-modularized](https://github.com/IvanRenison/xv6-modularized).

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**