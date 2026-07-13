#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#ifndef LZ_STR_FMT 
#define LZ_STR_FMT "%.*s"
#endif

#ifndef LZ_STR_PRINTF
#define LZ_STR_PRINTF(slc) (int)slc.cnt, slc.data
#endif

#ifndef LZ_SLC_DEF
#define LZ_SLC_DEF
typedef struct {
  char *data;
  size_t cnt;
} Lz_Slc;
#endif

void lz_trim_left_typ(Lz_Slc *src, int (*classfunc)(int));
void lz_trim_right_typ(Lz_Slc *src, int (*classfunc)(int));
void lz_trim_typ(Lz_Slc *src, int (*classfunc)(int));
void lz_drop_left(Lz_Slc *src, size_t n);
void lz_drop_right(Lz_Slc *src, size_t n);
void lz_split_slc(Lz_Slc *src, char delim, Lz_Slc *dst);
void lz_split_slc_typ(Lz_Slc *src, int (*classfunc)(int), Lz_Slc *dst);
size_t lz_hm_hash_raw_bytes(size_t cap, void *key, size_t k_s);
size_t lz_hm_hash_void_get(size_t cap, _Bool *exists, void *key,
                           void *keys, size_t k_s);
size_t lz_hm_hash_void_set(size_t cap, _Bool *exists, void* key, size_t k_s);
Lz_Slc lz_cstr_to_slc(char *dat);
char *lz_cstr_dup(const char *s);

#ifndef Lz_DA
#define Lz_DA(T) struct {T *data; size_t cnt; size_t cap; }
#endif

#ifndef lz_da_append
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
#endif

#ifndef lz_da_insert
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
#endif

#ifndef lz_da_pop
#define lz_da_pop(da) do {\
  if ((da).cnt > 0) {\
    (da).cnt--;\
  }\
} while(0)
#endif

#ifndef lz_da_concat
#define lz_da_concat(dst,src) do {\
  if (sizeof(*(dst).data) != sizeof(*(dst).data)) {\
    assert(0 && "Mismatched types of arrays being concatenated");\
  }\
  lz_da_reserve((dst),((src).cnt+(dst).cnt));\
  memcpy((dst).data+(dst).cnt, (src).data, (src).cnt*sizeof(*(dst).data));\
  (dst).cnt = ((src).cnt+(dst).cnt);\
} while(0)
#endif

#ifndef lz_da_destroy
#define lz_da_destroy(da) do { free((da).data); } while (0);
#endif

#ifndef lz_da_reserve
#define lz_da_reserve(da, size) do {\
  if ((da).cap < (size)) {\
    (da).data = realloc((da).data, (size)*sizeof(*(da).data));\
    (da).cap = (size);\
  }\
} while (0)
#endif

#ifndef lz_da_shrink_to_fit
#define lz_da_shrink_to_fit(da) do {\
  if ((da).cnt == (da).cap) break;\
  (da).data = realloc((da).data, (da).cnt * sizeof(*(da).data));\
  (da).cap = (da).cnt;\
} while(0)
#endif

#ifndef lz_da_foreach
#define lz_da_foreach(T, name, da) \
  for (T *name = (da).data; name-(da).data < (da).cnt; ++name)
#endif

typedef Lz_DA(char) Lz_SB;

#ifndef Lz_HM
#define Lz_HM(K_t, V_t) struct {\
  size_t cap;\
  size_t cnt;\
  size_t tmphash;\
  V_t defval;\
  K_t tmpkey;\
  V_t tmpval;\
  _Bool *exists;\
  K_t *keys;\
  V_t *vals;\
}
#endif

#ifndef lz_hm_init
#define lz_hm_init(hm) do {\
  (hm).cap = 256;\
  (hm).cnt = 0;\
  (hm).keys = malloc((hm).cap*sizeof(*(hm).keys));\
  (hm).vals = malloc((hm).cap*sizeof(*(hm).vals));\
  (hm).exists = malloc((hm).cap*sizeof(_Bool));\
} while (0)
#endif

#ifndef lz_hm_hash_set
#define lz_hm_hash_set(hm, k) (\
  (hm).tmpkey = k,\
  lz_hm_hash_void_set((hm).cap, (hm).exists, &(hm).tmpkey, sizeof(*(hm).keys))\
)
#endif

#ifndef lz_hm_hash_get
#define lz_hm_hash_get(hm, k) (\
  (hm).tmpkey = k,\
  lz_hm_hash_void_get((hm).cap, (hm).exists, &(hm).tmpkey, (hm).keys, sizeof(*(hm).keys))\
)
#endif

#ifndef lz_hm_exists
#define lz_hm_exists(hm, k) (\
  (hm).tmphash = lz_hm_hash_get(hm, k),\
  ((hm).tmphash != (size_t)-1)\
)
#endif

#ifndef lz_hm_get
#define lz_hm_get(hm, k) (\
  (hm).tmphash = lz_hm_hash_get(hm, k),\
  ((hm).tmphash == (size_t)-1) ? (hm).defval : ((hm).vals[(hm).tmphash])\
)
#endif

#ifndef lz_hm_grow
#define lz_hm_grow(hm) do {\
  size_t newcap = (hm).cap*2;\
  void *newkeys = malloc(newcap*sizeof(*(hm).keys));\
  void *newvals = malloc(newcap*sizeof(*(hm).vals));\
  _Bool *newexists = calloc(newcap, sizeof(_Bool));\
  \
  for (size_t i = 0; i < (hm).cap; ++i) {\
    if ((hm).exists[i]) {\
      size_t newhash = lz_hm_hash_void_set(newcap, newexists,\
                                           &(hm).keys[i], sizeof(*(hm).keys));\
      memcpy(newvals + sizeof(*(hm).vals)*newhash,\
             &(hm).vals[i], sizeof(*(hm).vals));\
      memcpy(newkeys + sizeof(*(hm).keys)*newhash,\
             &(hm).keys[i], sizeof(*(hm).keys));\
      newexists[newhash] = 1;\
    }\
  }\
  free((hm).vals);\
  (hm).vals = newvals;\
  free((hm).keys);\
  (hm).keys = newkeys;\
  free((hm).exists);\
  (hm).exists = newexists;\
  (hm).cap = newcap;\
} while(0)
#endif

#ifndef lz_hm_add
#define lz_hm_add(hm, k, v) do {\
  size_t idx = lz_hm_hash_set(hm, k);\
  (hm).keys[idx] = k;\
  (hm).vals[idx] = v;\
  ++(hm).cnt;\
  (hm).exists[idx] = 1;\
  if (4*(hm).cnt > 3*(hm).cap) {\
    lz_hm_grow(hm);\
  }\
} while(0)
#endif

#ifdef LAZY_IMPL

size_t lz_hm_hash_raw_bytes(size_t cap, void *key, size_t k_s) {
  size_t hash = 0;
  for (size_t i = 0; i < k_s; ++i) {
    size_t b = (size_t)(((uint8_t*)key)[i]);
    hash += b*31415 + 9265; //bytewise hash
    hash %= cap;
  }
  return hash % cap;
}

size_t lz_hm_hash_void_get(size_t cap, _Bool *exists, void *key,
                           void *keys, size_t k_s) {
  size_t hash = lz_hm_hash_raw_bytes(cap, key, k_s);
  
  for (size_t i = 0; i < cap; ++i) {
    size_t localhash = (hash + i) % cap;
    int diff = memcmp(keys+k_s*localhash, key, k_s);
    if (diff == 0) {
      return localhash;
    }
    
    size_t istaken = exists[localhash];
    if (!istaken) {
      return (size_t)-1;
    }
  }
  return (size_t)-1;
}

size_t lz_hm_hash_void_set(size_t cap, _Bool *exists, void* key, size_t k_s) {
  size_t hash = lz_hm_hash_raw_bytes(cap, key, k_s);

  for (size_t i = 0; i < cap; ++i) {
    size_t localhash = (hash + i) % cap;
    _Bool istaken = exists[localhash];
    if (!istaken) return localhash;
  }
  return (size_t)-1;
}

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
