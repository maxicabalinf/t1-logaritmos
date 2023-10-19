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
