/*
 * Purpose:         Single-header library for dynamic strings.
 * Date created:    November 2023
 * Written by:      Scott DiGregorio
 * License:         CC0 (public domain)
 * Dependencies:    <stddef.h>, <stdlib.h>, <string.h>, <assert.h>
*/

/*
 * Usage:
 * Define SCH_IMPL before including this file in *one* C file to create the implementation.
*/

#ifndef SCH_STRING_H
#define SCH_STRING_H

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

#define SCH_STRING_STACK_CAPACITY ((sizeof(size_t) * 2 + sizeof(char *)) - 1)

/// The string_t struct is a union of two structs: heapstr and stackstr.
/// The heapstr struct is used when the string is too large to fit on the stack.
/// The stackstr struct is used when the string is small enough to fit on the stack.
/// This is managed by the type bit, which is the most significant bit of the size_t size member.
/// If the type bit is set, then the string is a stack string, otherwise it is a heap string.
/// This behavior is managed automatically by the sch_string functions.
typedef struct sch_string
{
    size_t type;
    union
    {
        struct
        {
            size_t size;
            size_t capacity;
            char *data;
        } heapstr;
        struct
        {
            char data[SCH_STRING_STACK_CAPACITY];
            char room; // for a count of the number of bytes available (out of SCH_STRING_STACK_CAPACITY)
        } stackstr;
    } u;
} string_t;

// Functions =================================================

/// Initializes a string_t struct.
/// @param str The string to initialize.
/// @param cstr The C string to initialize the string with. (can be NULL)
void dstrnew(string_t *str, const char *cstr);

/// Frees the memory allocated by a string_t struct. (if any)
/// @param str The string to free.
void dstrfree(string_t *str);

/// Returns the length of a string_t struct.
/// @param str The string to get the length of.
/// @return The length of the string.
size_t dstrlen(const string_t *str);

/// Returns the capacity of a string_t struct.
/// @param str The string to get the capacity of.
/// @return The capacity of the string.
size_t dstrcap(const string_t *str);

/// Returns the C string of a string_t struct.
/// @param str The string to get the C string of.
/// @return The C string of the string.
const char *dstrc(const string_t *str);

/// Copies a C string to a string_t struct.
/// @param str The string to copy to.
/// @param cstr The C string to copy.
void dstrcpy(string_t *str, const char *cstr);

/// Copies a string_t struct to another string_t struct.
/// @param str The string to copy to.
/// @param other The string to copy.
void dstrcpyd(string_t *str, const string_t *other);

/// Appends a C string to a string_t struct.
/// @param str The string to append to.
/// @param cstr The C string to append.
void dstrcat(string_t *str, const char *cstr);

/// Appends a character to a string_t struct.
/// @param str The string to append to.
/// @param c The character to append.
void dstrcatc(string_t *str, char c);

/// Appends a string_t struct to another string_t struct.
/// @param str The string to append to.
/// @param other The string to append.
void dstrcatd(string_t *str, const string_t *other);

/// Clears a string_t struct.
/// @param str The string to clear.
void dstrclr(string_t *str);

/// Fits a string_t struct to take up the least amount of memory possible.
/// @param str The string to shrink.
void dstrfit(string_t *str);

/// Compares a string_t struct to a C string.
/// @param str The string to compare.
/// @param cstr The C string to compare.
/// @return 0 if the strings are equal, -1 if the string is less than the C string, 1 if the string is greater than the C string.
int dstrcmp(const string_t *str, const char *cstr);

/// Compares a string_t struct to another string_t struct.
/// @param str The string to compare.
/// @param other The string to compare.
/// @return 0 if the strings are equal, -1 if the string is less than the other string, 1 if the string is greater than the other string.
int dstrcmpd(const string_t *str, const string_t *other);

SCH_API_END // End extern "C" block

#endif // SCH_STRING_H

#ifdef SCH_IMPL

// Implementation =============================================

#include <stdlib.h>
#include <string.h>
#include <assert.h>

inline static int sch_dstr_can_fit_on_stack(size_t len)
{
    return len < SCH_STRING_STACK_CAPACITY ? 1 : 0;
}

inline static char *sch_dstr_stack_data(string_t *str)
{
    return str->u.stackstr.data;
}

inline static const char *sch_dstr_stack_const_data(const string_t *str)
{
    return str->u.stackstr.data;
}

inline static void sch_dstr_set_type_bit(string_t *str, int value)
{
    if (value)
    {
        str->type = 1;
    }
    else
    {
        str->type = 0;
    }
}

inline static int sch_dstr_get_type_bit(const string_t *str)
{
    return str->type & 1;
}

inline static int sch_dstr_is_stack(const string_t *str)
{
    return sch_dstr_get_type_bit(str) != 0;
}

inline static int sch_dstr_is_heap(const string_t *str)
{
    return sch_dstr_get_type_bit(str) == 0;
}

inline static void sch_make_heapstr(string_t *str)
{
    str->u.heapstr.size = 0;
    str->u.heapstr.capacity = 0;
    str->u.heapstr.data = NULL;
    sch_dstr_set_type_bit(str, 0);
}

inline static void sch_make_stackstr(string_t *str)
{
    memset(sch_dstr_stack_data(str), 0, SCH_STRING_STACK_CAPACITY);
    str->u.stackstr.room = SCH_STRING_STACK_CAPACITY;
    sch_dstr_set_type_bit(str, 1);
}

inline static void sch_dstr_grow_if_needed(string_t *str, size_t len)
{
    if (sch_dstr_is_stack(str))
    {
        if (len >= SCH_STRING_STACK_CAPACITY)
        {
            size_t capacity = (len + 1) * 2; // Grow by 2x to avoid reallocating too often
            size_t size = dstrlen(str);
            char *data = (char *)malloc(capacity);
            memcpy(data, sch_dstr_stack_data(str), size);

            sch_make_heapstr(str);
            str->u.heapstr.size = size;
            str->u.heapstr.capacity = capacity;
            str->u.heapstr.data = data;
            str->u.heapstr.data[str->u.heapstr.size] = '\0';
        }
    }
    else
    {
        if (len >= str->u.heapstr.capacity)
        {
            str->u.heapstr.capacity = (len + 1) * 2; // Grow by 2x to avoid reallocating too often
            str->u.heapstr.data = (char *)realloc(str->u.heapstr.data, str->u.heapstr.capacity);
        }
    }
}

void dstrnew(string_t *str, const char *cstr)
{
    assert(str);

    if (cstr)
    {
        size_t len = strlen(cstr);
        if (sch_dstr_can_fit_on_stack(len))
        {
            sch_make_stackstr(str);
            memcpy(sch_dstr_stack_data(str), cstr, len);
            if (len < SCH_STRING_STACK_CAPACITY)
            {
                sch_dstr_stack_data(str)[len] = '\0';
            }
            str->u.stackstr.room -= len;
        }
        else
        {
            sch_make_heapstr(str);
            str->u.heapstr.size = len;
            str->u.heapstr.capacity = len + 1;
            str->u.heapstr.data = (char *)malloc(str->u.heapstr.capacity);
            memcpy(str->u.heapstr.data, cstr, len);
            str->u.heapstr.data[len] = '\0';
        }
    }
    else
    {
        sch_make_stackstr(str);
    }
}

void dstrfree(string_t *str)
{
    assert(str);

    if (sch_dstr_is_heap(str))
    {
        free(str->u.heapstr.data);
    }
}

size_t dstrlen(const string_t *str)
{
    assert(str);

    if (sch_dstr_is_stack(str))
    {
        return SCH_STRING_STACK_CAPACITY - str->u.stackstr.room;
    }
    else
    {
        return str->u.heapstr.size;
    }
}

size_t dstrcap(const string_t *str)
{
    assert(str);

    if (sch_dstr_is_stack(str))
    {
        return SCH_STRING_STACK_CAPACITY;
    }
    else
    {
        return str->u.heapstr.capacity;
    }
}

const char *dstrc(const string_t *str)
{
    assert(str);

    if (sch_dstr_is_stack(str))
    {
        return sch_dstr_stack_const_data(str);
    }
    else
    {
        return str->u.heapstr.data;
    }
}

void dstrcpy(string_t *str, const char *cstr)
{
    assert(str);

    if (cstr)
    {
        size_t len = strlen(cstr);
        sch_dstr_grow_if_needed(str, len);
        if (sch_dstr_is_stack(str))
        {
            memcpy(sch_dstr_stack_data(str), cstr, len);
            if (len < SCH_STRING_STACK_CAPACITY)
            {
                sch_dstr_stack_data(str)[len] = '\0';
            }
            str->u.stackstr.room -= len;
        }
        else
        {
            str->u.heapstr.size = len;
            memcpy(str->u.heapstr.data, cstr, len);
            str->u.heapstr.data[len] = '\0';
        }
    }
    else
    {
        dstrclr(str);
    }
}

void dstrcpyd(string_t *str, const string_t *other)
{
    assert(str);
    assert(other);

    dstrcpy(str, dstrc(other));
}

void dstrcat(string_t *str, const char *cstr)
{
    assert(str);

    if (cstr)
    {
        size_t len = strlen(cstr);
        size_t size = dstrlen(str);
        sch_dstr_grow_if_needed(str, size + len);
        if (sch_dstr_is_stack(str))
        {
            memcpy(sch_dstr_stack_data(str) + size, cstr, len);
            if (size + len < SCH_STRING_STACK_CAPACITY)
            {
                sch_dstr_stack_data(str)[size + len] = '\0';
            }
            str->u.stackstr.room -= len;
        }
        else
        {
            memcpy(str->u.heapstr.data + size, cstr, len);
            str->u.heapstr.size += len;
            str->u.heapstr.data[str->u.heapstr.size] = '\0';
        }
    }
}

void dstrcatc(string_t *str, char c)
{
    assert(str);

    size_t size = dstrlen(str);
    sch_dstr_grow_if_needed(str, size + 1);
    if (sch_dstr_is_stack(str))
    {
        sch_dstr_stack_data(str)[size] = c;
        if (size + 1 < SCH_STRING_STACK_CAPACITY)
        {
            sch_dstr_stack_data(str)[size + 1] = '\0';
        }
        str->u.stackstr.room -= 1;
    }
    else
    {
        str->u.heapstr.data[size] = c;
        str->u.heapstr.size += 1;
        str->u.heapstr.data[str->u.heapstr.size] = '\0';
    }
}

void dstrcatd(string_t *str, const string_t *other)
{
    assert(str);
    assert(other);

    dstrcat(str, dstrc(other));
}

void dstrclr(string_t *str)
{
    assert(str);

    if (sch_dstr_is_heap(str))
    {
        str->u.heapstr.size = 0;
        str->u.heapstr.data[0] = '\0';
    }
    else
    {
        memset(sch_dstr_stack_data(str), 0, SCH_STRING_STACK_CAPACITY);
        str->u.stackstr.room = SCH_STRING_STACK_CAPACITY;
    }
}

void dstrfit(string_t *str)
{
    assert(str);

    if (sch_dstr_is_stack(str))
    {
        return;
    }

    size_t size = dstrlen(str);
    if (sch_dstr_can_fit_on_stack(size))
    {
        string_t temp;
        sch_make_stackstr(&temp);
        memcpy(temp.u.stackstr.data, str->u.heapstr.data, size);
        temp.u.stackstr.data[size] = '\0';
        temp.u.stackstr.room -= size;

        free(str->u.heapstr.data);
        *str = temp;
    }
    else
    {
        str->u.heapstr.capacity = size + 1;
        str->u.heapstr.data = (char *)realloc(str->u.heapstr.data, str->u.heapstr.capacity);
    }
}

int dstrcmp(const string_t *str, const char *cstr)
{
    assert(str);

    if (cstr)
    {
        return strcmp(dstrc(str), cstr);
    }
    else
    {
        return 1;
    }
}

int dstrcmpd(const string_t *str, const string_t *other)
{
    assert(str);
    assert(other);

    return strcmp(dstrc(str), dstrc(other));
}

#endif // SCH_IMPL