Proyecto 1 de Paradigma

Introduccion
------------------------------------------------------------------------------

En este proyecto creamos un traductor que traduce un documento de texto del 
español al ingles y viceversa, el traductor es bastante simple ya que solo
traduce palabra por palabra. 
Se lo puede correr con solo un archivo de entrada y sin ningun diccionario
cargado, y nos pedira que vayamos ingresando las palabras a medida se requiere
traducirlas, podemos elegir guardar estas palabras en un diccionario de usuario
por defecto que se guarda en un archivo en la mismas carpeta, de ahi en mas
en siguientes ejecuciones se consultara ese diccionario de usuario personal.
Como salida aparte del archivo con el texto traducido, esta la opcion de 
hacer dump a un diccionario de palabras que fuimos ignorando durante la 
traduccion, para que en proximas ejecuciones podamos pasar ese archivo y el
traductor ignore dichas palabras para esa ejecucion.

Diseño 
-------------------------------------------------------------------------------
En este Proyecto usamos Hashmaps que son una herramienta excelente para
acelerar la busqueda de las palabras a traducir. En nuestra implementacion al
encontrar colisiones decidimos sortearlas con la ayuda de listas enlazadas, un
metodo mas eficiente hubiese sido usar arboles de busqueda, pero debido al 
plazo del proyecto elegimos ir por listas enlazadas.
Esto resulta en que una posicion dentro del arreglo puede contener varias 
palabras a la vez, que bien puede resulta un poco mas costoso pero dado el 
tamaño del programa influye muy poco.
Adentro de la estructura del hashmap se encuentra el hasharray que a su vez 
contiene punteros en cada posicion que apunten a los hashnodes. Un hashnode esta 
compuesto por un key, que representa la palabra en español, un value, que 
representa la palabra en ingles y una variable next, que es un puntero al 
siguente hashnode. 
Por questiones de comodidad tartamos el dict_ignore igual que el dict comun 
con la misma palabra repetida.

Implementacion 
--------------------------------------------------------------------------------
Dividimos el proyecto en 5 archivos hashmap.c, translate.c, dict.c, word.c y en
main.c

En word.c nos ocupamos de las operaciones basicas con strings.
Implementamos funciones que nos ayuden a medir, comparar, copiar, borrar, 
extraer e implementa palabras del texto.

En hash\_map.c se encuentran todo los funciones relacionado con el hash\_
map. Funciones como hashmap\_create,hashnode\_create,get\_hash comunes en este
tipo de tad.

Translate.c se encarga de realizar la traduccion del documento,
dentro podemos encontrar funciones que traducen 1 sola palabra, o 1 linea, y
la funcion que se encarga de traducir todo el documento.
Para esto dentro de esta encontramos funciones para procesamiento de palabras 
como ser como remover signos, cambiar de mayuscula a minuscula, etc.
 
Dict.c esta pensado para manejar las funciones que se encargan del manejo del 
diccionario , creacion, carga, carga desde archivo, dump a archivo, etc.
Utiliza un hashmap para su implementacion, y usa las funciones de ese mismo tad


Funcionamiento
--------------------------------------------------------------------------------

El usario ingresa un archivo a traducir junto con el diccionario y la lista de
palabras ingnoradas.
El programa llama las fucnion file_from_file que llama dict_create que a su vez
usa las funciones del hashmap.c para crea un hashmap con hashnodes que contienen
las palabras y su traducciones. Despues llama a translate_document que a su vez 
llama translate_line que llama a translate_word y asi sucesivamente.Entonces 
empieza a traducir En el caso que una palabra no tiene una traduccion valida 
y tampoco se encuentra en la lista de ignoradas se abre call_menu que da los
opciones de ignorar,ignorar todas, traducir como, traducir siempre como,
si elegimos ignorar o traducir la operacion solo vale para esta vez.
En el caso que eligemos traducir siempre como incorporamos la palabra 
junto con su traduccion al diccionario del usuario. En el caso de ignorar todo
 guardamos las palabras en la lista de palabras ignoradas y dumpeamos al final
 del programa.

Una vez terminado el proceso, vuelca el resultado al archivo output especificado
y ademas modifica el diccionario del usuario y el de palabras ignoradas.

