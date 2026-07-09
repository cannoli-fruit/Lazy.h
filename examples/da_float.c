#include <stdio.h>
#define LAZY_IMPL
#include "lazy.h"

int main() {
  Lz_DA(float) nums = {0};
  lz_da_reserve(nums, 64);
  for (int i = 0; i < 32; ++i) {
    lz_da_append(nums, 2.0*i);
  }

  for (int i = 0; i < nums.cnt; ++i) {
    printf("%f\n", nums.elts[i]);
  }
  
  return 0;
}
