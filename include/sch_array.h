/*
 * Purpose:         Single-header library for dynamic arrays.
 * Date created:    November 2023
 * Written by:      Scott DiGregorio
 * License:         CC0 (public domain)
 * Dependencies:    <stddef.h>, <stdlib.h>, <string.h>, <assert.h>
*/

/*
 * Usage:
 * Define SCH_IMPL before including this file in *one* C file to create the implementation.
 *
 * To use the type-generic macros, define a struct with the following members:
 * - size_t size
 * - size_t capacity
 * - T* data
 * Where T is the type of the array's elements.
 *
 * For example:

typedef struct
{
    size_t size;
    size_t capacity;
    int* data;
} int_array;

 * Pass a pointer to instances of this struct to the macros.
 *
 * For example:

struct int_array arr;
darnew(&arr, 10);   // create a new array with capacity 10

darpush(&arr, 5);   // push 5 to the end of the array
darpush(&arr, 10);  // push 10 to the end of the array
darpush(&arr, 15);  // push 15 to the end of the array
darins(&arr, 7, 1); // insert 7 at index 1
darrem(&arr, 2);    // remove the element at index 2

darfree(&arr);      // free the memory used by the array

 *
*/

#ifndef SCH_ARRAY_H
#define SCH_ARRAY_H

// Definitions ===============================================

#ifndef SCH_API_BEGIN
# ifdef __cplusplus
#  define SCH_API_BEGIN extern "C" {
#  define SCH_API_END   }
# else
#  define SCH_API_BEGIN
#  define SCH_API_END
# endif // __cplusplus
#endif // SCH_API_BEGIN

SCH_API_BEGIN // Begin extern "C" block

// Includes ==================================================

#include <stddef.h> // for size_t

// Types =====================================================

/// This struct represents a generic dynamic array.
/// It is used by the macros to implement the type-generic functions.
/// Ensure that any specific array type you use has the same members as this struct
/// in the same order. (size, capacity, data)
struct sch_dar
{
    size_t size;
    size_t capacity;
    void *data;
};

// Functions =================================================

void sch_darnew(struct sch_dar *arr, size_t capacity, size_t elem_size);
void sch_darfree(struct sch_dar *arr);
void sch_darpush(struct sch_dar *arr, const void *elem, size_t elem_size);
void sch_darpop(struct sch_dar *arr, size_t elem_size);
void sch_darins(struct sch_dar *arr, const void *elem, size_t index, size_t elem_size);
void sch_darrem(struct sch_dar *arr, size_t index, size_t elem_size);
void sch_darclr(struct sch_dar *arr);
void sch_darcpy(struct sch_dar *arr, const void *src, size_t n, size_t elem_size);
void sch_darcat(struct sch_dar *arr, const void *src, size_t n, size_t elem_size);
void sch_darres(struct sch_dar *arr, size_t new_capacity, size_t elem_size);
void sch_darrez(struct sch_dar *arr, size_t new_size, size_t elem_size, const void *optional_filler);
size_t sch_darsiz(const struct sch_dar *arr);
size_t sch_darcap(const struct sch_dar *arr);
void *sch_dardat(const struct sch_dar *arr);
int sch_darempty(const struct sch_dar *arr);

// Macros ====================================================
// These macros are type-generic, but they require a struct with the following members:
// - size_t size
// - size_t capacity
// - T* data
// Where T is the type of the array's elements.

#define sch_to_dar(arr) ((struct sch_dar *)arr)
#define sch_elem_size(arr) (sizeof(*(arr)->data))

/// Create a new dynamic array with the given capacity.
/// @param arr A pointer to the dynamic array struct.
/// @param capacity The initial capacity of the array.
#define darnew(arr, capacity) sch_darnew(sch_to_dar(arr), capacity, sch_elem_size(arr))

/// Free the memory used by the dynamic array.
/// @param arr A pointer to the dynamic array struct.
#define darfree(arr) sch_darfree(sch_to_dar(arr))

/// Push an element to the end of the dynamic array.
/// @param arr A pointer to the dynamic array struct.
/// @param elem The element to push. (must be an lvalue)
#define darpush(arr, elem) sch_darpush(sch_to_dar(arr), &(elem), sch_elem_size(arr))

/// Pop an element from the end of the dynamic array.
/// @param arr A pointer to the dynamic array struct.
#define darpop(arr) sch_darpop(sch_to_dar(arr), sch_elem_size(arr))

/// Insert an element at the given index.
/// @param arr A pointer to the dynamic array struct.
/// @param elem The element to insert. (must be an lvalue)
/// @param index The index at which to insert the element.
#define darins(arr, elem, index) sch_darins(sch_to_dar(arr), &(elem), (index), sch_elem_size(arr))

/// Remove an element at the given index.
/// @param arr A pointer to the dynamic array struct.
/// @param index The index of the element to remove.
#define darrem(arr, index) sch_darrem(sch_to_dar(arr), (index), sch_elem_size(arr))

/// Clear the array.
/// @param arr A pointer to the array struct.
#define darclr(arr) sch_darclr(sch_to_dar(arr))

/// Copy the given array into the dynamic array.
/// @param arr A pointer to the dynamic array struct.
/// @param src A pointer to an array to copy. (regular C array or buffer)
/// @param n The number of elements to copy.
#define darcpy(arr, src, n) sch_darcpy(sch_to_dar(arr), (src), (n), sch_elem_size(arr))

/// Concatenate the given array to the dynamic array.
/// @param arr A pointer to the dynamic array struct.
/// @param src A pointer to an array to concatenate. (regular C array or buffer)
/// @param n The number of elements to concatenate.
#define darcat(arr, src, n) sch_darcat(sch_to_dar(arr), (src), (n), sch_elem_size(arr))

/// Reserve the dynamic array with the given capacity. If the new capacity is smaller than the current capacity, nothing happens.
/// @param arr A pointer to the dynamic array struct.
/// @param new_capacity The new capacity of the array.
#define darres(arr, new_capacity) sch_darres(sch_to_dar(arr), (new_capacity), sch_elem_size(arr))

/// Resize the dynamic array to the given size. If the new size is smaller than the current size, the array is truncated. 
/// If the new size is larger than the current size, the new elements are filled with the given filler.
/// @param arr A pointer to the dynamic array struct.
/// @param new_size The new size of the array.
/// @param optional_filler A pointer to an element to fill the new elements with, or NULL to fill with zeroes.
#define darrez(arr, new_size, optional_filler_ptr) sch_darrez(sch_to_dar(arr), (new_size), sch_elem_size(arr), (optional_filler_ptr))

/// Get the size of the dynamic array.
/// @param arr A pointer to the dynamic array struct.
/// @return The size of the array.
#define darsiz(arr) ((arr)->size)

/// Get the capacity of the dynamic array.
/// @param arr A pointer to the dynamic array struct.
/// @return The capacity of the array.
#define darcap(arr) ((arr)->capacity)

/// Get a pointer to the data of the dynamic array.
/// @param arr A pointer to the dynamic array struct.
/// @return A pointer to the data of the array.
#define dardat(arr) ((arr)->data)

/// Check if the dynamic array is empty.
/// @param arr A pointer to the dynamic array struct.
/// @return 1 if the array is empty, 0 otherwise.
#define darempty(arr) sch_darempty(sch_to_dar(arr))

SCH_API_END // End extern "C" block

#endif // SCH_ARRAY_H

#ifdef SCH_IMPL

// Implementation =============================================

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void sch_realloc_if_needed(struct sch_dar *arr, size_t new_size, size_t elem_size);
static void sch_grow_if_needed(struct sch_dar *arr, size_t elem_size);

void sch_darnew(struct sch_dar *arr, size_t capacity, size_t elem_size)
{
    assert(arr != NULL);
    assert(capacity > 0);
    assert(elem_size > 0);

    arr->size = 0;
    arr->capacity = capacity;
    arr->data = malloc(capacity * elem_size);
}

void sch_darfree(struct sch_dar *arr)
{
    assert(arr != NULL);

    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void sch_darpush(struct sch_dar *arr, const void *elem, size_t elem_size)
{
    assert(arr != NULL);
    assert(elem != NULL);
    assert(elem_size > 0);

    sch_grow_if_needed(arr, elem_size);

    memcpy((char *)arr->data + arr->size * elem_size, elem, elem_size);
    arr->size++;
}

void sch_darpop(struct sch_dar *arr, size_t elem_size)
{
    assert(arr != NULL);
    assert(elem_size > 0);

    if (arr->size > 0)
    {
        arr->size--;
    }
}

void sch_darins(struct sch_dar *arr, const void *elem, size_t index, size_t elem_size)
{
    assert(arr != NULL);
    assert(elem != NULL);
    assert(index < arr->size);
    assert(elem_size > 0);

    sch_grow_if_needed(arr, elem_size);

    memmove((char *)arr->data + (index + 1) * elem_size, (char *)arr->data + index * elem_size, (arr->size - index) * elem_size);
    memcpy((char *)arr->data + index * elem_size, elem, elem_size);
    arr->size++;
}

void sch_darrem(struct sch_dar *arr, size_t index, size_t elem_size)
{
    assert(arr != NULL);
    assert(index < arr->size);
    assert(elem_size > 0);

    memmove((char *)arr->data + index * elem_size, (char *)arr->data + (index + 1) * elem_size, (arr->size - index - 1) * elem_size);
    arr->size--;
}

void sch_darclr(struct sch_dar *arr)
{
    assert(arr != NULL);

    arr->size = 0;
}

void sch_darcpy(struct sch_dar *dest, const void *src, size_t n, size_t elem_size)
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(n > 0);
    assert(elem_size > 0);

    sch_realloc_if_needed(dest, n, elem_size);

    memcpy((char *)dest->data, src, n * elem_size);
    dest->size = n;
}

void sch_darcat(struct sch_dar *dest, const void *src, size_t n, size_t elem_size)
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(n > 0);
    assert(elem_size > 0);

    sch_realloc_if_needed(dest, dest->size + n, elem_size);

    memcpy((char *)dest->data + dest->size * elem_size, src, n * elem_size);
    dest->size += n;
}

void sch_darres(struct sch_dar *arr, size_t new_capacity, size_t elem_size)
{
    assert(arr != NULL);
    assert(new_capacity > 0);
    assert(elem_size > 0);

    sch_realloc_if_needed(arr, new_capacity, elem_size);
}

void sch_darrez(struct sch_dar *arr, size_t new_size, size_t elem_size, const void *optional_filler)
{
    assert(arr != NULL);
    assert(new_size > 0);
    assert(elem_size > 0);

    sch_realloc_if_needed(arr, new_size, elem_size);

    if (optional_filler != NULL)
    {
        for (size_t i = arr->size; i < new_size; i++)
        {
            memcpy((char *)arr->data + i * elem_size, optional_filler, elem_size);
        }
    }
    else
    {
        memset(arr->data, 0, new_size * elem_size);
    }

    arr->size = new_size;
}

size_t sch_darsiz(const struct sch_dar *arr)
{
    assert(arr != NULL);

    return arr->size;
}

size_t sch_darcap(const struct sch_dar *arr)
{
    assert(arr != NULL);

    return arr->capacity;
}

void *sch_dardat(const struct sch_dar *arr)
{
    assert(arr != NULL);

    return arr->data;
}

int sch_darempty(const struct sch_dar *arr)
{
    assert(arr != NULL);

    return arr->size == 0;
}

static void sch_realloc_if_needed(struct sch_dar *arr, size_t new_size, size_t elem_size)
{
    assert(arr != NULL);
    assert(elem_size > 0);

    if (arr->capacity < new_size)
    {
        arr->capacity = new_size;
        arr->data = realloc(arr->data, arr->capacity * elem_size);
    }
}

static void sch_grow_if_needed(struct sch_dar *arr, size_t elem_size)
{
    assert(arr != NULL);
    assert(elem_size > 0);

    if (arr->size == arr->capacity)
    {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * elem_size);
    }
}

#endif // SCH_IMPL