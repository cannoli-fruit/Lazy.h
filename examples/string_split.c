#define LAZY_IMPL
#include "../lazy.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int isnotalpha(int x) { return !isalpha(x); }

int main() {
  char *src =  lz_cstr_dup("This is a list of words, which apparently has been referred to previously as a sentence");
  Lz_Slc s = lz_cstr_to_slc(src);

  Lz_Slc word;
  int i = 0;
  do {
    lz_split_slc_typ(&s, isnotalpha, &word);
    printf("Word %d: "LZ_STR_FMT"\n", i, LZ_STR_PRINTF(word));
    ++i;
  } while(s.cnt != 0);

  free(src);

  return 0;

}
