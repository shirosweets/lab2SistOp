# Informe lab 2

- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**

---

 # Contenido

- [Comprensión del lab](#comprensión-del-lab)
    - [Librerías en xv6](#librerías-en-xv6)
    - [Manejo básico de qemu](#manejo-básico-de-qemu)
- [Recuperar las fuentes que se pierden cuando pasamos de modo gráfico a texto](#Recuperar las fuentes que se pierden cuando pasamos de modo gráfico a texto)

# Instalación

## QEMU
```bash
sudo apt-get update -y
```

```bash
sudo apt-get install -y qemu-system-i386
```

# Comprensión del lab

## Librerías en xv6

## Manejo básico de qemu

- Para listar los procesos dentro de xv6 hacer `<CRTL-p>`.

- Salir de QEMU: `<CTRL-a> x`.

- Para que iniciar qemu CON pantalla VGA: `make qemu`.

## Modos VGA
- Modo text (0x30 mode) (80x25)
- Modo gráfico (0x13 mode) (320x200)

Referencias:
- https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
- http://www.techhelpmanual.com/900-video_graphics_array_i_o_ports.html

## Standard VGA

- Rango: `0x000A0000` - `0x000BFFFF`

### Modo gráfico

El modo gráfico asume que cada celda de memoria corresponde a un píxel, es decir, si modificamos un valor en la matriz que representa la pantalla, entonces estamos cambiándole el color al pixel.

- Modo: `0x13`
- Resolución: `320x200`
- Bytes: `64000`
- Dirección de memoria: `0xA0000`
- Espacio de memoria continuo (se puede calcular linealmente) la dirección de memoria que le corresponde con cada pixel (x, y):

```c
unchar *VGA = 0xA0000;
offset = 320 * y + x;
VGA[offset] = color;
```

### Modo texto

Para el modo texto se utilizan dos buffers distintos, uno para definir cómo es elcarácter y otro para indicar como mostrarlo por pantalla (abajo hay algo más deinformación).

- Modo: `0x03`
- Celdas de texto: `80x25`

Referencias:
- http://www.techhelpmanual.com/47-character_set_matrix.html
- http://www.techhelpmanual.com/91-video_font_definition.html

# Extras

## Input del usuario

## Recuperar las fuentes que se pierden cuando pasamos de modo gráfico a texto

Referencias:
- http://www.osdever.net/FreeVGA/vga/vga.htm
- http://www.osdever.net/FreeVGA/vga/vgareg.htm
    - http://www.osdever.net/FreeVGA/vga/vgareg.htm#intro
- http://www.techhelpmanual.com/70-video_graphics_array__vga_.html
- http://www.techhelpmanual.com/900-video_graphics_array_i_o_ports.html

- http://www.techhelpmanual.com/89-video_memory_layouts.html

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**