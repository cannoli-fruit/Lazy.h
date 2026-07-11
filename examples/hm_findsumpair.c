#include <stdio.h>
#define LAZY_IMPL
#include "../lazy.h"

// This is some CS problem I heard of
// You need to find any two indecies in the array which add to k
// Hashmaps are the main way to solve this better than O(n^2)
int main() {
  int vals[] = {7, 12, 25, 30, 43, 60, 1000, 50, 38, 62, 50};
  int N = sizeof(vals)/sizeof(vals[0]);
  int K = 100;

  Lz_HM(int,size_t) LUT;
  lz_hm_init(LUT);

  for (size_t i = 0; i < N; ++i) {
    lz_hm_add(LUT, vals[i], i);
  }

  for (size_t i = 0; i < N; ++i) {
    int v = vals[i];
    int other = K-v;
    if (lz_hm_exists(LUT, other)) {
      size_t otherIdx = lz_hm_get(LUT, other);
      if (otherIdx == i) continue;
      printf("Found pair: idx: %zu, %zu, vals %d, %d\n",
             i, otherIdx,
             v, other);
      break;
    }
  }
  return 0;
}
