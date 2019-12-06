# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
#include <omp.h>
using namespace std;

# define TAMANO 24
# define THREADS 4
int main ( int argc, char **argv );
int *ppc_distance ( int matrix[TAMANO][TAMANO], int start_node );
void find_nearest ( int mind[TAMANO], bool connected[TAMANO], int *d, int *v );
void init ( int matrix[TAMANO][TAMANO] );
void timestamp ( void );
void update_mind ( int mv, bool connected[TAMANO], int matrix[TAMANO][TAMANO], int mind[TAMANO] );

//****************************************************************************80

int main ( int argc, char **argv ) {
  double t_final;
  double t_inicial;

//****************************************************************************80
//
//  Purpose:
//
//    MAIN runs an example of PPC's minimum distance algorithm.
//
//  Discussion:
//
//    Given the distance matrix that defines a graph, we seek a list
//    of the minimum distances between node 0 and all other nodes.
//
//    This program sets up a small example problem and solves it.
//
//    The correct minimum distances from node 0 are:
//
//      0   35   15   45   49   41
//
//

  int i;
  int i4_huge = 2147483647;
  int j;
  int *mind;
  int matrix[TAMANO][TAMANO];

  timestamp ( );
  cout << "\n";
  cout << "Programación Paralela y Concurrente\n";
  cout << "  C++ version\n";
  cout << "  Use PPC's algorithm to determine the minimum\n";
  cout << "  distance from node 0 to each node in a graph,\n";
  cout << "  given the distances between each pair of nodes.\n";
//
//  Initialize the problem data.
//
  init ( matrix );
//
//  Print the distance matrix.
//
  cout << "\n";
  cout << "  Distance matrix:\n";
  cout << "\n";
  //collapse


t_inicial=omp_get_wtime();
# pragma omp parallel for num_threads(THREADS) collapse(2)
  for ( i = 0; i < TAMANO; i++ ) {
    for ( j = 0; j < TAMANO; j++ ) {
      if ( matrix[i][j] == i4_huge ) {
#       pragma omp critical
        cout << "  Inf";
      }
      else {
#       pragma omp critical
        cout << "  " << setw(3) <<  matrix[i][j];
      }
    }
    //cout << "\n";
  }

  for ( i = 0; i < TAMANO; i++ ) {
//
//  Carry out the algorithm.
//

  mind = ppc_distance( matrix, i );
  t_final=omp_get_wtime()-t_inicial;
//
//  Print the results.
//
    cout << "\n";
    cout << "  Minimum distances from node " << i << ":\n";
    for ( j = 0; j < TAMANO; j++ ) {
      cout << "\t" << j;
    }
    cout << "\n";
    for ( j = 0; j < TAMANO; j++ ) {
      cout << "\t" << mind[j];
    }
    cout << "\n";
//
//  Terminate.
//
    delete [] mind;
  }

  cout << "\n";
  cout << "PPC\n";
  cout << "  Normal end of execution.\n";
  cout << " tiempo: "<<t_final<<" Segundos\n";
  cout << "\n";
  timestamp ( );

  return 0;
}


int *ppc_distance ( int matrix[TAMANO][TAMANO], int start_node ) {

//
//  Purpose:
//
//    PPC_DISTANCE uses PPC's minimum distance algorithm.
//
//  Discussion:
//
//    We essentially build a tree.  We start with only node 0 connected
//    to the tree, and this is indicated by setting CONNECTED[0] = TRUE.
//
//    We initialize MIND[I] to the one step distance from node 0 to node I.
//
//    Now we search among the unconnected nodes for the node MV whose minimum
//    distance is smallest, and connect it to the tree.  For each remaining
//    unconnected node I, we check to see whether the distance from 0 to MV
//    to I is less than that recorded in MIND[I], and if so, we can reduce
//    the distance.
//
//    After TAMANO-1 steps, we have connected all the nodes to 0, and computed
//    the correct minimum distances.
//
//  Parameters:
//
//    Input, int OHD[TAMANO][TAMANO], the distance of the direct link between
//    nodes I and J.
//
//    Output, int PPC_DISTANCE[TAMANO], the minimum distance from node 0
//    to each node.
//

  bool *connected;
  int i;
  int md;
  int *mind;
  int mv;
  int step;
//
//  Start out with only node 0 connected to the tree.
//
  connected = new bool[TAMANO];
  connected[start_node] = true;
  for ( i = 1; i < TAMANO; i++ ) {
    connected[i] = false;
  }
//
//  Initialize the minimum distance to the one-step distance.
//
  mind = new int[TAMANO];
#pragma omp for
  for ( i = 0; i < TAMANO; i++ ) {
    mind[i] = matrix[start_node][i];
  }
//
//  Attach one more node on each iteration.
//

  for ( step = 1; step < TAMANO; step++ ) {
//
//  Find the nearest unconnected node.
//

    find_nearest ( mind, connected, &md, &mv );

    if ( mv == - 1 ) {
      cout << "\n";
      cout << "PPC_DISTANCE - Warning!\n";
      cout << "  Search terminated early.\n";
      cout << "  Graph might not be connected.\n";
      break;
    }
//
//  Mark this node as connected.
//
    connected[mv] = true;
//
//  Having determined the minimum distance to node MV, see if
//  that reduces the minimum distance to other nodes.
//
#pragma omp critical
    update_mind ( mv, connected, matrix, mind );
  }

  delete [] connected;

  return mind;
}


void find_nearest ( int mind[TAMANO], bool connected[TAMANO], int *d, int *v ) {
//
//  Purpose:
//
//    FIND_NEAREST finds the nearest unconnected node.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Parameters:
//
//    Input, int MIND[TAMANO], the currently computed minimum distance from
//    node 0 to each node.
//
//    Input, bool CONNECTED[TAMANO], is true for each connected node, whose
//    minimum distance to node 0 has been determined.
//
//    Output, int *D, the distance from node 0 to the nearest unconnected node.
//
//    Output, int *V, the index of the nearest unconnected node.
//

  int i;
  int i4_huge = 2147483647;

  *d = i4_huge;
  *v = -1;
  # pragma omp parallel for num_threads(THREADS)
  for ( i = 0; i < TAMANO; i++ ) {
    if ( !connected[i] && mind[i] < *d ) {
      *d = mind[i];
      *v = i;
    }
  }
  return;
}


void init ( int matrix[TAMANO][TAMANO] ) {

//
//  Purpose:
//
//    INIT initializes the problem data.
//
//  Discussion:
//
//    The graph uses 6 nodes, and has the following diagram and
//    distance matrix:
//                                  0    1    2    3    4    5
//    N0--15--N2-100--N3       0    0   40   15  Inf  Inf  Inf
//      \      |     /         1   40    0   20   10   25    6
//       \     |    /          2   15   20    0  100  Inf  Inf
//        40  20  10           3  Inf   10  100    0  Inf  Inf
//          \  |  /            4  Inf   25  Inf  Inf    0    8
//           \ | /             5  Inf    6  Inf  Inf    8    0
//            N1
//            / \
//           /   \
//          6    25
//         /       \
//        /         \
//      N5----8-----N4
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Parameters:
//
//    Output, int OHD[TAMANO][TAMANO], the distance of the direct link between
//    nodes I and J.
//

  int i;
  int i4_huge = 2147483647;
  int j;
//collapse
  for ( i = 0; i < TAMANO; i++ ) {
    for ( j = 0; j < TAMANO; j++ ) {
      if ( i == j ) {
        matrix[i][i] = 0;
      }
      else {
        matrix[i][j] = i4_huge;
      }
    }
  }


// Version para el examen, recuerde cambiar TAMANO a 24
  matrix[0][1] = matrix[1][0] = 800;
  matrix[0][5] = matrix[5][0] = 1000;
  matrix[1][5] = matrix[5][1] = 950;
  matrix[1][2] = matrix[2][1] = 1000;
  matrix[2][3] = matrix[3][2] = 250;
  matrix[2][4] = matrix[4][2] = 1000;
  matrix[2][6] = matrix[6][2] = 1000;
  matrix[3][4] = matrix[4][3] = 800;
  matrix[4][7] = matrix[7][4] = 1200;
  matrix[3][6] = matrix[6][3] = 850;
  matrix[5][6] = matrix[6][5] = 1000;
  matrix[5][8] = matrix[8][5] = 1200;
  matrix[5][10] = matrix[10][5] = 1400;
  matrix[6][7] = matrix[7][6] = 1150;
  matrix[6][8] = matrix[8][6] = 1000;
  matrix[7][9] = matrix[9][7] = 900;
  matrix[8][9] = matrix[9][8] = 1000;
  matrix[8][10] = matrix[10][8] = 1400;
  matrix[8][11] = matrix[11][8] = 1000;
  matrix[9][12] = matrix[12][9] = 950;
  matrix[9][13] = matrix[13][9] = 850;
  matrix[10][11] = matrix[11][10] = 900;
  matrix[10][14] = matrix[14][10] = 1300;
  matrix[10][18] = matrix[18][10] = 2600;
  matrix[11][12] = matrix[12][11] = 900;
  matrix[11][15] = matrix[15][11] = 1000;
  matrix[12][13] = matrix[13][12] = 650;
  matrix[12][16] = matrix[16][12] = 1100;
  matrix[13][17] = matrix[17][13] = 1200;
  matrix[14][15] = matrix[15][14] = 600;
  matrix[14][19] = matrix[19][14] = 1300;
  matrix[15][20] = matrix[20][15] = 1000;
  matrix[15][21] = matrix[21][15] = 800;
  matrix[16][17] = matrix[17][16] = 800;
  matrix[16][21] = matrix[21][16] = 850;
  matrix[16][22] = matrix[22][16] = 1000;
  matrix[17][23] = matrix[23][17] = 900;
  matrix[18][10] = matrix[10][18] = 2600;
  matrix[18][19] = matrix[19][18] = 1200;
  matrix[19][20] = matrix[20][19] = 700;
  matrix[20][21] = matrix[20][21] = 300;
  matrix[21][22] = matrix[22][21] = 600;
  matrix[22][23] = matrix[23][22] = 900;
/*

// Version original de problema, de ser TAMANO 6
  matrix[0][1] = matrix[1][0] = 40;	// A, B -> 40
  matrix[0][2] = matrix[2][0] = 15;	// A, C -> 15
  matrix[1][2] = matrix[2][1] = 20;	// B, C -> 20
  matrix[1][3] = matrix[3][1] = 10;	// B, D -> 10
  matrix[3][4] = matrix[4][3] = 100;	// C, D -> 100
  matrix[1][4] = matrix[4][1] = 25;	// B, E -> 10
  matrix[1][5] = matrix[5][1] = 6;	// B, E -> 6
  matrix[4][5] = matrix[5][4] = 8;	// E, F -> 10
*/
  return;
}


void timestamp ( ) {

//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

void update_mind ( int mv, bool connected[TAMANO], int matrix[TAMANO][TAMANO], int mind[TAMANO] ) {
//
//  Purpose:
//
//    UPDATE_MIND updates the minimum distance vector.
//
//  Discussion:
//
//    We've just determined the minimum distance to node MV.
//
//    For each node I which is not connected yet,
//    check whether the route from node 0 to MV to I is shorter
//    than the currently known minimum distance.
//
//  Parameters:
//
//    Input, int MV, the node whose minimum distance to node 0
//    has just been determined.
//
//    Input, bool CONNECTED[TAMANO], is true for each connected node, whose
//    minimum distance to node 0 has been determined.
//
//    Input, int OHD[TAMANO][TAMANO], the distance of the direct link between
//    nodes I and J.
//
//    Input/output, int MIND[TAMANO], the currently computed minimum distances
//    from node 0 to each node.
//

  int i;
  int i4_huge = 2147483647;

  for ( i = 0; i < TAMANO; i++ ) {
    if ( !connected[i] ) {
//
//  If we really use the maximum integer (or something close) to indicate
//  no link, then we'll get burned if we add it to another value;
//  Integer arithmetic can "wrap around", so that 17 + i4_huge becomes
//  a very negative number!  So first we eliminate the possiblity that
//  the link is infinite.
//
      if ( matrix[mv][i] < i4_huge ) {
        if ( mind[mv] + matrix[mv][i] < mind[i] ) {
          mind[i] = mind[mv] + matrix[mv][i];
        }
      }
    }
  }
  return;
}
