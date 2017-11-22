# Valgrind OpenMP

Short background information about lipgomp based valgrind errors.
This problem will occur with clang and gcc as long as glibc is used with pthreads implementation which uses user space futex instead of kernel space mutexes.

## Valgrind Manual Statement (see reference doc)

Runtime support library for GNU OpenMP (part of GCC), at least for GCC versions 4.2 and 4.3. The GNU OpenMP runtime library (libgomp.so) constructs its own synchronisation primitives using combinations of atomic memory instructions and the futex syscall, which causes total chaos since in Helgrind since it cannot "see" those.

Fortunately, this can be solved using a configuration-time option (for GCC). Rebuild GCC from source, and configure using --disable-linux-futex. This makes libgomp.so use the standard POSIX threading primitives instead. Note that this was tested using GCC 4.2.3 and has not been re-tested using more recent GCC versions. We would appreciate hearing about any successes or failures with more recent versions.

## But I don't want to compile GCC without user space futexes ... and still use parallel test execution

To be clear, usage of valgrind to debug threads and parallel execution may 
result in errors outputs (false positives) as valgrind may not be able to identify allocations, deallocations and synchronisation primitives of OpenMP. 
Anyway, usage of atomar tests using OpenMP for parallel test invocation
based on available processer and threading power of the machine running the tests, is not affected from this inconsistancy between the tools, as the tests MUST be atomar anyway. (That is the general idea of atomar reproducible tests.)
What we want to test are allocation/deallocation and memory free's as well as buffer overruns of the software developed. 

## Which errors are produced ?

```
markus@pc:~/projects/testrun.info/testrun_lib$ valgrind --leak-check=full ./build/test/acceptance/testrun_example_tests.test 
==28495== Memcheck, a memory error detector
==28495== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==28495== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==28495== Command: ./build/test/acceptance/testrun_example_tests.test
==28495== 
    
testrun ./build/test/acceptance/testrun_example_tests.test
    ...     added 2 tests to array(current used 2 of 1000 slots)
    [OK]    testcase_block1 
    [OK]    testcase_block2 
    ---------------------------------------------------------
    NOTE PARALLEL TESTING
    
    This version is using OpenMP.
    Using GCC for compilation may produce unreliable valgrind
    output due to custom synchronization primitives(non POSIX).
    More information is included in docs/valgrind/openMP
    ---------------------------------------------------------
    Parallel RUN (2) TESTS in 4 threads: success 2 error 0)
    Clock ticks function: ( testrun_parallel ) | 0.038440 | 0.000038 ms 
    
    ...     added 2 tests to array(current used 2 of 1000 slots)
    ...     added 2 tests to array(current used 4 of 1000 slots)
    [OK]    testcase_block1 
    [OK]    testcase_block2 
    [OK]    testcase_block3 
    [OK]    testcase_block4 
    Serial RUN (4) TESTS: success 4 error 0)
    Clock ticks function: ( testrun_sequential ) | 0.001959 | 0.000002 ms 
    
    ALL TESTS RUN (6 tests)
    Clock ticks function: ( main ) | 0.078494 | 0.000078 ms 
    
==28495== 
==28495== HEAP SUMMARY:
==28495==     in use at exit: 3,184 bytes in 7 blocks
==28495==   total heap usage: 9 allocs, 2 frees, 37,024 bytes allocated
==28495== 
==28495== 864 bytes in 3 blocks are possibly lost in loss record 4 of 5
==28495==    at 0x4C2FB55: calloc (vg_replace_malloc.c:711)
==28495==    by 0x40138A4: allocate_dtv (dl-tls.c:322)
==28495==    by 0x40138A4: _dl_allocate_tls (dl-tls.c:539)
==28495==    by 0x506426E: allocate_stack (allocatestack.c:588)
==28495==    by 0x506426E: pthread_create@@GLIBC_2.2.5 (pthread_create.c:539)
==28495==    by 0x4E4999F: ??? (in /usr/lib/x86_64-linux-gnu/libgomp.so.1.0.0)
==28495==    by 0x4E45CB9: GOMP_parallel (in /usr/lib/x86_64-linux-gnu/libgomp.so.1.0.0)
==28495==    by 0x400F8F: testrun_parallel (testrun.h:471)
==28495==    by 0x401478: testrun_run_tests (testrun.h:643)
==28495==    by 0x401C36: run_tests (testrun_example_tests.c:201)
==28495==    by 0x401C7A: main (testrun_example_tests.c:210)
==28495== 
==28495== LEAK SUMMARY:
==28495==    definitely lost: 0 bytes in 0 blocks
==28495==    indirectly lost: 0 bytes in 0 blocks
==28495==      possibly lost: 864 bytes in 3 blocks
==28495==    still reachable: 2,320 bytes in 4 blocks
==28495==         suppressed: 0 bytes in 0 blocks
==28495== Reachable blocks (those to which a pointer was found) are not shown.
==28495== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==28495== 
==28495== For counts of detected and suppressed errors, rerun with: -v
==28495== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

So we do have 2 types of error:
* still reachable
* possibly lost

### Still reachable

This is memory, which is attached to the program and which will be freed, when the process terminates. This memory is related to the parallel runs using OpenMP and caused by general information management of the threads in use.

We don't need to worry about reported memory still reachable, as it is cleaned up on process execution, as long as it is not increasing.

### Possibly lost

This is the part of memory, which valgrind was not able to track.  

## Solution: Suppress errors realated to lipgomp

```
markus@pc:~/projects/testrun.info/testrun_lib$ valgrind --leak-check=full ./build/test/acceptance/testrun_example_tests.test 
...

==29162== 
==29162== HEAP SUMMARY:
==29162==     in use at exit: 3,184 bytes in 7 blocks
==29162==   total heap usage: 9 allocs, 2 frees, 37,024 bytes allocated
==29162== 
==29162== LEAK SUMMARY:
==29162==    definitely lost: 0 bytes in 0 blocks
==29162==    indirectly lost: 0 bytes in 0 blocks
==29162==      possibly lost: 864 bytes in 3 blocks
==29162==    still reachable: 2,320 bytes in 4 blocks
==29162==         suppressed: 0 bytes in 0 blocks
==29162== Rerun with --leak-check=full to see details of leaked memory
==29162== 
==29162== For counts of detected and suppressed errors, rerun with: -v
==29162== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

to suppress false positives created by libgomp, just run the valgrind check with a custom suppression file. **--suppressions=tests/tools/valgrind/ignore_openmp.supp** 
This will move libgomp related possible lost counters to suppressed and show only possible losts related to the code under test.

```
markus@pc:~/projects/testrun.info/testrun_lib$valgrind --leak-check=full --suppressions=tests/tools/valgrind/ignore_openmp.supp ./build/test/acceptance/testrun_example_tests.test 
...

==29172== 
==29172== HEAP SUMMARY:
==29172==     in use at exit: 3,184 bytes in 7 blocks
==29172==   total heap usage: 9 allocs, 2 frees, 37,024 bytes allocated
==29172== 
==29172== LEAK SUMMARY:
==29172==    definitely lost: 0 bytes in 0 blocks
==29172==    indirectly lost: 0 bytes in 0 blocks
==29172==      possibly lost: 0 bytes in 0 blocks
==29172==    still reachable: 2,320 bytes in 4 blocks
==29172==         suppressed: 864 bytes in 3 blocks
==29172== Reachable blocks (those to which a pointer was found) are not shown.
==29172== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==29172== 
==29172== For counts of detected and suppressed errors, rerun with: -v
==29172== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 1 from 1)
```

## How to generate a suppression file?

To generate a suppression proposal with valgrind, run the tool using
**--gen-suppressions=all** copy the content within **{}** to a suppression file and you are done.

```
markus@pc:~/projects/testrun.info/testrun_lib$ valgrind --leak-check=full --gen-suppressions=all ./build/test/acceptance/testrun_example_tests.test 
==29199== Memcheck, a memory error detector
==29199== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==29199== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==29199== Command: ./build/test/acceptance/testrun_example_tests.test
==29199== 
    
testrun ./build/test/acceptance/testrun_example_tests.test
    ...     added 2 tests to array(current used 2 of 1000 slots)
    [OK]    testcase_block2 
    [OK]    testcase_block1 
    ---------------------------------------------------------
    NOTE PARALLEL TESTING
    
    This version is using OpenMP.
    Using GCC for compilation may produce unreliable valgrind
    output due to custom synchronization primitives(non POSIX).
    More information is included in docs/valgrind/openMP.
    ---------------------------------------------------------
    Parallel RUN (2) TESTS in 4 threads: success 2 error 0)
    Clock ticks function: ( testrun_parallel ) | 0.036239 | 0.000036 ms 
    
    ...     added 2 tests to array(current used 2 of 1000 slots)
    ...     added 2 tests to array(current used 4 of 1000 slots)
    [OK]    testcase_block1 
    [OK]    testcase_block2 
    [OK]    testcase_block3 
    [OK]    testcase_block4 
    Serial RUN (4) TESTS: success 4 error 0)
    Clock ticks function: ( testrun_sequential ) | 0.010812 | 0.000011 ms 
    
    ALL TESTS RUN (6 tests)
    Clock ticks function: ( main ) | 0.079056 | 0.000079 ms 
    
==29199== 
==29199== HEAP SUMMARY:
==29199==     in use at exit: 3,184 bytes in 7 blocks
==29199==   total heap usage: 9 allocs, 2 frees, 37,024 bytes allocated
==29199== 
==29199== 864 bytes in 3 blocks are possibly lost in loss record 4 of 5
==29199==    at 0x4C2FB55: calloc (vg_replace_malloc.c:711)
==29199==    by 0x40138A4: allocate_dtv (dl-tls.c:322)
==29199==    by 0x40138A4: _dl_allocate_tls (dl-tls.c:539)
==29199==    by 0x506426E: allocate_stack (allocatestack.c:588)
==29199==    by 0x506426E: pthread_create@@GLIBC_2.2.5 (pthread_create.c:539)
==29199==    by 0x4E4999F: ??? (in /usr/lib/x86_64-linux-gnu/libgomp.so.1.0.0)
==29199==    by 0x4E45CB9: GOMP_parallel (in /usr/lib/x86_64-linux-gnu/libgomp.so.1.0.0)
==29199==    by 0x400F8F: testrun_parallel (testrun.h:478)
==29199==    by 0x401478: testrun_run_tests (testrun.h:650)
==29199==    by 0x401C36: run_tests (testrun_example_tests.c:201)
==29199==    by 0x401C7A: main (testrun_example_tests.c:210)
==29199== 
{
   <insert_a_suppression_name_here>
   Memcheck:Leak
   match-leak-kinds: possible
   fun:calloc
   fun:allocate_dtv
   fun:_dl_allocate_tls
   fun:allocate_stack
   fun:pthread_create@@GLIBC_2.2.5
   obj:/usr/lib/x86_64-linux-gnu/libgomp.so.1.0.0
   fun:GOMP_parallel
   fun:testrun_parallel
   fun:testrun_run_tests
   fun:run_tests
   fun:main
}
==29199== LEAK SUMMARY:
==29199==    definitely lost: 0 bytes in 0 blocks
==29199==    indirectly lost: 0 bytes in 0 blocks
==29199==      possibly lost: 864 bytes in 3 blocks
==29199==    still reachable: 2,320 bytes in 4 blocks
==29199==         suppressed: 0 bytes in 0 blocks
==29199== Reachable blocks (those to which a pointer was found) are not shown.
==29199== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==29199== 
==29199== For counts of detected and suppressed errors, rerun with: -v
==29199== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
