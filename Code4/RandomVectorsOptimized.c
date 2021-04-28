#include <stdio.h>
#include <stdlib.h>

// Define here as constant for easy change
#define REAL float

// Variable used to generate pseudo-random numbers
unsigned int seed;

// Function to generate pseudo-random numbers
int myRandom() {//3,7%
  seed = (214013*seed+2531011);
  return (seed>>3);
}

void printCheck ( REAL V[], int N )
{
  int x;

  REAL S=0;
  for (x=0; x<=N+1; x++)
    S = S + V[x];

  printf("\nCheckSum = %1.10e\n", S);

  //for (x=0; x<8; x++)
  //  printf("%1.10e\n", V[x]);
  for (x=0; x<=8; x++)
    printf("%1.10e\n", V[x*N/8]);
}

int main(int argc, char **argv)
{
  int  x, t, N= 10000000, T=1000;
  REAL L= 0.123456789;
  REAL auxNUM = 1 - 2.0f*L;
  REAL *U1;

  seed = 12345;

  if (argc>1) { T = atoi(argv[1]); }   // get  first command line parameter
  if (argc>2) { N = atoi(argv[2]); }   // get second command line parameter
  if (argc>3) { L = atof(argv[3]); }   // get  third command line parameter
  if (argc>4) { seed = atof(argv[4]);} // get fourth command line parameter
 
  if (N < 1 || L >= 0.5) {
    printf("arguments: T N L (T: steps, N: vector size, L < 0.5)\n");
    return 1;
  }

  U1 = malloc ( sizeof(*U1)*(N+2) );

  // initialize temperatures at time t=0  
  REAL t1, t2, t3;
  t1= 0.1234; t2 = -0.9456; t3 = 0.9789;
  for (x=0; x<=N+1; x++)
  {
    U1[x] = t1*t2*t3;
    t1= t1>1.3?   0.1234: t1+0.00567;
    t2= t2>1.8?  -0.9456: t2+0.00987;
    t3= t3<0.007? 0.9789: t3-0.00321;
  }
 
  // initialize fixed boundary conditions on U1
  {
    U1[0]  = 1.2345678901;
    U1[N+1]= 1.2345678901;
  }

  printf("Challenge #4: Simulate %d steps on 1-D vector of %d elements with L=%1.10e\n", T, N, L);

  for (t=1; t<=T; t++)
  {  // loop on time
	
		
	//5,6%
	int *List = malloc ( sizeof(*List)*N/10 );
	#pragma omp parallel num_threads (8)
	#pragma omp for schedule(static)
	for (int x=0; x<N/10; x++)
		List[x] = myRandom();

	int max= List[0];
	int min= List[0];
	#pragma omp parallel num_threads (8)
	#pragma omp for schedule(static)
	for (int x=1; x<N/10; x++){
		max =  (List[x]>max)? List[x]:max;
		min = (List[x]<min)? List[x]: min;
	}

	free(List);

	REAL Factor = (max-min)/(REAL)(max+min);
	/*
	REAL aux1 = U1[0];
	#pragma omp parallel num_threads (8)
	#pragma omp for schedule(static)
	for (int x=1; x<=N; x++){ //45%
		REAL aux2 = U1[x];
		U1[x] = Factor*(aux2*(auxNUM)+ L*(U1[x+1] +aux1));
		aux1 = aux2;
	}
	*/
	int x=1;
	REAL aux0 = U1[0];
	REAL aux1 = U1[1];
	REAL aux2 = U1[2];
	REAL aux3 = U1[3];
	if(N&1)
	{
		U1[x] = Factor*(aux2*(auxNUM) + L*(aux3 + aux1));
		aux0 = aux1;
		aux1 = aux2;
		aux2 = aux3;
		aux3 = U1[x+3];
		x++;
	}
	for (; x<=N; x+=2)
	{
		
		U1[x] = Factor*(aux1*(auxNUM)+ L*(aux2 +aux0)); //1 - 3
		U1[x+1] = Factor*(aux2*(auxNUM)+ L*(aux3 +aux1)); //2 - 4
		
		aux0 = aux2;
		aux1 = aux3;
		aux2 = U1[x+3];
		aux3 = U1[x+4];

		
	}	
	
  }

  printCheck(U1,N);
  free(U1); 
}