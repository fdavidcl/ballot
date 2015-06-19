ballot
===========================

Programa para la medición del rendimiento de distintos *governors* sobre
una misma CPU.

## Uso

Para ejecutar el benchmark y obtener los resultados estadísticos directamente,
utilizar el siguiente comando:

    sudo R < runbench.R --no-save > script.log

Para ejecutar paso a paso, se pueden realizar 3 ejecuciones del benchmark
mediante:

    make results

O bien compilar el programa y ejecutarlo manualmente:

    make
    sudo ./govbench

Y después abrir el guion R con un IDE (por ejemplo, RStudio) y ejecutar paso
a paso la obtención de los tests estadísticos y gráficos.

## Organización

El archivo `govbench.cc` contiene el código que realiza el benchmark, y el
archivo `perftests.h` proporciona las pruebas de rendimiento. El fichero
`msg_es.h` define las cadenas de texto para comunicación con el usuario; el
`Makefile` permite compilar y ejecutar el benchmark, y el guion `runbench.R`
automatiza todo el proceso, incluyendo el cálculo de P-values y visualización
gráfica de resultados.

En el directorio `sample_results` se proveen unos archivos de resultados de
ejemplo, obtenidos a partir de la ejecución del programa sobre una misma
máquina.

## Sobre los governors

El escalado de frecuencia y voltaje al que funciona la CPU es ajustable en
los sistemas Linux. Esto se realiza mediante módulos de código denominados
[*scaling governors*](https://www.kernel.org/doc/Documentation/cpu-freq/governors.txt)
(gobernadores de escalado). Cada *governor* posee
una política de ajuste de las frecuencias de la CPU, que puede ser tanto
estática (usar constantemente la misma frecuencia) como dinámica (variar la
frecuencia en función de la carga de trabajo encontrada).

Este benchmark analiza el rendimiento de una CPU al ser establecido uno
de cuatro *governors* distintos:

  * Performance: es un *governor* estático que ajusta la frecuencia al máximo
  de la CPU de forma constante. En nuestro caso, Performance la ajusta a 2.3 GHz.
  * Powersave: este es otro *governor* de tipo estático, que realiza la tarea
  contraria a Performance; establece la frecuencia al mínimo de la CPU buscando
  el ahorro de batería. En nuestro caso, Powersave ajusta la frecuencia a 800 MHz.
  * Ondemand: es un *governor* dinámico, lo que significa que ajusta la frecuencia
  entre el mínimo y el máximo, según la utilización existente en cada momento.
  Ondemand actúa además muy rápidamente, siendo capaz de subir la frecuencia al
  máximo en el momento en que haya algo de trabajo para la CPU.
  * Conservative: al igual que Ondemand, este es un *governor* dinámico. Sin
  embargo, trata de ser más sosegado en los cambios de frecuencia, y la va variando
  poco a poco sin dar saltos bruscos, como sí puede hacer Ondemand.

## Diseño del *benchmark*

Para comprobar la efectividad de cada uno de estos governors en cuanto a la
realización de tareas de cómputo, se ha desarrollado un *benchmark* que ejecuta
tanto tareas largas continuas como pequeñas tareas separadas por tiempo de
descanso de la CPU, para así ver cómo se comportan los *governors* dinámicos.

El benchmark se compondrá de dos tareas de longitud variable, que se ejecutarán
varias veces con varias longitudes:

  * Cálculo de elementos de la sucesión de Fibonacci: para testear las operaciones
  sobre números enteros.
  * Cálculo de aproximaciones a τ (=2π): se utiliza la aproximación mediante
  [la serie de Leibniz](https://proofwiki.org/wiki/Leibniz%27s_Formula_for_Pi),
  testeando por tanto operaciones en coma flotante.

Una vez escogido uno de los *governors*, en primer lugar, se ejecutarán 1000000000
iteraciones de cada tarea de forma continua. A continuación, se ejecutará 10 veces
la tarea con un décimo de carga (100000000 iteraciones), separadas por un tiempo de
descanso de 200 milisegundos. Por último, se repetirá cada tarea 100 veces con un
número de iteraciones de 10000000, y un tiempo de inactividad intermedio de 2
segundos.

Para cada tarea de cálculo, se registra la suma de tiempo de todas las veces que se
ejecuta, sin contar el tiempo de inactividad. Es decir, para la tarea que se repite
10 veces, se calcula el tiempo que se tarda en ejecutar cada vez y se suman. Puesto
que lo único que hay al empezar a contar tiempo es la llamada a la función de cálculo,
y tras terminar se deja de contar el tiempo, este no estará influenciado por ningún
otro comportamiento del programa.

Este procedimiento se realiza para cada *governor* y el programa completo se repite
3 veces, para recoger suficientes datos de tiempo, minimizando así el posible ruido.


## Licencia
¡Ballot es software libre! Se distribuye bajo los términos de la
[Mozilla Public License](https://www.mozilla.org/MPL/)
