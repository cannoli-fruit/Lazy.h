#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LZ_STR_FMT "%.*s"
#define LZ_STR_PRINTF(slc) (int)slc.cnt, slc.data

typedef struct {
  char *data;
  size_t cnt;
} Lz_Slc;

void lz_trim_left_typ(Lz_Slc *src, int (*classfunc)(int));
void lz_trim_right_typ(Lz_Slc *src, int (*classfunc)(int));
void lz_trim_typ(Lz_Slc *src, int (*classfunc)(int));
void lz_drop_left(Lz_Slc *src, size_t n);
void lz_drop_right(Lz_Slc *src, size_t n);
void lz_split_slc(Lz_Slc *src, char delim, Lz_Slc *dst);
void lz_split_slc_typ(Lz_Slc *src, int (*classfunc)(int), Lz_Slc *dst);
Lz_Slc lz_cstr_to_slc(char *dat);
char *lz_cstr_dup(const char *s);

#define Lz_DA(T) struct {T *data; size_t cnt; size_t cap; }
#define lz_da_append(da, x) do { \
  if ((da).cap > (da).cnt) {\
    (da).data[(da).cnt++] = (x);\
  } else if((da).cap == 0) {\
    (da).data = malloc(256*sizeof(*(da).data));\
    (da).cap = 256;\
    (da).cnt = 0;\
    (da).data[(da).cnt++] = (x);\
  } else {\
    (da).data = realloc((da).data, sizeof(*(da).data)*(da).cap*(2));\
    (da).cap *= 2;\
    (da).data[(da).cnt++] = (x);\
  }\
} while(0)
#define lz_da_insert(da, i, x) do {\
  if ((i) > (da).cnt) {\
    assert(0 && "Insert out of dynamic array size");\
  }\
  if ((i) < 0) {\
    assert(0 && "Negative index of insertion");\
  }\
  if ((da).cap > (da).cnt) {\
    memmove((da).data+(i)+1,(da).data+(i),((da).cnt-(i))*sizeof(*(da).data));\
    (da).data[i] = (x);\
    (da).cnt++;\
  } else if ((da).cap == (da).cnt) {\
    (da).data = realloc((da).data, sizeof(*(da).data)*(da).cap*2);\
    (da).cap *= 2;\
    memmove((da).data+(i)+1,(da).data+(i),((da).cnt-(i))*sizeof(*(da).data));\
    (da).data[i] = (x);\
    (da).cnt++;\
  } else {\
    assert(0 && "Unreachable thru proper dynamic array functions");\
  }\
} while(0)
#define lz_da_pop(da) do {\
  if ((da).cnt > 0) {\
    (da).cnt--;\
  }\
} while(0)
#define lz_da_concat(dst,src) do {\
  if (sizeof(*(dst).data) != sizeof(*(dst).data)) {\
    assert(0 && "Mismatched types of arrays being concatenated");\
  }\
  lz_da_reserve((dst),((src).cnt+(dst).cnt));\
  memcpy((dst).data+(dst).cnt, (src).data, (src).cnt*sizeof(*(dst).data));\
  (dst).cnt = ((src).cnt+(dst).cnt);\
} while(0)
#define lz_da_destroy(da) do { free((da).data); } while (0);
#define lz_da_reserve(da, size) do {\
  if ((da).cap < (size)) {\
    (da).data = realloc((da).data, (size)*sizeof(*(da).data));\
    (da).cap = (size);\
  }\
} while (0)
#define lz_da_shrink_to_fit(da) do {\
  if ((da).cnt == (da).cap) break;\
  (da).data = realloc((da).data, (da).cnt * sizeof(*(da).data));\
  (da).cap = (da).cnt;\
} while(0)

typedef Lz_DA(char) Lz_SB;

#ifdef LAZY_IMPL

// Horribly enough, strdup is a gnu extension
// Here's a portable implementation
char *lz_cstr_dup(const char *s) {
  if (!s) return NULL;
  size_t n = strlen(s);
  char *p = malloc((n+1)*sizeof(char));
  if (!p) return NULL;
  memcpy(p, s, n+1);
  return p;
}
void lz_drop_left(Lz_Slc *src, size_t n) {
  if (src->cnt < n) n = src->cnt;
  src->data += n;
  src->cnt -= n;
}

void lz_drop_right(Lz_Slc *src, size_t n) {
  if (src->cnt < n) n = src->cnt;
  src->cnt -= n;
}

void lz_trim_left_typ(Lz_Slc *src, int (*classfunc)(int)) {
  while (classfunc(src->data[0])) {
    lz_drop_left(src, 1);
  }
}

void lz_trim_right_typ(Lz_Slc *src, int (*classfunc)(int)) {
  while (classfunc(src->data[src->cnt-1])) {
    lz_drop_right(src, 1);
  }
}

void lz_trim_typ(Lz_Slc *src, int (*classfunc)(int)) {
  lz_trim_left_typ(src, classfunc);
  lz_trim_right_typ(src, classfunc);
}

void lz_split_slc(Lz_Slc *src, char delim, Lz_Slc *dst) {
  size_t i = 0;
  while (i < src->cnt && src->data[i] != delim) {
    ++i;
  }
  dst->data = src->data;
  dst->cnt = i;
  if (src->cnt != i) {
    src->data += i+1;
    src->cnt -= i+1;
  } else {
    src->cnt = 0;
  }
}

void lz_split_slc_typ(Lz_Slc *src, int (*classfunc)(int), Lz_Slc *dst) {
  size_t i = 0;
  while (i < src->cnt && !classfunc(src->data[i])) {
    ++i;
  }
  dst->data = src->data;
  dst->cnt = i;
  if (src->cnt != i) {
    src->data += i+1;
    src->cnt -= i+1;
  } else {
    src->cnt = 0;
  }
  lz_trim_left_typ(src, classfunc);
}

Lz_SB lz_sb_from_cstr(const char *dat) {
  size_t len = strlen(dat);
  Lz_SB sb = {0};
  lz_da_reserve(sb, len);
  memcpy(sb.data, dat, len*sizeof(char));
  sb.cnt = len;
  return sb;
}

Lz_Slc lz_cstr_to_slc(char *dat) {
  Lz_Slc out = {0};
  out.data = dat;
  out.cnt = strlen(dat);
  return out;
}
#endif
