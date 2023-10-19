# t1-logaritmos

El fichero se divide en dos secciones: la carpeta **src** contiene todos los archivos asociados a las estructuras y métodos de construcción y búsqueda de un R-Tree, 
mientras que la carpeta **test** contiene los archivos corespondientes a los test de evaluacion de estos métodos junto con un test de rendimiento de estos.

A continuación se resumirá el contenido en cada archivo:

# r_tree

En este archivo se encuentran definidas las estrcuturas **Rectangle** y **Nodo** de la siguiente forma:
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
- is_leaf: cooresponde al estado del nodo como nodo hoja (true si lo es, false si no).

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

# nearest_x

En este archivo se encuentra definido el método constructor de R-Tree usando el método Reanerst-X, usando el siguiente encabezado:
```c++
void Node::r_tree_nearest_X(vector<Rectangle> initial_associated_rectangles, int M, string file_name)
```
Crea y abre un archivo binario de nombre **file_name** y lo rellena con los nodos del arbol, parte del último nivel de este (los nodos hoja) hasta el nodo raiz. 

- Primero: convierte **initial_associated_rectangles** a nodo, luego calcula los centro de cada nodo para luego ordenarlos por la coordenada X del centro.
- Segundo: a partir del parámetro **M**, calculamos el número de nodos padres y dividimos la lista ordenada en dicho número con M nodos por grupo, luego creamos un nodo padre para cada grupo.
- Tercero: calcula el MBR asociado a cada nodo padre y lo coloca en una lista.
- Cuarto: si la cantidad de nodos padre es distinta de 1, se repite el procedimiento dejando la lista de MBR de los nodos padres en **initial_associated_rectangles**.
De esta forma el arbol queda guardado en **file_name**.

# hilbert

