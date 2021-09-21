- [x] Clonar el repositorio `https://github.com/mit-pdos/xv6-public` y eliminar el `.git` para que no exista un submódulo.

- [x] Agregar el kickstart al repositorio.

- [ ] Implementar lo obligatorio del laboratorio
  
  - [ ] Parte I
  - [ ] Parte II
  - [ ] Parte III
  - [ ] Parte IV

- [ ] Implementar los extras del laboratorio
  
  - [ ] Todo lo que implementaron puede ser modularizado de una manera más delicada. Teniendo en cuenta que son funciones para un mismo dispositivo pueden estar en un mismo archivo `vga.{c,h}`.
  - [x] Agregar una nueva `syscall` `plotrectangle(int x1, int y1, int x2, int y2, int color)` para dibujar rectángulos en la pantalla (la idea es no tener que usar `for` para pintar un pixel por vez).
  - [ ] Programar la paleta para poder usar todos los colores *(si bien el modo gráfico es de **256** colores, la paleta está programada para 64 colores)*. (Estrella 1 - Vale)
  - [ ] Recuperar las fuentes que se pierden cuando pasamos de modo gráfico a texto. (Estrella 2 - Vale)

- Vale:
  - [ ] Punto estrella 1 de la paleta de colores.
  - [ ] Punto estrella 2 de la actualización del modo gráfico al modo texto.
  - [ ] Investigar y explicar el modo texto-gráfico.
  - [ ] Investigar implementación para leer el input.