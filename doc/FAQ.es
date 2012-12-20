
FAQ
===

Conceptos
---------
P. ¿Qué es qvwm?
R. Qvwm es un manejador de ventanas similar a Windows 95/98/NT para el
   sistema X Window. El interfaz es casi el mismo que el de Windows
   95/98/NT. Originalmente, "Qvwm" derivó de "95 Window Manager" (en 
   japonés, 'q' se pronuncia igual que '9', y tiene casi la misma 
   forma, y 'v' es '5' en números romanos). Además, qvwm es "98
   Window Manager" ya que 'v' viene de '8' (la V invertida es parecida
   al número 8 en caracteres kanji).

P. ¿Por qué los pixmaps que vienen con el programa se mueven horizontalmente?
R. Qvwm proporciona una GUI parecida a Windows 95/98/NT. A primera vista
   puede que no se distinga el SO que corre en la máquina. Para evitar
   esta confusión, los pixmaps se mueven horizontalmente. La segunda razón
   es que nos rebelamos silenciosamente contra Microsoft.

P. ¿A dónde va qvwm?
R. Se está reescribiendo qvwm con la biblioteca libqv (una biblioteca de
   clases para Windows como la aplicación que se está desarrollando). Una 
   nueva versión de qvwm permitirá personalizarlo con una GUI como Windows
   y modificar completamente el diálogo de salida con el teclado.

   Más detalles de libqv en http://libqv.qvwm.org/index-en.html


Instalación
-----------
P. El script de configuración se para con "test: argument expected" en 
   Solaris 2.x.
R. Cambiar la línea del script de configuración de:

    if test $ac_cv_func_vfork_works = no; then
    
    a

    if test "$ac_cv_func_vfork_works" = no; then

    Parece ser un bug de la versión 2.12 de autoconf. Hágame saber si 
    tiene una solución mejor (qvwm-support@qvwm.org).

P. Qvwm muestra "Can't open display" al arrancar.
R. Intenta arrancar qvwm desde una consola. (La consola es una pantalla
   al estilo MS-DOS que básicamente no puede mostrar gráficos.) Ya que 
   qvwm es un manejador de ventanas que sólo puede correr bajo X Window, 
   arranque X Window primero mediante xinit (o startx). Si modifica 
   ~/.xinitrc según INSTALL.es, qvwm arranca automáticamente.

Funcionamiento
--------------
P. No puedo cerrar una ventana pulsando el botón de salir (botón [x] en
   la esquina superior derecha).
R. No se puede usar el botón de salir cuando la ventana está supendida. 
   Seleccione Matar en el menú de control para forzar la salida.

P. Pulso el botón de maximizar pero la ventana no se maximiza.
R. Algunas aplicaciones limitan el tamaño mázimo de la ventana. En ese caso
   la ventana se mueve a la esquina superior izquierda y pierde el marco.

P. No puedo cambiar el tamaño de una ventana.
R. Algunas aplicaciones limitan el tamaño mínimo y/o máximo de la ventana.
   En ese caso, no se puede cambiar el tamaño por encima de la limitación.

P. Tarda un poco en pasar algo una vez pulsado un botón.
R. Parece ser la naturaleza del sistema X Window.

P. Qvwm termina de forma rara y el teclado se queda colgado.
R. Conectese en remoto y lance qvwm 'qvwm -display esa_máquina_con_problemas:0'
   y cierre correctamente.

P. Tarda un poco en salir el menú de inicio después de seleccionar un botón.
R. Para hacerlo más rápido, qvwm lee los pixmaps cuando se pulsa el botón de
   inicio la primera vez. De modo que el menú de inicio se retrasa la primera
   vez.

P. No se ve la ventana, aunque hay un botón en la barra de tareas y no está
   minimizada. 
R. Puede que se muestre fuera de la ventana virtual. Seleccione 'mover' en
   el botón de la barra de tareas con un click del botón derecho del ratón y
   mueva el cursor. Si hay suerte la verá dentro de la ventana virtual. También
   puede cambiar el tamaño de la ventana virtual y reiniciar qvwm.

P. No puedo arrastrar la ventana a otra pantalla de forma correcta.
R. Mueva el ratón un poco sobre el borde de la pantalla. O puede modificar el
   valor de PagingBeltSize, haciendo mayor.

Configuración
-------------
P. No puedo hacer doble click de forma correcta.
R. Aumente el valor de DoubleClickTime o DoubleClickRange.

P. Me gustaría mover la barra de título más rápido al minimizar o maximizar.
R. Disminuya el valor de TitlebarMotionSpeed.

P. Me gustaría cambiar de ventana virtual más rápido.
R. Disminuya el valor de PagingResistance.

P. Me gustaría cambiar el tamaño de la ventana virtual.
A. Cambie el valor de PagingSize.

P. Quiero cambiar el tamaño y la posición del paginador.
R. Cambie el valor de PagerGeometry.

P. Quiero cambiar el fondo del escritorio.
R. Especifique la ruta completa del fichero xpm que quiere usar.

P. No quiero la caja de diálogos al salir.
R. Ajuste UseExitDialog False.

P. Mi PC tiene una CPU lenta, así que sólo quiero ver el marco al arrastrar.
R. Ajuste OpaqueMove False.

P. Quiero la ventana activa bajo el cursor del ratón.
R. Ajuste ClickToFocus False.

P. Prefiero que la ventana activa sea la que está más arriba.
R. Ajuste AutoRaise True.

P. Quiero cambiar el retardo de AutoRaise.
R. Disminuya el valor de AutoRaiseDelay.

P. Quiero usar mis propios iconos. 
R. Especifique su directorio de iconos y ponga los ficheros xpm allí. El 
   tamaño de los iconos es de 32x32.

P. Una ventana de kinput2 parpadea cuando introduzco caracteres japoneses
   en Netscape.
R. Añada la siguiente línea en la sección [Applications] de su fichero de 
   configuración:

   "Kinput2"	NO_FOCUS

---
Traducido parcialmente por Yukihiro Nakai <Nakai@Mlab.t.u-tokyo.ac.jp>
Traducción traducida por Bill Richter <wlr@jps.net>
Traducida al español por Benjamín Albiñana <foobar@escomposlinux.org>
