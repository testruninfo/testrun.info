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
 *      @file           testrun_vector_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-18
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Unit testing of testrun_vector
 *
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include <limits.h>

#include "../tools/testrun.h"
#include "../../src/testrun_vector.c"

static int testing = 1;

/*******************************************************************************
 *
 *      TEST HELPER
 *
 ******************************************************************************/

void *dummy_testrun_copy_failure() {
        return NULL;
};

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/

int test_testrun_vector_create(){

        testrun_vector *vector = NULL;

        vector = testrun_vector_create(0, NULL, NULL);
        testrun_assert(vector != NULL,  "... without content");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1, NULL, NULL);
        testrun_assert(vector != NULL,  "... with rate 1");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(2, NULL, NULL);
        testrun_assert(vector != NULL,  "... with rate 2");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 2);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1000, NULL, NULL);
        testrun_assert(vector != NULL,  "... with rate 1000");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1000);
        testrun_assert(vector->rate      == 1000);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1, &free, NULL);
        testrun_assert(vector != NULL,  "... with rate 1 and free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == free);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1,
            testrun_vector_int64_example_free,
            testrun_vector_int64_example_copy);
        testrun_assert(vector != NULL,  "... with rate 1 int64 int64 example");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == testrun_vector_int64_example_copy);
        vector = testrun_vector_terminate(vector);

        vector = testrun_vector_create(1,
            NULL,
            testrun_vector_int64_example_copy);
        testrun_assert(vector != NULL,  "... with rate 1 NULL int64 example");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == testrun_vector_int64_example_copy);
        vector = testrun_vector_terminate(vector);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_free(){

        testrun_vector *vector = NULL;

        void *pointer = NULL;

        vector = testrun_vector_create(0,
            testrun_vector_int64_example_free,
            testrun_vector_int64_example_copy);
        testrun_assert(vector != NULL,  "... with default rate & int64");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == testrun_vector_int64_example_copy);
        for (int i = 0; i < 5; i++){
                pointer = calloc(1, sizeof(int64_t));
                vector->items[i] = pointer;
                vector->last = i;
        }

        // free everything
        testrun_vector_free(vector);

        vector = testrun_vector_create(0, NULL, NULL);
        testrun_assert(vector != NULL,  "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        pointer = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer;
        // free everything (except items)
        testrun_vector_free(vector);
        // free item pointer
        free(pointer);

        // check with valgrind run
        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_clear(){

        testrun_vector *vector = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun_assert(vector != NULL,  "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(true  == testrun_vector_clear(vector), "clear empty");
        testrun_assert(false == testrun_vector_clear(NULL),   "clear NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        // non default init
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,  "... with 100, NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(testrun_vector_clear(vector), "clear empty");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        // clear no value_free
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,  "... no value free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun_assert(testrun_vector_clear(vector), "clear vector");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);
        testrun_assert(pointer1 != NULL);
        testrun_assert(pointer2 != NULL);
        testrun_assert(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // clear with value_free
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free, NULL);
        testrun_assert(vector != NULL,  "... with value free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun_assert(testrun_vector_clear(vector), "clear vector");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == NULL);
        vector = testrun_vector_terminate(vector);

        // items termination check with valgrind run
        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_destroy(){

        testrun_vector *vector = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun_assert(vector != NULL,  "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL  == testrun_vector_destroy(vector), "empty");
        testrun_assert(NULL  == testrun_vector_destroy(NULL),   "NULL");

        // non default init
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,  "... non default init");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL  == testrun_vector_destroy(vector), "no default");

        // clear no value_free
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,  "... no value free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun_assert(NULL  == testrun_vector_destroy(vector), "no free");
        testrun_assert(pointer1 != NULL);
        testrun_assert(pointer2 != NULL);
        testrun_assert(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // clear with value_free
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free, NULL);
        testrun_assert(vector != NULL,  "... no value free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun_assert(NULL  == testrun_vector_destroy(vector), "with free");
        testrun_assert(pointer1 != NULL);
        testrun_assert(pointer2 != NULL);
        testrun_assert(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // items termination check with valgrind run
        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_terminate(){

        testrun_vector *vector = NULL;
        void *pointer1 = NULL;
        void *pointer2 = NULL;
        void *pointer3 = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun_assert(vector != NULL,  "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL  == testrun_vector_terminate(vector), "empty");
        testrun_assert(NULL  == testrun_vector_terminate(NULL),   "NULL");

        // non default init
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,  "... non default init");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL  == testrun_vector_terminate(vector), "no default");

        // no value_free
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,  "... no value free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun_assert(NULL  == testrun_vector_terminate(vector), "no free");
        testrun_assert(pointer1 != NULL);
        testrun_assert(pointer2 != NULL);
        testrun_assert(pointer3 != NULL);
        free(pointer1);
        free(pointer2);
        free(pointer3);

        // with value_free
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free, NULL);
        testrun_assert(vector != NULL,  "... no value free");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == NULL);
        pointer1 = calloc(1, sizeof(int64_t));
        pointer2 = calloc(1, sizeof(int64_t));
        pointer3 = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer1;
        vector->items[1] = pointer2;
        vector->items[2] = pointer3;
        vector->last = 2;
        testrun_assert(NULL  == testrun_vector_terminate(vector), "with free");

        // items termination check with valgrind run
        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->rate      == TESTRUN_VECTOR_DEFAULT_CHANGE_RATE);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL == testrun_vector_copy(vector), "no item_copy");
        vector = testrun_vector_terminate(vector);

        // no items contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                testrun_vector_int64_example_copy);
        testrun_assert(vector != NULL,  "... non default init");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == testrun_vector_int64_example_copy);
        copy = testrun_vector_copy(vector);
        testrun_assert(copy != NULL,     "... with NULL");
        testrun_assert(copy->last        == 0);
        testrun_assert(copy->size        == 100);
        testrun_assert(copy->rate        == 100);
        testrun_assert(copy->items       != NULL);
        testrun_assert(copy->item_free   == testrun_vector_int64_example_free);
        testrun_assert(copy->item_copy   == testrun_vector_int64_example_copy);
        vector = testrun_vector_terminate(vector);
        copy   = testrun_vector_terminate(copy);

        // items contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                testrun_vector_int64_example_copy);
        testrun_assert(vector != NULL,  "... non default init");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == testrun_vector_int64_example_copy);
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
        testrun_assert(copy != NULL,     "... non default");
        testrun_assert(copy->last        == 2);
        testrun_assert(copy->size        == 100);
        testrun_assert(copy->rate        == 100);
        testrun_assert(copy->items       != NULL);
        testrun_assert(copy->item_free   == testrun_vector_int64_example_free);
        testrun_assert(copy->item_copy   == testrun_vector_int64_example_copy);
        testrun_assert(*(int64_t*)copy->items[0] == 1);
        testrun_assert(*(int64_t*)copy->items[1] == 2);
        testrun_assert(*(int64_t*)copy->items[2] == 1234567890);
        vector = testrun_vector_terminate(vector);
        copy   = testrun_vector_terminate(copy);

        // item contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                testrun_vector_int64_example_copy);
        testrun_assert(vector != NULL,  "... non default init");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == testrun_vector_int64_example_copy);
        pointer1  = calloc(1, sizeof(int64_t));
        *(int64_t*)pointer1 = 1;
        vector->items[0] = pointer1;
        vector->last = 0;
        copy = testrun_vector_copy(vector);
        testrun_assert(copy != NULL,     "... non default");
        testrun_assert(copy->last        == 0);
        testrun_assert(copy->size        == 100);
        testrun_assert(copy->rate        == 100);
        testrun_assert(copy->items       != NULL);
        testrun_assert(copy->item_free   == testrun_vector_int64_example_free);
        testrun_assert(copy->item_copy   == testrun_vector_int64_example_copy);
        testrun_assert(false == testrun_vector_is_empty(vector));
        testrun_assert(false == testrun_vector_is_empty(copy));
        testrun_assert(*(int64_t*)copy->items[0] == 1);
        vector = testrun_vector_terminate(vector);
        copy   = testrun_vector_terminate(copy);

        // item copy failure contained
        vector = testrun_vector_create(100,
                testrun_vector_int64_example_free,
                dummy_testrun_copy_failure);
        testrun_assert(vector != NULL,  "... non default init");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == testrun_vector_int64_example_free);
        testrun_assert(vector->item_copy == dummy_testrun_copy_failure);
        pointer1  = calloc(1, sizeof(int64_t));
        *(int64_t*)pointer1 = 1;
        vector->items[0] = pointer1;
        vector->last = 0;
        copy = testrun_vector_copy(vector);
        testrun_assert(copy == NULL,     "... item copy failure");
        vector = testrun_vector_terminate(vector);

        return testrun_log_OK();
}


/*----------------------------------------------------------------------------*/

int test_testrun_vector_is_empty(){

        testrun_vector *vector = NULL;
        void *pointer = NULL;

        // default init
        vector = testrun_vector_create(0, NULL, NULL);
        testrun_assert(vector != NULL,  "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(testrun_vector_is_empty(vector));

        // vector end not 0
        vector->last = 1;
        testrun_assert(!testrun_vector_is_empty(vector));

        // vector items at NULL not empty
        vector->last = 0;
        pointer = calloc(1, sizeof(int64_t));
        vector->items[0] = pointer;
        testrun_assert(!testrun_vector_is_empty(vector));
        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_int64_example_copy(){

        void *pointer1 = NULL;
        void *pointer2 = NULL;

        testrun_assert(!testrun_vector_int64_example_copy(NULL), "NULL");

        pointer1  = calloc(1, sizeof(int64_t));
        *(int64_t*) pointer1 = 1;
        pointer2  = testrun_vector_int64_example_copy(pointer1);
        testrun_assert(pointer2  != NULL);
        testrun_assert( *(int64_t*)pointer2 == 1);
        // check idependence with free
        free(pointer1);
        free(pointer2);

        return testrun_log_OK();
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
        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_resize(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        // default init
        vector = testrun_vector_create(1, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_resize(NULL,     0));
        testrun_assert(false == testrun_vector_resize(vector,   0));
        testrun_assert(false == testrun_vector_resize(NULL,     10));
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(true == testrun_vector_resize(vector, 2), "grow min");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(true == testrun_vector_resize(vector, 10), "grow");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 9);
        testrun_assert(vector->rate      == 1);
        // verify allocated space (don't set end, so resize will loose slot)
        vector->items[9] = pointer;
        testrun_assert(true == testrun_vector_resize(vector, 5), "shrink");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 1);
        vector->last = 4;
        testrun_assert(false == testrun_vector_resize(vector, 4), "below end");
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 1);
        testrun_assert(true == testrun_vector_resize(vector, 100), "expand");
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 99);
        testrun_assert(vector->rate      == 1);
        // verify allocated space (don't set end, so resize will loose slot)
        vector->items[99] = pointer;
        testrun_assert(true == testrun_vector_resize(vector, 5), "to end");
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 1);
        // verify allocated space
        vector->items[0] = pointer;
        vector->items[1] = pointer;
        vector->items[2] = pointer;
        vector->items[3] = pointer;
        vector->items[4] = pointer;

        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_auto_adjust(){

        testrun_vector *vector = NULL;

        // rate 1
        vector = testrun_vector_create(1, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_auto_adjust(NULL));
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        vector->last = 1;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 1);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 1);
        vector->last = 2;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 2);
        testrun_assert(vector->size      == 3);
        testrun_assert(vector->rate      == 1);
        vector->last = 1;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 1);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 1);
        vector->last = 0;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 1);
        testrun_assert(vector->rate      == 1);
        vector = testrun_vector_terminate(vector);


        // rate 5
        vector = testrun_vector_create(5, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        vector->last = 1;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 1);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        vector->last = 2;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 2);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        vector->last = 3;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 3);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        vector->last = 4;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        vector->last = 5;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 5);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 6;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 6);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 7;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 7);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 8;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 8);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 9;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 9);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 10;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 10);
        testrun_assert(vector->size      == 15);
        testrun_assert(vector->rate      == 5);
        vector->last = 9;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 9);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 5;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 5);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 5);
        vector->last = 4;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        vector = testrun_vector_terminate(vector);


        // rate 10
        vector = testrun_vector_create(10, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 10);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 10);
        vector->last = 9;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 9);
        testrun_assert(vector->size      == 10);
        testrun_assert(vector->rate      == 10);
        vector->last = 10;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 10);
        testrun_assert(vector->size      == 20);
        testrun_assert(vector->rate      == 10);
        vector->last = 19;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 19);
        testrun_assert(vector->size      == 20);
        testrun_assert(vector->rate      == 10);
        vector->last = 20;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 20);
        testrun_assert(vector->size      == 30);
        testrun_assert(vector->rate      == 10);
        vector->last = 29;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 29);
        testrun_assert(vector->size      == 30);
        testrun_assert(vector->rate      == 10);
        vector->last = 30;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 30);
        testrun_assert(vector->size      == 40);
        testrun_assert(vector->rate      == 10);
        vector->last = 21;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 21);
        testrun_assert(vector->size      == 30);
        testrun_assert(vector->rate      == 10);
        vector->last = 20;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 20);
        testrun_assert(vector->size      == 30);
        testrun_assert(vector->rate      == 10);
        vector->last = 19;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 19);
        testrun_assert(vector->size      == 20);
        testrun_assert(vector->rate      == 10);
        vector = testrun_vector_terminate(vector);

        // rate 2
        vector = testrun_vector_create(2, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 2);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 2);
        vector->last = 1;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 1);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 2);
        vector->last = 2;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 2);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 2);
        vector->last = 3;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 3);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 2);
        vector->last = 4;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 6);
        testrun_assert(vector->rate      == 2);
        vector->last = 5;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 5);
        testrun_assert(vector->size      == 6);
        testrun_assert(vector->rate      == 2);
        vector->last = 6;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 6);
        testrun_assert(vector->size      == 8);
        testrun_assert(vector->rate      == 2);
        vector->last = 5;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 5);
        testrun_assert(vector->size      == 6);
        testrun_assert(vector->rate      == 2);
        vector->last = 4;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 4);
        testrun_assert(vector->size      == 6);
        testrun_assert(vector->rate      == 2);
        vector->last = 3;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 3);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 2);
        vector->last = 2;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 2);
        testrun_assert(vector->size      == 4);
        testrun_assert(vector->rate      == 2);
        vector->last = 1;
        testrun_assert(true  == testrun_vector_auto_adjust(vector));
        testrun_assert(vector->last      == 1);
        testrun_assert(vector->size      == 2);
        testrun_assert(vector->rate      == 2);
        vector = testrun_vector_terminate(vector);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_set_new_end(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        // rate 1
        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_set_new_end(NULL,   0));
        testrun_assert(false == testrun_vector_set_new_end(NULL,   1));
        testrun_assert(true  == testrun_vector_set_new_end(vector, 0));
        testrun_assert(vector->last == 0);

        testrun_assert(true == testrun_vector_set_new_end(vector, 1),"no item");
        testrun_assert(vector->last == 0);
        testrun_assert(true == testrun_vector_set_new_end(vector, 2),"no item");
        testrun_assert(vector->last == 0);
        testrun_assert(true == testrun_vector_set_new_end(vector, 3),"no item");
        testrun_assert(vector->last == 0);

        vector->items[1] = pointer;
        vector->last = 1;
        testrun_assert(false == testrun_vector_set_new_end(vector, 0),"< end");
        testrun_assert(vector->last == 1);
        testrun_assert(true == testrun_vector_set_new_end(vector, 1), "= end");
        testrun_assert(vector->last == 1);
        testrun_assert(true == testrun_vector_set_new_end(vector, 2), "> end");
        testrun_assert(vector->last == 1);
        testrun_assert(true == testrun_vector_set_new_end(vector, 3), "> end");
        testrun_assert(vector->last == 1);
        testrun_assert(true == testrun_vector_set_new_end(vector, 4), "> end");
        testrun_assert(vector->last == 1);

        vector->items[3] = pointer;
        vector->last = 5;
        testrun_assert(false == testrun_vector_set_new_end(vector, 4),"< end");
        testrun_assert(vector->last == 5);
        testrun_assert(true == testrun_vector_set_new_end(vector, 5), "= end");
        testrun_assert(vector->last == 3);
        vector->last = 5;
        testrun_assert(true == testrun_vector_set_new_end(vector, 6), "> end");
        testrun_assert(vector->last == 3);
        vector->last = 10;
        vector->items[6] = pointer;
        testrun_assert(true == testrun_vector_set_new_end(vector, 23), "> end");
        testrun_assert(vector->last == 6);
        vector->items[6] = NULL;
        vector->items[3] = NULL;
        vector->items[1] = NULL;
        testrun_assert(true == testrun_vector_set_new_end(vector, 23), "> end");
        testrun_assert(vector->last == 0);

        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_get(){

        testrun_vector *vector = NULL;
        void *pointer1 = calloc(1, sizeof(void*));
        void *pointer2 = calloc(1, sizeof(void*));
        void *pointer3 = calloc(1, sizeof(void*));
        void *result   = NULL;

        vector = testrun_vector_create(100, NULL, NULL);
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL == testrun_vector_get(NULL,   0));
        testrun_assert(NULL == testrun_vector_get(NULL,   1));
        testrun_assert(NULL == testrun_vector_get(vector, 0));
        testrun_assert(vector->last == 0);
        testrun_assert(testrun_vector_set(vector, 1, pointer1));
        testrun_assert(testrun_vector_set(vector, 2, pointer2));
        testrun_assert(testrun_vector_set(vector, 3, pointer3));
        testrun_assert(testrun_vector_set(vector, 6, pointer1));
        testrun_assert(testrun_vector_set(vector, 7, pointer2));
        testrun_assert(testrun_vector_set(vector, 8, pointer3));
        testrun_assert(testrun_vector_set(vector, vector->size - 1, pointer3));
        testrun_assert(NULL == testrun_vector_get(vector, 0));
        result = testrun_vector_get(vector, 1);
        testrun_assert(result == pointer1, "content check");
        result = testrun_vector_get(vector, 2);
        testrun_assert(result == pointer2, "content check");
        result = testrun_vector_get(vector, 3);
        testrun_assert(result == pointer3, "content check");
        testrun_assert(NULL == testrun_vector_get(vector, 4));
        testrun_assert(NULL == testrun_vector_get(vector, 5));
        result = testrun_vector_get(vector, 6);
        testrun_assert(result == pointer1, "content check");
        result = testrun_vector_get(vector, 7);
        testrun_assert(result == pointer2, "content check");
        result = testrun_vector_get(vector, 8);
        testrun_assert(result == pointer3, "content check");
        result = testrun_vector_get(vector, 99);
        testrun_assert(result == pointer3, "content check MAX");
        testrun_assert(NULL == testrun_vector_get(vector, 100), "> MAX");
        testrun_assert(NULL == testrun_vector_get(vector, 101), "> MAX");
        vector = testrun_vector_terminate(vector);

        free(pointer1);
        free(pointer2);
        free(pointer3);
        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_set(NULL,   0,    NULL));
        testrun_assert(false == testrun_vector_set(NULL,   0,    pointer1));
        testrun_assert(true  == testrun_vector_set(vector, 0,    NULL));
        testrun_assert(vector->last == 0);
        // set to NULL
        testrun_assert(testrun_vector_set(vector, 0, pointer1));
        testrun_assert(vector->last == 0);
        testrun_assert(testrun_vector_set(vector, 1, pointer2));
        testrun_assert(vector->last == 1);
        testrun_assert(testrun_vector_set(vector, 2, pointer3));
        testrun_assert(vector->last == 2);
        testrun_assert(testrun_vector_set(vector, 3, pointer4));
        testrun_assert(vector->last == 3);
        result = testrun_vector_get(vector, 0);
        testrun_assert(result == pointer1, "content check");
        result = testrun_vector_get(vector, 1);
        testrun_assert(result == pointer2, "content check");
        result = testrun_vector_get(vector, 2);
        testrun_assert(result == pointer3, "content check");
        result = testrun_vector_get(vector, 3);
        testrun_assert(result == pointer4, "content check");
        // set to used (no item_free configured)
        testrun_assert(testrun_vector_set(vector, 0, pointer3));
        testrun_assert(testrun_vector_set(vector, 1, pointer1));
        testrun_assert(testrun_vector_set(vector, 2, pointer2));
        result = testrun_vector_get(vector, 0);
        testrun_assert(result == pointer3, "content check");
        result = testrun_vector_get(vector, 1);
        testrun_assert(result == pointer1, "content check");
        result = testrun_vector_get(vector, 2);
        testrun_assert(result == pointer2, "content check");
        result = testrun_vector_get(vector, 3);
        testrun_assert(result == pointer4, "content check");
        // prepare set to used (item_free configured)
        testrun_assert(testrun_vector_set(vector, 0, pointer1));
        testrun_assert(testrun_vector_set(vector, 1, pointer2));
        testrun_assert(testrun_vector_set(vector, 2, NULL));
        testrun_assert(testrun_vector_set(vector, 3, NULL));
        vector->item_free = free;
        // check state
        result = testrun_vector_get(vector, 0);
        testrun_assert(result == pointer1, "content check");
        result = testrun_vector_get(vector, 1);
        testrun_assert(result == pointer2, "content check");
        testrun_assert(NULL == testrun_vector_get(vector, 2));
        testrun_assert(NULL == testrun_vector_get(vector, 3));
        // set to used (item_free configured)
        testrun_assert(testrun_vector_set(vector, 0, pointer3));
        testrun_assert(testrun_vector_set(vector, 1, pointer4));
        result = testrun_vector_get(vector, 0);
        testrun_assert(result == pointer3, "content check");
        result = testrun_vector_get(vector, 1);
        testrun_assert(result == pointer4, "content check");
        // set to NULL (item_free configured)
        testrun_assert(testrun_vector_set(vector, 0, NULL));
        testrun_assert(testrun_vector_set(vector, 1, NULL));
        // check result
        testrun_assert(NULL == testrun_vector_get(vector, 0));
        testrun_assert(NULL == testrun_vector_get(vector, 1));
        testrun_assert(NULL == testrun_vector_get(vector, 2));
        testrun_assert(NULL == testrun_vector_get(vector, 3));

        // set behind end
        pointer1 = calloc(1, sizeof(void*));
        testrun_assert(false == testrun_vector_set(vector, 100, pointer1));
        // set last
        testrun_assert(true == testrun_vector_set(vector, 99, pointer1));
        testrun_assert(vector->last == 99);
        vector = testrun_vector_terminate(vector);

        // check item free with valgrind run
        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(-1 == testrun_vector_delete(NULL,   0));
        testrun_assert(-1 == testrun_vector_delete(NULL,   0));
        testrun_assert( 0 == testrun_vector_delete(vector, 0),   "no item");
        testrun_assert( 0 == testrun_vector_delete(vector, 1),   "no item");
        testrun_assert( 0 == testrun_vector_delete(vector, 2),   "no item");
        testrun_assert( 0 == testrun_vector_delete(vector, 3),   "no item");
        testrun_assert( 0 == testrun_vector_delete(vector, 99),  "max item");
        testrun_assert( 0 == testrun_vector_delete(vector, 100), "> max item");
        testrun_assert(vector->last == 0);
        testrun_assert(testrun_vector_set(vector, 0, pointer1));
        testrun_assert(testrun_vector_set(vector, 1, pointer2));
        testrun_assert(testrun_vector_set(vector, 4, pointer3));
        testrun_assert(testrun_vector_set(vector, 5, pointer4));
        testrun_assert( 5    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 5),"delete last");
        testrun_assert( NULL == testrun_vector_get(   vector, 5));
        testrun_assert( 4    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 4),"delete last");
        testrun_assert( NULL == testrun_vector_get(   vector, 4));
        testrun_assert( 1    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 0),"non last");
        testrun_assert( NULL == testrun_vector_get(   vector, 0));
        testrun_assert( 1    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 1),"delete last");
        testrun_assert( NULL == testrun_vector_get(   vector, 1));
        testrun_assert( 0    == vector->last);
        // check with free
        vector->item_free = free;
        testrun_assert(testrun_vector_set(vector, 5, pointer1));
        testrun_assert(testrun_vector_set(vector, 6, pointer2));
        testrun_assert(testrun_vector_set(vector, 7, pointer3));
        testrun_assert(testrun_vector_set(vector, 8, pointer4));
        testrun_assert( 8    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 7),"delete");
        testrun_assert( NULL == testrun_vector_get(   vector, 7));
        testrun_assert( 8    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 6),"delete");
        testrun_assert( NULL == testrun_vector_get(   vector, 6));
        testrun_assert( 8    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 8),"last");
        testrun_assert( NULL == testrun_vector_get(   vector, 8));
        testrun_assert( 5    == vector->last);
        testrun_assert( 1    == testrun_vector_delete(vector, 5),"last");
        testrun_assert( NULL == testrun_vector_get(   vector, 5));
        testrun_assert( 0    == vector->last);
        vector = testrun_vector_terminate(vector);

        // check item free with valgrind run
        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL == testrun_vector_remove(NULL,   0));
        testrun_assert(NULL == testrun_vector_remove(NULL,   0));
        testrun_assert(NULL == testrun_vector_remove(vector, 0),   "no item");
        testrun_assert(NULL == testrun_vector_remove(vector, 1),   "no item");
        testrun_assert(NULL == testrun_vector_remove(vector, 2),   "no item");
        testrun_assert(NULL == testrun_vector_remove(vector, 3),   "no item");
        testrun_assert(NULL == testrun_vector_remove(vector, 99),  "max item");
        testrun_assert(NULL == testrun_vector_remove(vector, 100), "> max ");
        testrun_assert(vector->last == 0);
        testrun_assert(testrun_vector_set(vector, 0, pointer1));
        testrun_assert(testrun_vector_set(vector, 1, pointer2));
        testrun_assert(testrun_vector_set(vector, 4, pointer3));
        testrun_assert(testrun_vector_set(vector, 5, pointer4));
        testrun_assert( 5        == vector->last);
        testrun_assert( pointer4 == testrun_vector_remove(vector, 5),"last");
        testrun_assert( NULL     == testrun_vector_get(   vector, 5));
        testrun_assert( 4        == vector->last);
        testrun_assert( pointer3 == testrun_vector_remove(vector, 4),"last");
        testrun_assert( NULL     == testrun_vector_get(   vector, 4));
        testrun_assert( 1        == vector->last);
        testrun_assert( pointer1 == testrun_vector_remove(vector, 0),"no last");
        testrun_assert( NULL     == testrun_vector_get(   vector, 0));
        testrun_assert( 1        == vector->last);
        testrun_assert( pointer2 == testrun_vector_remove(vector, 1),"last");
        testrun_assert( NULL     == testrun_vector_get(   vector, 1));
        testrun_assert( 0        == vector->last);
        // check with free
        vector->item_free = free;
        testrun_assert(testrun_vector_set(vector, 5, pointer1));
        testrun_assert(testrun_vector_set(vector, 6, pointer2));
        testrun_assert(testrun_vector_set(vector, 7, pointer3));
        testrun_assert(testrun_vector_set(vector, 8, pointer4));
        testrun_assert( 8        == vector->last);
        testrun_assert( pointer3 == testrun_vector_remove(vector, 7),"delete");
        testrun_assert( NULL     == testrun_vector_get(   vector, 7));
        testrun_assert( 8        == vector->last);
        testrun_assert( pointer2 == testrun_vector_remove(vector, 6),"delete");
        testrun_assert( NULL     == testrun_vector_get(   vector, 6));
        testrun_assert( 8        == vector->last);
        testrun_assert( pointer4 == testrun_vector_remove(vector, 8),"last");
        testrun_assert( NULL     == testrun_vector_get(   vector, 8));
        testrun_assert( 5        == vector->last);
        testrun_assert( pointer1 == testrun_vector_remove(vector, 5),"last");
        testrun_assert( NULL     == testrun_vector_get(   vector, 5));
        testrun_assert( 0        == vector->last);
        vector = testrun_vector_terminate(vector);

        // pointers are removed
        free(pointer1);
        free(pointer2);
        free(pointer3);
        free(pointer4);
        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 100);
        testrun_assert(vector->rate      == 100);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_add(NULL,   NULL));
        testrun_assert(false == testrun_vector_add(NULL,   pointer1));
        testrun_assert(false == testrun_vector_add(vector, NULL));
        testrun_assert(vector->last == 0);
        // add to empty
        testrun_assert(true     == testrun_vector_add(vector, pointer1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert( 0       == vector->last);
        // add to non empty
        testrun_assert(true     == testrun_vector_add(vector, pointer2));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert( 1       == vector->last);
        testrun_assert(testrun_vector_set(vector, 4, pointer5));
        testrun_assert( 4       == vector->last);
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(NULL     == testrun_vector_get(vector, 2));
        testrun_assert(NULL     == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        // check slot fill
        testrun_assert(true     == testrun_vector_add(vector, pointer3));
        testrun_assert(true     == testrun_vector_add(vector, pointer4));
        // check push
        testrun_assert(true     == testrun_vector_add(vector, pointer6));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer3 == testrun_vector_get(vector, 2));
        testrun_assert(pointer4 == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        testrun_assert(pointer6 == testrun_vector_get(vector, 5));
        testrun_assert(NULL     == testrun_vector_get(vector, 6));
        vector->item_free = free;
        vector = testrun_vector_terminate(vector);

        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 5);
        testrun_assert(vector->rate      == 5);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_insert(NULL,   0, NULL));
        testrun_assert(false == testrun_vector_insert(NULL,   0, pointer1));
        testrun_assert(false == testrun_vector_insert(vector, 0, NULL));
        testrun_assert(vector->last == 0);
        // insert to empty
        testrun_assert(true     == testrun_vector_insert(vector, 0, pointer1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert( 0       == vector->last);
        // insert to used slot
        testrun_assert(true     == testrun_vector_insert(vector, 0, pointer2));
        testrun_assert(pointer2 == testrun_vector_get(vector, 0));
        testrun_assert(pointer1 == testrun_vector_get(vector, 1));
        testrun_assert( 1       == vector->last);
        // insert to used slot
        testrun_assert(true     == testrun_vector_insert(vector, 0, pointer3));
        testrun_assert(pointer3 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 2));
        testrun_assert( 2       == vector->last);
        // insert > last
        testrun_assert(true     == testrun_vector_insert(vector, 4, pointer4));
        testrun_assert(pointer3 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 2));
        testrun_assert(NULL     == testrun_vector_get(vector, 3));
        testrun_assert(pointer4 == testrun_vector_get(vector, 4));
        testrun_assert( 4       == vector->last);
        // insert before trigger adjust
        testrun_assert(vector->size == 5);
        testrun_assert(vector->rate == 5);
        testrun_assert(true     == testrun_vector_insert(vector, 4, pointer5));
        testrun_assert(vector->size == 10);
        testrun_assert(vector->rate == 5);
        testrun_assert(pointer3 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 2));
        testrun_assert(NULL     == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        testrun_assert(pointer4 == testrun_vector_get(vector, 5));
        testrun_assert( 5       == vector->last);
        // insert last possible
        testrun_assert(true     == testrun_vector_insert(vector, 9, pointer6));
        testrun_assert(vector->size == 10);
        testrun_assert(vector->rate == 5);
        testrun_assert(pointer3 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 2));
        testrun_assert(NULL     == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        testrun_assert(pointer4 == testrun_vector_get(vector, 5));
        testrun_assert(NULL     == testrun_vector_get(vector, 6));
        testrun_assert(NULL     == testrun_vector_get(vector, 7));
        testrun_assert(NULL     == testrun_vector_get(vector, 8));
        testrun_assert(pointer6 == testrun_vector_get(vector, 9));
        testrun_assert(NULL     == testrun_vector_get(vector,10));
        testrun_assert(NULL     == testrun_vector_get(vector,11));
        testrun_assert(NULL     == testrun_vector_get(vector,12));
        testrun_assert(NULL     == testrun_vector_get(vector,13));
        testrun_assert(NULL     == testrun_vector_get(vector,14));
        // try to insert beyond slot size
        testrun_assert(vector->size == 10);
        testrun_assert(false    == testrun_vector_insert(vector, 10, pointer6));
        testrun_assert(false    == testrun_vector_insert(vector, 11, pointer1));
        testrun_assert(false    == testrun_vector_insert(vector, 20, pointer2));
        testrun_assert(false    == testrun_vector_insert(vector, 44, pointer3));
        vector->item_free = free;
        vector = testrun_vector_terminate(vector);

        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 3);
        testrun_assert(vector->rate      == 3);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(false == testrun_vector_push(NULL,   NULL));
        testrun_assert(false == testrun_vector_push(NULL,   pointer1));
        testrun_assert(false == testrun_vector_push(vector, NULL));
        testrun_assert(vector->last == 0);
        // push to empty
        testrun_assert(true     == testrun_vector_push(vector, pointer1));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert( 0       == vector->last);
        // push to used
        testrun_assert(true     == testrun_vector_push(vector, pointer2));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert( 1       == vector->last);
        // push to used
        testrun_assert(true     == testrun_vector_push(vector, pointer3));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer3 == testrun_vector_get(vector, 2));
        testrun_assert( 2       == vector->last);
        // push to used and grow
        testrun_assert(vector->last == 2);
        testrun_assert(vector->size == 3);
        testrun_assert(vector->rate == 3);
        testrun_assert(true     == testrun_vector_push(vector, pointer4));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer3 == testrun_vector_get(vector, 2));
        testrun_assert(pointer4 == testrun_vector_get(vector, 3));
        testrun_assert(NULL     == testrun_vector_get(vector, 4));
        testrun_assert(NULL     == testrun_vector_get(vector, 5));
        testrun_assert(vector->last == 3);
        testrun_assert(vector->size == 6);
        testrun_assert(vector->rate == 3);
        // push to used
        testrun_assert(true     == testrun_vector_push(vector, pointer5));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer3 == testrun_vector_get(vector, 2));
        testrun_assert(pointer4 == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        testrun_assert(NULL     == testrun_vector_get(vector, 5));
        // push to used
        testrun_assert(true     == testrun_vector_push(vector, pointer6));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer3 == testrun_vector_get(vector, 2));
        testrun_assert(pointer4 == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        testrun_assert(pointer6 == testrun_vector_get(vector, 5));
        // push to used and grow
        testrun_assert(vector->last == 5);
        testrun_assert(vector->size == 6);
        testrun_assert(vector->rate == 3);
        testrun_assert(true     == testrun_vector_push(vector, pointer7));
        testrun_assert(pointer1 == testrun_vector_get(vector, 0));
        testrun_assert(pointer2 == testrun_vector_get(vector, 1));
        testrun_assert(pointer3 == testrun_vector_get(vector, 2));
        testrun_assert(pointer4 == testrun_vector_get(vector, 3));
        testrun_assert(pointer5 == testrun_vector_get(vector, 4));
        testrun_assert(pointer6 == testrun_vector_get(vector, 5));
        testrun_assert(pointer7 == testrun_vector_get(vector, 6));
        testrun_assert(vector->last == 6);
        testrun_assert(vector->size == 9);
        testrun_assert(vector->rate == 3);
        vector->item_free = free;
        vector = testrun_vector_terminate(vector);

        return testrun_log_OK();
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
        testrun_assert(vector != NULL,   "... with NULL");
        testrun_assert(vector->last      == 0);
        testrun_assert(vector->size      == 3);
        testrun_assert(vector->rate      == 3);
        testrun_assert(vector->items     != NULL);
        testrun_assert(vector->item_free == NULL);
        testrun_assert(vector->item_copy == NULL);
        testrun_assert(NULL  == testrun_vector_pop(NULL));

        // pop from empty
        testrun_assert(!testrun_vector_pop(vector));
        testrun_assert(vector->last == 0);

        // pop non empty to empty
        testrun_assert(testrun_vector_push(vector, pointer1));
        testrun_assert(!testrun_vector_is_empty(vector));
        testrun_assert(testrun_vector_pop(vector) == pointer1);
        testrun_assert(testrun_vector_is_empty(vector));
        testrun_assert(vector->last == 0);

        // pop from non empty
        testrun_assert(testrun_vector_push(vector, pointer1));
        testrun_assert(testrun_vector_push(vector, pointer2));
        testrun_assert(testrun_vector_push(vector, pointer3));
        testrun_assert(testrun_vector_push(vector, pointer4));
        testrun_assert(testrun_vector_push(vector, pointer5));
        testrun_assert(testrun_vector_push(vector, pointer6));
        testrun_assert(testrun_vector_push(vector, pointer7));
        testrun_assert(vector->last == 6);
        testrun_assert(vector->size == 9);
        testrun_assert(vector->rate == 3);

        // pop & shrink
        testrun_assert(testrun_vector_pop(vector) == pointer7);
        testrun_assert(vector->last == 5);
        testrun_assert(vector->size == 6);
        testrun_assert(vector->rate == 3);

        testrun_assert(testrun_vector_pop(vector) == pointer6);
        testrun_assert(vector->last == 4);
        testrun_assert(vector->size == 6);
        testrun_assert(vector->rate == 3);

        testrun_assert(testrun_vector_pop(vector) == pointer5);
        testrun_assert(vector->last == 3);
        testrun_assert(vector->size == 6);
        testrun_assert(vector->rate == 3);

        // pop & shrink
        testrun_assert(testrun_vector_pop(vector) == pointer4);
        testrun_assert(vector->last == 2);
        testrun_assert(vector->size == 3);
        testrun_assert(vector->rate == 3);

        testrun_assert(testrun_vector_pop(vector) == pointer3);
        testrun_assert(vector->last == 1);
        testrun_assert(vector->size == 3);
        testrun_assert(vector->rate == 3);

        testrun_assert(testrun_vector_pop(vector) == pointer2);
        testrun_assert(!testrun_vector_is_empty(vector));
        testrun_assert(vector->last == 0);
        testrun_assert(vector->size == 3);
        testrun_assert(vector->rate == 3);

        testrun_assert(testrun_vector_pop(vector) == pointer1);
        testrun_assert(testrun_vector_is_empty(vector));
        testrun_assert(vector->last == 0);
        testrun_assert(vector->size == 3);
        testrun_assert(vector->rate == 3);

        vector = testrun_vector_terminate(vector);

        // pointers are poped
        free(pointer1);
        free(pointer2);
        free(pointer3);
        free(pointer4);
        free(pointer5);
        free(pointer6);
        free(pointer7);
        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_dump_function_backtrace(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        vector = testrun_vector_create(20, NULL, NULL);
        testrun_assert(testrun_vector_set(vector, 2, test_testrun_vector_set));
        testrun_assert(testrun_vector_push(vector, test_testrun_vector_get));
        testrun_assert(testrun_vector_push(vector, test_testrun_vector_add));
        testrun_assert(testrun_vector_push(vector, test_testrun_vector_pop));
        testrun_assert(testrun_vector_set(vector, 10, test_testrun_vector_add));
        testrun_assert(testrun_vector_set(vector, 11, test_testrun_vector_add));
        testrun_assert(testrun_vector_push(vector, pointer));

        testrun_assert(!testrun_vector_dump_function_backtrace(NULL));
        testrun_assert(testrun_vector_dump_function_backtrace(vector));
        vector = testrun_vector_terminate(vector);

        free(pointer);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_vector_dump(){

        testrun_vector *vector = NULL;
        void *pointer = calloc(1, sizeof(void*));

        vector = testrun_vector_create(20, NULL, NULL);
        testrun_assert(testrun_vector_set(vector, 2, test_testrun_vector_set));
        testrun_assert(testrun_vector_push(vector, test_testrun_vector_get));
        testrun_assert(testrun_vector_push(vector, test_testrun_vector_add));
        testrun_assert(testrun_vector_push(vector, test_testrun_vector_pop));
        testrun_assert(testrun_vector_set(vector, 10, test_testrun_vector_add));
        testrun_assert(testrun_vector_set(vector, 11, test_testrun_vector_add));
        testrun_assert(testrun_vector_push(vector, pointer));

        testrun_assert(!testrun_vector_dump(NULL, vector));
        testrun_assert(!testrun_vector_dump(stdout, NULL));
        testrun_assert(testrun_vector_dump(stdout, vector));
        vector = testrun_vector_terminate(vector);
        free(pointer);

        return testrun_log_OK();
}

/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

int all_tests() {

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

        return 1;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
