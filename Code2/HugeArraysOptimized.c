#include <stdio.h>
#include <stdlib.h>

// Variable used to generate pseudo-random numbers
unsigned int seed;

// Function to generate pseudo-random numbers
int myRandom() {
  seed = (214013*seed+2531011);
  return (seed>>13);
}


void LongNumInit( unsigned char *L, unsigned N )
{
  for ( int i=0; i< N; i++ ) 
  {
    L[i] = myRandom() % 10;  // digito decimal
  }
}

void LongNumPrint( unsigned char *L, unsigned N,const char *Name )
{
  printf("%s:", Name);
  for ( int i=N; i>0; i-- ) 
  {
    printf("%d", L[i-1]);
  }
  printf("\n");
}


void LongNumSet( unsigned char * __restrict L, unsigned N,unsigned char digit ) 
{
  for ( int i=0; i< N; i++ ) 
  {
    L[i] = digit;
  }
}


void LongNumCopy( unsigned char * __restrict Vin,unsigned char * __restrict Vout, unsigned N ) 	
{
  for ( int i=0; i< N; i++ ) 
  {
    Vout[i] = Vin[i];
  }
}
void LongNumAddition( unsigned char * __restrict Vin1, unsigned char * __restrict Vin2, unsigned char *Vout, unsigned N )
{

  

  for ( int i=0; i< N; i++ ) 
    Vout[i] = Vin1[i] + Vin2[i];
  
  
		
  for ( int i=0; i< N-1; i++ ) 
  {
		Vout[i+1] = (Vout[i]>=10)? Vout[i+1]+1: Vout[i+1];

  }
  for ( int i=0; i< N; i++ ) 
  {
		Vout[i] = (Vout[i]>=10)? Vout[i] - 10: Vout[i];
  }
  

}


unsigned char LongNumAddDigit( unsigned char *V,unsigned char digit, unsigned N )
{
  int i=0;
  unsigned char R = V[0] + digit;
  if (R < 10)
  {
    V[0] = R; return 0; // No carry
  }

  V[0] = R-10;
  // add carry, maybe iteratively for all digits
  unsigned char CARRY = 1;
  i = 1;
  while ( CARRY && i < N ) 
  {
    if ( V[i] < 9 )
    {
      V[i] = V[i] + 1; CARRY = 0;
    }
    else 
    {
      V[i] = 0; i++;  // CARRY remains set to 1
    }
  }
  return CARRY;
}


void LongNumHorizAdd( unsigned char *Vin,unsigned char *Vout, unsigned N )
{
  LongNumSet ( Vout, N, 0 );
  for ( int i=0; i< N; i++ ) 
  {
    LongNumAddDigit ( Vout, Vin[i], N );
  }
}

void SumarIMult(unsigned char * __restrict Vin1, unsigned char * __restrict Vin2, unsigned char *Vout, unsigned N,  unsigned char digit )
{
  for ( int i=0; i< N; i++ ){
	Vout[i] = (Vin1[i] * digit) + Vin2[i];
  }


  
for ( int i=1; i< N; i+=2 ) 
{
	Vout[i] = Vout[i]+Vout[i-1]/10;
	Vout[i-1] -=(Vout[i-1]/10)*10 ;
}

for ( int i=2; i< N; i+=2 ) 
{
	Vout[i] = Vout[i]+Vout[i-1]/10;
	Vout[i-1] -= (Vout[i-1]/10)*10;
	}

   
}


int main (int argc, char **argv)
{
  int i, sum1, sum2, sum3, N=1, Rep=1000;

  seed = 12345;

  // obtain parameters at run time
  if (argc>1) { N    = atoi(argv[1]); }
  if (argc>2) { Rep  = atoi(argv[2]); }

  printf("Challenge #2: Vector size is %d. Repeat %d times\n", N, Rep);

  // Create Long Nums
  unsigned char *V1= (unsigned char*) malloc( N*sizeof(unsigned char) );
  unsigned char *V2= (unsigned char*) malloc( N*sizeof(unsigned char) );
  unsigned char *V3= (unsigned char*) malloc( N*sizeof(unsigned char) );
  unsigned char *V4= (unsigned char*) malloc( N*sizeof(unsigned char) );
 
  LongNumInit ( V1, N ); LongNumInit ( V2, N ); LongNumInit ( V3, N );

  // Repeat
  for (i=0; i<Rep; i++)
  {
    LongNumAddition ( V1, V2, V4, N );
	
	// Create Temporal Long Integer with double size
	unsigned char *RES = (unsigned char*) malloc( 2*N*sizeof(unsigned char) );

	LongNumSet  ( RES, 2*N, 0 );    // Set RES to 0

	int NewN = N;
	for (  int i=0;i<N; i++ ){
		
	  SumarIMult(V3, RES+i, RES+i,NewN ,  V4[i] );
	  NewN = N-i;
	}

	// Result goes to V1-V2
	LongNumCopy ( RES,   V1, N );  // Copy RES   -> V1
	LongNumCopy ( RES+N, V2, N );  // Copy RES+N -> V2
	
	
    LongNumHorizAdd ( V1, V2, N );
    LongNumAddDigit ( V3, V2[0], N );
  }

  // Print last 32 digits of Long Numbers
  LongNumPrint( V1, 32, "V1" );
  LongNumPrint( V2, 32, "V2" );
  LongNumPrint( V3, 32, "V3" );
  LongNumPrint( V4, 32, "V4" );

  free(V1); free(V2); free(V3); free(V4);
  return 0;
}