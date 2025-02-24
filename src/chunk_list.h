/**
 * @file chunk_list.h
 * Manages and navigates the list of chunks.
 *
 * @author  Ben Gardner
 * @license GPL v2+
 */

#ifndef CHUNK_LIST_H_INCLUDED
#define CHUNK_LIST_H_INCLUDED

#include "uncrustify_types.h"
// necessary to not sort it
#include "char_table.h"
#include "language_tools.h"


/*
 * TODO: better use a namespace for all chunk related operations.
 * The function "chunk_is_comment()" would for instance
 * become "chunk::is_comment()". This makes the usage of the chunks easier
 * and more intuitive.
 */


static constexpr int ANY_LEVEL = -1;


/**
 * Specifies which chunks should/should not be found.
 * ALL (default)
 *  - return the true next/prev
 *
 * PREPROC
 *  - If not in a preprocessor, skip over any encountered preprocessor stuff
 *  - If in a preprocessor, fail to leave (return nullptr)
 */
enum class scope_e : unsigned int
{
   ALL,      //! search in all kind of chunks
   PREPROC,  //! search only in preprocessor chunks
};


/**
 * duplicate a chunk in a chunk list
 *
 * @param pc_in  chunk to duplicate
 */
chunk_t *chunk_dup(const chunk_t *pc_in);


/**
 * @brief Add a copy of a chunk to a chunk list after the given position.
 *
 * @note If ref is nullptr, add at the tail of the chunk list
 *
 * @todo is ref=nullptr really useful ?
 *
 * @param pc_in  pointer to chunk to add to list
 * @param ref    position where insertion takes place
 *
 * @return pointer to the added chunk
 */
chunk_t *chunk_add_after(const chunk_t *pc_in, chunk_t *ref);


/**
 * @brief Add a copy of a chunk to a chunk list before the given position
 *
 * @note If ref is nullptr, add at the head of the chunk list
 *
 * @todo is ref=nullptr really useful ?
 *
 * \bug code adds it before the tail, either code or comment is wrong
 *
 * @param pc_in  pointer to chunk to add to list
 * @param ref    position where insertion takes place
 *
 * @retval pointer to the added chunk
 */
chunk_t *chunk_add_before(const chunk_t *pc_in, chunk_t *ref);


/**
 * delete a chunk from a chunk list
 *
 * @param pc  chunk to delete
 */
void chunk_del(chunk_t * &pc);


/**
 * move a chunk to after the reference position in a chunk list
 *
 * @param pc_in  chunk to move
 * @param ref    chunk after which to move
 */
void chunk_move_after(chunk_t *pc_in, chunk_t *ref);


/**
 * @brief returns the head of a chunk list
 *
 * @return pointer to the first chunk
 */
chunk_t *chunk_get_head(void);


//! get the last chunk in a chunk list
chunk_t *chunk_get_tail(void);


/**
 * @brief returns the next chunk in a list of chunks
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 *
 * @return pointer to next chunk or nullptr if no chunk was found
 */
chunk_t *chunk_get_next(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * @brief returns the previous chunk in a list of chunks
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 *
 * @return pointer to previous chunk or nullptr if no chunk was found
 */
chunk_t *chunk_get_prev(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Swaps two chunks
 *
 * @param pc1  The first chunk
 * @param pc2  The second chunk
 */
void chunk_swap(chunk_t *pc1, chunk_t *pc2);


/**
 * Swaps two lines that are started with the specified chunks.
 *
 * @param pc1  The first chunk of line 1
 * @param pc2  The first chunk of line 2
 */
void chunk_swap_lines(chunk_t *pc1, chunk_t *pc2);


/**
 * Finds the first chunk on the line that pc is on.
 * This just backs up until a newline or nullptr is hit.
 *
 * given: [ a - b - c - n1 - d - e - n2 ]
 * input: [ a | b | c | n1 ] => a
 * input: [ d | e | n2 ]     => d
 *
 * @param pc  chunk to start with
 */
chunk_t *chunk_first_on_line(chunk_t *pc);


//! check if a given chunk is the last on its line
bool chunk_is_last_on_line(chunk_t &pc);


/**
 * Gets the next NEWLINE chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_next_nl(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the next non-comment chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_next_nc(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the next non-NEWLINE
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_next_nnl(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the next non-NEWLINE and non-comment chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_next_ncnnl(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the next non-NEWLINE and non-comment chunk, non-preprocessor chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_next_ncnnlnp(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the next non-NEWLINE and non-comment chunk (preprocessor aware).
 * Unlike chunk_get_next_ncnnl, this will also ignore a line continuation if
 * the starting chunk is in a preprocessor directive, and may return a newline
 * if the search reaches the end of a preprocessor directive.
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_ppa_get_next_ncnnl(chunk_t *cur);


/**
 * Gets the next chunk not in or part of balanced square
 * brackets. This handles stacked [] instances to accommodate
 * multi-dimensional array declarations
 *
 * @param  cur    chunk to use as start point
 * @param  scope  code region to search in
 *
 * @return nullptr or the next chunk not in or part of square brackets
 */
chunk_t *chunk_get_next_nisq(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the next non-blank chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_next_nblank(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev non-blank chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_nblank(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev NEWLINE chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_nl(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev non-comment chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_nc(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev non-NEWLINE chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_nnl(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev non-NEWLINE and non-comment chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_ncnnl(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev non-NEWLINE and non-comment and non-ignored chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_ncnnlni(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Gets the prev non-NEWLINE and non-comment chunk, non-preprocessor chunk
 *
 * @param cur    chunk to use as start point
 * @param scope  code region to search in
 */
chunk_t *chunk_get_prev_ncnnlnp(chunk_t *cur, scope_e scope = scope_e::ALL);


/**
 * Grabs the next chunk of the given type at the level.
 *
 * @param cur    chunk to use as start point
 * @param type   the type to look for
 * @param level  -1 or ANY_LEVEL (any level) or the level to match
 * @param scope  code region to search in
 *
 * @return nullptr or the match
 */
chunk_t *chunk_get_next_type(chunk_t *cur, c_token_t type, int level, scope_e scope = scope_e::ALL);


/**
 * Grabs the prev chunk of the given type at the level.
 *
 * @param cur    chunk to use as start point
 * @param type   The type to look for
 * @param level  -1 or ANY_LEVEL (any level) or the level to match
 * @param scope  code region to search in
 *
 * @return nullptr or the match
 */
chunk_t *chunk_get_prev_type(chunk_t *cur, c_token_t type, int level, scope_e scope = scope_e::ALL);


/**
 * @brief find a chunk that holds a given string
 *
 * Traverses a chunk list in forward direction until a chunk of a given category is found.
 *
 * @param cur    chunk to use as start point
 * @param str    string to search for
 * @param len    length of string
 * @param level  the level to match or -1 or ANY_LEVEL
 * @param scope  code region to search in
 *
 * @retval nullptr  no chunk found or invalid parameters provided
 * @retval chunk_t  pointer to the found chunk
 */
chunk_t *chunk_get_next_str(chunk_t *cur, const char *str, size_t len, int level, scope_e scope = scope_e::ALL);


/**
 * @brief find a chunk that holds a given string
 *
 * Traverses a chunk list in backward direction until a chunk of a given category is found.
 *
 * @param cur    chunk to use as start point
 * @param str    string to search for
 * @param len    length of string
 * @param level  the level to match or -1 or ANY_LEVEL
 * @param scope  code region to search in
 *
 * @retval nullptr  no chunk found or invalid parameters provided
 * @retval chunk_t  pointer to the found chunk
 */
chunk_t *chunk_get_prev_str(chunk_t *cur, const char *str, size_t len, int level, scope_e scope = scope_e::ALL);


/**
 * @brief Gets the next non-vbrace chunk
 *
 * @param  cur    chunk to start search
 * @param  scope  chunk section to consider
 *
 * @return pointer to found chunk or nullptr if no chunk was found
 */
chunk_t *chunk_get_next_nvb(chunk_t *cur, const scope_e scope = scope_e::ALL);


/**
 * @brief Gets the previous non-vbrace chunk
 *
 * @param  cur    chunk to start search
 * @param  scope  chunk section to consider
 *
 * @return pointer to found chunk or nullptr if no chunk was found
 */
chunk_t *chunk_get_prev_nvb(chunk_t *cur, const scope_e scope = scope_e::ALL);


/**
 * Gets the next chunk not in or part of balanced square
 * brackets.This handles stacked[] instances to accommodate
 * multi - dimensional array declarations
 *
 * @param  cur    chunk to use as start point
 *
 * @return nullptr or the next chunk not in or part of square brackets
 */
chunk_t *chunk_get_next_ssq(chunk_t *cur);

/**
 * Gets the prev chunk not in or part of balanced square
 * brackets.This handles stacked[] instances to accommodate
 * multi - dimensional array declarations
 *
 * @param  cur    chunk to use as start point
 *
 * @return nullptr or the prev chunk not in or part of square brackets
 */
chunk_t *chunk_get_prev_ssq(chunk_t *cur);

/**
 * Gets the corresponding start chunk if the given chunk is within a
 * preprocessor directive, or nullptr otherwise.
 *
 * @param  cur    chunk to use as start point
 *
 * @return nullptr or start chunk of the preprocessor directive
 */
chunk_t *chunk_get_pp_start(chunk_t *cur);

/**
 * @brief reverse search a chunk of a given category in a chunk list
 *
 * @param  pc   chunk list to search in
 * @param  cat  category to search for
 *
 * @retval nullptr  no object found, or invalid parameters provided
 * @retval chunk_t  pointer to the found object
 */
chunk_t *chunk_search_prev_cat(chunk_t *pc, const c_token_t cat);


/**
 * @brief forward search a chunk of a given category in a chunk list
 *
 * @param  pc   chunk list to search in
 * @param  cat  category to search for
 *
 * @retval nullptr  no object found, or invalid parameters provided
 * @retval chunk_t  pointer to the found object
 */
chunk_t *chunk_search_next_cat(chunk_t *pc, const c_token_t cat);

/**
 * @brief checks wether two chunks are in same line
 *
 * @param  start
 * @param  end
 *
 * @return true if there is no newline between start and end chunks
 */
bool are_chunks_in_same_line(chunk_t *start, chunk_t *end);

/*
 * TODO: better move the function implementations to the source file.
 * No need to make the implementation public.
 */


/*
 * TODO: I doubt that inline is required for the functions below.
 * The compiler should know how to optimize the code itself.
 * To clarify do a profiling run with and without inline
 */
static inline bool is_expected_type_and_level(chunk_t *pc, c_token_t type, int level)
{
   // we don't care if the pointer is invalid or about the level (if it is negative),
   // or it is as expected and the type is as expected
   return(  pc == nullptr
         || (  (  level < 0
               || pc->level == static_cast<size_t>(level))
            && pc->type == type));
}


static inline bool is_expected_string_and_level(chunk_t *pc, const char *str, int level, size_t len)
{
   // we don't care if the pointer is invalid or about the level (if it is negative) or it is as expected
   return(  pc == nullptr
         || (  (  level < 0
               || pc->level == static_cast<size_t>(level))
            && pc->len() == len                        // and the length is as expected
            && memcmp(str, pc->text(), len) == 0));    // and the strings are equal
}


static inline bool chunk_is_token(const chunk_t *pc, c_token_t c_token)
{
   return(  pc != nullptr
         && pc->type == c_token);
}


static inline bool chunk_is_not_token(const chunk_t *pc, c_token_t c_token)
{
   return(  pc != nullptr
         && pc->type != c_token);
}


/**
 * Skips to the closing match for the current paren/brace/square.
 *
 * @param  cur    The opening or closing paren/brace/square
 * @param  scope  chunk section to consider
 *
 * @return nullptr or the matching paren/brace/square
 */
static inline chunk_t *chunk_skip_to_match(chunk_t *cur, scope_e scope = scope_e::ALL)
{
   if (  cur != nullptr
      && (  chunk_is_token(cur, CT_PAREN_OPEN)
         || chunk_is_token(cur, CT_SPAREN_OPEN)
         || chunk_is_token(cur, CT_FPAREN_OPEN)
         || chunk_is_token(cur, CT_TPAREN_OPEN)
         || chunk_is_token(cur, CT_BRACE_OPEN)
         || chunk_is_token(cur, CT_VBRACE_OPEN)
         || chunk_is_token(cur, CT_ANGLE_OPEN)
         || chunk_is_token(cur, CT_SQUARE_OPEN)))
   {
      return(chunk_get_next_type(cur, (c_token_t)(cur->type + 1), cur->level, scope));
   }
   return(cur);
}


static inline chunk_t *chunk_skip_to_match_rev(chunk_t *cur, scope_e scope = scope_e::ALL)
{
   if (  cur != nullptr
      && (  chunk_is_token(cur, CT_PAREN_CLOSE)
         || chunk_is_token(cur, CT_SPAREN_CLOSE)
         || chunk_is_token(cur, CT_FPAREN_CLOSE)
         || chunk_is_token(cur, CT_TPAREN_CLOSE)
         || chunk_is_token(cur, CT_BRACE_CLOSE)
         || chunk_is_token(cur, CT_VBRACE_CLOSE)
         || chunk_is_token(cur, CT_ANGLE_CLOSE)
         || chunk_is_token(cur, CT_SQUARE_CLOSE)))
   {
      return(chunk_get_prev_type(cur, (c_token_t)(cur->type - 1), cur->level, scope));
   }
   return(cur);
}


//! skip to the final word/type in a :: chain
chunk_t *chunk_skip_dc_member(chunk_t *start, scope_e scope = scope_e::ALL);
chunk_t *chunk_skip_dc_member_rev(chunk_t *start, scope_e scope = scope_e::ALL);


/**
 * checks if a chunk is valid and is a comment
 *
 * comment means any kind of
 * - single line comment
 * - multiline comment
 * - C comment
 * - C++ comment
 */
static inline bool chunk_is_comment(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_COMMENT)
         || chunk_is_token(pc, CT_COMMENT_MULTI)
         || chunk_is_token(pc, CT_COMMENT_CPP));
}


/**
 * Returns true if the chunk under test is an inheritance access specifier
 */
static inline bool chunk_is_cpp_inheritance_access_specifier(chunk_t *pc)
{
   return(  language_is_set(LANG_CPP)
         && pc != nullptr
         && (  chunk_is_token(pc, CT_ACCESS)
            || chunk_is_token(pc, CT_QUALIFIER))
         && (  std::strncmp(pc->str.c_str(), "private", 7) == 0
            || std::strncmp(pc->str.c_str(), "protected", 9) == 0
            || std::strncmp(pc->str.c_str(), "public", 6) == 0));
} // chunk_is_cpp_inheritance_access_specifier


static inline bool chunk_is_colon(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_ACCESS_COLON)
         || chunk_is_token(pc, CT_ASM_COLON)
         || chunk_is_token(pc, CT_BIT_COLON)
         || chunk_is_token(pc, CT_CASE_COLON)
         || chunk_is_token(pc, CT_CLASS_COLON)
         || chunk_is_token(pc, CT_COLON)
         || chunk_is_token(pc, CT_COND_COLON)
         || chunk_is_token(pc, CT_CONSTR_COLON)
         || chunk_is_token(pc, CT_CS_SQ_COLON)
         || chunk_is_token(pc, CT_D_ARRAY_COLON)
         || chunk_is_token(pc, CT_FOR_COLON)
         || chunk_is_token(pc, CT_LABEL_COLON)
         || chunk_is_token(pc, CT_OC_COLON)
         || chunk_is_token(pc, CT_OC_DICT_COLON)
         || chunk_is_token(pc, CT_TAG_COLON)
         || chunk_is_token(pc, CT_WHERE_COLON));
}


static inline bool chunk_is_single_line_comment(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_COMMENT)
         || chunk_is_token(pc, CT_COMMENT_CPP));
}


static inline bool chunk_is_newline(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_NEWLINE)
         || chunk_is_token(pc, CT_NL_CONT));
}


static inline bool chunk_is_semicolon(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_SEMICOLON)
         || chunk_is_token(pc, CT_VSEMICOLON));
}


/**
 * checks if a chunk is valid and is a blank character
 *
 * @note check compares if len == 0
 *
 * @todo rename function: blank is a space not an empty string
 */
static inline bool chunk_is_blank(chunk_t *pc)
{
   return(  pc != nullptr
         && (pc->len() == 0));
}


//! checks if a chunk is valid and either a comment or newline
static inline bool chunk_is_comment_or_newline(chunk_t *pc)
{
   return(  chunk_is_comment(pc)
         || chunk_is_newline(pc));
}


//! checks if a chunk is valid and either a comment or newline or ignored
static inline bool chunk_is_comment_or_newline_or_ignored(chunk_t *pc)
{
   return(  chunk_is_comment(pc)
         || chunk_is_newline(pc)
         || chunk_is_token(pc, CT_IGNORED));
}


static inline bool chunk_is_balanced_square(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_SQUARE_OPEN)
         || chunk_is_token(pc, CT_TSQUARE)
         || chunk_is_token(pc, CT_SQUARE_CLOSE));
}


static inline bool chunk_is_preproc(chunk_t *pc)
{
   return(  pc != nullptr
         && pc->flags.test(PCF_IN_PREPROC));
}


static inline bool chunk_is_comment_or_newline_in_preproc(chunk_t *pc)
{
   return(  pc != nullptr
         && chunk_is_preproc(pc)
         && (  chunk_is_comment(pc)
            || chunk_is_newline(pc)));
}


static inline bool chunk_is_comment_newline_or_preproc(chunk_t *pc)
{
   return(  chunk_is_comment(pc)
         || chunk_is_newline(pc)
         || chunk_is_preproc(pc));
}


static inline bool chunk_is_comment_newline_or_blank(chunk_t *pc)
{
   return(  chunk_is_comment_or_newline(pc)
         || chunk_is_blank(pc));
}


static inline bool chunk_is_Doxygen_comment(chunk_t *pc)
{
   if (  pc == nullptr
      || !chunk_is_comment(pc))
   {
      return(false);
   }
   // check the third character
   const char   *sComment = pc->text();
   const size_t len       = strlen(sComment);

   if (len < 3)
   {
      return(false);
   }
   return(  (sComment[2] == '/')
         || (sComment[2] == '!')
         || (sComment[2] == '@'));
}


static inline bool chunk_is_type(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_TYPE)
         || chunk_is_token(pc, CT_PTR_TYPE)
         || chunk_is_token(pc, CT_BYREF)
         || chunk_is_token(pc, CT_DC_MEMBER)
         || chunk_is_token(pc, CT_QUALIFIER)
         || chunk_is_token(pc, CT_STRUCT)
         || chunk_is_token(pc, CT_ENUM)
         || chunk_is_token(pc, CT_UNION));
}


static inline bool chunk_is_str(chunk_t *pc, const char *str, size_t len)
{
   return(  pc != nullptr                         // valid pc pointer
         && (pc->len() == len)                    // token size equals size parameter
         && (memcmp(pc->text(), str, len) == 0)); // token name is the same as str parameter

   /*
    * TODO: possible access beyond array for memcmp, check this
    * why not use strncmp here?
    */
}


static inline bool chunk_is_str_case(chunk_t *pc, const char *str, size_t len)
{
   return(  pc != nullptr
         && (pc->len() == len)
         && (strncasecmp(pc->text(), str, len) == 0));
}


static inline bool chunk_is_word(chunk_t *pc)
{
   return(  pc != nullptr
         && (pc->len() >= 1)
         && CharTable::IsKw1(pc->str[0]));
}


static inline bool chunk_is_star(chunk_t *pc)
{
   return(  pc != nullptr
         && (pc->len() == 1)
         && (pc->str[0] == '*')
         && pc->type != CT_OPERATOR_VAL);
}


static inline bool chunk_is_nullable(chunk_t *pc)
{
   return(  language_is_set(LANG_CS)
         && (pc != nullptr)
         && (pc->len() == 1)
         && (pc->str[0] == '?'));
}


static inline bool chunk_is_addr(chunk_t *pc)
{
   if (  pc != nullptr
      && (  chunk_is_token(pc, CT_BYREF)
         || (  (pc->len() == 1)
            && (pc->str[0] == '&')
            && pc->type != CT_OPERATOR_VAL)))
   {
      chunk_t *prev = chunk_get_prev(pc);

      if (  pc->flags.test(PCF_IN_TEMPLATE)
         && (  chunk_is_token(prev, CT_COMMA)
            || chunk_is_token(prev, CT_ANGLE_OPEN)))
      {
         return(false);
      }
      return(true);
   }
   return(false);
}


static inline bool chunk_is_msref(chunk_t *pc) // ms compilers for C++/CLI and WinRT use '^' instead of '*' for marking up reference types vs pointer types
{
   return(  language_is_set(LANG_CPP)
         && (  pc != nullptr
            && (pc->len() == 1)
            && (pc->str[0] == '^')
            && pc->type != CT_OPERATOR_VAL));
}


static inline bool chunk_is_ptr_operator(chunk_t *pc)
{
   return(  (  chunk_is_star(pc)
            || chunk_is_addr(pc)
            || chunk_is_msref(pc))
         || chunk_is_nullable(pc));
}


static inline bool chunk_is_pointer_or_reference(chunk_t *pc)
{
   return(  chunk_is_ptr_operator(pc)
         || chunk_is_token(pc, CT_BYREF));
}


//! Check to see if there is a newline between the two chunks
bool chunk_is_newline_between(chunk_t *start, chunk_t *end);


static inline bool chunk_is_closing_brace(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_BRACE_CLOSE)
         || chunk_is_token(pc, CT_VBRACE_CLOSE));
}


static inline bool chunk_is_opening_brace(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_BRACE_OPEN)
         || chunk_is_token(pc, CT_VBRACE_OPEN));
}


static inline bool chunk_is_vbrace(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_VBRACE_CLOSE)
         || chunk_is_token(pc, CT_VBRACE_OPEN));
}


static inline bool chunk_is_paren_open(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_PAREN_OPEN)
         || chunk_is_token(pc, CT_SPAREN_OPEN)
         || chunk_is_token(pc, CT_TPAREN_OPEN)
         || chunk_is_token(pc, CT_FPAREN_OPEN)
         || chunk_is_token(pc, CT_LPAREN_OPEN));
}


static inline bool chunk_is_paren_close(chunk_t *pc)
{
   return(  chunk_is_token(pc, CT_PAREN_CLOSE)
         || chunk_is_token(pc, CT_SPAREN_CLOSE)
         || chunk_is_token(pc, CT_TPAREN_CLOSE)
         || chunk_is_token(pc, CT_FPAREN_CLOSE));
}


/**
 * Returns true if either chunk is null or both have the same preproc flags.
 * If this is true, you can remove a newline/nl_cont between the two.
 */
static inline bool chunk_same_preproc(chunk_t *pc1, chunk_t *pc2)
{
   return(  pc1 == nullptr
         || pc2 == nullptr
         || ((pc1->flags & PCF_IN_PREPROC) == (pc2->flags & PCF_IN_PREPROC)));
}


/**
 * Returns true if it is safe to delete the newline token.
 * The prev and next chunks must have the same PCF_IN_PREPROC flag AND
 * the newline can't be after a C++ comment.
 */
static inline bool chunk_safe_to_del_nl(chunk_t *nl)
{
   chunk_t *tmp = chunk_get_prev(nl);

   if (chunk_is_token(tmp, CT_COMMENT_CPP))
   {
      return(false);
   }
   return(chunk_same_preproc(chunk_get_prev(nl), chunk_get_next(nl)));
}


/**
 * Checks if a chunk points to the opening parenthese of a
 * for(...in...) loop in Objective-C.
 *
 * @return true  - the chunk is the opening parentheses of a for in loop
 */
static inline bool chunk_is_forin(chunk_t *pc)
{
   if (  language_is_set(LANG_OC)
      && chunk_is_token(pc, CT_SPAREN_OPEN))
   {
      chunk_t *prev = chunk_get_prev_ncnnl(pc);

      if (chunk_is_token(prev, CT_FOR))
      {
         chunk_t *next = pc;

         while (  next != nullptr
               && next->type != CT_SPAREN_CLOSE
               && next->type != CT_IN)
         {
            next = chunk_get_next_ncnnl(next);
         }

         if (chunk_is_token(next, CT_IN))
         {
            return(true);
         }
      }
   }
   return(false);
}


/**
 * Returns true if pc is an CT_ATTRIBUTE or CT_DECLSPEC
 */
bool chunk_is_attribute_or_declspec(chunk_t *pc);


/**
 * Returns true if pc is one of CT_CLASS, CT_ENUM, CT_ENUM_CLASS, CT_STRUCT or CT_UNION
 */
bool chunk_is_class_enum_struct_union(chunk_t *pc);


/**
 * Returns true if pc is a CT_CLASS or CT_STRUCT
 */
bool chunk_is_class_or_struct(chunk_t *pc);


/**
 * Returns true if pc is one of CT_CLASS, CT_STRUCT or CT_UNION
 */
bool chunk_is_class_struct_union(chunk_t *pc);


/**
 * Returns true if pc is a CT_ENUM or CT_ENUM_CLASS
 */
bool chunk_is_enum(chunk_t *pc);


void set_chunk_type_real(chunk_t *pc, c_token_t tt, const char *func, int line);


void set_chunk_parent_real(chunk_t *pc, c_token_t tt, const char *func, int line);


#define set_chunk_type(pc, tt)      set_chunk_type_real((pc), (tt), __unqualified_func__, __LINE__)


#define set_chunk_parent(pc, tt)    set_chunk_parent_real((pc), (tt), __unqualified_func__, __LINE__)


c_token_t get_chunk_parent_type(chunk_t *pc);


void chunk_flags_set_real(chunk_t *pc, pcf_flags_t clr_bits, pcf_flags_t set_bits);


#define chunk_flags_upd(pc, cc, ss)    chunk_flags_set_real((pc), (cc), (ss))


#define chunk_flags_set(pc, ss)        chunk_flags_set_real((pc), {}, (ss))


#define chunk_flags_clr(pc, cc)        chunk_flags_set_real((pc), (cc), {})


void chunk_set_parent(chunk_t *pc, chunk_t *parent);


c_token_t get_type_of_the_parent(chunk_t *pc);


/**
 * @brief compare the positions of two tokens in a file.
 *
 * The function compares the two positions of two tokens.
 *
 * @param A_token
 * @param B_token
 *
 * @return returns an integer less than, equal to, or greater than zero
 *         if A_token is found, respectively, to be less/before than, to
 *         match, or be greater/after than B_token.
 */
int chunk_compare_position(const chunk_t *A_token, const chunk_t *B_token);


#endif /* CHUNK_LIST_H_INCLUDED */
