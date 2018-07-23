/***
        ------------------------------------------------------------------------

        Copyright 2018 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        ------------------------------------------------------------------------
*//**
  
        @file           testrun_exec.c
        @author         Markus Toepfer
        @date           2018-05-11
  
        @ingroup        testrun_project
  
        @brief          MAIN executable to run the app.
  
        @Note           Functional definition done in
                        @See testrun_app.h
  
  
        ------------------------------------------------------------------------
 **/

#include "../include/testrun_app.h"

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[]){

        return testrun_app_run(argc, argv);
}