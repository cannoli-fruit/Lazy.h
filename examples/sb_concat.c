#include <stdio.h>
#define LAZY_IMPL
#include "../lazy.h"

int main() {
  Lz_SB sb1 = lz_sb_from_cstr("Hello ");
  Lz_SB sb2 = lz_sb_from_cstr("World!");

  lz_da_concat(sb1,sb2);

  printf(LZ_STR_FMT"\n", LZ_STR_PRINTF(sb1));
  
  return 0;
}
