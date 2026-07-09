#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define LAZY_IMPL
#include "../lazy.h"

int main() {
  char *src = lz_cstr_dup(" \t   This text has some extra whitespace  \t ");
  Lz_Slc s = lz_cstr_to_slc(src);

  printf("|"LZ_STR_FMT"|\n", LZ_STR_PRINTF(s));
  lz_trim_typ(&s, isspace);
  printf("|"LZ_STR_FMT"|\n", LZ_STR_PRINTF(s));

  free(src); // excessive but this is how you should do it

  return 0;
}
