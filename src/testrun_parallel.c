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
 *      @file           testrun_parallel.c
 *      @author         Markus Toepfer
 *      @date           2017-11-17
 *
 *      @ingroup
 *
 *      @brief
 *
 *
 *      ------------------------------------------------------------------------
 **/

#include "../include/testrun_parallel.h"

bool testrun_parallel(
	void (*functions[]) (),
	size_t amount,
	size_t const * successful,
	size_t const * failed){
/*
	if (!functions || !successful || !failed)
		return false;

	clock_t start, end;
        start = clock();

        size_t nthreads = 0, tid = 0;

	#pragma omp parallel for
	for (size_t i = 0; i < amount; i++){

		if (functions[i] != NULL) {
			functions[i]();

			tid = omp_get_thread_num();
                 	printf("tid = %jd\n", tid);

                 	if (nthreads == 0)
                 		if (tid == 0)
                 			nthreads = omp_get_num_threads();

		}
	}

	testrun_log("RUN %jd TESTS using %jd threads: success %jd error %jd)",
                testrun_counter_success + testrun_counter_failure, nthreads,
                testrun_counter_success, testrun_counter_failure);

        end = clock();
        testrun_log_clock(start, end);
        testrun_log("");
*/
	return true;
}




