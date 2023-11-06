#ifndef SCH_STRING_H
#define SCH_STRING_H
#include <stddef.h> // for size_t

// Constants =================================================

/// Constant for a null dynamic string. This is used to indicate
/// that a dynamic string is null.
#define DSTRNULL NULL

#ifndef SCH_DSTR_GROWTH_FACTOR
/// The default growth factor for dynamic strings.
/// @note This is a double. Although it is not recommended to change this,
///       it is possible to do so by defining SCH_DSTR_GROWTH_FACTOR before
///       including this header.
# define SCH_DSTR_GROWTH_FACTOR (2.0)
#endif // SCH_DAR_GROWTH_FACTOR

// Types =====================================================

/// An error code for dynamic strings.
typedef enum sch_dstrerr_t
{
    DSTRERR_NONE = 0,
    DSTRERR_ALLOC,
    DSTRERR_NULL,
    DSTRERR_COUNT_
} dstrerr_t;

/// A dynamic string character.
typedef struct sch_dchar_t
{
    char c;
} dchar_t;

/// A dynamic string.
typedef dchar_t *dstr_t;

// Function Prototypes =======================================

dstr_t sch_dstrnew(const char *str);
dstrerr_t sch_dstrfree(dstr_t *dstr_ptr);
dstrerr_t sch_dstrcpy(dstr_t *dstr_ptr, const char *str);
dstrerr_t sch_dstrcat(dstr_t *dstr_ptr, const char *str);
dstrerr_t sch_dstrpush(dstr_t *dstr_ptr, char c);
dstrerr_t sch_dstrpop(dstr_t *dstr_ptr);
dstrerr_t sch_dstrres(dstr_t *dstr_ptr, size_t new_cap);
dstrerr_t sch_dstrsiz(dstr_t *dstr_ptr, size_t new_len, char c_fill);
dstrerr_t sch_dstrclr(dstr_t *dstr_ptr);
dstrerr_t sch_dstrfit(dstr_t *dstr_ptr);
size_t sch_dstrlen(const dstr_t dstr);
size_t sch_dstrcap(const dstr_t dstr);

// Macros ====================================================

/// Interprets a dynamic string as a C string.
/// @param dstr The dynamic string to interpret as a C string.
/// @return The C string. Will return an empty string if the dynamic string
///         is null.
#define cstr(dstr) ((dstr) ? (const char *)(dstr) : "")

/// Creates a new dynamic string.
/// @param str The C string to copy into the dynamic string. Can be NULL.
/// @return The new dynamic string, or DSTRNULL if an error occurred.
#define dstrnew(str) sch_dstrnew(str)

/// Frees a dynamic string. Sets the pointer to DSTRNULL.
/// @param dstr The dynamic string to free.
/// @return An error code, or 0 if no error occurred.
#define dstrfree(dstr) sch_dstrfree(&(dstr))

/// Copies a C string into a dynamic string. Reallocates the dynamic string
/// if necessary.
/// @param dstr The dynamic string to copy the C string into.
/// @param str The C string to copy into the dynamic string.
/// @return An error code, or 0 if no error occurred.
#define dstrcpy(dstr, str) sch_dstrcpy(&(dstr), str)

/// Concatenates a C string onto a dynamic string. Reallocates the dynamic
/// string if necessary.
/// @param dstr The dynamic string to concatenate the C string onto.
/// @param str The C string to concatenate onto the dynamic string.
/// @return An error code, or 0 if no error occurred.
#define dstrcat(dstr, str) sch_dstrcat(&(dstr), str)

/// Pushes a character onto a dynamic string. Reallocates the dynamic string
/// if necessary. This can be '\0', but this can't grow the length of the
/// dynamic string.
/// @param dstr The dynamic string to push the character onto.
/// @param c The character to push onto the dynamic string.
/// @return An error code, or 0 if no error occurred.
#define dstrpush(dstr, c) sch_dstrpush(&(dstr), c)

/// Pops a character off of a dynamic string. Reallocates the dynamic string
/// if necessary.
/// @param dstr The dynamic string to pop the character off of.
/// @return An error code, or 0 if no error occurred.
#define dstrpop(dstr) sch_dstrpop(&(dstr))

/// Reserves a certain capacity for a dynamic string. Reallocates the dynamic string if necessary.
/// @param dstr The dynamic string to resize.
/// @param new_cap The new capacity of the dynamic string.
/// @return An error code, or 0 if no error occurred.
#define dstrres(dstr, new_cap) sch_dstrres(&(dstr), new_cap)

/// Resizes a dynamic string to a certain length. Reallocates the dynamic string if necessary.
/// @param dstr The dynamic string to resize.
/// @param new_len The new length of the dynamic string.
/// @param c_fill The character to fill the new space with. Can be '\0', but
///               this can't grow the length of the dynamic string.
/// @return An error code, or 0 if no error occurred.
#define dstrsiz(dstr, new_len, c_fill) sch_dstrsiz(&(dstr), new_len, c_fill)

/// Clears a dynamic string.
/// @param dstr The dynamic string to clear.
/// @return An error code, or 0 if no error occurred.
#define dstrclr(dstr) sch_dstrclr(&(dstr))

/// Fits a dynamic string to its length.
/// @param dstr The dynamic string to fit.
/// @return An error code, or 0 if no error occurred.
#define dstrfit(dstr) sch_dstrfit(&(dstr))

/// Gets the length of a dynamic string (not including the null terminator).
/// @param dstr The dynamic string to get the length of.
/// @return The length of the dynamic string.
#define dstrlen(dstr) sch_dstrlen(dstr)

/// Gets the capacity of a dynamic string.
/// @param dstr The dynamic string to get the capacity of.
/// @return The capacity of the dynamic string.
#define dstrcap(dstr) sch_dstrcap(dstr)

#endif // SCH_STRING_H

// Implementation =============================================

#ifdef SCH_IMPL


#endif // SCH_IMPL