/*
cstring_arrays represent n strings stored contiguously, delimited by the NUL byte.

Instead of storing an array of char pointers (char **), cstring_arrays use this format:

array->indices = {0, 4, 9};
array->str = {'f', 'o', 'o', '\0', 'b', 'a', 'r', '\0', 'b', 'a', 'z', '\0'};

Each value in array->indices is the start position of a token in array->str. Each string
is NUL-terminated, so array->str->a + 4 is "bar", a valid NUL-terminated C string

array->str is a char_array, so all of the functions from char_array like char_array_cat_printf
can be used when building the contiguous string arrays as well.
*/

#ifndef CSTRING_ARRAY_BASE_H
#define CSTRING_ARRAY_BASE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

#define INVALID_INDEX(i, n) ((i) < 0 || (i) >= (n))

#endif

#ifdef CSTRING_ARRAY_ALIGNED
#define CSTRING_ARRAY_NAME cstring_array_aligned
#include "char_array/char_array_aligned.h"

#define CHAR_ARRAY_NAME char_array_aligned

#define INDEX_ARRAY_NAME index_array_aligned
#define ARRAY_NAME index_array_aligned
#define ARRAY_TYPE uint32_t
#include "aligned_array/aligned_array.h"
#undef ARRAY_NAME
#undef ARRAY_TYPE

#else
#define CSTRING_ARRAY_NAME cstring_array
#include "char_array/char_array.h"

#define CHAR_ARRAY_NAME char_array
#define INDEX_ARRAY_NAME index_array
#define ARRAY_NAME index_array
#define ARRAY_TYPE uint32_t
#include "array/array.h"
#undef ARRAY_NAME
#undef ARRAY_TYPE

#endif

typedef struct {
    INDEX_ARRAY_NAME *indices;
    CHAR_ARRAY_NAME *str;
} CSTRING_ARRAY_NAME;

#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)
#define CSTRING_ARRAY_FUNC(func) CONCAT(CSTRING_ARRAY_NAME, _##func)
#define CHAR_ARRAY_FUNC(func) CONCAT(CHAR_ARRAY_NAME, _##func)
#define INDEX_ARRAY_FUNC(func) CONCAT(INDEX_ARRAY_NAME, _##func)


static CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(new)(void) {
    CSTRING_ARRAY_NAME *array = malloc(sizeof(CSTRING_ARRAY_NAME));
    if (array == NULL) return NULL;

    array->indices = INDEX_ARRAY_FUNC(new)();
    if (array->indices == NULL) {
        free(array);
        return NULL;
    }

    array->str = CHAR_ARRAY_FUNC(new)();
    if (array->str == NULL) {
        INDEX_ARRAY_FUNC(destroy)(array->indices);
        free(array);
        return NULL;
    }

    return array;
}

static void CSTRING_ARRAY_FUNC(destroy)(CSTRING_ARRAY_NAME *self) {
    if (self == NULL) return;
    if (self->indices) {
        INDEX_ARRAY_FUNC(destroy)(self->indices);
    }
    if (self->str) {
        CHAR_ARRAY_FUNC(destroy)(self->str);
    }
    free(self);
}

static CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(new_size)(size_t size) {
    CSTRING_ARRAY_NAME *array = CSTRING_ARRAY_FUNC(new)();
    CHAR_ARRAY_FUNC(resize)(array->str, size);
    return array;
}

static CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(from_char_array)(CHAR_ARRAY_NAME *str) {
    if (str == NULL) return NULL;
    if (str->n == 0)
        return CSTRING_ARRAY_FUNC(new)();

    CSTRING_ARRAY_NAME *array = malloc(sizeof(CSTRING_ARRAY_NAME));
    if (array == NULL) return NULL;

    array->str = str;
    array->indices = INDEX_ARRAY_FUNC(new_size)(1);

    INDEX_ARRAY_FUNC(push)(array->indices, 0);
    char *ptr = str->a;
    for (uint32_t i = 0; i < str->n - 1; i++, ptr++) {
        if (*ptr == '\0') {
            INDEX_ARRAY_FUNC(push)(array->indices, i + 1);
        }
    }
    return array;
}

static inline size_t CSTRING_ARRAY_FUNC(num_strings)(CSTRING_ARRAY_NAME *self) {
    if (self == NULL) return 0;
    return self->indices->n;
}

static inline uint32_t CSTRING_ARRAY_FUNC(start_token)(CSTRING_ARRAY_NAME *self) {
    uint32_t index = (uint32_t)self->str->n;
    INDEX_ARRAY_FUNC(push)(self->indices, index);
    return index;
}

static inline size_t CSTRING_ARRAY_FUNC(capacity)(CSTRING_ARRAY_NAME *self) {
    return self->str->m;
}

static inline size_t CSTRING_ARRAY_FUNC(used)(CSTRING_ARRAY_NAME *self) {
    return self->str->n;
}

static inline void CSTRING_ARRAY_FUNC(terminate)(CSTRING_ARRAY_NAME *self) {
    CHAR_ARRAY_FUNC(terminate)(self->str);
}

static inline uint32_t CSTRING_ARRAY_FUNC(add_string)(CSTRING_ARRAY_NAME *self, char *str) {
    uint32_t index = CSTRING_ARRAY_FUNC(start_token)(self);
    CHAR_ARRAY_FUNC(append)(self->str, str);
    CHAR_ARRAY_FUNC(terminate)(self->str);
    return index;
}

static CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(from_strings)(char **strings, size_t n) {
    CSTRING_ARRAY_NAME *array = CSTRING_ARRAY_FUNC(new)();
    for (size_t i = 0; i < n; i++) {
        CSTRING_ARRAY_FUNC(add_string)(array, strings[i]);
    }
    return array;
}


static inline int32_t CSTRING_ARRAY_FUNC(get_offset)(CSTRING_ARRAY_NAME *self, uint32_t i) {
    if (INVALID_INDEX(i, self->indices->n)) {
        return -1;
    }
    return (int32_t)self->indices->a[i];
}

static inline char *CSTRING_ARRAY_FUNC(get_string)(CSTRING_ARRAY_NAME *self, uint32_t i) {
    int32_t data_index = CSTRING_ARRAY_FUNC(get_offset)(self, i);
    if (data_index < 0) return NULL;
    return self->str->a + data_index;
}

static bool CSTRING_ARRAY_FUNC(extend)(CSTRING_ARRAY_NAME *array, CSTRING_ARRAY_NAME *other) {
    if (array == NULL || other == NULL) return false;
    size_t n = CSTRING_ARRAY_FUNC(num_strings)(other);

    for (size_t i = 0; i < n; i++) {
        char *s_i = CSTRING_ARRAY_FUNC(get_string)(other, i);
        CSTRING_ARRAY_FUNC(add_string)(array, s_i);
    }
    return true;
}


static inline void CSTRING_ARRAY_FUNC(resize)(CSTRING_ARRAY_NAME *self, size_t size) {
    if (size < CSTRING_ARRAY_FUNC(capacity)(self)) return;
    CHAR_ARRAY_FUNC(resize)(self->str, size);
}

static inline void CSTRING_ARRAY_FUNC(clear)(CSTRING_ARRAY_NAME *self) {
    if (self == NULL) return;

    if (self->indices != NULL) {
        INDEX_ARRAY_FUNC(clear)(self->indices);
    }

    if (self->str != NULL) {
        CHAR_ARRAY_FUNC(clear)(self->str);
    }
}

static inline uint32_t CSTRING_ARRAY_FUNC(add_string_len)(CSTRING_ARRAY_NAME *self, char *str, size_t len) {
    uint32_t index = CSTRING_ARRAY_FUNC(start_token)(self);
    CHAR_ARRAY_FUNC(append_len)(self->str, str, len);
    CHAR_ARRAY_FUNC(terminate)(self->str);
    return index;
}

static inline void CSTRING_ARRAY_FUNC(append_string)(CSTRING_ARRAY_NAME *self, char *str) {
    CHAR_ARRAY_FUNC(append)(self->str, str);
}

static inline void CSTRING_ARRAY_FUNC(append_string_len)(CSTRING_ARRAY_NAME *self, char *str, size_t len) {
    CHAR_ARRAY_FUNC(append_len)(self->str, str, len);
}

static inline void CSTRING_ARRAY_FUNC(cat_string)(CSTRING_ARRAY_NAME *self, char *str) {
    CHAR_ARRAY_FUNC(cat)(self->str, str);
}

static inline void CSTRING_ARRAY_FUNC(cat_string_len)(CSTRING_ARRAY_NAME *self, char *str, size_t len) {
    CHAR_ARRAY_FUNC(cat_len)(self->str, str, len);
}

static inline int64_t CSTRING_ARRAY_FUNC(token_length)(CSTRING_ARRAY_NAME *self, uint32_t i) {
    if (INVALID_INDEX(i, self->indices->n)) {
        return -1;
    }
    if (i < self->indices->n - 1) {
        return self->indices->a[i+1] - self->indices->a[i] - 1;
    } else {        
        return self->str->n - self->indices->a[i] - 1;
    }
}

static CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(split_options)(char *str, const char *separator, size_t separator_len, bool ignore_consecutive, size_t *count) {
    *count = 0;
    CHAR_ARRAY_NAME *array = CHAR_ARRAY_FUNC(new_size)(strlen(str));

    bool last_was_separator = false;
    bool first_char = false;

    while (*str) {
        if ((separator_len == 1 && *str == separator[0]) || (memcmp(str, separator, separator_len) == 0)) {
            if (first_char && (!ignore_consecutive || !last_was_separator)) {
                CHAR_ARRAY_FUNC(push)(array, '\0');
            }
            str += separator_len;
            last_was_separator = true;
        } else {
            CHAR_ARRAY_FUNC(push)(array, *str);
            str++;
            last_was_separator = false;
            first_char = true;
        }
    }
    CHAR_ARRAY_FUNC(push)(array, '\0');

    CSTRING_ARRAY_NAME *string_array = CSTRING_ARRAY_FUNC(from_char_array)(array);
    *count = CSTRING_ARRAY_FUNC(num_strings)(string_array);

    return string_array;
}


static inline CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(split)(char *str, const char *separator, size_t separator_len, size_t *count) {
    return CSTRING_ARRAY_FUNC(split_options)(str, separator, separator_len, false, count);
}


static inline CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(split_ignore_consecutive)(char *str, const char *separator, size_t separator_len, size_t *count) {
    return CSTRING_ARRAY_FUNC(split_options)(str, separator, separator_len, true, count);
}


static CSTRING_ARRAY_NAME *CSTRING_ARRAY_FUNC(split_no_copy)(char *str, char separator, size_t *count) {
    *count = 0;
    char *ptr = str;
    size_t len = strlen(str);

    for (int i = 0; i < len; i++, ptr++) {
        if (*ptr == separator) {
            *ptr = '\0';
        }
    }

    CHAR_ARRAY_NAME *array = CHAR_ARRAY_FUNC(from_string_no_copy)(str, len);
    CSTRING_ARRAY_NAME *string_array = CSTRING_ARRAY_FUNC(from_char_array)(array);
    *count = CSTRING_ARRAY_FUNC(num_strings)(string_array);

    return string_array;
}

static char **CSTRING_ARRAY_FUNC(to_strings)(CSTRING_ARRAY_NAME *self) {
    char **strings = malloc(self->indices->n * sizeof(char *));

    for (int i = 0; i < CSTRING_ARRAY_FUNC(num_strings)(self); i++) {
        char *str = CSTRING_ARRAY_FUNC(get_string)(self, i);
        strings[i] = strdup(str);
    }

    CSTRING_ARRAY_FUNC(destroy)(self);
    return strings;
}


#undef CONCAT_
#undef CONCAT
#undef CSTRING_ARRAY_FUNC
#undef CHAR_ARRAY_FUNC
#undef CSTRING_ARRAY_NAME
#undef CHAR_ARRAY_NAME
#undef INDEX_ARRAY_NAME
#undef INDEX_ARRAY_FUNC