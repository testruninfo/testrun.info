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
 *      @file           tr_string_tests.c_omp
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Unit testing for tr_strings using openMP
 *
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../../include/testrun2.h"
#include "../../src/tr_string.c"

static int testing = 1;


int testrun_parallel( int(*functions[])(), int amount) {

        testrun_counter_success = 0;
        testrun_counter_failure = 0;

        clock_t start, end;
        start = clock();

        int nthreads, tid;

        #pragma omp parallel for
        for (int i = 0; i < amount; i++){

                if (functions[i] != 0) {
                        functions[i]();

                 tid = omp_get_thread_num();
                 printf("tid = %d\n", tid);

                /* Only master thread does this */
                if (tid == 0) {
                        nthreads = omp_get_num_threads();
                        printf("Number of threads = %d\n", nthreads);
                }
                }


        }



        testrun_log("Parallel RUN (%d) TESTS using %d threads: success %d error %d)",
                testrun_counter_success + testrun_counter_failure, nthreads,
                testrun_counter_success, testrun_counter_failure);
        end = clock();
        testrun_log_clock(start, end);
        testrun_log("");

        return -testrun_counter_failure;
}

/*----------------------------------------------------------------------------*/

int testrun_serial( int(*functions[])(), int amount) {

        testrun_counter_success = 0;
        testrun_counter_failure = 0;

        clock_t start, end;
        start = clock();

        int nthreads, tid;

        for (int i = 0; i < amount; i++){

                if (functions[i] != 0)
                        functions[i]();

        }

        testrun_log("Serial RUN (%d) TESTS: success %d error %d)",
                testrun_counter_success + testrun_counter_failure,
                testrun_counter_success, testrun_counter_failure);
        end = clock();
        testrun_log_clock(start, end);
        testrun_log("");

        return -testrun_counter_failure;
}


/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/


int test_tr_string_free(){

        char *string = calloc(1, sizeof(char));

        return testrun_counter_failure;

}

/*----------------------------------------------------------------------------*/

int test_tr_string_prepare(){

        char   *ptr = NULL;
        size_t size = 0;
        size_t open = 0;
        size_t used = 0;

        testrun(false == tr_string_prepare(NULL, NULL,  NULL,  NULL));
        testrun(false == tr_string_prepare(NULL, NULL,  NULL,  NULL), "OK test");
        testrun(true  == tr_string_prepare(NULL, NULL,  NULL,  NULL), "failed test");

        return testrun_counter_failure;

}

/*----------------------------------------------------------------------------*/

int test_1(){

        testrun(true);

        return testrun_counter_failure;

}

/*----------------------------------------------------------------------------*/

int test_2(){

        testrun(true);

        return testrun_counter_failure;

}

/*----------------------------------------------------------------------------*/

int test_3(){

        testrun(true);

        return testrun_counter_failure;

}

/*----------------------------------------------------------------------------*/

int test_4(){

        testrun(true);

        return testrun_counter_failure;

}

/*----------------------------------------------------------------------------*/

int test_5(){

        testrun(true);

        return testrun_counter_failure;

}


/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

int all_tests() {

        int max = 100;
        int (*tests[max]) ();

        for (int i = 0; i < max; i++){
                tests[i] = NULL;
        }


        testrun_add(test_tr_string_free);
        testrun_add(test_tr_string_prepare);
        testrun_add(test_1);
        testrun_add(test_2);
        testrun_add(test_3);
        testrun_add(test_4);
        testrun_add(test_5);
        testrun_add(test_1);
        testrun_add(test_2);
        testrun_add(test_3);
        testrun_add(test_4);
        testrun_add(test_5);
         testrun_add(test_1);
        testrun_add(test_2);
        testrun_add(test_3);
        testrun_add(test_4);
        testrun_add(test_5);
        testrun_add(test_1);
        testrun_add(test_2);
        testrun_add(test_3);
        testrun_add(test_4);
        testrun_add(test_5);

        // parallel
        testrun_parallel(tests, testrun_counter_all);

        // parallel
        testrun_serial(tests, testrun_counter_all);
        return -testrun_counter_failure;
}


/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_exec(all_tests);
