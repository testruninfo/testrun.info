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
        @file           testrun_utils.c
        @author         Markus Toepfer
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief          Implementation of supporting utilities for testrun_lib
                        development.

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

bool testrun_utils_generate_project_root_path(
        const struct testrun_config *config,
        char *buffer,
        size_t size){

        if (!config || !buffer)
                return false;

        if (!config->project.path || !config->project.name)
                return false;

        if (strlen(config->project.path) + strlen(config->project.name) + 2 > size)
                return false;

        if (!snprintf(buffer, size, "%s/%s",
                config->project.path,
                config->project.name))
                return false;

        return true;
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

        if (!path)
                return false;

        size_t size = strlen(path);
        if (size > PATH_MAX)
                return false;

        bool created = false;

        DIR *dp;

        char *slash   = NULL;
        char *pointer = NULL;
        size_t open   = PATH_MAX;

        char new_path[open];
        bzero(new_path, open);

        pointer = (char*) path;
        while(size > (size_t) (pointer - path)) {

                slash = memchr(pointer, '/', size - (pointer - path)); 
                if (slash == path) {
                        // creating absolute path
                        pointer++;
                        continue;
                }

                if (slash >= path + size)
                        break;

                if (!slash) {

                        if (0 > snprintf(new_path, size + 1, "%s", path))
                                goto error;

                } else {

                        if (0 > snprintf(new_path, (slash - path) + 1, "%s", path))
                                goto error;

                }

                dp = opendir(new_path);
                if (dp){
                        // PATH exists access OK
                        (void) closedir (dp);

                } else if (mkdir(new_path,
                                S_IRUSR | S_IWUSR | S_IXUSR |
                                S_IRGRP | S_IXGRP |
                                S_IROTH | S_IXOTH) != 0){

                        log_error("NO ACCESS TO PATH %s", new_path);
                        goto error;

                } else {
                        created = true;
                }

                if (slash) {
                        pointer = slash + 1;
                } else {
                        break;
                }
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

bool testrun_utils_create_project_paths(const struct testrun_config *config){

        if (!config || !testrun_config_validate(config))
                return false;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        char project_root[PATH_MAX];
        memset(project_root, 0, PATH_MAX);

        if (    (strlen(config->project.name) + 
                 strlen(config->project.path) + 1) > PATH_MAX)
                return false;

        if (!testrun_utils_generate_project_root_path(
                config, project_root, PATH_MAX))
                return false;

        // create project path
        if (!testrun_utils_create_path(project_root))
                return false;

        // create relative pathes
        if (!testrun_utils_create_relative_path(
                project_root,
                config->path.include))
                return false;

        if (!testrun_utils_create_relative_path(
                project_root,
                config->path.source))
                return false;

        if (!testrun_utils_create_relative_path(
                project_root,
                config->path.tests))
                return false;

        if (!testrun_utils_create_relative_path(
                project_root,
                config->path.tools))
                return false;

        if (!testrun_utils_create_relative_path(
                project_root,
                config->path.docs))
                return false;

        if (!testrun_utils_create_relative_path(
                project_root,
                config->path.copyright))
                return false;

        if (config->path.service){

                if (!testrun_utils_create_relative_path(
                        project_root,
                        config->path.service))
                        return false;

                // create folder for default config
                memset(path, 0, PATH_MAX);
                if (!snprintf(path, PATH_MAX, "%s/%s/etc/%s",
                        project_root,
                        config->path.service,
                        config->project.name))
                        return false;

                if (!testrun_utils_create_path(path))
                        return false;

        }

        if (config->path.doxygen)
                if (!testrun_utils_create_relative_path(
                        project_root,
                        config->path.doxygen))
                        return false;

        // create default test folders under config->path.tests
        memset(path, 0, PATH_MAX);
        if (!snprintf(path, PATH_MAX, "%s/%s",
                project_root,
                config->path.tests))
                return false;

        if (!testrun_utils_create_relative_path(
                path,
                TESTRUN_FOLDER_UNIT_TESTS))
                return false;

        if (!testrun_utils_create_relative_path(
                path,
                TESTRUN_FOLDER_ACCEPTANCE_TESTS))
                return false;
        
        if (!testrun_utils_create_relative_path(
                path,
                TESTRUN_FOLDER_RESOURCES))
                return false;


        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_utils_get_git_author(
        char *buffer, size_t size){

        if (!buffer || size < 1)
                return false;

        bool set = false;
        FILE *in;
        FILE *popen();

        if((in = popen("git config user.name", "r")) != NULL){

                if (fgets(buffer, size, in)!=NULL){

                        fprintf(stdout, "... using git username %s as AUTHOR\n",
                                buffer);
                        set = true;

                }
                pclose(in);
        }

        if (!set) {

                // FALLBACK default AUTHOR TAG
                if (snprintf(buffer, size, "%s",
                        TESTRUN_TAG_AUTHOR) < 0){
                        goto error;
                }
        }

        return true;
error:
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_utils_create_file(
        const char *filename,
        const char *content,
        const char *root_path,
        const char *relative_path){

        if (!filename || !content || !root_path)
                return false;

        FILE *file;

        int r;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        if (relative_path) {

                if (snprintf(path, PATH_MAX, "%s/%s",
                        root_path, relative_path) < 0)
                        return false;
        } else {

                if (snprintf(path, PATH_MAX, "%s",
                        root_path) < 0)
                        return false;
        }

        // ensure create path
        if (!testrun_utils_create_path(path))
                return false;

        // add file to path
        if (relative_path) {

                if (snprintf(path, PATH_MAX, "%s/%s/%s",
                        root_path, relative_path, filename) < 0)
                        return false;
        } else {

                if (snprintf(path, PATH_MAX, "%s/%s",
                        root_path, filename) < 0)
                        return false;
        }

        if (access(path, F_OK) != -1){
                log_error("File exists %s", path);
                return false;
        }

        file = fopen(path, "w");
        if (!file) {
                log_error("Could not create/open file %s\n", path);
                return false;
        }

        r = fputs(content, file);
        if (r < 0)
                return false;

        fclose(file);

        log_info("Wrote file %s", path);

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_utils_chmod(
        const char *filename,
        const char *absolute_path,
        const char *relative_path,
        int flags){

        if (!filename || !absolute_path)
                return false;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        if (relative_path) {

                if (snprintf(path, PATH_MAX, "%s/%s/%s",
                        absolute_path, relative_path, filename) < 0)
                        return false;
        } else {

                if (snprintf(path, PATH_MAX, "%s/%s",
                        absolute_path, filename) < 0)
                        return false;
        }

        if (access(path, F_OK) == -1)
                return false;

        if ( 0 != chmod(path, flags)) 
                return false;

        return true;
}

