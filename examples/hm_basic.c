#include <stdio.h>
#define LAZY_IMPL
#include "../lazy.h"

void printHmIdx(void *hm, int k) {
  typedef Lz_HM(int,int) HM;
  HM map = *(HM*)hm;
  if (lz_hm_exists(map,k)) {
    printf("hm[%d] = %d\n", k, lz_hm_get(map,k));
  } else {
    printf("hm[%d] = <N/A>\n", k);
  }
}

int main() {
  Lz_HM(int, int) hm;
  lz_hm_init(hm);

  lz_hm_add(hm, 1, 2);
  lz_hm_add(hm, 2, 4);

  printf("Hello World!\n");
  printHmIdx(&hm, 1);
  printHmIdx(&hm, 3);
}
