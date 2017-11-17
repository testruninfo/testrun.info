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
 *      @file           tr_log.h
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Implementation of a logging interface for testrun.
 *                      This library based logging is (by default) logging
 *                      to standard out, using standard syslog LOGLEVEL extended
 *                      with a log_dev method to log development messages only.
 *                      This log_dev is send to stdout, but NOT forwarded to
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef tr_log_h
#define tr_log_h

#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <syslog.h>

#include <time.h>

/*
 *      To use systemd based logging include the sd_jounral.h like stated below,
 *      change the tr_log_journal line to the one below, as well as compile
 *      the system using with system libsystemd enabled in the makefile
 *
 *      #include <systemd/sd-journal.h>
 *      #define tr_log_journal(level, ...) sd_journal_print(level, __VA_ARGS__)
 */

#define tr_log_journal(level, ...)

/*
 *      SYSLOG LOGLEVEL
 *
 *      LOG_EMERG      system is unusable
 *      LOG_ALERT      action must be taken immediately
 *      LOG_CRIT       critical conditions
 *      LOG_ERR        error conditions
 *      LOG_WARNING    warning conditions
 *      LOG_NOTICE     normal, but significant, condition
 *      LOG_INFO       informational message
 *      LOG_DEBUG      debug-level message
 *
 */

#define LOG_LEVEL_DEVELOPMENT
#define tr_log_file(level, file, ...) \
        fprintf(file, "%s (level:%d) (file: %s func: %s line: %d) %s \n", \
        __TIMESTAMP__ ,level, __FILE__,__FUNCTION__,__LINE__, __VA_ARGS__)

#define tr_log_forward(level, ...) \
        tr_log_file(level, stdout, __VA_ARGS__)\
        tr_log_journal(level, ... __VA_ARGS__)

/* Development logging */
#define log_dev(...)           tr_log_file(LOG_DEBUG+1, stderr, __VA_ARGS__)
/* Normal logging */
#define log_debug(...)         tr_log_forward(LOG_DEBUG,   __VA_ARGS__)
#define log_info(...)          tr_log_forward(LOG_INFO,    __VA_ARGS__)
#define log_notice(...)        tr_log_forward(LOG_NOTICE,  __VA_ARGS__)
#define log_warning(...)       tr_log_forward(LOG_WARNING, __VA_ARGS__)
#define log_error(...)         tr_log_forward(LOG_ERR,     __VA_ARGS__)
#define log_critical(...)      tr_log_forward(LOG_CRIT,    __VA_ARGS__)
#define log_alert(...)         tr_log_forward(LOG_ALERT,   __VA_ARGS__)
#define log_emergency(...)     tr_log_forward(LOG_EMERG,   __VA_ARGS__)

#endif /* ov_log_h */

/*
#define ov_log_file(level, file, message, ...) \
  fprintf(file, "level(%d) file(%s) function(%s) line(%d) message: " message "\n", \
  level,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
  */
