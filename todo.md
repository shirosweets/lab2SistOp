- [x] Clonar el repositorio `https://github.com/mit-pdos/xv6-public` y eliminar el `.git` para que no exista un submódulo.

- [x] Agregar el kickstart al repositorio.

- [x] Implementar lo obligatorio del laboratorio

  - [x] Parte I
  - [x] Parte II
  - [x] Parte III
  - [x] Parte IV

- [ ] Implementar los extras del laboratorio

  - [ ] Todo lo que implementaron puede ser modularizado de una manera más delicada. Teniendo en cuenta que son funciones para un mismo dispositivo pueden estar en un mismo archivo `vga.{c,h}`.  - [x] Agregar una nueva `syscall` `plotrectangle(int x1, int y1, int x2, int y2, int color)` para dibujar rectángulos en la pantalla (la idea es no tener que usar `for` para pintar un pixel por vez).
  - [x] Programar la paleta para poder usar todos los colores *(si bien el modo gráfico es de **256** colores, la paleta está programada para 64 colores)*. (Estrella 1 - Vale)
  - [ ] Recuperar las fuentes que se pierden cuando pasamos de modo gráfico a texto. (Estrella 2 - Vale)

- [ ] Si queda tiempo:

  - [ ] Implementar menús de pausa en el flappy.
  - [ ] Hacer que a medida que pase un tiempo 'X' se vaya aunmentando progresivamente la velocidad.
  - [ ] Implementar un sistema de puntajes. (Vale)
  - [ ] Investigar e implementar sonido.

- [ ] Estilo y calidad del código:
  - [ ] Modularizar mejor el código.
  - [ ] Implementar las **docstrings** de las funciones del archivo [`so21lab2g27/xv6-public/sysproc.c`](so21lab2g27/xv6-public/sysproc.c).
  - [ ] Añadir en el [informe.md](so21lab2g27/informe.md) o [README.md](so21lab2g27/README.md).

- Vale:
  - [/] Punto estrella 1 de la paleta de colores.
  - [ ] Actualización sin pérdida de información del modo texto al modo gráfico.
  - [ ] Actualización sin pérdida de información del modo gráfico al modo texto. (si es posible!)
  - [ ] Investigar y explicar el modo texto-gráfico.
  - [/] Investigar implementación para leer el input.
  - [x] Borrar `backup.txt`
  - [ ] Borrar `console.h`