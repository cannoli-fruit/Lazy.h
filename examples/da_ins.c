#include <stdio.h>
#define LAZY_IMPL
#include "../lazy.h"

int main() {
  Lz_DA(int) xs = {0};
  lz_da_append(xs, 1);
  lz_da_append(xs, 3);
  lz_da_append(xs, 4);
  lz_da_shrink_to_fit(xs);

  for (int i = 0; i < xs.cnt; ++i) {
    printf("%d: %d\n", i, xs.elts[i]);
  }

  printf("\n");
  lz_da_insert(xs,1,2);
  
  for (int i = 0; i < xs.cnt; ++i) {
    printf("%d: %d\n", i, xs.elts[i]);
  }
}
