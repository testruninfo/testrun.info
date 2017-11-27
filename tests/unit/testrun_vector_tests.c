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

        @file           testrun_vector_tests.c
        @author         Markus Toepfer
        @date           2017-11-18

        @ingroup        testrun_lib

        @brief          Unit testing of testrun_vector


        ------------------------------------------------------------------------
*/

#include <limits.h>

#include "../../include/testrun2.h"
#include "../../src/testrun_vector.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                    #HELPER
 *
 *      ------------------------------------------------------------------------
 */

void *dummy_testrun_copy_failure() {
        return NULL;
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

int test_testrun_vector_create(){

        testrun_vector *vector = NULL;

        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,  "... without content");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1, NULL, NULL);
        testrun(vector != NULL,  "... with rate 1");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(2, NULL, NULL);
        testrun(vector != NULL,  "... with rate 2");
        testrun(vector->last      == 0);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 2);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1000, NULL, NULL);
        testrun(vector != NULL,  "... with rate 1000");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1000);
        testrun(vector->rate      == 1000);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1, &free, NULL);
        testrun(vector != NULL,  "... with rate 1 and free");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == free);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1,
            testrun_vector_int64_example_free,
            testrun_vector_int64_example_copy);
        testrun(vector != NULL,  "... with rate 1 int64 int64 example");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == testrun_vector_int64_example_copy);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1,
            NULL,
            testrun_vector_int64_example_copy);
        testrun(vector != NULL,  "... with rate 1 NULL int64 example");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == testrun_vector_int64_example_copy);
        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_free(){

        testrun_vector *vector = NULL;

        void *pointer = NULL;

        vector = testrun_vector_create(0,
            testrun_vector_int64_example_free,
            testrun_vector_int64_example_copy);
        testrun(vector != NULL,  "... with default rate & int64");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == testrun_vector_int64_example_copy);
        for (int i = 0; i < 5; i++){
                pointer = calloc(1, sizeof(int64_t));
                vector->items[i] = pointer;
                vector->last = i;
        }

        // free everything
        testrun_vector_free(vector);

        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,  "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        pointer = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer;
        // free everything (except items)
        testrun_vector_free(vector);
        // free item pointer
        free(pointer);

        // check with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_clear(){

        testrun_vector *vector = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,  "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(true  == testrun_vector_clear(vector), "clear empty");
        testrun(false == testrun_vector_clear(NULL),   "clear NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        // non default init
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,  "... with 100, NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(testrun_vector_clear(vector), "clear empty");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        // clear no value_free
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,  "... no value free");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun(testrun_vector_clear(vector), "clear vector");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);
        testrun(pointer1 != NULL);
        testrun(pointer2 != NULL);
        testrun(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // clear with value_free
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free, NULL);
        testrun(vector != NULL,  "... with value free");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun(testrun_vector_clear(vector), "clear vector");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        // items termination check with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_destroy(){

        testrun_vector *vector = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,  "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL  == testrun_vector_destroy(vector), "empty");
        testrun(NULL  == testrun_vector_destroy(NULL),   "NULL");

        // non default init
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,  "... non default init");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL  == testrun_vector_destroy(vector), "no default");

        // clear no value_free
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,  "... no value free");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun(NULL  == testrun_vector_destroy(vector), "no free");
        testrun(pointer1 != NULL);
        testrun(pointer2 != NULL);
        testrun(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // clear with value_free
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free, NULL);
        testrun(vector != NULL,  "... no value free");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun(NULL  == testrun_vector_destroy(vector), "with free");
        testrun(pointer1 != NULL);
        testrun(pointer2 != NULL);
        testrun(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // items termination check with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_terminate(){

        testrun_vector *vector = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,  "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL  == testrun_vector_terminate(vector), "empty");
        testrun(NULL  == testrun_vector_terminate(NULL),   "NULL");

        // non default init
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,  "... non default init");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL  == testrun_vector_terminate(vector), "no default");

        // no value_free
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,  "... no value free");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun(NULL  == testrun_vector_terminate(vector), "no free");
        testrun(pointer1 != NULL);
        testrun(pointer2 != NULL);
        testrun(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // with value_free
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free, NULL);
        testrun(vector != NULL,  "... no value free");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun(NULL  == testrun_vector_terminate(vector), "with free");

        // items termination check with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_copy(){

        testrun_vector *vector = NULL;
        testrun_vector *copy   = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL == testrun_vector_copy(vector), "no item_copy");
        vector = testrun_vector_terminate(vector);

        // no items contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                testrun_vector_int64_example_copy);
        testrun(vector != NULL,  "... non default init");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == testrun_vector_int64_example_copy);
        copy = testrun_vector_copy(vector);
        testrun(copy != NULL,     "... with NULL");
        testrun(copy->last        == 0);
        testrun(copy->size        == 100);
        testrun(copy->rate        == 100);
        testrun(copy->items       != NULL);
        testrun(copy->item_free   == testrun_vector_int64_example_free);
        testrun(copy->item_copy   == testrun_vector_int64_example_copy);
        vector = testrun_vector_terminate(vector);
        copy   = testrun_vector_terminate(copy);

        // items contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                testrun_vector_int64_example_copy);
        testrun(vector != NULL,  "... non default init");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == testrun_vector_int64_example_copy);
        pointer1  = calloc(1, sizeof(int64_t));
        pointer2  = calloc(1, sizeof(int64_t));
        pointer3  = calloc(1, sizeof(int64_t));
        *(int64_t*)pointer1 = 1;
        *(int64_t*)pointer2 = 2;
        *(int64_t*)pointer3 = 1234567890;
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        copy = testrun_vector_copy(vector);
        testrun(copy != NULL,     "... non default");
        testrun(copy->last        == 2);
        testrun(copy->size        == 100);
        testrun(copy->rate        == 100);
        testrun(copy->items       != NULL);
        testrun(copy->item_free   == testrun_vector_int64_example_free);
        testrun(copy->item_copy   == testrun_vector_int64_example_copy);
        testrun(*(int64_t*)copy->items[0] == 1);
        testrun(*(int64_t*)copy->items[1] == 2);
        testrun(*(int64_t*)copy->items[2] == 1234567890);
        vector = testrun_vector_terminate(vector);
        copy   = testrun_vector_terminate(copy);

        // item contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                testrun_vector_int64_example_copy);
        testrun(vector != NULL,  "... non default init");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == testrun_vector_int64_example_copy);
        pointer1  = calloc(1, sizeof(int64_t));
        *(int64_t*)pointer1 = 1;
        vector->items[0] = pointer1;
        vector->last = 0;
        copy = testrun_vector_copy(vector);
        testrun(copy != NULL,     "... non default");
        testrun(copy->last        == 0);
        testrun(copy->size        == 100);
        testrun(copy->rate        == 100);
        testrun(copy->items       != NULL);
        testrun(copy->item_free   == testrun_vector_int64_example_free);
        testrun(copy->item_copy   == testrun_vector_int64_example_copy);
        testrun(false == testrun_vector_is_empty(vector));
        testrun(false == testrun_vector_is_empty(copy));
        testrun(*(int64_t*)copy->items[0] == 1);
        vector = testrun_vector_terminate(vector);
        copy   = testrun_vector_terminate(copy);

        // item copy failure contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                dummy_testrun_copy_failure);
        testrun(vector != NULL,  "... non default init");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == testrun_vector_int64_example_free);
        testrun(vector->item_copy == dummy_testrun_copy_failure);
        pointer1  = calloc(1, sizeof(int64_t));
        *(int64_t*)pointer1 = 1;
        vector->items[0] = pointer1;
        vector->last = 0;
        copy = testrun_vector_copy(vector);
        testrun(copy == NULL,     "... item copy failure");
        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_vector_is_empty(){

        testrun_vector *vector = NULL;
        void *pointer = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun(vector != NULL,  "... with NULL");
        testrun(vector->last      == 0);
        testrun(testrun_vector_is_empty(vector));

        // vector end not 0
        vector->last = 1;
        testrun(!testrun_vector_is_empty(vector));

        // vector items at NULL not empty
        vector->last = 0;
        pointer = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer;
        testrun(!testrun_vector_is_empty(vector));
        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_int64_example_copy(){

        void *pointer1 = NULL;
        void *pointer2 = NULL;

        testrun(!testrun_vector_int64_example_copy(NULL), "NULL");

        pointer1  = calloc(1, sizeof(int64_t));
        *(int64_t*) pointer1 = 1;
        pointer2  = testrun_vector_int64_example_copy(pointer1);
        testrun(pointer2  != NULL);
        testrun( *(int64_t*)pointer2 == 1);
        // check idependence with free
        free(pointer1);
        free(pointer2);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_int64_example_free(){

        void *pointer1 = NULL;
        void *pointer2 = NULL;

        pointer1  = calloc(1, sizeof(int64_t));
        pointer2  = calloc(1, sizeof(int64_t));

        testrun_vector_int64_example_free(pointer1);
        testrun_vector_int64_example_free(pointer2);

        // check with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_item_free(){

        void *pointer1 = NULL;
        void *pointer2 = NULL;

        pointer1  = calloc(1, sizeof(int64_t));
        pointer2  = calloc(1, sizeof(int64_t));

        testrun_vector_item_free(pointer1);
        testrun_vector_item_free(pointer2);

        // check with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_resize(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        // default init
        vector = testrun_vector_create(1, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_resize(NULL,     0));
        testrun(false == testrun_vector_resize(vector,   0));
        testrun(false == testrun_vector_resize(NULL,     10));
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(true == testrun_vector_resize(vector, 2), "grow min");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(true == testrun_vector_resize(vector, 10), "grow");
        testrun(vector->last      == 0);
        testrun(vector->size      == 9);
        testrun(vector->rate      == 1);
        // verify allocated space (don't set end, so resize will loose slot)
        vector->items[9] = pointer;
        testrun(true == testrun_vector_resize(vector, 5), "shrink");
        testrun(vector->last      == 0);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 1);
        vector->last = 4;
        testrun(false == testrun_vector_resize(vector, 4), "below end");
        testrun(vector->last      == 4);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 1);
        testrun(true == testrun_vector_resize(vector, 100), "expand");
        testrun(vector->last      == 4);
        testrun(vector->size      == 99);
        testrun(vector->rate      == 1);
        // verify allocated space (don't set end, so resize will loose slot)
        vector->items[99] = pointer;
        testrun(true == testrun_vector_resize(vector, 5), "to end");
        testrun(vector->last      == 4);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 1);
        // verify allocated space
        vector->items[0] = pointer;
        vector->items[1] = pointer;
        vector->items[2] = pointer;
        vector->items[3] = pointer;
        vector->items[4] = pointer;

        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_auto_adjust(){

        testrun_vector *vector = NULL;

        // rate 1
        vector = testrun_vector_create(1, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_auto_adjust(NULL));
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        vector->last = 1;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 1);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 1);
        vector->last = 2;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 2);
        testrun(vector->size      == 3);
        testrun(vector->rate      == 1);
        vector->last = 1;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 1);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 1);
        vector->last = 0;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 0);
        testrun(vector->size      == 1);
        testrun(vector->rate      == 1);
        vector = testrun_vector_terminate(vector);


        // rate 5
        vector = testrun_vector_create(5, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 0);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        vector->last = 1;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 1);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        vector->last = 2;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 2);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        vector->last = 3;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 3);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        vector->last = 4;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 4);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        vector->last = 5;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 5);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 6;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 6);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 7;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 7);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 8;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 8);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 9;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 9);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 10;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 10);
        testrun(vector->size      == 15);
        testrun(vector->rate      == 5);
        vector->last = 9;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 9);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 5;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 5);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 5);
        vector->last = 4;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 4);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        vector = testrun_vector_terminate(vector);


        // rate 10
        vector = testrun_vector_create(10, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 10);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 0);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 10);
        vector->last = 9;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 9);
        testrun(vector->size      == 10);
        testrun(vector->rate      == 10);
        vector->last = 10;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 10);
        testrun(vector->size      == 20);
        testrun(vector->rate      == 10);
        vector->last = 19;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 19);
        testrun(vector->size      == 20);
        testrun(vector->rate      == 10);
        vector->last = 20;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 20);
        testrun(vector->size      == 30);
        testrun(vector->rate      == 10);
        vector->last = 29;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 29);
        testrun(vector->size      == 30);
        testrun(vector->rate      == 10);
        vector->last = 30;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 30);
        testrun(vector->size      == 40);
        testrun(vector->rate      == 10);
        vector->last = 21;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 21);
        testrun(vector->size      == 30);
        testrun(vector->rate      == 10);
        vector->last = 20;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 20);
        testrun(vector->size      == 30);
        testrun(vector->rate      == 10);
        vector->last = 19;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 19);
        testrun(vector->size      == 20);
        testrun(vector->rate      == 10);
        vector = testrun_vector_terminate(vector);

        // rate 2
        vector = testrun_vector_create(2, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 2);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 0);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 2);
        vector->last = 1;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 1);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 2);
        vector->last = 2;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 2);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 2);
        vector->last = 3;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 3);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 2);
        vector->last = 4;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 4);
        testrun(vector->size      == 6);
        testrun(vector->rate      == 2);
        vector->last = 5;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 5);
        testrun(vector->size      == 6);
        testrun(vector->rate      == 2);
        vector->last = 6;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 6);
        testrun(vector->size      == 8);
        testrun(vector->rate      == 2);
        vector->last = 5;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 5);
        testrun(vector->size      == 6);
        testrun(vector->rate      == 2);
        vector->last = 4;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 4);
        testrun(vector->size      == 6);
        testrun(vector->rate      == 2);
        vector->last = 3;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 3);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 2);
        vector->last = 2;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 2);
        testrun(vector->size      == 4);
        testrun(vector->rate      == 2);
        vector->last = 1;
        testrun(true  == testrun_vector_auto_adjust(vector));
        testrun(vector->last      == 1);
        testrun(vector->size      == 2);
        testrun(vector->rate      == 2);
        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_set_new_end(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        // rate 1
        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_set_new_end(NULL,   0));
        testrun(false == testrun_vector_set_new_end(NULL,   1));
        testrun(true  == testrun_vector_set_new_end(vector, 0));
        testrun(vector->last == 0);

        testrun(true == testrun_vector_set_new_end(vector, 1),"no item");
        testrun(vector->last == 0);
        testrun(true == testrun_vector_set_new_end(vector, 2),"no item");
        testrun(vector->last == 0);
        testrun(true == testrun_vector_set_new_end(vector, 3),"no item");
        testrun(vector->last == 0);

        vector->items[1] = pointer;
        vector->last = 1;
        testrun(false == testrun_vector_set_new_end(vector, 0),"< end");
        testrun(vector->last == 1);
        testrun(true == testrun_vector_set_new_end(vector, 1), "= end");
        testrun(vector->last == 1);
        testrun(true == testrun_vector_set_new_end(vector, 2), "> end");
        testrun(vector->last == 1);
        testrun(true == testrun_vector_set_new_end(vector, 3), "> end");
        testrun(vector->last == 1);
        testrun(true == testrun_vector_set_new_end(vector, 4), "> end");
        testrun(vector->last == 1);

        vector->items[3] = pointer;
        vector->last = 5;
        testrun(false == testrun_vector_set_new_end(vector, 4),"< end");
        testrun(vector->last == 5);
        testrun(true == testrun_vector_set_new_end(vector, 5), "= end");
        testrun(vector->last == 3);
        vector->last = 5;
        testrun(true == testrun_vector_set_new_end(vector, 6), "> end");
        testrun(vector->last == 3);
        vector->last = 10;
        vector->items[6] = pointer;
        testrun(true == testrun_vector_set_new_end(vector, 23), "> end");
        testrun(vector->last == 6);
        vector->items[6] = NULL;
        vector->items[3] = NULL;
        vector->items[1] = NULL;
        testrun(true == testrun_vector_set_new_end(vector, 23), "> end");
        testrun(vector->last == 0);

        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_get(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL == testrun_vector_get(NULL,   0));
        testrun(NULL == testrun_vector_get(NULL,   1));
        testrun(NULL == testrun_vector_get(vector, 0));
        testrun(vector->last == 0);
        testrun(testrun_vector_set(vector, 1, pointer1));
        testrun(testrun_vector_set(vector, 2, pointer2));
        testrun(testrun_vector_set(vector, 3, pointer3));
        testrun(testrun_vector_set(vector, 6, pointer1));
        testrun(testrun_vector_set(vector, 7, pointer2));
        testrun(testrun_vector_set(vector, 8, pointer3));
        testrun(testrun_vector_set(vector, vector->size - 1, pointer3));
        testrun(NULL == testrun_vector_get(vector, 0));
        result = testrun_vector_get(vector, 1);
        testrun(result == pointer1, "content check");
        result = testrun_vector_get(vector, 2);
        testrun(result == pointer2, "content check");
        result = testrun_vector_get(vector, 3);
        testrun(result == pointer3, "content check");
        testrun(NULL == testrun_vector_get(vector, 4));
        testrun(NULL == testrun_vector_get(vector, 5));
        result = testrun_vector_get(vector, 6);
        testrun(result == pointer1, "content check");
        result = testrun_vector_get(vector, 7);
        testrun(result == pointer2, "content check");
        result = testrun_vector_get(vector, 8);
        testrun(result == pointer3, "content check");
        result = testrun_vector_get(vector, 99);
        testrun(result == pointer3, "content check MAX");
        testrun(NULL == testrun_vector_get(vector, 100), "> MAX");
        testrun(NULL == testrun_vector_get(vector, 101), "> MAX");
        vector = testrun_vector_terminate(vector);

        free(pointer1);
        free(pointer2);
        free(pointer3);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_set(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_set(NULL,   0,    NULL));
        testrun(false == testrun_vector_set(NULL,   0,    pointer1));
        testrun(true  == testrun_vector_set(vector, 0,    NULL));
        testrun(vector->last == 0);
        // set to NULL
        testrun(testrun_vector_set(vector, 0, pointer1));
        testrun(vector->last == 0);
        testrun(testrun_vector_set(vector, 1, pointer2));
        testrun(vector->last == 1);
        testrun(testrun_vector_set(vector, 2, pointer3));
        testrun(vector->last == 2);
        testrun(testrun_vector_set(vector, 3, pointer4));
        testrun(vector->last == 3);
        result = testrun_vector_get(vector, 0);
        testrun(result == pointer1, "content check");
        result = testrun_vector_get(vector, 1);
        testrun(result == pointer2, "content check");
        result = testrun_vector_get(vector, 2);
        testrun(result == pointer3, "content check");
        result = testrun_vector_get(vector, 3);
        testrun(result == pointer4, "content check");
        // set to used (no item_free configured)
        testrun(testrun_vector_set(vector, 0, pointer3));
        testrun(testrun_vector_set(vector, 1, pointer1));
        testrun(testrun_vector_set(vector, 2, pointer2));
        result = testrun_vector_get(vector, 0);
        testrun(result == pointer3, "content check");
        result = testrun_vector_get(vector, 1);
        testrun(result == pointer1, "content check");
        result = testrun_vector_get(vector, 2);
        testrun(result == pointer2, "content check");
        result = testrun_vector_get(vector, 3);
        testrun(result == pointer4, "content check");
        // prepare set to used (item_free configured)
        testrun(testrun_vector_set(vector, 0, pointer1));
        testrun(testrun_vector_set(vector, 1, pointer2));
        testrun(testrun_vector_set(vector, 2, NULL));
        testrun(testrun_vector_set(vector, 3, NULL));
        vector->item_free = free;
        // check state
        result = testrun_vector_get(vector, 0);
        testrun(result == pointer1, "content check");
        result = testrun_vector_get(vector, 1);
        testrun(result == pointer2, "content check");
        testrun(NULL == testrun_vector_get(vector, 2));
        testrun(NULL == testrun_vector_get(vector, 3));
        // set to used (item_free configured)
        testrun(testrun_vector_set(vector, 0, pointer3));
        testrun(testrun_vector_set(vector, 1, pointer4));
        result = testrun_vector_get(vector, 0);
        testrun(result == pointer3, "content check");
        result = testrun_vector_get(vector, 1);
        testrun(result == pointer4, "content check");
        // set to NULL (item_free configured)
        testrun(testrun_vector_set(vector, 0, NULL));
        testrun(testrun_vector_set(vector, 1, NULL));
        // check result
        testrun(NULL == testrun_vector_get(vector, 0));
        testrun(NULL == testrun_vector_get(vector, 1));
        testrun(NULL == testrun_vector_get(vector, 2));
        testrun(NULL == testrun_vector_get(vector, 3));

        // set behind end
        pointer1 = calloc(1, sizeof(void*));
        testrun(false == testrun_vector_set(vector, 100, pointer1));
        // set last
        testrun(true == testrun_vector_set(vector, 99, pointer1));
        testrun(vector->last == 99);
        vector = testrun_vector_terminate(vector);

        // check item free with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_delete(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(-1 == testrun_vector_delete(NULL,   0));
        testrun(-1 == testrun_vector_delete(NULL,   0));
        testrun( 0 == testrun_vector_delete(vector, 0),   "no item");
        testrun( 0 == testrun_vector_delete(vector, 1),   "no item");
        testrun( 0 == testrun_vector_delete(vector, 2),   "no item");
        testrun( 0 == testrun_vector_delete(vector, 3),   "no item");
        testrun( 0 == testrun_vector_delete(vector, 99),  "max item");
        testrun( 0 == testrun_vector_delete(vector, 100), "> max item");
        testrun(vector->last == 0);
        testrun(testrun_vector_set(vector, 0, pointer1));
        testrun(testrun_vector_set(vector, 1, pointer2));
        testrun(testrun_vector_set(vector, 4, pointer3));
        testrun(testrun_vector_set(vector, 5, pointer4));
        testrun( 5    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 5),"delete last");
        testrun( NULL == testrun_vector_get(   vector, 5));
        testrun( 4    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 4),"delete last");
        testrun( NULL == testrun_vector_get(   vector, 4));
        testrun( 1    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 0),"non last");
        testrun( NULL == testrun_vector_get(   vector, 0));
        testrun( 1    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 1),"delete last");
        testrun( NULL == testrun_vector_get(   vector, 1));
        testrun( 0    == vector->last);
        // check with free
        vector->item_free = free;
        testrun(testrun_vector_set(vector, 5, pointer1));
        testrun(testrun_vector_set(vector, 6, pointer2));
        testrun(testrun_vector_set(vector, 7, pointer3));
        testrun(testrun_vector_set(vector, 8, pointer4));
        testrun( 8    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 7),"delete");
        testrun( NULL == testrun_vector_get(   vector, 7));
        testrun( 8    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 6),"delete");
        testrun( NULL == testrun_vector_get(   vector, 6));
        testrun( 8    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 8),"last");
        testrun( NULL == testrun_vector_get(   vector, 8));
        testrun( 5    == vector->last);
        testrun( 1    == testrun_vector_delete(vector, 5),"last");
        testrun( NULL == testrun_vector_get(   vector, 5));
        testrun( 0    == vector->last);
        vector = testrun_vector_terminate(vector);

        // check item free with valgrind run
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_remove(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL == testrun_vector_remove(NULL,   0));
        testrun(NULL == testrun_vector_remove(NULL,   0));
        testrun(NULL == testrun_vector_remove(vector, 0),   "no item");
        testrun(NULL == testrun_vector_remove(vector, 1),   "no item");
        testrun(NULL == testrun_vector_remove(vector, 2),   "no item");
        testrun(NULL == testrun_vector_remove(vector, 3),   "no item");
        testrun(NULL == testrun_vector_remove(vector, 99),  "max item");
        testrun(NULL == testrun_vector_remove(vector, 100), "> max ");
        testrun(vector->last == 0);
        testrun(testrun_vector_set(vector, 0, pointer1));
        testrun(testrun_vector_set(vector, 1, pointer2));
        testrun(testrun_vector_set(vector, 4, pointer3));
        testrun(testrun_vector_set(vector, 5, pointer4));
        testrun( 5        == vector->last);
        testrun( pointer4 == testrun_vector_remove(vector, 5),"last");
        testrun( NULL     == testrun_vector_get(   vector, 5));
        testrun( 4        == vector->last);
        testrun( pointer3 == testrun_vector_remove(vector, 4),"last");
        testrun( NULL     == testrun_vector_get(   vector, 4));
        testrun( 1        == vector->last);
        testrun( pointer1 == testrun_vector_remove(vector, 0),"no last");
        testrun( NULL     == testrun_vector_get(   vector, 0));
        testrun( 1        == vector->last);
        testrun( pointer2 == testrun_vector_remove(vector, 1),"last");
        testrun( NULL     == testrun_vector_get(   vector, 1));
        testrun( 0        == vector->last);
        // check with free
        vector->item_free = free;
        testrun(testrun_vector_set(vector, 5, pointer1));
        testrun(testrun_vector_set(vector, 6, pointer2));
        testrun(testrun_vector_set(vector, 7, pointer3));
        testrun(testrun_vector_set(vector, 8, pointer4));
        testrun( 8        == vector->last);
        testrun( pointer3 == testrun_vector_remove(vector, 7),"delete");
        testrun( NULL     == testrun_vector_get(   vector, 7));
        testrun( 8        == vector->last);
        testrun( pointer2 == testrun_vector_remove(vector, 6),"delete");
        testrun( NULL     == testrun_vector_get(   vector, 6));
        testrun( 8        == vector->last);
        testrun( pointer4 == testrun_vector_remove(vector, 8),"last");
        testrun( NULL     == testrun_vector_get(   vector, 8));
        testrun( 5        == vector->last);
        testrun( pointer1 == testrun_vector_remove(vector, 5),"last");
        testrun( NULL     == testrun_vector_get(   vector, 5));
        testrun( 0        == vector->last);
        vector = testrun_vector_terminate(vector);

        // pointers are removed
        free(pointer1);
        free(pointer2);
        free(pointer3);
        free(pointer4);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_add(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *pointer5 = calloc(1, sizeof(void*));
        void *pointer6 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(100, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 100);
        testrun(vector->rate      == 100);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_add(NULL,   NULL));
        testrun(false == testrun_vector_add(NULL,   pointer1));
        testrun(false == testrun_vector_add(vector, NULL));
        testrun(vector->last == 0);
        // add to empty
        testrun(true     == testrun_vector_add(vector, pointer1));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun( 0       == vector->last);
        // add to non empty
        testrun(true     == testrun_vector_add(vector, pointer2));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun( 1       == vector->last);
        testrun(testrun_vector_set(vector, 4, pointer5));
        testrun( 4       == vector->last);
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(NULL     == testrun_vector_get(vector, 2));
        testrun(NULL     == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        // check slot fill
        testrun(true     == testrun_vector_add(vector, pointer3));
        testrun(true     == testrun_vector_add(vector, pointer4));
        // check push
        testrun(true     == testrun_vector_add(vector, pointer6));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(pointer6 == testrun_vector_get(vector, 5));
        testrun(NULL     == testrun_vector_get(vector, 6));
        vector->item_free = free;
        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_insert(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *pointer5 = calloc(1, sizeof(void*));
        void *pointer6 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(5, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 5);
        testrun(vector->rate      == 5);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_insert(NULL,   0, NULL));
        testrun(false == testrun_vector_insert(NULL,   0, pointer1));
        testrun(false == testrun_vector_insert(vector, 0, NULL));
        testrun(vector->last == 0);
        // insert to empty
        testrun(true     == testrun_vector_insert(vector, 0, pointer1));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun( 0       == vector->last);
        // insert to used slot
        testrun(true     == testrun_vector_insert(vector, 0, pointer2));
        testrun(pointer2 == testrun_vector_get(vector, 0));
        testrun(pointer1 == testrun_vector_get(vector, 1));
        testrun( 1       == vector->last);
        // insert to used slot
        testrun(true     == testrun_vector_insert(vector, 0, pointer3));
        testrun(pointer3 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer1 == testrun_vector_get(vector, 2));
        testrun( 2       == vector->last);
        // insert > last
        testrun(true     == testrun_vector_insert(vector, 4, pointer4));
        testrun(pointer3 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer1 == testrun_vector_get(vector, 2));
        testrun(NULL     == testrun_vector_get(vector, 3));
        testrun(pointer4 == testrun_vector_get(vector, 4));
        testrun( 4       == vector->last);
        // insert before trigger adjust
        testrun(vector->size == 5);
        testrun(vector->rate == 5);
        testrun(true     == testrun_vector_insert(vector, 4, pointer5));
        testrun(vector->size == 10);
        testrun(vector->rate == 5);
        testrun(pointer3 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer1 == testrun_vector_get(vector, 2));
        testrun(NULL     == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(pointer4 == testrun_vector_get(vector, 5));
        testrun( 5       == vector->last);
        // insert last possible
        testrun(true     == testrun_vector_insert(vector, 9, pointer6));
        testrun(vector->size == 10);
        testrun(vector->rate == 5);
        testrun(pointer3 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer1 == testrun_vector_get(vector, 2));
        testrun(NULL     == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(pointer4 == testrun_vector_get(vector, 5));
        testrun(NULL     == testrun_vector_get(vector, 6));
        testrun(NULL     == testrun_vector_get(vector, 7));
        testrun(NULL     == testrun_vector_get(vector, 8));
        testrun(pointer6 == testrun_vector_get(vector, 9));
        testrun(NULL     == testrun_vector_get(vector,10));
        testrun(NULL     == testrun_vector_get(vector,11));
        testrun(NULL     == testrun_vector_get(vector,12));
        testrun(NULL     == testrun_vector_get(vector,13));
        testrun(NULL     == testrun_vector_get(vector,14));
        // try to insert beyond slot size
        testrun(vector->size == 10);
        testrun(false    == testrun_vector_insert(vector, 10, pointer6));
        testrun(false    == testrun_vector_insert(vector, 11, pointer1));
        testrun(false    == testrun_vector_insert(vector, 20, pointer2));
        testrun(false    == testrun_vector_insert(vector, 44, pointer3));
        vector->item_free = free;
        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_push(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *pointer5 = calloc(1, sizeof(void*));
        void *pointer6 = calloc(1, sizeof(void*));
        void *pointer7 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(3, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 3);
        testrun(vector->rate      == 3);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(false == testrun_vector_push(NULL,   NULL));
        testrun(false == testrun_vector_push(NULL,   pointer1));
        testrun(false == testrun_vector_push(vector, NULL));
        testrun(vector->last == 0);
        // push to empty
        testrun(true     == testrun_vector_push(vector, pointer1));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun( 0       == vector->last);
        // push to used
        testrun(true     == testrun_vector_push(vector, pointer2));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun( 1       == vector->last);
        // push to used
        testrun(true     == testrun_vector_push(vector, pointer3));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun( 2       == vector->last);
        // push to used and grow
        testrun(vector->last == 2);
        testrun(vector->size == 3);
        testrun(vector->rate == 3);
        testrun(true     == testrun_vector_push(vector, pointer4));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(NULL     == testrun_vector_get(vector, 4));
        testrun(NULL     == testrun_vector_get(vector, 5));
        testrun(vector->last == 3);
        testrun(vector->size == 6);
        testrun(vector->rate == 3);
        // push to used
        testrun(true     == testrun_vector_push(vector, pointer5));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(NULL     == testrun_vector_get(vector, 5));
        // push to used
        testrun(true     == testrun_vector_push(vector, pointer6));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(pointer6 == testrun_vector_get(vector, 5));
        // push to used and grow
        testrun(vector->last == 5);
        testrun(vector->size == 6);
        testrun(vector->rate == 3);
        testrun(true     == testrun_vector_push(vector, pointer7));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(pointer6 == testrun_vector_get(vector, 5));
        testrun(pointer7 == testrun_vector_get(vector, 6));
        testrun(vector->last == 6);
        testrun(vector->size == 9);
        testrun(vector->rate == 3);
        vector->item_free = free;
        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_pop(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *pointer5 = calloc(1, sizeof(void*));
        void *pointer6 = calloc(1, sizeof(void*));
        void *pointer7 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(3, NULL, NULL);
        testrun(vector != NULL,   "... with NULL");
        testrun(vector->last      == 0);
        testrun(vector->size      == 3);
        testrun(vector->rate      == 3);
        testrun(vector->items     != NULL);
        testrun(vector->item_free == NULL);
        testrun(vector->item_copy == NULL);
        testrun(NULL  == testrun_vector_pop(NULL));

        // pop from empty
        testrun(!testrun_vector_pop(vector));
        testrun(vector->last == 0);

        // pop non empty to empty
        testrun(testrun_vector_push(vector, pointer1));
        testrun(!testrun_vector_is_empty(vector));
        testrun(testrun_vector_pop(vector) == pointer1);
        testrun(testrun_vector_is_empty(vector));
        testrun(vector->last == 0);

        // pop from non empty
        testrun(testrun_vector_push(vector, pointer1));
        testrun(testrun_vector_push(vector, pointer2));
        testrun(testrun_vector_push(vector, pointer3));
        testrun(testrun_vector_push(vector, pointer4));
        testrun(testrun_vector_push(vector, pointer5));
        testrun(testrun_vector_push(vector, pointer6));
        testrun(testrun_vector_push(vector, pointer7));
        testrun(vector->last == 6);
        testrun(vector->size == 9);
        testrun(vector->rate == 3);

        // pop & shrink
        testrun(testrun_vector_pop(vector) == pointer7);
        testrun(vector->last == 5);
        testrun(vector->size == 6);
        testrun(vector->rate == 3);

        testrun(testrun_vector_pop(vector) == pointer6);
        testrun(vector->last == 4);
        testrun(vector->size == 6);
        testrun(vector->rate == 3);

        testrun(testrun_vector_pop(vector) == pointer5);
        testrun(vector->last == 3);
        testrun(vector->size == 6);
        testrun(vector->rate == 3);

        // pop & shrink
        testrun(testrun_vector_pop(vector) == pointer4);
        testrun(vector->last == 2);
        testrun(vector->size == 3);
        testrun(vector->rate == 3);

        testrun(testrun_vector_pop(vector) == pointer3);
        testrun(vector->last == 1);
        testrun(vector->size == 3);
        testrun(vector->rate == 3);

        testrun(testrun_vector_pop(vector) == pointer2);
        testrun(!testrun_vector_is_empty(vector));
        testrun(vector->last == 0);
        testrun(vector->size == 3);
        testrun(vector->rate == 3);

        testrun(testrun_vector_pop(vector) == pointer1);
        testrun(testrun_vector_is_empty(vector));
        testrun(vector->last == 0);
        testrun(vector->size == 3);
        testrun(vector->rate == 3);

        vector = testrun_vector_terminate(vector);

        // pointers are poped
        free(pointer1);
        free(pointer2);
        free(pointer3);
        free(pointer4);
        free(pointer5);
        free(pointer6);
        free(pointer7);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_dump_function_backtrace(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        vector = testrun_vector_create(20, NULL, NULL);
        testrun(testrun_vector_set(vector, 2, test_testrun_vector_set));
        testrun(testrun_vector_push(vector, test_testrun_vector_get));
        testrun(testrun_vector_push(vector, test_testrun_vector_add));
        testrun(testrun_vector_push(vector, test_testrun_vector_pop));
        testrun(testrun_vector_set(vector, 10, test_testrun_vector_add));
        testrun(testrun_vector_set(vector, 11, test_testrun_vector_add));
        testrun(testrun_vector_push(vector, pointer));

        testrun(!testrun_vector_dump_function_backtrace(NULL));
        testrun(testrun_vector_dump_function_backtrace(vector));
        vector = testrun_vector_terminate(vector);

        free(pointer);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_dump(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        vector = testrun_vector_create(20, NULL, NULL);
        testrun(testrun_vector_set(vector, 2, test_testrun_vector_set));
        testrun(testrun_vector_push(vector, test_testrun_vector_get));
        testrun(testrun_vector_push(vector, test_testrun_vector_add));
        testrun(testrun_vector_push(vector, test_testrun_vector_pop));
        testrun(testrun_vector_set(vector, 10, test_testrun_vector_add));
        testrun(testrun_vector_set(vector, 11, test_testrun_vector_add));
        testrun(testrun_vector_push(vector, pointer));

        testrun(!testrun_vector_dump(NULL, vector));
        testrun(!testrun_vector_dump(stdout, NULL));
        testrun(testrun_vector_dump(stdout, vector));
        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_testrun_header_example(){

        /*
         *      This is not prefixed with test_ as it is not a
         *      unit test.
         *
         */

        testrun_vector *vector = NULL;

        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *pointer4 = calloc(1, sizeof(void*));
        void *pointer5 = calloc(1, sizeof(void*));
        void *pointer6 = calloc(1, sizeof(void*));
        void *pointer7 = calloc(1, sizeof(void*));

        vector = testrun_vector_create(10, free, NULL);
        testrun(testrun_vector_push(vector, pointer1));
        testrun(testrun_vector_push(vector, pointer2));
        testrun(testrun_vector_set(vector, 4, pointer6));
        testrun(testrun_vector_push(vector, pointer7));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(NULL     == testrun_vector_get(vector, 2));
        testrun(NULL     == testrun_vector_get(vector, 3));
        testrun(pointer6 == testrun_vector_get(vector, 4));
        testrun(pointer7 == testrun_vector_get(vector, 5));
        testrun(NULL     == testrun_vector_get(vector, 6));
        testrun(NULL     == testrun_vector_get(vector, 7));
        testrun(NULL     == testrun_vector_get(vector, 8));
        testrun(NULL     == testrun_vector_get(vector, 9));

        testrun(testrun_vector_add(vector, pointer3));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(NULL     == testrun_vector_get(vector, 3));
        testrun(pointer6 == testrun_vector_get(vector, 4));
        testrun(pointer7 == testrun_vector_get(vector, 5));
        testrun(NULL     == testrun_vector_get(vector, 6));
        testrun(NULL     == testrun_vector_get(vector, 7));
        testrun(NULL     == testrun_vector_get(vector, 8));
        testrun(NULL     == testrun_vector_get(vector, 9));

        testrun(testrun_vector_insert(vector, 3, pointer4));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(pointer6 == testrun_vector_get(vector, 4));
        testrun(pointer7 == testrun_vector_get(vector, 5));
        testrun(NULL     == testrun_vector_get(vector, 6));
        testrun(NULL     == testrun_vector_get(vector, 7));
        testrun(NULL     == testrun_vector_get(vector, 8));
        testrun(NULL     == testrun_vector_get(vector, 9));

        testrun(testrun_vector_insert(vector, 4, pointer5));
        testrun(pointer1 == testrun_vector_get(vector, 0));
        testrun(pointer2 == testrun_vector_get(vector, 1));
        testrun(pointer3 == testrun_vector_get(vector, 2));
        testrun(pointer4 == testrun_vector_get(vector, 3));
        testrun(pointer5 == testrun_vector_get(vector, 4));
        testrun(pointer6 == testrun_vector_get(vector, 5));
        testrun(pointer7 == testrun_vector_get(vector, 6));
        testrun(NULL     == testrun_vector_get(vector, 7));
        testrun(NULL     == testrun_vector_get(vector, 8));
        testrun(NULL     == testrun_vector_get(vector, 9));

        vector = testrun_vector_terminate(vector);

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 *
 *      Cluster all tests in a way to be run and executed one after the other
 *      that is the same way the setup SHOULD be done for assert based testing.
 *
 *      YOU MAY RUN THIS CLUSTER BY CHANGING testrun_run(???); TO
 *
 *              testrun_run(cluster_tests_non_configurable);
 *
 *      ------------------------------------------------------------------------
 */
int cluster_tests_non_configurable() {

        testrun_init();

        testrun_test(test_testrun_vector_create);
        testrun_test(test_testrun_vector_free);
        testrun_test(test_testrun_vector_clear);
        testrun_test(test_testrun_vector_destroy);
        testrun_test(test_testrun_vector_terminate);
        testrun_test(test_testrun_vector_copy);

        testrun_test(test_testrun_vector_is_empty);
        testrun_test(test_testrun_vector_int64_example_copy);
        testrun_test(test_testrun_vector_int64_example_free);
        testrun_test(test_testrun_vector_item_free);

        testrun_test(test_testrun_vector_resize);
        testrun_test(test_testrun_vector_auto_adjust);
        testrun_test(test_testrun_vector_set_new_end);

        testrun_test(test_testrun_vector_get);
        testrun_test(test_testrun_vector_set);
        testrun_test(test_testrun_vector_delete);
        testrun_test(test_testrun_vector_remove);
        testrun_test(test_testrun_vector_add);
        testrun_test(test_testrun_vector_insert);

        testrun_test(test_testrun_vector_push);
        testrun_test(test_testrun_vector_pop);

        testrun_test(test_testrun_vector_dump_function_backtrace);
        testrun_test(test_testrun_vector_dump);

        testrun_test(check_testrun_header_example);

        return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TESTS CONFIGURABLE
 *
 *      Cluster the same test series as above in a way to be configurable for
 *      the type of testrun (parallel or sequential).
 *
 *      ------------------------------------------------------------------------
 */

int64_t cluster_tests_configurable(
        int(*tests[])(),
        size_t slot,
        size_t max){

        testrun_init();

        testrun_add(test_testrun_vector_create);
        testrun_add(test_testrun_vector_free);
        testrun_add(test_testrun_vector_clear);
        testrun_add(test_testrun_vector_destroy);
        testrun_add(test_testrun_vector_terminate);
        testrun_add(test_testrun_vector_copy);
        testrun_add(test_testrun_vector_is_empty);
        testrun_add(test_testrun_vector_int64_example_copy);
        testrun_add(test_testrun_vector_int64_example_free);
        testrun_add(test_testrun_vector_item_free);
        testrun_add(test_testrun_vector_resize);
        testrun_add(test_testrun_vector_auto_adjust);
        testrun_add(test_testrun_vector_set_new_end);
        testrun_add(test_testrun_vector_get);
        testrun_add(test_testrun_vector_set);
        testrun_add(test_testrun_vector_delete);
        testrun_add(test_testrun_vector_remove);
        testrun_add(test_testrun_vector_add);
        testrun_add(test_testrun_vector_insert);
        testrun_add(test_testrun_vector_push);
        testrun_add(test_testrun_vector_pop);
        testrun_add(test_testrun_vector_dump_function_backtrace);
        testrun_add(test_testrun_vector_dump);
        testrun_add(check_testrun_header_example);


        return testrun_counter;
}


/*
 *      ------------------------------------------------------------------------
 *
 *      TESTS CONFIGURATION
 *
 *      Implementation of the required functions for parallel and
 *      sequential testcase configuration of the testrun2.h header.
 *
 *      ------------------------------------------------------------------------
 */

bool testrun_configure_parallel(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max){

        int r = 0;

        if (!testcases || !start || !max)
                return false;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests_configurable);

        return (r < 0)? false : true;
}

/*----------------------------------------------------------------------------*/

bool testrun_configure_sequential(
        int (*testcases[])(),
        size_t *const start,
        size_t const * const max){

        int r = 0;

        if (!testcases || !start || !max)
                return false;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests_configurable);

        return (r < 0)? false : true;

}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 *
 *      EXECUTION sequence for the test series:
 *
 *      OPTIONS
 *
 *      (1) run the testconfiguration in parallel and sequential:
 *
 *              testrun_run_tests(25,25,false);
 *
 *      (2) run the testconfiguration in parallel only:
 *
 *              testrun_run_tests(25,0,false);
 *
 *      (3) run the testconfiguration in sequential only:
 *
 *              testrun_run_tests(0,25,false);
 *
 *      (4) run the testconfiguration in sequential only, break_on_first_error:
 *
 *              testrun_run_tests(0,25,true);
 *
 *      NOTE the last execution sequence (4) is the same as running the test
 *      using the function cluster_tests_non_configurable.
 *
 *      YOU MAY RUN THIS EXECUTION SEQUENCE BY CHANGING testrun_run(???); TO
 *
 *              testrun_run(run_tests);
 *
 *      ------------------------------------------------------------------------
 */
int64_t run_tests() {

        return testrun_run_tests(25,0,true);
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST ACTIVATED                                                  #ACTIV
 *
 *      ------------------------------------------------------------------------
 */

//testrun_run(run_tests);
testrun_run(cluster_tests_non_configurable);

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST RESULTS AND NOTES                                         #RESULTS
 *
 *      ------------------------------------------------------------------------
 *
 *      NOTE example testrun with 25 enabled test cases in parallel and
 *      sequential shows a slightly better runtime in sequential run.
 *
 *      testrun_run(run_tests); with sequential testrun_run_tests(0,25,true);
 *      results around 150 ms
 *
 *      testrun_run(run_tests); with sequential testrun_run_tests(0,25,false);
 *      results around 150 ms
 *
 *      testrun_run(cluster_tests_non_configurable);
 *      results around 135 ms
 *
 *      testrun_run(run_tests); with parallel testrun_run_tests(25,0,false);
 *      results around 280 ms
 *
 *      So the overhead for OpenMP is not worth to run the tests in parallel
 *      here. This may be different for other test scenarios. Check your
 *      tests and scenarios to select the best fitting testrun for you.
 *
 *      NOTE For these tests here non configuration tests runs the fastest!
 *      NOTE as soon as this test series is compiled using OpenMP there will
 *      be 8 bytes in 1 block shown as still reachable when running
 *      with valgrind.
 *
 *      ------------------------------------------------------------------------
 *
 *      EXAMPLE RUN TIMES:
 *
 *      ALL TESTS RUN (23 tests)
 *      Clock ticks function: ( main ) | 0.135426 | 0.000135 ms
 *
 *      Serial RUN (23) TESTS: success 23 error 0)
 *      Clock ticks function: ( testrun_sequential ) | 0.120804 | 121 ms
 *      ALL TESTS RUN (23 tests)
 *      Clock ticks function: ( main ) | 0.148684 | 149 ms
 *
 *      Parallel RUN (23) TESTS in 0 threads: success 23 error 0)
 *      Clock ticks function: ( testrun_parallel ) | 0.286677 | 287 ms
 *      ALL TESTS RUN (23 tests)
 *      Clock ticks function: ( main ) | 0.317900 | 318 ms
 *
 *      ------------------------------------------------------------------------
 *
 *      TEST RUN with following machine data:
 *      markus@PC:~/projects/testrun.info/testrun_lib$ lscpu
 *      Architecture:          x86_64
 *      CPU op-mode(s):        32-bit, 64-bit
 *      Byte Order:            Little Endian
 *      CPU(s):                4
 *      On-line CPU(s) list:   0-3
 *      Thread(s) pro Kern:    2
 *      Kern(e) pro Socket:    2
 *      Socket(s):             1
 *      NUMA-Knoten:           1
 *      Anbieterkennung:       GenuineIntel
 *      Prozessorfamilie:      6
 *      Modell:                69
 *      Model name:            Intel(R) Core(TM) i7-4600U CPU @ 2.10GHz
 *      Stepping:              1
 *      CPU MHz:               2654.681
 *      CPU max MHz:           3300,0000
 *      CPU min MHz:           800,0000
 *      BogoMIPS:              5387.67
 *      Virtualisierung:       VT-x
 *      L1d Cache:             32K
 *      L1i Cache:             32K
 *      L2 Cache:              256K
 *      L3 Cache:              4096K
 *      NUMA node0 CPU(s):     0-3
 *      Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr
 *                             pge mca cmov pat pse36 clflush dts acpi mmx fxsr
 *                             sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp
 *                             lm constant_tsc arch_perfmon pebs bts rep_good
 *                             nopl xtopology nonstop_tsc aperfmperf pni
 *                             pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2
 *                             ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2
 *                             x2apic movbe popcnt tsc_deadline_timer aes xsave
 *                             avx f16c rdrand lahf_lm abm epb tpr_shadow vnmi
 *                             flexpriority ept vpid fsgsbase tsc_adjust bmi1
 *                             avx2 smep bmi2 erms invpcid xsaveopt dtherm ida
 *                             arat pln pts
 *      markus@PC:~/projects/testrun.info/testrun_lib$ cat /proc/version
 *      Linux version 4.10.0-38-generic (buildd@lgw01-amd64-059)
 *      (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4) )
 *      #42~16.04.1-Ubuntu SMP Tue Oct 10 16:32:20 UTC 2017
 *
 *      ------------------------------------------------------------------------
 */
