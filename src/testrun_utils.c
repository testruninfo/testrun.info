/***
        ------------------------------------------------------------------------

        Copyright 2018 [COPYRIGHT_OWNER]

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
        @file           testrun_utils.c
        @author         [AUTHOR]
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#include "../include/testrun_utils.h"
#include "../include/testrun_log.h"

char *testrun_utils_insert_at_each_line(
        const char* text,
        const char* intro,
        const char* outro){

        if (!text)
                return NULL;

        char *result = NULL;
        char *ptr    = NULL;
        char *nxt    = NULL;
        char *cur    = NULL;

        size_t len   = 0;
        size_t tlen  = 0;
        size_t ilen  = 0;
        size_t olen  = 0;
        size_t lines = 0;
        size_t size  = 0;

        if (text)
                tlen = strlen(text);

        if (intro)
                ilen = strlen(intro);

        if (outro)
                olen = strlen(outro);

        cur = (char*) text;
        nxt = memchr(cur, '\n', tlen);
        while (nxt) {

                lines++;
                cur = nxt + 1;
                nxt = memchr(cur, '\n', tlen - (cur - text)); 
        }

        if (0 == lines)
                goto error;

        size   = (lines * ilen) + (lines * olen) + tlen + 1;
        result = calloc(size, sizeof(char));
        if (!result)
                goto error;

        ptr = result;
        cur = (char*) text;
        nxt = memchr(cur, '\n', tlen);

        while (nxt) {

                len = nxt - cur;

                if (len > 0){

                        if (!strncat(ptr, intro, ilen))
                                goto error;
        
                        ptr += ilen;
        
                        if (!strncat(ptr, cur, len))
                                goto error;
        
                        ptr += len;
        
                        if (!strncat(ptr, outro, olen))
                                goto error;
        
                        ptr += olen;

                }

                if (!strcat(ptr, "\n"))
                        goto error;

                ptr ++;

                cur = nxt + 1;
                nxt = memchr(cur, '\n', tlen - (cur - text));
        }

        // copy rest after last linebreak
        len = tlen - (cur - text);
        if (len > 0)
                if (!strncat(ptr, cur, len))
                        goto error;

        return result;

error:
        if (result)
                free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

static bool testrun_utils_path_is_project_top_dir(
        char const * const path){

        if (!path)
                return false;

        bool include    = false;
        bool source     = false;
        bool tests      = false;

        DIR *dp;
        struct dirent *ep;

        dp = opendir(path);
        if (!dp)
                return false;

        while((ep = readdir (dp))) {
                if (ep->d_type == DT_DIR) {

                        if (!include) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_INCLUDE,
                                        strlen(TESTRUN_FOLDER_INCLUDE))
                                        == 0) {
                                        include = true;
                                        continue;
                                }
                        }

                        if (!source) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_SOURCE,
                                        strlen(TESTRUN_FOLDER_SOURCE))
                                        == 0) {
                                        source = true;
                                        continue;
                                }
                        }

                        if (!tests) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_TESTS,
                                        strlen(TESTRUN_FOLDER_TESTS))
                                        == 0) {
                                        tests = true;
                                        continue;
                                }
                        }
                }
        }
        (void) closedir (dp);

    return (include && source && tests);
}

/*----------------------------------------------------------------------------*/

char* testrun_utils_search_project_path(const char *input){

        char *parent  = NULL;
        char *current = NULL;

        if (!input)
                return NULL;

        char path[PATH_MAX];

        current = realpath(input, current);
        if (!current)
                goto error;

        if (!testrun_utils_path_is_project_top_dir(current)){

                /* walk up one level */
                sprintf(path, "%s/%s/",current,"..");
                parent = realpath(path, parent);
                if (!parent)
                        goto error;

                if (strcmp(current, parent) == 0) {

                        goto error;

                } else {

                        free(current);
                        current = testrun_utils_search_project_path(
                                parent);

                        free(parent);
                        parent = NULL;

                }
        }

        return current;

error:
        if (current)
                free(current);
        if (parent)
                free(parent);
        return NULL;
}

/*----------------------------------------------------------------------------*/

static bool testrun_utils_create_path(const char *path){

        if (path)
                return false;

        size_t size = strlen(path);
        if (size > PATH_MAX)
                return false;

        bool created = false;

        DIR *dp;

        char *slash   = NULL;
        char *pointer = NULL;
        char *element = NULL;
        size_t open   = PATH_MAX;

        char new_path[open];
        bzero(new_path, open);

        pointer = path;
        while(size < (pointer - path)) {

                slash = memchr(pointer, '/', size - (pointer - path)); 

                if (snprintf(new_path, slash - path, "%s", path) < 0)
                        goto error;

                dp = opendir(new_path);
                if (dp){
                        // PATH exists access OK
                        (void) closedir (dp);

                } else if (mkdir(new_path,
                                S_IRUSR | S_IWUSR | S_IXUSR |
                                S_IRGRP | S_IXGRP |
                                S_IROTH | S_IXOTH) != 0){

                        goto error;

                } else {
                        created = true;
                }

                if (slash)
                        pointer = slash + 1;
        }

        log_debug("PATH OK %s (%s)", new_path, created ? "created" : "existing");
        return true;
error:
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_utils_create_relative_path(
        const char *root,
        const char *relative){

        if (!root || !relative)
                return false;

        if (strlen(root) + strlen(relative) + 1 > PATH_MAX)
                return false;

        char buffer[PATH_MAX];
        memset(buffer, 0, PATH_MAX);

        if (snprintf(buffer, PATH_MAX, "%s/%s",
                root, relative) < 0)
                return false;

        return testrun_utils_create_path(buffer);
}
/*----------------------------------------------------------------------------*/

bool testrun_utils_create_paths(struct testrun_config *config){

        if (!config || !testrun_config_validate(config))
                return false;

        // create project path
        if (!testrun_utils_create_path(config->project.path))
                return false;

        // create relative pathes
        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.bin))
                return false;

        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.build))
                return false;

        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.include))
                return false;

        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.source))
                return false;

        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.tests))
                return false;

        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.tools))
                return false;

        if (!testrun_utils_create_relative_path(
                config->project.path,
                config->path.service))
                return false;

        return true;
}