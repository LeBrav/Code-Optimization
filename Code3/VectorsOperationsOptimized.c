#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>

// Define here as constant for easy change
#define REAL float

void printCheck ( REAL V[], int N )
{
  int x;

  REAL S=0;
  for (x=0; x<=N+1; x++)
    S = S + V[x];

  printf("\nCheckSum = %1.10e\nSome values: ", S);

  for (x=0; x<10; x++)
    printf("(%d)=%1.10f, ", x*N/10, V[x*N/10]);

  printf("(%d)=%1.10f\n", x*N/10, V[x*N/10]);
}

int main(int argc, char **argv)
{
  int  x, t, N= 10000000, T=1000 ;
  REAL L= 0.123456, L2, S;
  REAL *U1;

  if (argc>1) { T = atoi(argv[1]); } // get  first command line parameter
  if (argc>2) { N = atoi(argv[2]); } // get second command line parameter
  if (argc>3) { L = atof(argv[3]); } // get  third command line parameter
 
  if (N < 1 || T < 1 || L >= 0.5) {
    printf("arguments: T N L (T: steps, N: vector size, L < 0.5)\n");
    return 1;
  }

  U1 = (REAL *) malloc ( sizeof(REAL)*(N+2) );

  // initialize temperatures at time t=0  
  for (x=1; x<N+1; x++)
    U1[x] = x*3.1416;
 
  // initialize fixed boundary conditions on U1
    U1[0]  = 1.2345678e+12;
    U1[N+1]= -1.2345678e+16;

  printf("Challenge #3: Simulate %d steps on 1-D vector of %d elements with L=%1.10e\n", T, N, L);


  for (t=0; t<T; t++)
  {  // loop on time
    
	REAL aux1 = U1[0];
	for (int x=1; x<=N; x+=16)
	{
		_mm_prefetch((const char*) &U1[x+16], _MM_HINT_T0);
		for (int j=0; j<16; j++)
		{
			REAL aux2 = U1[x+j];
			U1[x+j] = aux2*(1 - 2.0f*L)+ L*(U1[x+1+j] +aux1);
			aux1 = aux2;
		}
		
		
	}

	
  }

  printCheck(U1,N);
  free(U1); 
}