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
 *      @file           testrun_log.h
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Implementation of a logging interface for testrun.
 *                      This library based logging is (by default) logging
 *                      to standard out, using standard syslog LOGLEVEL extended
 *                      with a log_dev method to log development messages only.
 *                      This log_dev is send to stdout, but NOT forwarded.
 *
 *                      The header shall be used to customize logging for
 *                      projects, based on rounting the internal log messages
 *                      to external outputs.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_log_h
#define testrun_log_h

#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/time.h>

#include <time.h>

/*
 *      To use systemd based logging include the sd_journal.h like stated below,
 *      change the testrun_log_journal line to the one below, as well as compile
 *      the system using with system libsystemd enabled in the makefile
 *
 *      #include <systemd/sd-journal.h>
 *      #define testrun_log_journal(level, ...) \
 *              sd_journal_print(level, __VA_ARGS__)
 */

#define testrun_log_journal(level, ...)

/*
 *      SYSLOG LOGLEVEL
 *
 *      LOG_EMERG       system is unusable
 *      LOG_ALERT       action must be taken immediately
 *      LOG_CRIT        critical conditions
 *      LOG_ERR         error conditions
 *      LOG_WARNING     warning conditions
 *      LOG_NOTICE      normal, but significant, condition
 *      LOG_INFO        informational message
 *      LOG_DEBUG       debug-level message
 *
 *
 *      CUSTOM LOGLEVEL
 *
 *      LOG_DEV         Development logging, always send to stdout with level
 *                      LOG_DEBUG + 1
 *                      LOG_DEV is not forwarded to the journal, even if the
 *                      journal is enabled.
 *
 *      ------------------------------------------------------------------------
 *
 *      @NOTE This logging interface is plugable to standard system loggers
 *      like journald or syslog or custom file based loggers and used to
 *      route internal log messages. It is not meant to be a complete logging
 *      solution, but a log message router.
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

/**
 *      Create an ISO 8601 or an ISO 8601 enhanced timestamp including
 *      microseconds. The result will be either NULL on error or one of:
 *
 *      2017-11-20T14:57:01Z
 *      2017-11-20T14:57:01.123456Z
 *
 *      @param micro            if true use microseconds
 *      @returns                Timestring buffer of 25 or 30 byte including the
 *                              timestamp or NULL. Char MUST not be freed.
 */
static inline char *testrun_log_create_timestamp(bool micro) {

        time_t t;
        time(&t);

        struct timeval tv;

        /*
         *      Use of static char array to be initialized once,
         *      and reused in each call to testrun_log_timestamp.
         *      e.g. use of this function as a char for a current timestamp.
         *
         *      printf("Current time is %s", testrun_log_timestamp(true));
         *
         *      NOTE: Due to static variable use,
         *      this function is not threadsafe.
         */

        static char time_utc[25];
        static char time_utc_microseconds[30];

        if (0 != gettimeofday(&tv, NULL))
                return NULL;

        if (!micro){
                if (!strftime(time_utc, 25, "%FT%TZ", gmtime(&tv.tv_sec)))
                        return NULL;
                return time_utc;
        }

        if (!strftime(time_utc, 25, "%FT%T", gmtime(&tv.tv_sec)))
                        return NULL;

        if (!snprintf(time_utc_microseconds, 30, "%s.%.6jdZ",
                time_utc, (int64_t) (tv.tv_usec)))
                return NULL;

        return time_utc_microseconds;
}

/*----------------------------------------------------------------------------*/

/**
 *      Create an ISO 8601 or an ISO 8601 enhanced timestamp including
 *      microseconds. The result will be either NULL on error or one of:
 *
 *      2017-11-20T14:57:01Z
 *      2017-11-20T14:57:01.123456Z
 *
 *      This is a threadsafe version of testrun_log_timestamp,
 *      which fills an external array with a timestamp and makes
 *      use of internal limited scope variables.
 *
 *      This function must be executed, to fill a timestamp buffer.
 *
 *      @param micro            if true use microseconds
 *      @param array            pointer to array to be filled with chars
 *      @param size             size of the array (incl. terminating zero)
 *      @returns                true on success, false on error
 */
static inline bool testrun_log_create_timestamp_save(
        bool micro, char *array, size_t size) {

        if ((NULL == array) || (size < 21))
                return false;

        if (micro)
                if (size < 28)
                        return false;

        time_t t;
        time(&t);

        struct timeval tv;
        char time_utc[25];

        if (0 != gettimeofday(&tv, NULL))
                return false;

        if (!micro){

                if (!strftime(array, size, "%FT%TZ", gmtime(&tv.tv_sec)))
                        return false;
                return true;
        }

        if (!strftime(time_utc, 25, "%FT%T", gmtime(&tv.tv_sec)))
                        return false;

        if (!snprintf(array, size, "%s.%.6jdZ",
                time_utc, (int64_t) (tv.tv_usec)))
                return false;

        return true;
}

/*      ------------------------------------------------------------------------
 *
 *      EXAMPLE LOG METHODS
 *
 *      The following part includes some example log destinations and log
 *      formatting.
 *
 *      ------------------------------------------------------------------------
 */

#define testrun_log_file_print_plain(file, fmt, ...) \
        fprintf(file, fmt "\n", ##__VA_ARGS__)

#define testrun_log_file_print_level_plain(file, level, fmt, ...) \
        fprintf(file, "|%d| " fmt "\n", level, ##__VA_ARGS__)

#define testrun_log_file_print_time_plain(file, level, timestamp, fmt, ...) \
        fprintf(file, "%s |%d| " \
                fmt "\n", timestamp, level, ##__VA_ARGS__)

#define testrun_log_file_print_all_plain(file, level, timestamp, fmt, ...) \
        fprintf(file, "%s |%d| " \
                "file: %s func: %s line: %d " \
                fmt "\n", \
                timestamp, level, \
                __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define testrun_log_file_print_time_json(file, level, timestamp, fmt, ...) \
        fprintf(file, "{\"timestamp\":\"%s\","\
                        "\"level\":%d," \
                        "\t\"message\":\"" fmt "\"\n}",\
                        timestamp, level, ##__VA_ARGS__)

#define testrun_log_file_print_all_json_format1(file, lvl, tstamp, fmt, ...) \
        fprintf(file, "{\n\t\"level\":%d,\n" \
                        "\t\"timestamp\":\"%s\",\n"\
                        "\t\"source\":\n"\
                        "\t{\n"\
                        "\t\t\"file\":\"%s\",\n" \
                        "\t\t\"func\":\"%s\",\n" \
                        "\t\t\"line\":%d\n" \
                        "\t},\n"\
                        "\t\"message\":\"" fmt "\"\n"\
                        "}\n",\
                        lvl, tstamp, \
                        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define testrun_log_file_print_all_json_format2(file, lvl, tstamp, fmt, ...) \
        fprintf(file, "{\n\t\"level\":%d,\n" \
                        "\t\"timestamp\":\"%s\",\n"\
                        "\t\"source\":"\
                        "{"\
                        "\"file\":\"%s\"," \
                        "\"func\":\"%s\"," \
                        "\"line\":%d" \
                        "},\n"\
                        "\t\"message\":\"" fmt "\"\n"\
                        "}\n",\
                        lvl, tstamp, \
                        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define testrun_log_file_print_all_json_no_format(file, lvl, tstamp, fmt, ...) \
        fprintf(file, "{\"level\":%d," \
                        "\"timestamp\":\"%s\","\
                        "\"source\":"\
                        "{"\
                        "\"file\":\"%s\"," \
                        "\"func\":\"%s\"," \
                        "\"line\":%d" \
                        "},"\
                        "\t\"message\":\"" fmt "\"\n"\
                        "}\n",\
                        lvl, tstamp, \
                        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/*      ------------------------------------------------------------------------
 *
 *      FILE ROUTER
 *
 *      testrun_log_file may be used to route to the desired file based logging
 *      method.
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

/*
 *      Thread save forwarding of log messages,
 *      using local timestamp bufferering.
 */
#define testrun_log_file(level, file, ...) \
        do { \
                char timestamp[30]; \
                if (!testrun_log_create_timestamp_save(true, timestamp, 30))\
                        testrun_log_file_print_level_plain(\
                                file, level, __VA_ARGS__);\
                else \
                        testrun_log_file_print_all_json_format2(\
                                file, level, timestamp, __VA_ARGS__); \
        } while(0)

/*
 *      Non thread save forwarding, using shared timestamp buffers.
 */
#define testrun_log_file_timestamp_shared(level, file, ...) \
        testrun_log_file_print_all_json_format2(file, level, \
                testrun_log_create_timestamp(true), __VA_ARGS__)

/*      ------------------------------------------------------------------------
 *
 *      MESSAGE ROUTER
 *
 *      testrun_log_forward is used to forward the internal log messages to the
 *      external logging environment. This function MUST be adapted to the
 *      needs of a project if testrun_log.h shall be used as logging interface
 *      within a project.
 *
 *      ------------------------------------------------------------------------
 */

#define testrun_log_forward(level, ...) \
        testrun_log_file(level, stdout, __VA_ARGS__)\
        testrun_log_journal(level, __VA_ARGS__ )

/*      ------------------------------------------------------------------------
 *
 *      INTERNAL INTERFACE
 *
 *      The following functions are used within the tool to log messages.
 *      Any logging may be redirected, using a redirection of this functions.
 *
 *      ------------------------------------------------------------------------
 */


/* Development logging */
#define log_dev(...)          testrun_log_file(LOG_DEBUG+1, stderr, __VA_ARGS__)

/* special logging */
#define log_oom(...)          log_critical("Out of memory.");

/* Normal logging */
#define log_debug(...)        testrun_log_forward(LOG_DEBUG,   __VA_ARGS__)
#define log_info(...)         testrun_log_forward(LOG_INFO,    __VA_ARGS__)
#define log_notice(...)       testrun_log_forward(LOG_NOTICE,  __VA_ARGS__)
#define log_warning(...)      testrun_log_forward(LOG_WARNING, __VA_ARGS__)
#define log_error(...)        testrun_log_forward(LOG_ERR,     __VA_ARGS__)
#define log_critical(...)     testrun_log_forward(LOG_CRIT,    __VA_ARGS__)
#define log_alert(...)        testrun_log_forward(LOG_ALERT,   __VA_ARGS__)
#define log_emergency(...)    testrun_log_forward(LOG_EMERG,   __VA_ARGS__)

#endif /* testrun_log_h */
