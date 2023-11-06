#ifndef SCH_ARRAY_H
#define SCH_ARRAY_H
#include <stddef.h> // for size_t

// Constants =================================================

/// A value that represents a new dynamic array. (NULL)
#define DARNEW NULL

#ifndef SCH_DAR_GROWTH_FACTOR
/// The default growth factor for dynamic arrays.
/// @note This is a double. Although it is not recommended to change this,
///       it is possible to do so by defining SCH_DAR_GROWTH_FACTOR before
///       including this header.
# define SCH_DAR_GROWTH_FACTOR (2.0)
#endif // SCH_DAR_GROWTH_FACTOR

// Types =====================================================

/// An error code for array and dynamic array functions.
typedef enum sch_arerr_t
{
    ARERR_NONE = 0,
    ARERR_ALLOC,
    ARERR_INDEX,
    ARERR_NULL,
    ARERR_LEN,
    ARERR_CAP,
    ARERR_COUNT_
} arerr_t;

// Function Prototypes =======================================

arerr_t sch_arpush(void *arr, size_t elm_size, size_t len, size_t cap, const void *elm);
arerr_t sch_arins(void *arr, size_t elm_size, size_t len, size_t cap, size_t index, const void *elm);
arerr_t sch_arrem(void *arr, size_t elm_size, size_t len, size_t cap, size_t index);
arerr_t sch_arcpy(void *dst, const void *src, size_t elm_size, size_t len);
arerr_t sch_darfree(void **darr_ptr);
arerr_t sch_darpush(void **darr_ptr, size_t elm_size, const void *elm);
arerr_t sch_darpop(void **darr_ptr, size_t elm_size);
arerr_t sch_darins(void **darr_ptr, size_t elm_size, size_t index, const void *elm);
arerr_t sch_darrem(void **darr_ptr, size_t elm_size, size_t index);
arerr_t sch_darres(void **darr_ptr, size_t elm_size, size_t cap);
arerr_t sch_darfit(void **darr_ptr, size_t elm_size);
arerr_t sch_darsiz(void **darr_ptr, size_t elm_size, size_t len, const void *opt_fill);
arerr_t sch_darclr(void **darr_ptr, size_t elm_size);
size_t sch_darlen(const void *darr);
size_t sch_darcap(const void *darr);

/// Get a string representation of an array-related error code.
/// @param err The error code to get a string representation of.
/// @return A string representation of the error code, or NULL if the error code is invalid.
const char *arerr_str(arerr_t err);

// Generic Macros ============================================

/// Add an element to the end of an array. Do not use this macro with dynamic arrays.
/// @param arr The array to push to.
/// @param len The current length of the array.
/// @param cap The total capacity of the array.
/// @param elm The element to push.
/// @return An error code, or 0 if no error occurred.
# define arpush(arr, len, cap, elm) sch_arpush((arr), sizeof(*(arr)), (len), (cap), &(elm))

/// Insert an element into an array at a given index. Do not use this macro with dynamic arrays.
/// @param arr The array to insert into.
/// @param len The current length of the array.
/// @param cap The total capacity of the array.
/// @param index The index to insert at.
/// @param elm The element to insert.
/// @return An error code, or 0 if no error occurred.
# define arins(arr, len, cap, index, elm) sch_arins((arr), sizeof(*(arr)), (len), (cap), (index), &(elm))

/// Remove an element from an array at a given index. Do not use this macro with dynamic arrays.
/// @param arr The array to remove from.
/// @param len The current length of the array.
/// @param cap The total capacity of the array.
/// @param index The index to remove from.
/// @return An error code, or 0 if no error occurred.
#define arrem(arr, len, cap, index) sch_arrem((arr), sizeof(*(arr)), (len), (cap), (index))

/// Copy an array. Memory regions may overlap.
/// @param dst The destination array.
/// @param src The source array.
/// @param len The length of both arrays.
/// @return An error code, or 0 if no error occurred.
#define arcpy(dst, src, len) sch_arcpy((dst), (src), sizeof(*(dst)), (len))

/// Free a dynamic array. Sets the pointer to DARNEW (NULL), for reusability.
/// @param darr The dynamic array to free.
/// @return An error code, or 0 if no error occurred.
#define darfree(darr) sch_darfree((void **)&(darr))

/// Add an element to the end of a dynamic array.
/// @param darr The dynamic array to push to.
/// @param elm The element to push.
/// @return An error code, or 0 if no error occurred.
# define darpush(darr, elm) sch_darpush((void **)&(darr), sizeof(*(darr)), &(elm))

/// Remove an element from the end of a dynamic array.
/// @param darr The dynamic array to pop from.
/// @return An error code, or 0 if no error occurred.
#define darpop(darr) sch_darpop((void **)&(darr), sizeof(*(darr)))

/// Insert an element into a dynamic array at a given index.
/// @param darr The dynamic array to insert into.
/// @param index The index to insert at.
/// @param elm The element to insert.
/// @return An error code, or 0 if no error occurred.
# define darins(darr, index, elm) sch_darins((void **)&(darr), sizeof(*(darr)), (index), &(elm))

/// Remove an element from a dynamic array at a given index.
/// @param darr The dynamic array to remove from.
/// @param index The index to remove from.
/// @return An error code, or 0 if no error occurred.
#define darrem(darr, index) sch_darrem((void **)&(darr), sizeof(*(darr)), (index))

/// Reserve space for a number of elements in a dynamic array.
/// @param darr The dynamic array to reserve space in.
/// @param cap The number of elements to reserve space for.
/// @return An error code, or 0 if no error occurred.
#define darres(darr, cap) sch_darres((void **)&(darr), sizeof(*(darr)), (cap))

/// Fit a dynamic array to its length.
/// @param darr The dynamic array to fit.
/// @return An error code, or 0 if no error occurred.
#define darfit(darr) sch_darfit((void **)&(darr), sizeof(*(darr)))

/// Set the length of a dynamic array.
/// @param darr The dynamic array to set the length of.
/// @param len The length to set.
/// @param opt_fill_ptr A pointer to an optional element to fill the array with. (NULL to fill with zeroes)
/// @return An error code, or 0 if no error occurred.
#define darsiz(darr, len, opt_fill_ptr) sch_darsiz((void **)&(darr), sizeof(*(darr)), (len), (opt_fill_ptr))

/// Clear a dynamic array.
/// @param darr The dynamic array to clear.
/// @return An error code, or 0 if no error occurred.
#define darclr(darr) sch_darclr((void **)&(darr), sizeof(*(darr)))

/// Get the length of a dynamic array.
/// @param darr The dynamic array to get the length of.
/// @return The length of the dynamic array. (0 if NULL)
#define darlen(darr) sch_darlen((darr))

/// Get the capacity of a dynamic array.
/// @param darr The dynamic array to get the capacity of.
/// @return The capacity of the dynamic array. (0 if NULL)
#define darcap(darr) sch_darcap((darr))

/// Check if a dynamic array is empty.
/// @param darr The dynamic array to check.
/// @return 1 if the dynamic array is empty, 0 otherwise.
#define darempty(darr) (darlen((darr)) == 0)

#endif // SCH_ARRAY_H

// Implementation =============================================

#ifdef SCH_IMPL

#include <stdlib.h>
#include <string.h>

struct dar_header_t
{
    size_t len;
    size_t cap;
};

static inline struct dar_header_t *dar_header(const void *p)
{
    return ((struct dar_header_t *)p) - 1;
}

static inline void *sch_darmalloc(size_t n, size_t elm_size)
{
    struct dar_header_t *header = malloc(sizeof(struct dar_header_t) + (n * elm_size));
    if (header == NULL)
    {
        return NULL;
    }
    header->len = 0;
    header->cap = n;
    return (void *)(header + 1);
}

static inline void *sch_darrealloc(void *p, size_t n, size_t elm_size)
{
    if (p == NULL)
    {
        return sch_darmalloc(n, elm_size);
    }
    struct dar_header_t *header = dar_header(p);
    struct dar_header_t *new_header = realloc(header, sizeof(struct dar_header_t) + (n * elm_size));
    if (new_header == NULL)
    {
        return NULL;
    }
    header = new_header;
    header->cap = n;
    return (void *)(header + 1);
}

static inline arerr_t sch_dargrow(void **arr_ptr, size_t elm_size)
{
    struct dar_header_t *header = dar_header(*arr_ptr);
    size_t new_cap = (size_t)((double)header->cap * SCH_DAR_GROWTH_FACTOR);
    void *new_arr = sch_darrealloc(*arr_ptr, new_cap, elm_size);
    if (new_arr == NULL)
    {
        return ARERR_ALLOC;
    }
    *arr_ptr = new_arr;
    return ARERR_NONE;
}

static inline arerr_t sch_daralloc_if_null(void **arr_ptr, size_t elm_size)
{
    if (*arr_ptr == NULL)
    {
        *arr_ptr = sch_darmalloc(1, elm_size);
        if (*arr_ptr == NULL)
        {
            return ARERR_ALLOC;
        }
    }
    return ARERR_NONE;
}

arerr_t sch_arpush(void *arr, size_t elm_size, size_t len, size_t cap, const void *elm)
{
    if (len >= cap)
    {
        return ARERR_CAP;
    }
    char *write_ptr = ((char *)arr) + (len * elm_size);
    memcpy(write_ptr, elm, elm_size);
    return ARERR_NONE;
}

arerr_t sch_arins(void *arr, size_t elm_size, size_t len, size_t cap, size_t index, const void *elm)
{
    if (len >= cap)
    {
        return ARERR_CAP;
    }
    if (index > len)
    {
        return ARERR_INDEX;
    }
    char *write_ptr = ((char *)arr) + (index * elm_size);
    memmove(write_ptr + elm_size, write_ptr, (len - index) * elm_size);
    memcpy(write_ptr, elm, elm_size);
    return ARERR_NONE;
}

arerr_t sch_arrem(void *arr, size_t elm_size, size_t len, size_t cap, size_t index)
{
    (void)cap;
    if (len == 0)
    {
        return ARERR_LEN;
    }
    char *write_ptr = ((char *)arr) + (index * elm_size);
    memmove(write_ptr, write_ptr + elm_size, (len - index) * elm_size);
    return ARERR_NONE;
}

arerr_t sch_arcpy(void *dst, const void *src, size_t elm_size, size_t len)
{
    memmove(dst, src, elm_size * len);
    return ARERR_NONE;
}

arerr_t sch_darfree(void **dar_ptr)
{
    if (*dar_ptr == NULL)
    {
        return ARERR_NULL;
    }
    struct dar_header_t *header = dar_header(*dar_ptr);
    free(header);
    *dar_ptr = NULL;
    return ARERR_NONE;
}

arerr_t sch_darpush(void **arr_ptr, size_t elm_size, const void *elm)
{
    if (sch_daralloc_if_null(arr_ptr, elm_size) != ARERR_NONE)
    {
        return ARERR_ALLOC;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == header->cap)
    {
        if (sch_dargrow(arr_ptr, elm_size) != ARERR_NONE)
        {
            return ARERR_ALLOC;
        }
    }
    sch_arpush(*arr_ptr, elm_size, header->len, header->cap, elm);
    header->len++;
    return ARERR_NONE;
}

arerr_t sch_darpop(void **arr_ptr, size_t elm_size)
{
    (void)elm_size;
    if (*arr_ptr == NULL)
    {
        return ARERR_NULL;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == 0)
    {
        return ARERR_LEN;
    }
    header->len--;
    return ARERR_NONE;
}

arerr_t sch_darins(void **arr_ptr, size_t elm_size, size_t index, const void *elm)
{
    if (sch_daralloc_if_null(arr_ptr, elm_size) != ARERR_NONE)
    {
        return ARERR_ALLOC;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == header->cap)
    {
        if (sch_dargrow(arr_ptr, elm_size) != ARERR_NONE)
        {
            return ARERR_ALLOC;
        }
    }
    if (sch_arins(*arr_ptr, elm_size, header->len, header->cap, index, elm) != ARERR_NONE)
    {
        return ARERR_INDEX;
    }
    header->len++;
    return ARERR_NONE;
}

arerr_t sch_darrem(void **arr_ptr, size_t elm_size, size_t index)
{
    if (*arr_ptr == NULL)
    {
        return ARERR_NULL;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == 0)
    {
        return ARERR_LEN;
    }
    if (sch_arrem(*arr_ptr, elm_size, header->len, header->cap, index) != ARERR_NONE)
    {
        return ARERR_INDEX;
    }
    header->len--;
    return ARERR_NONE;
}

arerr_t sch_darres(void **darr_ptr, size_t elm_size, size_t cap)
{
    if (sch_daralloc_if_null(darr_ptr, elm_size) != ARERR_NONE)
    {
        return ARERR_ALLOC;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    if (header->cap == cap)
    {
        return ARERR_NONE;
    }
    void *new_arr = sch_darrealloc(*darr_ptr, cap, elm_size);
    if (new_arr == NULL)
    {
        return ARERR_ALLOC;
    }
    *darr_ptr = new_arr;
    return ARERR_NONE;
}

arerr_t sch_darfit(void **darr_ptr, size_t elm_size)
{
    if (*darr_ptr == NULL)
    {
        return ARERR_NULL;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    if (header->len == header->cap)
    {
        return ARERR_NONE;
    }
    void *new_arr = sch_darrealloc(*darr_ptr, header->len, elm_size);
    if (new_arr == NULL)
    {
        return ARERR_ALLOC;
    }
    *darr_ptr = new_arr;
    return ARERR_NONE;
}

arerr_t sch_darsiz(void **darr_ptr, size_t elm_size, size_t len, const void *opt_fill)
{
    if (sch_daralloc_if_null(darr_ptr, elm_size) != ARERR_NONE)
    {
        return ARERR_ALLOC;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    if (header->len == len)
    {
        return ARERR_NONE;
    }
    if (header->len > len)
    {
        header->len = len;
        return ARERR_NONE;
    }
    if (header->cap < len)
    {
        arerr_t err = sch_darres(darr_ptr, elm_size, len);
        if (err != ARERR_NONE)
        {
            return err;
        }
    }
    if (opt_fill == NULL)
    {
        memset(*darr_ptr, 0, (len - header->len) * elm_size);
        header->len = len;
        return ARERR_NONE;
    }
    for (size_t i = header->len; i < len; i++)
    {
        sch_arpush(*darr_ptr, elm_size, header->len, header->cap, opt_fill);
        header->len++;
    }
    return ARERR_NONE;
}

arerr_t sch_darclr(void **darr_ptr, size_t elm_size)
{
    (void)elm_size;
    if (*darr_ptr == NULL)
    {
        return ARERR_NULL;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    header->len = 0;
    return ARERR_NONE;
}

size_t sch_darlen(const void *arr)
{
    if (arr == NULL)
    {
        return 0;
    }
    struct dar_header_t *header = dar_header(arr);
    return header->len;
}

size_t sch_darcap(const void *arr)
{
    if (arr == NULL)
    {
        return 0;
    }
    struct dar_header_t *header = dar_header(arr);
    return header->cap;
}

const char *arerr_str(arerr_t err)
{
    switch (err)
    {
    case ARERR_NONE:
        return "No error";
    case ARERR_ALLOC:
        return "Allocation error";
    case ARERR_INDEX:
        return "Index out of bounds error";
    case ARERR_NULL:
        return "NULL pointer error";
    case ARERR_LEN:
        return "Incompatible length error";
    case ARERR_CAP:
        return "Insufficient capacity error";
    default:
        return NULL;
    }
}

#endif // SCH_IMPL