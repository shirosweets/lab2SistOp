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

    

    A esas funciones en ese momento las hicimos en `concole.c`, pero después las movimos a `VGA_reg.c`, que es donde se pueden ver ahora.

## Parte 2

        Explicar como lo logramos al principio, y que si bien en ese momento lo hicimos en el `console.c`, después lo sacamos de ahí

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