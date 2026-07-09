#include <stdio.h>
#include <ctype.h>
#define LAZY_IMPL
#include "lazy.h"

int main() {
  char *src = strdup(" \t   This text has some extra whitespace  \t ");
  Lz_Slc s = lz_cstr_to_slc(src);

  printf("|"LZ_SLC_FMT"|\n", LZ_SLC_PRINTF(s));
  lz_trim_typ(&s, isspace);
  printf("|"LZ_SLC_FMT"|\n", LZ_SLC_PRINTF(s));

  free(src); // excessive but this is how you should do it

  return 0;
}
