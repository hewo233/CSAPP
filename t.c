<<<<<<< HEAD
#include<stdio.h>

int floatFloat2Int(unsigned uf) {

  printf("%u\n", uf);

  int sign = (uf & 0x80000000);
  int exp = ((uf & 0x7f800000) >> 23) - 127;
  int frac = (uf & 0x007fffff);

  printf("sign: %d exp: %d frac: %d\n", sign, exp, frac);

  if(exp == 128 && frac ==0) return 1;
  if(exp > 30 ) return 0x80000000u;
  if(exp == -127) return 0;
  if(exp == 0)
  {
    if(sign) return -1;
    return 1;
  }

  frac = frac | 0x00800000;

  int result;

  if(exp < 0) result = frac >> -exp;
  else result = frac << exp;

  if(sign) return -result;
  return result;

}
int main()
{
    unsigned x;
    scanf("%u", &x);
    printf("%d\n", floatFloat2Int(x));
=======
#include <stdio.h>

int howManyBits(int x) {
  int isP = x >> 31; // - 111 + 000
  int laP = x & 1;
  int xx = (x & (~isP)) | ((~(x-1)) & isP);

  int bit_16 = (!!(xx >> 16)) << 4;
  xx = xx >> bit_16;

  int bit_8 = (!!(xx >> 8)) << 3;
  xx = xx >> bit_8;

  int bit_4 = (!!(xx >> 4)) << 2;
  xx = xx >> bit_4;

  int bit_2 = (!!(xx >> 2)) << 1;
  xx = xx >> bit_2;

  int bit_1 = (!!(xx >> 1));
  xx = xx >> bit_1;

  int bit_0 = (!!(xx));

  int checkINF = !(x ^ (1 << 31));
  checkINF = ~checkINF + 1;

  printf("%d %d %d %d %d %d\n", bit_16, bit_8, bit_4, bit_2, bit_1, bit_0);

  return bit_16 + bit_8 + bit_4 + bit_2 + bit_1 + bit_0 + 1 + checkINF;
  
}

int main()
{
    int x;
    printf("%d\n", ~0);
    scanf("%d", &x);
    printf("%d\n", howManyBits(x));
>>>>>>> bbb6e377eccb51ee231448f4d2f75603ed6dea65
    return 0;
}