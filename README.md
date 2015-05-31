Governor Benchmark
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
