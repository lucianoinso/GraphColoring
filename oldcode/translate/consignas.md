#Laboratorio 1: Programación Imperativa en C.

En este laboratorio implementaremos un traductor de documentos  español-ingles en **C**.
Nuestro objetivo será poner a trabajar los conceptos más importantes del paradigma imperativo y comprender cómo afectan a la arquitectura de un programa.


## Traductor Español-Inglés
El traductor es un programa de línea de comando que utiliza un diccionario español/inglés el cual se carga en memoria cuando el programa se inicia. Además cuenta con un diccionario de palabras ignoradas que contiene aquellas palabras que el usuario decide no traducir de un idioma al otro.

Dado un documento de entrada, el traductor procesa el mismo palabra por palabra, buscando para cada una la traducción correspondiente. Si encuentra una palabra para la cual no hay una traducción especificada, consulta al usuario sobre que hacer con la misma. En este caso, el usuario puede elegir ignorarla solo esta vez, ignorarla siempre, proveer una traducción para usar solo una vez, o proveer una traducción definitiva para dicha palabra.
Al finalizar el procesamiento del documento, el traductor genera un nuevo documento con el texto traducido.

###Requerimientos Funcionales

A continuación se especifican en detalle las distintas funcionalidades que deberá implementar el traductor.


1. El traductor deberá aceptar al menos los siguientes argumentos:

		-i FILE | --input=FILE      : Documento de entrada. (Requerido)
		-d FILE | --dictionary=FILE : Diccionario de traducción.
		-g FILE | --ignored=FILE    : Diccionario de palabras ignoradas.
		-o FILE | --output=FILE     : Archivo de salida.
		-r      | --reverse  		: Dirección de la traducción.
	 	
2. Generar un archivo de salida que respete los signos de puntuación del archivo original, excepto aquellos que no existen en el inglés.  Ejemplo: si en el archivo de entrada tenemos `Hola, ¿Cómo te va?`, en el archivo de salida debemos obtener `Hello, How are you?`.

3. Ante una palabra para la cual no hay definida una traducción, mostrar el siguiente mensaje:

	
		No hay traducción para la palabra: XXXXXXXX

		Ignorar (i) - Ignorar Todas (h) - Traducir como (t) - Traducir siempre como (s)
	

4. Si el usuario decide **Ignorar** entonces solo esa ocurrencia no se traduce. 

5. Si el usuario decide **Ignorar Todas**, entonces todas las ocurrencias de la palabra son ignoradas y no se traducen.

6. Si el usuario decide **Traducir como** o **Traducir siempre como** el sistema le permite ingresar la traducción correspondiente de la siguiente  forma:

		Traducir XXXXXXXX como: YYYYYYYYY
	
En el primero de los casos, **Traducir como**, la traducción se utiliza para todas las ocurrencias de dicha palabra en el documento pero no actualiza el diccionario. Mientras que en el segundo caso, **Traducir siempre como**, ademas de utilizar dicha traducción para todas las ocurrencias de la palabra en el documento, también agrega la traducción al diccionario. 

6. Si una palabra tiene una traducción disponible, el sistema no propone ninguna acción.

7. Si el parámetro `--reverse` está presente cuando se ejecuta el traductor, entonces en el mismo se comporta como un traductor inglés-español.

9. El diccionario español-inglés tiene el siguiente formato:
	
		palabra-español,palabra-inglés
		palabra-español,palabra-inglés
		...
		palabra-español,palabra-inglés
	
##Consignas

1. Implementar el sistema utilizando una arquitectura que sea lo más desacoplada posible. Para ello deberán:
	* Evitar el uso de variables globales.
	* Implementar TADs que sean desempaquetados y seguros usando punteros opacos.

2. El parsing de los argumentos debe ser realizado utilizando [Getopt](http://www.gnu.org/software/libc/manual/html_node/Getopt.html), [Argp](http://www.gnu.org/software/libc/manual/html_node/Argp.html) u algún otro componente para dicha tarea.

3. El proyecto debe respetar la siguiente estructura de carpetas:

		bin/
		src/
		README.md
		makefile

4. Deberán crear un `makefile` 	que permita compilar el programa usando el comando `make`. El binario resultante debe alojarse en la carpeta `bin`


5. El código deberá respetar un coding-style. Se recomienda leer el [style guide](https://google.github.io/styleguide/cppguide.html) de Google y usar la herramienta [cpplint](https://github.com/google/styleguide/tree/gh-pages/cpplint). También se recomienda utilizar herramientas de chequeo de código estático como [cppckeck](http://cppcheck.sourceforge.net/).

5. Deberán completar el archivo README explicando: las decisiones de diseño que tomaron, las dependencias que se tienen, de donde instalarlas y como hacerlo, como compilar, como ejecutar, etc.

## Características de la presentación

* Fecha de entrega: hasta el 01/04/2016 a las 23:59:59.999
* Deberán crear un tag indicando el release para corregir.

		git tag -a 1.0 -m 'the initial release'

* Si no está el tag no se corrige. Tampoco se consideran commits posteriores al tag.

## Recomendaciones
* Tómense el tiempo necesario para pensar la arquitectura del programa y diseñar los TADs antes de empezar a codificar.
* Diseñen pensando en reutilizar el código o las abstracciones cuando sea posible.
* Si un fragmento de código se repite frecuentemente, abstraiganlo  mediante una función o una macro.
* `cpplint` corrige solo cuestiones de estilo, pero no verifica consistencia en el nombramiento de las variables y funciones. Esto es responsabilidad de Uds.
* Documenten las funciones y las porciones de código que no son intuitivas.
* No abusen de los comentarios en el código.
* Cuiden la memoria.
* Si la definición de una funcionalidad es ambigua, busquen clarificaciones antes de codificar basados en supuestos. Esto es responsabilidad de Uds.