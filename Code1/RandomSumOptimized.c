#include <stdio.h>
#include <stdlib.h>

// Variable used to generate pseudo-random numbers
unsigned int seed;

// Function to generate pseudo-random numbers
inline int myRandom() {
  seed = (214013*seed+2531011);
  return(seed>>13);
}
/*
inline int opMod(int rand, int mod)
{
	int resultat = 0;
	for(int i = rand; i<0; i -= mod*2)
		resultat = i;
	if (resultat + mod <0){resultat += mod;}
	return resultat + mod;
}*/

void InitKernel( int * __restrict k, unsigned Klen )
{
  for ( int i=0; i< Klen; i++ ) {
    k[i] = myRandom() & 7; //el maxim nombre sera 7
  }
}

//struct pixel { unsigned char x; unsigned char y; unsigned char z; }; //3 bytes


void InitImage( unsigned char * __restrict x, unsigned char * __restrict y, unsigned char * __restrict z, unsigned N )
{


  for ( int j=0; j< N; j++ ) 
  {
    x[j] = myRandom() % 31;
    y[j] = myRandom() % 31;
    z[j] = myRandom() % 31;

  }
}

void TransfImage(unsigned char * __restrict x, unsigned char * __restrict y, unsigned char * __restrict z, unsigned N, const int *K, unsigned Klen )
{
  //struct pixel *T= (struct pixel*) malloc( N*sizeof(struct pixel) );



 //treballar amb arrays, mes rapid que struct
	unsigned char tx[N]; 
	unsigned char ty[N]; 
	unsigned char tz[N];
  // copy I to T in order to prevent data dependencies
  

	for(int i = 0; i<N; i++)
	{
		tx[i] = 0;
		ty[i] = 0;
		tz[i] = 0;
	}
 


	for (	int k=0  ;k < Klen; k++ )
		{
			int aux = K[k];
			int ITER = N-k;
			if((aux & 4) == 4){
				int i=0;
				if((ITER & 1) == 1)
				{
					tx[0] += x[k];
					i = 1;
				}
				for ( ; i < ITER ; i+=2 ) 
				{
					tx[i] += x[i+k];
					tx[i+1] +=  x[i+k+1];
				}
			}
			if((aux & 2) == 2){
				int i=0;
				if((ITER & 1) == 1)
				{
					ty[0] += y[k];
					i = 1;
				}
				for ( ; i< ITER ; i+=2 )
				{					
					ty[i] +=  y[i+k];
					ty[i+1] +=  y[i+k+1];
				}
			}
			if((aux & 1) == 1){
				int i=0;
				if((ITER & 1) == 1)
				{
					tz[0] += z[k];
					i = 1;
				}
				for ( ; i< ITER ; i+=2 ) 
				{
					tz[i] += z[i+k];
					tz[i+1] += z[i+k+1];
				}
			}
		}	
		
   
    
	for(int i = 0; i< N;i++){
		 unsigned sum;
		sum = tx[i] + ty[i] + tz[i]+1;
		x[i] = tx[i]*31/ sum;
		y[i] = ty[i]*31/ sum;
		z[i] = tz[i]*31/ sum;
	}
}


int main (int argc, char **argv)
{
  int i, sumX = 0, sumY= 0, sumZ = 0, N=10000, Klen=2000, Rep=1000;

  seed = 12345;

unsigned char * x =  (unsigned char *) malloc(N*sizeof(unsigned char));
unsigned char * y = (unsigned char *) malloc(N*sizeof(unsigned char));;
unsigned char * z = (unsigned char *) malloc(N*sizeof(unsigned char));;

  // obtain parameters at run time
  if (argc>1) { N    = atoi(argv[1]); }
  if (argc>2) { Klen = atoi(argv[2]); }
  if (argc>3) { Rep  = atoi(argv[3]); }

  printf("Challenge #1: Vector size is %d. Kernel size is %d. Repeat %d times\n", N, Klen, Rep);

  // Create Image & Kernel
  //struct pixel *image= (struct pixel*) malloc(30000); //N*sizeof(struct pixel) );
  int *K = (int *) malloc( Klen<<2 ); //*sizeofint
 
  InitImage ( x,y,z, N );
  InitKernel( K, Klen );

  // Repeat
  int ii;
  for (i=0; i<Rep; i++)
  {
    TransfImage( x,y,z, N, K, Klen ); 
    ii = myRandom() % N;  x[ii] = myRandom() % 31;
    ii = myRandom() % N;  y[ii] = myRandom() % 31;
    ii = myRandom() % N;  z[ii] = myRandom() % 31;
  }

	for (i = 0; i<N; i+=2)
	  {
		  sumX +=  x[i];
		  sumY += y[i];
		  sumZ += z[i];
		  sumX +=  x[i+1];
		  sumY += y[i+1];
		  sumZ += z[i+1];
	  }
    
  printf("Result: sumX= %d, sumY= %d, sumZ= %d\n", sumX, sumY, sumZ);

   free(K);

  return 0;
}