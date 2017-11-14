/***
 *      ------------------------------------------------------------------------
 *
 *      Copyright 2017 Markus Toepfer
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 *      This file is part of the testrun project. http://testrun.info
 *
 *      ------------------------------------------------------------------------
 ***//**
 *
 *      @file           tr_string.h
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Interface for string manipulation within testrun_lib
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef tr_string_h
#define tr_string_h

#define TR_STRING_DEFAULT_SIZE  96

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*----------------------------------------------------------------------------*/

/**
 *      Free a pointer and return NULL. Basically just a pointer unset.
 *      Intended use as replacement for free(string); string = NULL;
 *
 *      USE CASE:       string = tr_string_free(string);
 *
 *      @param string   pointer to be freed
 *      @return         returns always NULL
 */
char *tr_string_free(char *string);

/*----------------------------------------------------------------------------*/

/**
 *      Prepare a result pointer (allocate if NULL)
 *      This function check if the poniter *result is used, and if it is
 *      used, if the use is a valid string up to size. (is NULL terminated)
 *
 *      It the string at result is not a NULL terminated string within
 *      size, the function fails. (used > size)
 *
 *      If the pointer result points to NULL, new space if allocated using
 *      a default allocation size TR_STRING_DEFAULT_SIZE
 *
 *      After success, open will contain the number of open bytes including
 *      the terminating \0, so on success open will be positive (at least 1)
 *      Used will contain all bytes of size used as string, so used will be
 *      strlen(*result), but take into account the maximum length of the
 *      allocation at *result.
 *
 *      @param  result  (mandatory) pointer to be checked
 *      @param  size    (mandatory) size of memory at *pointer
 *      @param  open    (mandatory) pointer to open counter (will be set)
 *      @param  used    (mandatory) pointer to used counter (will be set)
 *
 *      @return         returns 0 on success,  -1 on error
 */
int tr_string_prepare(
        char **string,
        size_t * const size,
        size_t * const open,
        size_t * const used);

/*----------------------------------------------------------------------------*/

/**
 *      Append a source string to a destination string. The destination MUST
 *      be an allocated string, as the function is based on reallocations.
 *
 *      If the allocated buffer at destination is big enough to contain the
 *      source string, the string will be append without reallocation,
 *      otherwise a reallocation with the minimum required size to contain
 *      both string will be done and set at *dest. In addition the size of
 *      the reallocation will be set at size.
 *
 *      @param dest     pointer to buffer for the the result of the string
 *                      concationation. If the dest points to NULL, a new
 *                      buffer will be allocated and size set to the allocated
 *                      size.
 *      @param size     size of the destination buffer
 *      @param source   pointer to buffer to be appended to dest
 *      @param len      length of the source to be added to dest (at most)
 *
 *      @return         returns 0 on success,  -1 on error
 */
int tr_string_append(
        char **dest, size_t * const size,
        char const * const source, size_t len);


/*----------------------------------------------------------------------------*/


/**
 *      Write a string embeded with a prefix, suffix and delimiter.
 *      This function basically concats 4 strings in order and (re) allocates
 *      the required size to fit all in a target string.
 *
 *      USE CASE 1
 *
 *      size = tr_write_embeded(
 *              &buffer,&open,&used,1
 *              "* ",2                          prefix & length,
 *              "new comment line", 16          source & length,
 *              0,0,                            suffix & length,
 *              "\n", 1);                       delimiter & length
 *
 *      will write "* new comment line\n" to buffer and set open and used
 *      If size_t open is less than the amount of required bytes, this
 *      function will reallocated *buffer and set used and open to the
 *      amount of bytes used, as well as open allocated bytes at the pointer
 *      *buffer.
 *
 *      USE CASE 2#
 *
 *              write a string in brackets
 *
 *      size = tr_write_embeded(&b,&o,&u,1 "(",1, "string",5, ")",1, 0,0);
 *      will write "(string)" at *buffer.
 *
 *
 *      @param  buffer  (mandatory) pointer to write to (allocated OR null)
 *      @param  open    (mandatory) pointer to size of unused bytes in buffer
 *      @param  used    (manadtory) pointer to size of used bytes in buffer
 *      @param  rate    (optional)  rate to be used for realloaction
 *      @param  prefix  (optional)  prefix to be written before source
 *      @param  px_len  (optional)  length of the prefix to write
 *      @param  source  (optional)  source string to write to the buffer
 *      @param  sc_len  (optional)  length of the source string to write
 *      @param  suffix  (optional)  suffix to be written after source
 *      @param  sx_len  (optional)  length of the suffix to write
 *      @param  lbreak  (optional)  linebreak/delimiter to write after suffix
 *      @param  lb_len  (optional)  length of the delimiter (not incl \0)
 *
 *      @return         -1 on error, size written to *buffer
 */
int64_t tr_string_write_embeded(
        char  **buffer,
        size_t * const open,
        size_t * const used,
        size_t rate,
        char const * const prefix,  size_t px_len,
        char const * const source,  size_t sc_len,
        char const * const suffix,  size_t sx_len,
        char const * const lbreak,  size_t lb_len);


/*----------------------------------------------------------------------------*/

/**
 *      Embed a string into something. May be used for different kinds
 *      of string manipulations in form of adding something around a string,
 *      as well as around a bunch of token strings separated by a common
 *      separator (e.g. comma lists or simply lines)
 *
 *      An addtional use case of this function is a delimiter replacement.
 *      This may be used to exchange a lineending e.g. "\n" to another "\r\n"
 *      or a commalist to a space separated list.
 *
 *      Both standard functionalities may be combined to implement functions
 *      like commalist to html list.
 *
 *      USE CASE COMMALIST TO HTML LIST
 *
 *              r = NULL; s=0;
 *              tr_string_embed(
 *                       &r,            // result pointer
 *                       &s,            // pointer to result size
 *                      "a,b,c", 5,     // source list
 *                      "<li>", 4,      // PREFIX all tokens with <li>
 *                      "</li>",5,      // SUFFIX all tokens with </li>
 *                      ",",1,          // use token separator ","
 *                      0,0);           // do not set a new token separator
 *
 *              r = "<li>a</li><li>b</li><li>c</li>"; s = strlen(r);
 *
 *      This example is a showcase example using a lot of functionality of
 *      this function in parallel. Some more streamlined use cases are:
 *
 *      USE CASE 1 embed a string between prefix and suffix
 *
 *              r = NULL; s = 0;
 *              tr_string_embed(&r, &s, "abcdef", 6, "_",1, "s",!, 0,0, 0,0);
 *              r = "_abcdef!"; s = 8;
 *
 *      USE CASE 2 append an allocated string with an embeded string
 *
 *              r = "test"; s = 4;
 *              tr...(&r, &s, "abcdef", 6, "-",1, "+",1, 0,0, 0,0);
 *              r = "test-abcdef+" s = 12;
 *
 *      USE CASE 3 delete all delimiter
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "ab\ncd\nef\n", 9, 0,0, 0,0 "\n",1, 0,0);
 *              r = "abcdef" s = 6;
 *
 *      USE CASE 4 delimiter to () token
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "ab\ncd\nef\n", 9, "(",1, ")",1 "\n",1, 0,0);
 *              r = "(ab)(cd)(ef)" s = 12;
 *
 *      USE CASE 5 delimiter exchange
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "ab\ncd\nef\n", 9, 0,0, 0,0, "\n",1, ":",1);
 *              r = "ab,cd,ef" s = 9;
 *
 *      USE CASE 6 prefix all lines
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "ab\ncd\nef\n", 9, "_",1, 0,0, "\n",1, "\n",1);
 *              r = "_ab\n_cd\_nef\n" s = 12;
 *
 *      USE CASE 7 suffix all lines
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "ab\ncd\nef\n", 9, 0,0, "_",1, "\n",1, "\n",1);
 *              r = "ab_\ncd_\nef_\n" s = 12;
 *
 *      USE CASE 8 char to JSON char
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "string", 5, "\",1, "\"",1, 0,0, 0,0);
 *              r = "\"string\"" s = 7;
 *
 *      USE CASE 9 Tagging in general (e.g. HTML tagging)
 *
 *              r = NULL; s = 0;
 *              tr...(&r, &s, "string", 5, "<div>",5, "</div>"",6, 0,0, 0,0);
 *              r = "<div>string</div>" s = 16;
 *
 *      Primary intended use case of this function are line manipulations in
 *      form of prefixing (e.g. uncomment) lines, as well as string embeding
 *      if form of pre and suffixing a string (e.g. char to JSON char).
 *
 *      @param  result  (mandatory) pointer to write to (allocated OR null)
 *      @param  size    (mandatory) pointer to size of result (may be 0)
 *      @param  source  (mandatory) source to write to result
 *      @param  sc_len  (mandatory) length of source (at most)
 *      @param  prefix  (mandatory) content to write to result before source
 *      @param  px_len  (mandatory) length of prefix (at most)
 *      @param  suffix  (mandatory) content to write to result after source
 *      @param  sx_len  (mandatory) length of suffix (at most)
 *      @param  delim1  (mandatory) delimiter of source tokens
 *      @param  d1_len  (mandatory) length of delim1 (at most)
 *      @param  delim2  (mandatory) new delimiter for source tokens
 *      @param  d2_len  (mandatory) length of delim2 (at most)
 *
 *      @return         -1 on error, 0 on success
 */
int tr_string_embed(
        char **result, size_t * const size,
        char const * const source, size_t sc_len,
        char const * const prefix, size_t px_len,
        char const * const suffix, size_t sx_len,
        char const * const delim1, size_t d1_len,
        char const * const delim2, size_t d2_len);

/*----------------------------------------------------------------------------*/

/**
 *      Unset a string with len from the end of another string.
 *
 *      @param  result  (mandatory) pointer to pointer of source string
 *      @param  size    (mandatory) pointer to size of source string (incl \0)
 *      @param  string  (mandatory) string to be unset
 *      @param  len     (mandatory) length of string to be unset (incl \0)
 *
 *      @return         -1 on error, 0 on success
 */
int tr_string_unset_end(
        char * const result, size_t size,
        char const * const string, size_t len);

/*----------------------------------------------------------------------------*/

/**
 *      Replace the first occurance of an string within a source and write the
 *      result to an exsting or empty buffer. (append or new).
 *
 *      On success *result will contain the source string with replaced content
 *      old, appended to the existing string at result.
 *
 *      EXAMPLE
 *
 *              r = calloc(10, sizeof(char));
 *              strcat(r, "test");
 *
 *              s = "whenever_whenever";
 *              o = "en";
 *              n = "_at_"
 *
 *              tr...(&r, &size, s, strlen(s), o, 3, n, 3);
 *
 *              r = "testwh_at_ever_whenever"
 *
 *
 *      USE CASE 1 - string exchange before appending to another string
 *      USE CASE 2 - string exchange only
 *
 *      @param  result   (mandatory) pointer to write to (allocated OR null)
 *      @param  size     (mandatory) pointer to size of result (may be 0)
 *      @param  source   (mandatory) source to write to result
 *      @param  sc_len   (mandatory) length of source (at most)
 *      @param  old_item (mandatory) item to be replaced
 *      @param  old_len  (mandatory) length of item to be replaced (incl \0)
 *      @param  new_item (optional)  new_item to be written at position of old
 *      @param  new_len  (optional)  length of new_item (incl \0)
 *
 *      @return         -1 on error, 0 on success
 */
int tr_string_replace_first(
        char **result, size_t * const size,
        char const * const source,   size_t sc_len,
        char const * const old_item, size_t old_len,
        char const * const new_item, size_t new_len);

/*----------------------------------------------------------------------------*/

/**
 *      Change all occurances of old_item in source to new_item.
 *
 *      USE CASE - exchange of all occurances of any string
 *
 *              e.g. linbreak exchange
 *              tr_...(&r, &s, "a\nb\nc\n", "\n", 1, "\r\n", 2, true);
 *              r = "a\r\nb\r\nc\r\n"
 *
 *
 *      USE CASE of set_last
 *
 *              r = "a\nr\nc"           // no final linebreak if false
 *              r = "a\nb\nc\n"         // ensured final linebreak if true
 *
 *      EXAMPLE
 *
 *              r = calloc(10, sizeof(char));
 *              strcat(r, "test");
 *
 *              s = "whenever_whenever";
 *              o = "en";
 *              n = "_at_"
 *
 *              tr...(&r, &size, s, strlen(s), o, 3, n, 3);
 *
 *              r = "testwh_at_ever_wh_at_ever"
 *
 *      @param  result  (mandatory) pointer to write to (allocated OR null)
 *      @param  size    (mandatory) pointer to size of result (may be 0)
 *      @param  source  (mandatory) source to write to result
 *      @param  sc_len  (mandatory) length of source (at most)
 *      @param  old_item (mandatory) item to be replaced
 *      @param  old_len  (mandatory) length of item to be replaced
 *      @param  new_item (optional) new_item to be written at position of old
 *      @param  new_len  (optional) length of new_item
 *      @param  set_last (mandatory) if false a possible last item will be unset
 *
 *      @return         -1 on error, 0 on success
 */
int tr_string_replace_all(
        char **result, size_t * const size,
        char const * const source,   size_t sc_len,
        char const * const old_item, size_t old_len,
        char const * const new_item, size_t new_len,
        bool set_last);




#endif /* tr_string_h */
