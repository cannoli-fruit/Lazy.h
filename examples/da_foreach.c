#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LAZY_IMPL
#include "../lazy.h"

int main() {
  srand(time(NULL));
  Lz_DA(int) nums = {0};
  for (int i = 0; i < 10; ++i) {
    lz_da_append(nums, rand()%100);
  }

  lz_da_foreach(int, x, nums) {
    printf("%d\n", *x);
  }
}
