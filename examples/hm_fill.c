#include <stdio.h>
#define LAZY_IMPL
#include "../lazy.h"

int main() {
  Lz_HM(size_t,size_t) hm;
  lz_hm_init(hm);
  for (size_t i = 0; i < 384; ++i) {
    lz_hm_add(hm, i, 2*i);
  }

  for (size_t i = 0; i < 384; ++i) {
    printf("hm[%zu] = %zu\n", i, lz_hm_get(hm,i));
  }
}
