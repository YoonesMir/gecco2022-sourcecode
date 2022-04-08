#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
 
int randn (double mu, double sigma, int multiplier)
{
  srand(time(NULL));
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
  int returnValue = (int)((mu + sigma * (double) X1) * multiplier);
  printf("%f \n", (mu + sigma * (double) X1));
  printf("%d \n",returnValue );
  return returnValue;
}

void main(){
    randn(0,1,10);
}