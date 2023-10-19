# t1-logaritmos

El fichero se divide en dos secciones: la carpeta **src** contiene todos los archivos asociados a las estructuras y métodos de construcción y búsqueda de un R-Tree, 
mientras que la carpeta **test** contiene los archivos correspondientes a los test de evaluacion de estos métodos junto con un test de rendimiento de estos.

A continuación se resumirá el contenido en cada archivo:

# r_tree

En este archivo se encuentran definidas las estructuras **Rectangle** y **Node** de la siguiente forma:
```c++
typedef array<long int, 4> Rectangle;
template <typename T>
using MaxChildrenArray = array<T, MAX_CHILDREN>;
class Node {
   public:
    MaxChildrenArray<Rectangle> keys;
    MaxChildrenArray<long int> children;
    int keys_qty = 0;
    bool is_leaf = false;
}
```

Donde Rectangle tiene la forma {x_min, y_min, x_max, y_max} y los campos de Nodo son:

- keys: los Rectangle MBR asociados a los hijos del Nodo.
- children: los índices de los nodos hijos.
- keys_qty: la cantidad de nodos hijos.
- is_leaf: corresponde al estado del nodo como nodo hoja (true si lo es, false si no).

Dentro de la clase nodo, tambien de encuentran definidos otros métodos:
```c++
static Node toNode(Rectangle rectangle);
template <typename T>
static Node readNode(T& file, long int index);
string toText(void);
```
- toNode: retorna un nodo hoja asociado a **rectangle**.
- readNode: abre el archivo **file** y retorna el nodo con índice **index**.
- toText: retorna un string con la información del nodo.

También se encuentran los encabezados de los métodos contructores y búsqueda.
Finalmente en el archivo se encuentran métodos para ordenar puntos por sus coordenadas X e Y.

# nearest_x

En este archivo se encuentra definido el método constructor de R-Tree usando el método Nearest-X, usando el siguiente encabezado:
```c++
void Node::r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name)
```
Crea y abre un archivo binario de nombre **file_name** y lo rellena con los nodos del arbol, parte de la última generación (los nodos hoja) hasta el nodo raiz. Luego realiza el siguiente procedimiento:

- Primero: convierte **initial_associated_rectangles** a nodo, luego calcula los centro de cada nodo para luego ordenarlos por la coordenada X de su centro.
- Segundo: a partir del parámetro **M**, calculamos el número de nodos padres y dividimos la lista ordenada en dicho número con M nodos por grupo, luego creamos un nodo padre para cada grupo.
- Tercero: calcula el MBR asociado a cada nodo padre y los coloca en una lista.
- Cuarto: si la cantidad de nodos padre es distinta de 1, se repite el procedimiento dejando la lista de MBR de los nodos padres en **initial_associated_rectangles**.

De esta forma el arbol queda guardado en **file_name**.

# hilbert

En este archivo se encuentra definido el método de ordenamiento de hilbert y el método constructor de R-Tree usando el método de la Curva de Hilbert, usando el siguiente encabezado:
```c++
void Node::r_tree_hilbert(vector<Rectangle> initial_associated_rectangles, int M, string file_name)
```
Crea y abre un archivo binario de nombre **file_name** y lo rellena con los nodos del arbol, parte del último nivel de este (los nodos hoja) hasta el nodo raiz. Luego realiza el siguiente procedimiento:

- Primero: convierte **initial_associated_rectangles** a nodo, luego calcula su valor dentro de la curva de Hilbert y ordena la lista do nodos en torno a esta distancia.
- Segundo: calcula la cantidad de nodos padres y divide la lista en dicho número en grupos de **M** nodos.
- Tercero: calcula el MBR de los nodos padres y los coloca en una lista.
-Cuarto: si la cantidad de nodos padre es distinto de 1, se repite el procedimiento dejando la lista de MBR de los nodos padres en **initial_associated_rectangles**.

De esta forma el arbol queda guardado en **file_name**.

# sort_tile_recursive

En este archivo se encuentra definido el método constructor de R-Tree usando el método Sort-Tile-Recursive, usando el siguiente encabezado:
```c++
void Node::r_tree_sort_tile_recursive(vector<Rectangle> initial_associated_rectangles, int M, string file_name)
```
Crea y abre un archivo binario de nombre **file_name** y lo rellena con los nodos del arbol, parte del último nivel de este (los nodos hoja) hasta el nodo raiz. Luego realiza el siguiente procedimiento:

- Primero: convierte **initial_associated_rectangles** a nodo, luego calcula los centro de cada nodo para luego ordenarlos por la coordenada X del centro.
- Segundo: calculamos el valor S como √(n_nodos/**M**) y dividimos los nodos en grupos de S*M nodos.
- Tercero: ordenamos cada grupo por la coordenada Y de los centros y nuevamente agrupamos en grupos de a **M** nodos.
- Cuarto: calculamos la cantidad de nodos padres y creamos uno para cada nuevo grupo y luego calcula el MBR de cada nodo padre y lo guarda en una lista.
- Quinto: si la cantidad de nodos padres en distinta de 1, se repite el procedimiento dejando la lista de MBR de los nodos padres en **initial_associated_rectangles**.

De esta forma el arbol queda guardado en **file_name**.

# search

En este archivo se encuentra definido el método de búsqueda, usando el siguiente encabezado:

```c++
tuple<vector<Rectangle>, int> Node::r_tree_rectangle_search(Rectangle rec_to_search, string filename)
```
Este método busca en un R-Tree ubicado en el archivo **filename** todos los rectangulos en los nodos hojas que esten intersectados con **rec_to_search**.

Para esto realiza una búsqueda secuencial donde, comenzando en el nodo raíz, evalua si hay una intersección entre **rec_to_search** y alguno de los Rectangles dentro del campo keys del nodo, de ser asi busca en el nodo asociado a dicho Rectangle y asi sucesivamente hasta llegar a la hoja. Repite este procedimiento para todos los nodos pero solo accediendo a los cuales se encuentra una interseccion con su MBR asociado. 

La función finalmente retorna un vector con los Rectangle que intersectan a **rec_to_search** en una tupla junto con el número de accesos realizados a memoria durante la búsqueda.