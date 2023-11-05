#ifndef SCH_ARRAY_H
#define SCH_ARRAY_H
#include <stddef.h> // for size_t

// Check if C23 is supported (for typeof)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)

# ifndef SCH_C23
#  define SCH_C23
# endif // SCH_C23

# ifndef sch_unqvar
#  define sch_unqvar(name) _x_sch_##name
# endif // sch_unqvar

#endif // SCH_C23

void sch_arpush(void *arr, size_t elm_size, size_t *len_ptr, const void *elm);
void sch_arpop(void *arr, size_t elm_size, size_t *len_ptr);
void sch_arins(void *arr, size_t elm_size, size_t *len_ptr, size_t index, const void *elm);
void sch_arrem(void *arr, size_t elm_size, size_t *len_ptr, size_t index);
void sch_arcpy(void *dst, const void *src, size_t elm_size, size_t len);
void sch_arclr(void *arr, size_t elm_size, size_t *len_ptr);
void sch_darfree(void *p);
void sch_darpush(void **darr_ptr, size_t elm_size, const void *elm);
void sch_darpop(void **darr_ptr, size_t elm_size);
void sch_darins(void **darr_ptr, size_t elm_size, size_t index, const void *elm);
void sch_darrem(void **darr_ptr, size_t elm_size, size_t index);
void sch_darres(void **darr_ptr, size_t elm_size, size_t cap);
void sch_darfit(void **darr_ptr, size_t elm_size);
void sch_darsiz(void **darr_ptr, size_t elm_size, size_t len, const void *opt_fill);
void sch_darclr(void **darr_ptr, size_t elm_size);
size_t sch_darlen(const void *darr);
size_t sch_darcap(const void *darr);

#define DARNEW NULL

#ifdef SCH_C23
# define arpush(arr, len_ptr, elm) do \
{ \
    typeof(*(arr)) sch_unqvar(Element) = (elm); \
    sch_arpush((arr), sizeof(*(arr)), (len_ptr), &sch_unqvar(Element)); \
} while (0)
#else
# define arpush(arr, len_ptr, elm) sch_arpush((arr), sizeof(*(arr)), (len_ptr), &(elm))
#endif // SCH_C23

#define arpop(arr, len_ptr) sch_arpop((arr), sizeof(*(arr)), (len_ptr))

#ifdef SCH_C23
# define arins(arr, len_ptr, index, elm) do \
{ \
    typeof(*(arr)) sch_unqvar(Element) = (elm); \
    sch_arins((arr), sizeof(*(arr)), (len_ptr), (index), &sch_unqvar(Element)); \
} while (0)
#else
# define arins(arr, len_ptr, index, elm) sch_arins((arr), sizeof(*(arr)), (len_ptr), (index), &(elm))
#endif // SCH_C23

#define arrem(arr, len_ptr, index) sch_arrem((arr), sizeof(*(arr)), (len_ptr), (index))

#define arcpy(dst, src, len) sch_arcpy((dst), (src), sizeof(*(dst)), (len))

#define arclr(arr, len_ptr) sch_arclr((arr), sizeof(*(arr)), (len_ptr))

#define darfree(darr) sch_darfree((void *)darr)

#ifdef SCH_C23
# define darpush(darr, elm) do \
{ \
    typeof(*(darr)) sch_unqvar(Element) = (elm); \
    sch_darpush((void **)&(darr), sizeof(*(darr)), &sch_unqvar(Element)); \
} while (0)
#else
# define darpush(darr, elm) sch_darpush((void **)&(darr), sizeof(*(darr)), &(elm))
#endif // SCH_C23

#define darpop(darr) sch_darpop((void **)&(darr), sizeof(*(darr)))

#ifdef SCH_C23
# define darins(darr, index, elm) do \
{ \
    typeof(*(darr)) sch_unqvar(Element) = (elm); \
    sch_darins((void **)&(darr), sizeof(*(darr)), (index), &sch_unqvar(Element)); \
} while (0)
#else
# define darins(darr, index, elm) sch_darins((void **)&(darr), sizeof(*(darr)), (index), &(elm))
#endif // SCH_C23

#define darrem(darr, index) sch_darrem((void **)&(darr), sizeof(*(darr)), (index))

#define darres(darr, cap) sch_darres((void **)&(darr), sizeof(*(darr)), (cap))

#define darfit(darr) sch_darfit((void **)&(darr), sizeof(*(darr)))

#define darsiz(darr, len, opt_fill_ptr) sch_darsiz((void **)&(darr), sizeof(*(darr)), (len), (opt_fill_ptr))

#define darclr(darr) sch_darclr((void **)&(darr), sizeof(*(darr)))

#define darlen(darr) sch_darlen((void *)darr)

#define darcap(darr) sch_darcap((void *)darr)

#define darempty(darr) (darlen(darr) == 0)

#endif // SCH_ARRAY_H
// ==========================================================
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

static inline void sch_dargrow(void **arr_ptr, size_t elm_size)
{
    struct dar_header_t *header = dar_header(*arr_ptr);
    size_t new_cap = header->cap * 2;
    void *new_arr = sch_darrealloc(*arr_ptr, new_cap, elm_size);
    if (new_arr == NULL)
    {
        return;
    }
    *arr_ptr = new_arr;
}

static inline void sch_daralloc_if_null(void **arr_ptr, size_t elm_size)
{
    if (*arr_ptr == NULL)
    {
        *arr_ptr = sch_darmalloc(1, elm_size);
        if (*arr_ptr == NULL)
        {
            return;
        }
    }
}

void sch_arpush(void *arr, size_t elm_size, size_t *len_ptr, const void *elm)
{
    char *write_ptr = ((char *)arr) + (*len_ptr * elm_size);
    memcpy(write_ptr, elm, elm_size);
    ++(*len_ptr);
}

void sch_arpop(void *arr, size_t elm_size, size_t *len_ptr)
{
    (void)arr;
    (void)elm_size;
    --(*len_ptr);
}

void sch_arins(void *arr, size_t elm_size, size_t *len_ptr, size_t index, const void *elm)
{
    char *write_ptr = ((char *)arr) + (index * elm_size);
    memmove(write_ptr + elm_size, write_ptr, (*len_ptr - index) * elm_size);
    memcpy(write_ptr, elm, elm_size);
    ++(*len_ptr);
}

void sch_arrem(void *arr, size_t elm_size, size_t *len_ptr, size_t index)
{
    char *write_ptr = ((char *)arr) + (index * elm_size);
    memmove(write_ptr, write_ptr + elm_size, (*len_ptr - index) * elm_size);
    --(*len_ptr);
}

void sch_arcpy(void *dst, const void *src, size_t elm_size, size_t len)
{
    memcpy(dst, src, elm_size * len);
}

void sch_arclr(void *arr, size_t elm_size, size_t *len_ptr)
{
    (void)arr;
    (void)elm_size;
    *len_ptr = 0;
}

void sch_darfree(void *p)
{
    if (p == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(p);
    free(header);
}

void sch_darpush(void **arr_ptr, size_t elm_size, const void *elm)
{
    sch_daralloc_if_null(arr_ptr, elm_size);
    if (*arr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == header->cap)
    {
        sch_dargrow(arr_ptr, elm_size);
    }
    sch_arpush(*arr_ptr, elm_size, &(header->len), elm);
}

void sch_darpop(void **arr_ptr, size_t elm_size)
{
    if (*arr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == 0)
    {
        return;
    }
    sch_arpop(*arr_ptr, elm_size, &(header->len));
}

void sch_darins(void **arr_ptr, size_t elm_size, size_t index, const void *elm)
{
    sch_daralloc_if_null(arr_ptr, elm_size);
    if (*arr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == header->cap)
    {
        sch_dargrow(arr_ptr, elm_size);
    }
    sch_arins(*arr_ptr, elm_size, &(header->len), index, elm);
}

void sch_darrem(void **arr_ptr, size_t elm_size, size_t index)
{
    if (*arr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*arr_ptr);
    if (header->len == 0)
    {
        return;
    }
    sch_arrem(*arr_ptr, elm_size, &(header->len), index);
}

void sch_darres(void **darr_ptr, size_t elm_size, size_t cap)
{
    sch_daralloc_if_null(darr_ptr, elm_size);
    if (*darr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    if (header->cap == cap)
    {
        return;
    }
    void *new_arr = sch_darrealloc(*darr_ptr, cap, elm_size);
    if (new_arr == NULL)
    {
        return;
    }
    *darr_ptr = new_arr;
}

void sch_darfit(void **darr_ptr, size_t elm_size)
{
    if (*darr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    if (header->len == header->cap)
    {
        return;
    }
    void *new_arr = sch_darrealloc(*darr_ptr, header->len, elm_size);
    if (new_arr == NULL)
    {
        return;
    }
    *darr_ptr = new_arr;
}

void sch_darsiz(void **darr_ptr, size_t elm_size, size_t len, const void *opt_fill)
{
    sch_daralloc_if_null(darr_ptr, elm_size);
    if (*darr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    if (header->len == len)
    {
        return;
    }
    if (header->len > len)
    {
        header->len = len;
        return;
    }
    if (header->cap < len)
    {
        sch_darres(darr_ptr, elm_size, len);
        if (*darr_ptr == NULL)
        {
            return;
        }
    }
    if (opt_fill == NULL)
    {
        memset(*darr_ptr, 0, (len - header->len) * elm_size);
        header->len = len;
        return;
    }
    for (size_t i = header->len; i < len; i++)
    {
        sch_arpush(*darr_ptr, elm_size, &(header->len), opt_fill);
    }
}

void sch_darclr(void **darr_ptr, size_t elm_size)
{
    (void)elm_size;
    if (*darr_ptr == NULL)
    {
        return;
    }
    struct dar_header_t *header = dar_header(*darr_ptr);
    header->len = 0;
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

#endif // SCH_IMPL