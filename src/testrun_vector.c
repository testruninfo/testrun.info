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

        @file           testrun_vector.c
        @author         Markus Toepfer
        @date           2017-11-18

        @ingroup        testrun_lib

        @brief          Implementation of testrun_vector related functionality.


        ------------------------------------------------------------------------
*/

#include "../include/testrun_vector.h"

/*
 *      ------------------------------------------------------------------------
 *
 *      STATIC FUNCTIONS                                                #STATIC
 *
 *      Static, private functions
 *
 *      ------------------------------------------------------------------------
 */

static bool testrun_vector_resize(
        testrun_vector * const self,
        size_t  item_slots){

        if (!self || !self->items)
                return false;

        if (item_slots < 1)
                return false;

        if (self->last >= item_slots)
                return false;

        size_t old_max = self->size;

        void *newPtr = realloc(self->items, item_slots * sizeof(void*) );
        if (!newPtr)
                return false;

        self->items = newPtr;
        self->size   = item_slots - 1;

        // initialize new pointer slots to NULL
        for (size_t i = old_max; i < self->size; i++){
                self->items[i] = NULL;
        }

        return true;
}

/*----------------------------------------------------------------------------*/

static bool testrun_vector_auto_adjust(
        testrun_vector * const self){

        if (!self || !self->items)
                return false;

        if (self->last > self->size)
                return false;

        /* grow if the vector is used by 100 % */
        if (self->last == self->size)
                return testrun_vector_resize(self, (self->size + self->rate + 1));

        /* shrink if the use of the vector is smaller than the rate */
        if ((self->size - self->last) > self->rate)
                return testrun_vector_resize(self, (self->size - self->rate + 1));

        return true;
}

/*----------------------------------------------------------------------------*/

static bool testrun_vector_set_new_end(
        testrun_vector * const vector,
        size_t pos){

        if (!vector || !vector->items)
                return false;

        if (pos >= vector->size)
                return false;

        if (pos < vector->last)
                return false;

        size_t i = 0;

        for (i = pos; i > 0; i--){

                if (vector->items[i] != NULL){

                        vector->last = i;
                        break;
                }

        }

        if (i == 0)
                vector->last = 0;

        return true;
}

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

testrun_vector *testrun_vector_create(
        size_t rate,
        void (*item_free)  (void *),
        void *(*item_copy) (void const * const)){

        if (rate == 0)
                rate = TESTRUN_VECTOR_DEFAULT_CHANGE_RATE;

        testrun_vector *vector  = calloc(1, sizeof(struct testrun_vector));
        if (!vector){
                log_oom();
                goto error;
        }

        /* DATA -------------------------------------------------------  */

        vector->last             = 0;
        vector->size            = rate;
        vector->rate            = rate;

        vector->items           = calloc(vector->size, sizeof(void*));
        if (!vector->items){
                log_dev("test");
                goto error;
        }

        /* pluggable item functions -----------------------------------  */

        vector->item_free       = item_free;
        vector->item_copy       = item_copy;

        return vector;

error:
        if (vector){
                if (vector->items)
                        free(vector->items);
                free(vector);
        }
        return NULL;
}

/*----------------------------------------------------------------------------*/

void testrun_vector_free(testrun_vector * self){

        if (self)
                self = testrun_vector_terminate(self);
}

/*----------------------------------------------------------------------------*/

bool testrun_vector_clear(testrun_vector * const self){

        if (!self)
                return false;

        if (self->items){

                if (self->item_free) {

                        for (size_t i = 0; i <= self->last; i++){

                                self->item_free(self->items[i]);
                        }

                }

                free(self->items);
                self->items = NULL;
        }

        self->last       = 0;
        self->size       = self->rate;

        /* create a default pointer store with current rate*/
        self->items = calloc(self->size, sizeof(void*));
        if (!self->items){
                log_oom();
                return false;
        }

        return true;
}

/*----------------------------------------------------------------------------*/

testrun_vector *testrun_vector_destroy(testrun_vector * self){

        if (!self)
                return NULL;

        /* DROP the pointer store, ignore any value setting */
        if (self->items){
                free(self->items);
                self->items = NULL;
        }

        /* DROP self */
        if (self){
                free(self);
                self = NULL;
        }

        return NULL;
}

/*----------------------------------------------------------------------------*/

testrun_vector *testrun_vector_terminate(testrun_vector * self){

        if (!self)
                return NULL;

        testrun_vector_clear(self);
        return testrun_vector_destroy(self);
}

/*----------------------------------------------------------------------------*/

testrun_vector *testrun_vector_copy(testrun_vector const * const self){

        if (!self)
                return NULL;

        if (!self->item_free)
                return NULL;

        if (!self->item_copy)
                return NULL;

        testrun_vector *copy = testrun_vector_create(
                self->rate,
                self->item_free,
                self->item_copy);

        if (!copy)
                goto error;

        void *item      = NULL;
        void *item_copy = NULL;

        if (testrun_vector_is_empty(self))
                return copy;

        for (size_t i = 0; i <= self->last; i++){

                item = testrun_vector_get(self, i);
                if (!item)
                        goto error;

                item_copy = self->item_copy(item);
                if (!item_copy)
                        goto error;

                if (!testrun_vector_set(copy, i, item_copy)){
                        self->item_free(item_copy);
                        goto error;
                }

                copy->last = i;
        }

        return copy;
error:
        return testrun_vector_terminate(copy);
}

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

void *testrun_vector_get(
        testrun_vector const * const self,
        size_t pos){

        if (!self || !self->items)
                return NULL;

        if (pos >= self->size)
                return NULL;

        return self->items[pos];

}

/*----------------------------------------------------------------------------*/

bool testrun_vector_set(
        testrun_vector * const self,
        size_t pos,
        void  * const value){

        if (!self || !self->items)
                return false;

        if (pos >= self->size)
                return false;

        if (self->items[pos] != NULL) {
                if (self->item_free)
                        self->item_free(self->items[pos]);
        }

        self->items[pos] = value;

        if (self->last < pos)
                self->last = pos;

        return true;

}

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

int testrun_vector_delete(
        testrun_vector * const self,
        size_t pos){

        if (!self || !self->items)
                return -1;

        void *value = testrun_vector_remove(self, pos);
        if (!value)
                return 0;

        if (self->item_free)
                self->item_free(value);

        return 1;
}

/*----------------------------------------------------------------------------*/

void *testrun_vector_remove(
        testrun_vector * const self,
        size_t pos){

        if (!self || !self->items)
                return NULL;

        if (pos >= self->size)
                return NULL;

        void *value = self->items[pos];
        self->items[pos] = NULL;

        if (self->last == pos){

                if (!testrun_vector_set_new_end(self, pos))
                        log_dev("Failed to set vector end.");

                if (!testrun_vector_auto_adjust(self))
                        log_dev("Failed to auto adjust vector.");

        }

        return value;
}

/*----------------------------------------------------------------------------*/

bool testrun_vector_add(
        testrun_vector * const self,
        void  * const value){

        if (!self || !self->items || !value)
                return false;

        for (size_t i = 0; i < self->last; i++){

                if (!testrun_vector_get(self, i))
                        return testrun_vector_set(self, i, value);

        }

        return testrun_vector_push(self, value);
}


/*----------------------------------------------------------------------------*/

bool testrun_vector_insert(
        testrun_vector * const self,
        size_t pos,
        void  * const value){

        if (!self || !self->items || !value)
                return false;

        if (pos >= self->size)
                return false;

        size_t slot = 0;

        if (pos > self->last) {

                slot = self->last;
                self->last = pos;

                if (!testrun_vector_auto_adjust(self)){
                        self->last = slot;
                        return false;
                }

        } else if (testrun_vector_get(self, pos)){

                self->last++;

                if (!testrun_vector_auto_adjust(self)){
                        self->last--;
                        return false;
                }

                for (slot = self->last; slot > pos; slot--) {

                        if (self->items[slot-1])
                                self->items[slot] = self->items[slot - 1];

                }

        }

        self->items[pos] = value;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_vector_push(
        testrun_vector * const self,
        void  * const value){

        if (!self || !self->items || !value)
                return false;

        if (!testrun_vector_is_empty(self))
                self->last++;

        if (!testrun_vector_auto_adjust(self)){
                self->last--;
                return false;
        }

        self->items[self->last] = value;

        return true;
}

/*----------------------------------------------------------------------------*/

void *testrun_vector_pop(
        testrun_vector * const self){

        if (!self || !self->items)
                return NULL;

        if (testrun_vector_is_empty(self))
                return NULL;

        return testrun_vector_remove(self, self->last);
}

/*
 *      ------------------------------------------------------------------------
 *
 *      ADDITIONAL FUNCTIONS                                            #ADD
 *
 *      Additional usefull external defined functionality.
 *
 *      ------------------------------------------------------------------------
 */

bool testrun_vector_is_empty(
        testrun_vector const * const self){

        if (!self || !self->items)
                return false;

        if (0 < self->last)
                return false;

        if (self->items[0] == NULL)
                return true;

        return false;
}

/*----------------------------------------------------------------------------*/

void *testrun_vector_int64_example_copy(
        void const * const pointer){

        if (!pointer)
                return false;

        void *cpy = calloc(1, sizeof(int64_t));
        if (!cpy){
                log_oom();
                return NULL;
        }

        if (!memcpy(cpy, pointer, sizeof(int64_t))){
                free(cpy);
                return NULL;
        }

        return cpy;
}

/*----------------------------------------------------------------------------*/

void testrun_vector_int64_example_free(
        void * pointer){

        if (pointer)
                free(pointer);
}

/*----------------------------------------------------------------------------*/

void testrun_vector_item_free(
        void * pointer){

        if (pointer)
                free(pointer);
}

/*----------------------------------------------------------------------------*/

bool testrun_vector_dump_function_backtrace(
        testrun_vector const * const self){

        if (!self)
                return false;

        fprintf(stdout, "\n---- VECTOR DUMP ---------------> \n\n");
        fprintf(stdout, "     ----------------------------\n");
        fprintf(stdout, "     HEADER of vector %p\n", self);
        fprintf(stdout, "     ----------------------------\n");
        fprintf(stdout, "     vector->last: %jd \n",     self->last);
        fprintf(stdout, "     vector->size: %jd \n",     self->size);
        fprintf(stdout, "     vector->rate: %jd \n\n",   self->rate);
        fprintf(stdout, "     vector->item_free: %p \n", self->item_free);
        fprintf(stdout, "     vector->item_copy: %p \n", self->item_copy);
        fprintf(stdout, "     vector->items: %p \n\n",   self->items);
        fprintf(stdout, "     ----------------------------\n");
        fprintf(stdout, "     file(function_name)[pointer]\n");
        fprintf(stdout, "     ----------------------------\n");

        for (size_t i = 0; i < self->size; i++) {
                backtrace_symbols_fd(&self->items[i], 1, STDOUT_FILENO);
        }

        fprintf(stdout, "\n<--------------------------------- \n\n");

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_vector_dump(
        FILE *stream, testrun_vector const * const self){

        if (!stream || !self)
                return false;


        fprintf(stream, "\n---- VECTOR DUMP ---------------> \n\n");
        fprintf(stream, "     ----------------------------\n");
        fprintf(stream, "     HEADER of vector %p\n", self);
        fprintf(stream, "     ----------------------------\n");
        fprintf(stream, "     vector->last: %jd \n",     self->last);
        fprintf(stream, "     vector->size: %jd \n",     self->size);
        fprintf(stream, "     vector->rate: %jd \n\n",   self->rate);
        fprintf(stream, "     vector->item_free: %p \n", self->item_free);
        fprintf(stream, "     vector->item_copy: %p \n", self->item_copy);
        fprintf(stream, "     vector->items: %p \n\n",   self->items);
        fprintf(stream, "     ----------------------------\n");
        fprintf(stream, "     vector->items[x] | [pointer]\n");
        fprintf(stream, "     ----------------------------\n");

        for (size_t i = 0; i < self->size; i++) {
                fprintf(stream, "%20jd  | %p \n", i, self->items[i]);
        }

        fprintf(stream, "\n<--------------------------------- \n\n");

        return true;
}

/*----------------------------------------------------------------------------*/
