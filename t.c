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
    return 0;
}