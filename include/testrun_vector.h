/***
        ------------------------------------------------------------------------

        Copyright 2017 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        This file is part of the testrun project. http://testrun.info

        ------------------------------------------------------------------------
*//**

        @file           testrun_vector.h
        @author         Markus Toepfer
        @date           2017-11-18

        @ingroup        testrun_lib

        @brief          Definition of an interface for testrun_vector.

        This structure is a management structure for a dynamic array of
        void pointers. Provided core functionality is the management of
        a dynamically growing and shrinking memory area to store void* data.
        In addition functions to copy and free the data pointed to may be
        attached to the vector manager to automatically free the pointers,
        if the whole vector will be freed.

        @example

        Create a vector with 10 items and use free() for item release

                testrun_vector *v = testrun_vector_create(10, free, NULL);

        Add some content (using all included functions to add a value)

                testrun_vector_push(    v,      "string 1");
                testrun_vector_push(    v,      "string 2");
                testrun_vector_set(     v, 4,   "string 6");
                testrun_vector_push(    v,      "string 7");
                testrun_vector_add(     v,      "string 3");
                testrun_vector_insert(  v, 3,   "string 4"); // fill empty at 3
                testrun_vector_insert(  v, 4,   "string 5"); // left shift from 4

        creates the following vector with items set as shown:

        v->items[0] = "string 1";
        v->items[1] = "string 2";
        v->items[2] = "string 3";
        v->items[3] = "string 4";
        v->items[4] = "string 5";
        v->items[5] = "string 6";
        v->items[6] = "string 7";
        v->items[7] = NULL;
        v->items[8] = NULL;
        v->items[9] = NULL;


        ------------------------------------------------------------------------
*/

#ifndef testrun_vector_h
#define testrun_vector_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <execinfo.h>

#include "testrun_log.h"

#define TESTRUN_VECTOR_DEFAULT_CHANGE_RATE 100

typedef struct testrun_vector testrun_vector;

/*******************************************************************************
 *
 *      STRUCTURE DEFINITION
 *
 ******************************************************************************/

struct testrun_vector{

        /* DATA */

        size_t last;    /* @param last item used */
        size_t size;    /* @param maximum available item slots */
        size_t rate;    /* @param rate growth and shrink rate of the vector */

        void **items;   /* @param items pointer store of items */

        /* custom pluggable item functions */

        void  (*item_free)   (void *);             /* free item ptr content*/
        void  *(*item_copy)  (void const * const); /* copy item ptr content*/
};

/*
 *      ------------------------------------------------------------------------
 *
 *      STRUCTURE RELATED FUNCTIONS                                     #SRF
 *
 *      The first block contains function realted to the structure itself,
 *      including creation, termination and copying.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Create a new vector with initialization of vector parameter.

        @param  rate            (optional) change rate for item reallocations
        @param  item_free       (optional) function to free item content
        @parem  item_copy       (optional) function to copy item content

        @return                 new vector or NULL on error
 */
testrun_vector *testrun_vector_create(
        size_t rate,
        void (*item_free) (void *),
        void *(*item_copy) (void const * const));

/*----------------------------------------------------------------------------*/

/**
        Deallocated the structure, previous allocated using create. This a
        synonym function to the standard free() call of C. Anyway this function
        is added for convinience only and using @see testrun_vector_terminate()
        to free and clear the structure.

        @param  self            vector to be freed

        @return                 This function will not return any value.
 */
void testrun_vector_free(
        testrun_vector * self);

/*----------------------------------------------------------------------------*/

/**
        Clear all vector item pointers and reset the vector size to self->rate.
        If an items_free function is set within the instance running, this
        function will be used to free the items content of each item pointer.

        @param  self            pointer to vector structure
 */
bool testrun_vector_clear(
        testrun_vector * const self);

/*----------------------------------------------------------------------------*/

/**
        Destroy all allocated memory of the vector structure.
        This functions free the space allocated for the instance structure, as
        well as for the items structure allocated. Content pointers
        will be untouched, even if an items_free is configured.

        @param  self            pointer to vector to be destroyed

        @return                 allways NULL
 */
testrun_vector *testrun_vector_destroy(
        testrun_vector *self);

/*----------------------------------------------------------------------------*/

/**
        Clear and destroy a vector structure. This function simply calls
        testrun_vector_clear, followed by testrun_vector_destroy.

        @param  self            pointer to vector to be terminated

        @returns                allways NULL
 */
testrun_vector *testrun_vector_terminate(
        testrun_vector *self);

/*----------------------------------------------------------------------------*/

/**
        Copy a vector and all vector items.

        @param self             vector to be copied

        @returns                independend copy of the vector
 */
testrun_vector *testrun_vector_copy(
        testrun_vector const * const self);

/*
 *      ------------------------------------------------------------------------
 *
 *      GETTER / SETTER FUNCTIONS                                      #GETTER
 *
 *      The second part defines standard vector functions to GET and SET
 *      a value within vector->items, whitout changing the amount of
 *      possible elements of vector items.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Get a pointer to value content from a position within the vector.

        @param self             vector with items
        @param pos              position within the items vector

        @returns                pointer to value content at pos in items
 */
void *testrun_vector_get(
        testrun_vector const * const self,
        size_t pos);

/*----------------------------------------------------------------------------*/

/**
        Set a pointer value content to a position within the vector.
        If the content at the value is already set, the content will be
        freed using items_free, if items_free is set.
        This works like overriding the old value with a new one.

        @param self             vector with items
        @param pos              position within the items vector
        @param value            pointer to value content

        @returns                true on success, error on failure
 */
bool testrun_vector_set(
        testrun_vector * const self,
        size_t pos,
        void  * const value);

/*
 *      ------------------------------------------------------------------------
 *
 *      DYNAMIC VECTOR FUNCTIONS                                        #DVF
 *
 *      The third part defines dynamic vector functions, like
 *      DELETE, REMOVE, ADD, INSERT, PUSH and POP, which shrink and expand the
 *      amount of possible vector items based on vector->rate.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Delete a pointer value content to a position within the vector.
        If the content at the value is set, the content will be
        freed using items_free, if items_free is set.

        @param self             vector with items
        @param pos              position within the items vector

        @returns                -1 on error
                                 1 on success item found and deleted
                                 0 on success no item found (nothing deleted)
 */
int testrun_vector_delete(
        testrun_vector * const self,
        size_t pos);

/*----------------------------------------------------------------------------*/

/**
        Remove a pointer value content from a position within the vector.
        This function updates end and size of the items array.

        @param self             vector with items
        @param pos              position within the items vector

        @returns                pointer to value removed from position or NULL
 */
void *testrun_vector_remove(
        testrun_vector * const self,
        size_t pos);

/*----------------------------------------------------------------------------*/

/**
        Add a value pointer to the first free slot within the vector. This
        function will fill gaps left with an earlier remove.
        If no free slot is found, the value will be pushed to the end of the
        vector.

        @param self             vector with items
        @param value            pointer to value to be added

        @returns                true on success, errror on failure
 */
bool testrun_vector_add(
        testrun_vector * const self,
        void * const value);

/*----------------------------------------------------------------------------*/

/**
        Insert a value a position. If the position is not empty, the value will
        be inserted in place, which means all possible following items will be
        shifted by one in direction vector->end.

        @param self             vector with items
        @param pos              position within the items vector
        @param value            pointer to value content

        @returns                true on success, error on failure
 */
bool testrun_vector_insert(
        testrun_vector * const self,
        size_t pos,
        void  * const value);


/*----------------------------------------------------------------------------*/

/**
        PUSH a value at the end of the vector (push last). If the vector has
        no free space for the pointer, the vector will be expanded by
        vector->rate.

        @param self             vector with items
        @param value            pointer to value to be added

        @returns                true on success, errror on failure
 */
bool testrun_vector_push(
        testrun_vector * const self,
        void * const value);

/*----------------------------------------------------------------------------*/

/**
        POP a value from the end of the vector (pop last).
        The vector will be shrinked automatically by vector->rate if free space
        is > as rate.

        @param self             vector with items

        @returns                poped value or NULL
 */
void *testrun_vector_pop(
        testrun_vector * const self);


/*
 *      ------------------------------------------------------------------------
 *
 *      ADDITIONAL FUNCTIONS                                            #ADD
 *
 *      Additional usefull external defined functionality.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Check if the vector is empty.

        @param self             vector with items

        @returns                poped value or NULL
 */
bool testrun_vector_is_empty(
        testrun_vector const * const self);

/*----------------------------------------------------------------------------*/

/**
        Very basic example for an item pointer copy function.

        @param pointer          pointer to be copied

        @returns                A void pointer to a copied memory area or NULL
 */
void *testrun_vector_int64_example_copy(
        void const * const pointer);

/*----------------------------------------------------------------------------*/

/**
        Very basic example for an item pointer free function.

        @param pointer          pointer to be freed

        @returns                This function returns nothing.
 */
void testrun_vector_int64_example_free(
        void  * pointer);

/*----------------------------------------------------------------------------*/

/**

        @param pointer          pointer to be freed
        @returns                This function returns nothing.
 */
void testrun_vector_item_free(
        void  * pointer);


/*----------------------------------------------------------------------------*/

/**
        Dump the vector content to stdout. This file prints a vector header,
        and uses backtracing to identify function names of function pointer.
        Therefore the source should be compiled using -rdynamic. Otherwise
        backtracing will fail.

        @param self             pointer to self

        @returns                True if the input is ok and the dump is started.
 */
bool testrun_vector_dump_function_backtrace(
        testrun_vector const * const self);

/*----------------------------------------------------------------------------*/

/**
        Dump the vector content to stdout. This file prints a vector header,
        followed by a content dump including all slots of the vector.
        This function may be usefull during development as well as manual
        checking of vector content.

        @param self             pointer to self

        @returns                True if the input is ok and the dump is started.
 */
bool testrun_vector_dump(
        FILE *stream, testrun_vector const * const self);


#endif /* testrun_vector_h */
