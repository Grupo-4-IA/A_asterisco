#include <bits/stdc++.h>
using namespace std;
#define ROW 9
#define COL 10
typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;
 
//Estructura de cuadros
struct cell {
    // Notar que 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};
 
// Verificar si un cuadro es valido
bool isValid(int row, int col)
{
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}
 
// Verificar si el cuadro está bloqueado
bool isUnBlocked(int grid[][COL], int row, int col)
{
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}
 
// Verificar si se ha alcanzado el destino
bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}
 
// Funcion para calcular el valor de h
double calculateHValue(int row, int col, Pair dest)
{
    // Return using the distance formula
    return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}
 
// Funcion para encontrar el camino desde el inicio al fin
void tracePath(cell cellDetails[][COL], Pair dest)
{
    printf("\nEl camino es ");
    int row = dest.first;
    int col = dest.second;
 
    stack<Pair> Path;
 
    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
 
    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }
 
    return;
}


// Funcion para encontrar el camino
// más corto desde un cuadro dado hasta
// un cuadro destino en base al algoritmo A*
void aStarSearch(int grid[][COL], Pair src, Pair dest)
{
    // Si el origen no está en rango
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }
 
    // Si el destino esta fuera de rango
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }
 
    // Si el origen o el destino están bloqueados
    if (isUnBlocked(grid, src.first, src.second) == false
        || isUnBlocked(grid, dest.first, dest.second)
               == false) {
        printf("Source or the destination is blocked\n");
        return;
    }
 
    // Si el origen es el mismo que el destino
    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return;
    }
 

    // Crear la lista cerrada con los valores en falso
    // lo que indica que ningun cuadro ha sido incluido
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));
 
    // Declarar un array de 2 dimensiones 
    // para los detalles de un cuadro
    cell cellDetails[ROW][COL];
 
    int i, j;
 
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }
 
    // Inicializar los parametros del nodo inicial
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
 
    /*
     Crear la lista abiaerta con la informacion
     <f, <i, j>>
     donde f = g + h,
     e i, j son las coordenadas del nodo
     Notar que 0 <= i <= ROW-1 & 0 <= j <= COL-1
    */
    set<pPair> openList;
 
    // Introducir el nodo inicial en la lista abierta
    // 'f' como 0
    openList.insert(make_pair(0.0, make_pair(i, j)));
 
    // Definimos esta variable en false ya que
    // el destino no se encuentra aun
    bool foundDest = false;
 
    while (!openList.empty()) {
        pPair p = *openList.begin();
 
        // Se retira este nodo de la lista abierta
        openList.erase(openList.begin());
 
        // Se añade este nodo a la lista cerrada
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
 
        /*
         Generar los  8 sucesores del nodo
 
             N.W   N   N.E
               \   |   /
                \  |  /
             W----Cell----E
                  / | \
                /   |  \
             S.W    S   S.E
 
         Nodo-->Nodo eliminado (i, j)
         N -->  Norte    (i-1, j)
         S -->  Sur       (i+1, j)
         E -->  Este        (i, j+1)
         W -->  Oesta           (i, j-1)
         N.E--> Nor-Este  (i-1, j+1)
         N.W--> Nor-Oeste  (i-1, j-1)
         S.E--> Sur-Este  (i+1, j+1)
         S.W--> Sur-Oeste  (i+1, j-1)*/
 
        // Variables para generar los sucesores
        double gNew, hNew, fNew;
 
        //----------- 1st Sucesor (Norte) ------------
        // El proceso para el resto de sucesores es el mismo
 
        // Se procesa este nodo solo si es valido
        if (isValid(i - 1, j) == true) {
            // Si el destino es el mismo que el actual sucesor
            if (isDestination(i - 1, j, dest) == true) {
                // Definir el padre del sucesor
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // Si el sucesor esta ya en la lista cerrada
            // o si esta bloqueado, ignorarlo
            // Hacer lo siguiente de lo contrario
            else if (closedList[i - 1][j] == false
                     && isUnBlocked(grid, i - 1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;
 
                // Si no esta en la lista abierta, añadirlo
                // Hacer el nodo actual el padre de este 
                // nodo. Guardar los valores f, g y h
                // Si el nodo esta ya en la lista abierta, 
                // verificar que el camino a ese cuadrado es mejor
                // usando f como medida.
                
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));
 
                    // Actualizar los valores del nodo
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }
 
        //----------- 2nd Sucesor (Sur) ------------

        if (isValid(i + 1, j) == true) {

            if (isDestination(i + 1, j, dest) == true) {

                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }


            else if (closedList[i + 1][j] == false
                     && isUnBlocked(grid, i + 1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;
 
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }
 
        //----------- 3rd Sucesor (Este) ------------
 
        if (isValid(i, j + 1) == true) {
            if (isDestination(i, j + 1, dest) == true) {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i][j + 1] == false
                     && isUnBlocked(grid, i, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));
 

                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 4th Sucesor (Oeste) ------------
 
        if (isValid(i, j - 1) == true) {
            if (isDestination(i, j - 1, dest) == true) {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
 
            else if (closedList[i][j - 1] == false
                     && isUnBlocked(grid, i, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;
 
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));

                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }
 
        //----------- 5th Sucesor (Nor-Este)

        if (isValid(i - 1, j + 1) == true) {

            if (isDestination(i - 1, j + 1, dest) == true) {
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i - 1][j + 1] == false
                     && isUnBlocked(grid, i - 1, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j + 1].f == FLT_MAX
                    || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j + 1)));
 

                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 6th Successor (Nor-Oeste)
 

        if (isValid(i - 1, j - 1) == true) {

            if (isDestination(i - 1, j - 1, dest) == true) {

                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
 

            else if (closedList[i - 1][j - 1] == false
                     && isUnBlocked(grid, i - 1, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;
 

                if (cellDetails[i - 1][j - 1].f == FLT_MAX
                    || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j - 1)));
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }
 
        //----------- 7th Sucesor (Sur-Este)

        if (isValid(i + 1, j + 1) == true) {

            if (isDestination(i + 1, j + 1, dest) == true) {

                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
 

            else if (closedList[i + 1][j + 1] == false
                     && isUnBlocked(grid, i + 1, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j + 1].f == FLT_MAX
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j + 1)));
 
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 8th Successor (Sur-Oeste)

        if (isValid(i + 1, j - 1) == true) {

            if (isDestination(i + 1, j - 1, dest) == true) {

                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                printf("El nodo destino ha sido encontrado\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
 

            else if (closedList[i + 1][j - 1] == false
                     && isUnBlocked(grid, i + 1, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j - 1].f == FLT_MAX
                    || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j - 1)));
 

                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }
 
    // Si no se encuentra el nodo destino y la lista abierta 
    // esta vacia, concluimos que no se pudo llegar al nodo
    // destino de ninguna manera debido a los bloqueos.

    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");
 
    return;
}
 
// Programa para probar la funcion
int main()
{
    /* Descripcion de la matriz-
     1--> La celda no esta bloqueada
     0--> La celda esta bloqueada    */
    int grid[ROW][COL]
        = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
            { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
            { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };
 

    Pair src = make_pair(8, 0);

    Pair dest = make_pair(0, 0);
 
    aStarSearch(grid, src, dest);
 
    return (0);
}