// Implementación del método Nearest-X para construcción de árboles R.

// Generar centros de c/u de los n rectangulos.
// Ordenar centros según orden de Hilbert.
// Agrupar ordenadamente en M puntos.
// Formar n/M nodos de tamaño M. El último nodo tiene x<=M nodos.
// Identificar MBR de cada grupo
// Si se agrupó todo en un único nodo raíz, terminar. Si no, repetir el procedimiento usando los MBR generados en el paso anterior.