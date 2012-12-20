Acerca de qvwm
--------------
qvwm es un manejador de ventanas para el sistema X Window muy parecido
a Windows 95/98/NT. Permite utilizar el sistema X Window al usuario de
Windows 95/98/NT sin vacilaciones, e igualmente permite al usuario del 
sistema X Window utilizar Windows 95/98/NT.

El nombre de qvwm proviene de lo siguiente: 'q' significa '9' (la misma
pronunciación en japonés, y casi idéntica forma) y 'v', '5' (V es 5 en
números romanos). Además, qvwm es también "98 Window Manager" ya que 'v'
quiere decir '8' (invertida, la V es parecida al 8 en caracteres kanji).

La última versión es la 1.1.12 (Senzoku).


Características
---------------
- Escrito desde cero en C++.
- Soporte en varios idiomas sin parches.
- Los mismos gráficos que Windows 95/98/NT a nivel de pixel.
- Permite al usuario cambiar entre ventanas usando el conmutador de tareas.
  (Alt+Tab)
- Permite al usuario crear indicadores de ventanas en la barra de tareas.
- Permite al usuario mover y redimensionar una ventana completa .(arraste
  completo)
- Pantalla virtual. (extensión de Windows 95/98/NT)
- Caja de diálogo para salir.
- Animación de la barra de título al minimizar/maximizar una ventana.
- Permite al usuario crear iconos e imágenes animadas de fondo.


La última versión
-----------------
Se puede bajar la última versión de qvwm de

    http://www.qvwm.org/

Además, este página tiene espejos en los siguientes sitios:

U.S.A
    http://qvwm.kuntrynet.com/qvwm/
        (Peter Hartman  <hartman@kuntrynet.com>)

También se puede bajar qvwm de los siguientes sitios:

Japan
    ftp://ring.asahi-net.or.jp/pub/X/window_managers/qvwm/

Japan
    http://www.bkr-lab.utmc.or.jp/software/index.html#QVWM
        (UTMC Ikebukuro Laboratory)


Avisos de errores y preguntas
-----------------------------
Para informa de errores o hacer preguntas sobre qvwm, por favor, envíe
un e-mail a qvwm-support@qvwm.org. Sin embargo, antes de enviar el mensaje, 
por favor lea BUGS.es, TODO.es, FAQ.es y demás. También son de utilidad
mensajes anteriores en la lista de correo.

En el caso de que qvwm produzca una violación de segmento, se puede 
conseguir más información sobre el error usando la opción de compilación
DEBUG. Incluso en el caso de que no pueda obtener el error, se puede
identificar donde falla qvwm analizando el volcado de memoria con gdb.
El análisis del volcado se hace como sigue:

  1. Ejecute 'gdb {qvwm} {core}' en el directorio donde se encuentre
     el código fuente de qvwm. ({qvwm} es la ruta donde el ejecutable
     qvwm, {core}, la ruta del volcado de memoria.)
  2. Ejecute 'backtrace' en la línea de comando de gdb.

Por favor, póngase en contacto con nosotros si tiene alguna petición.


Lista de correo
---------------
Se ofrece una lista de correo para el intercambio de información sobre
qvwm (qvwm-e ML). Se permite el uso del inglés en esta lista. Para poder
participar, envíe un correo con el cuerpo

	subscribe

a

	qvwm-e-ctl@qvwm.org

También hay una lista de correo para usuarios japoneses (qvwm ML).
Se permite el uso del japonés y el inglés. Para participar en esta
lista, envíe un correo con el cuerpo

	subscribe
a

	qvwm-ctl@qvwm.org


Copyright
---------
qvwm se distribuye bajo la `GNU general public license.'
En el fichero 'COPYING' se encuentra una copia de dicha licencia.


AVISO
-----
La versión oficial de qvwm publicada puede no ser suficientemente estable
en algunos entornos. El autor no se hace responsable de ningún daño
producido al usar qvwm.


Reconocimientos
---------------
Se han recibido parches para qvwm de las siguientes personas. El autor
está profundamente agradecido a ellos.

  Miodrag Vallat        <miodrag@ifrance.com>
  Byung Yang            <byung@wam.umd.edu>
  Jun Obama             <george@yk.rim.or.jp>
  Falk Hueffner         <falk.hueffner@student.uni-tuebingen.de>
  Toshiyuki Amagasa     <amagasa@dbms.cs.gunma-u.ac.jp>
  Hajime Morito         <Hajime.Morito@ccsr.cam.ac.uk>
  Chris Meadors         <cmeadors@acc7.ac.cc.md.us>
  Mofeed Shahin         <shahin@ist.flinders.edu.au>
  Osamu Kurati          <kurati@bigfoot.com>
  Youichiro Koga        <y-koga@w3.mag.keio.ac.jp>
  Kenji Minoura         <minoru@i21x12.fuis.fukui-u.ac.jp>
  Don Crimbchin         <djc2@andrew.cmu.edu>
  Nozomu Kobayashi      <nozomu@ssf.abk.nec.co.jp>
  Shuichi Oono          <jr5pdx@debian.or.jp>
  Tamito Kajiyama       <kajiyama@etl.go.jp>
  Takayuki Kusano       <AE5T-KSN@asahi-net.or.jp>
  Francois Petitjean    <francois.petitjean@bureauveritas.com>
  Kasamatsu Kenichi     <camatsu@geocities.co.jp>
  
Agradecimientos a todos aquellos que han enviado información útil
a la lista de correo, etc.

---
Kenichi Kourai
kourai@qvwm.org

---
Traducido parcialmente por Yukihiro Nakai <Nakai@Mlab.t.u-tokyo.ac.jp>
Traducida al español por Benjamín Albiñana <foobar@escomposlinux.org>
