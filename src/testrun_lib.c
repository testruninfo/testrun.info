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

        This file is part of the testrun project. http://testrun.info

        ------------------------------------------------------------------------
*//**
        @file           testrun_lib.c
        @author         Markus Toepfer
        @date           2018-07-06

        @ingroup        testrun_lib

        @brief          Implementation of a default testrun_lib library instance.

        ------------------------------------------------------------------------
*/
#include "../include/testrun_lib.h"

bool testrun_lib_create_project_paths(const testrun_lib *lib);
bool testrun_lib_create_project_files(const testrun_lib *lib);
bool testrun_lib_create_module_files(const testrun_lib *lib, const char *module_name);

struct testrun_lib testrun_lib_default(){

        struct testrun_lib lib = {

                .config = testrun_config_default(),
                .tools  = testrun_tools_default(),

                .create_project_paths   = testrun_lib_create_project_paths,
                .create_project_files   = testrun_lib_create_project_files,
                .search_project_path    = testrun_utils_search_project_path,
                .create_module_files    = testrun_lib_create_module_files

        };

        return lib;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_validate(const testrun_lib *self){

        if (!self)
                return false;

        // check required config
        if (!testrun_config_validate(&self->config))
                return false;

        // check required tools
        if (!testrun_tools_validate(&self->tools))
                return false;

        // check required functions
        if (    !self->create_project_paths ||
                !self->create_project_files ||
                !self->search_project_path  ||
                !self->create_module_files)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project_paths(const testrun_lib *lib){

        if (!lib)
                return false;

        return testrun_utils_create_project_paths(&lib->config);
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_makefiles(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        char *content = NULL;

        content = lib->tools.makefile_configurable(
                        lib->config.project.name,
                        lib->config.makefile.configurable,
                        lib->config.version,
                        lib->config.cflags,
                        lib->config.project.url,
                        lib->config.project.description,
                        lib->config.path.service,
                        lib->config.makefile.common);

        if (!content)
                goto error;

        if (!testrun_utils_create_file(
                       lib->config.makefile.configurable,
                       content,
                       project_root_path,
                       NULL))
                goto error;

        free(content);
        content = NULL;

        content = lib->tools.makefile_common(
                        lib->config.project.name,
                        lib->config.makefile.common,
                        lib->config.path.bin,
                        lib->config.path.build,
                        lib->config.path.include,
                        lib->config.path.source,
                        lib->config.path.tests,
                        lib->config.path.tools,
                        lib->config.path.doxygen,
                        lib->config.test_suffix_source,
                        lib->config.test_suffix_exec,
                        lib->config.script.test_unit,
                        lib->config.script.test_acceptance,
                        lib->config.script.test_coverage,
                        lib->config.script.lines_of_code,
                        lib->config.script.test_gcov,
                        lib->config.script.test_gprof,
                        lib->config.makefile.target);
        if (!content)
                goto error;

        if (!testrun_utils_create_file(
                       lib->config.makefile.common,
                       content,
                       project_root_path,
                       NULL))
                goto error;

        free(content);
        content = NULL;
        return true;

error:
        if (content)
                free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_gitignore(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        char *content = lib->tools.gitignore();
        if (!content)
                goto error;

        if (!testrun_utils_create_file(
                       TESTRUN_FILE_GIT_IGNORE,
                       content,
                       project_root_path,
                       NULL))
                goto error;

        free(content);
        content = NULL;

        return true;

error:
        if (content)
                free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_readme(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        char *time_string = testrun_time_string(TESTRUN_SCOPE_YEAR);

        char *copyright = lib->config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                time_string,
                                lib->config.copyright.owner,
                                lib->config.copyright.note,
                                NULL,
                                lib->config.indent.standard,
                                true,
                                lib->config.copyright.gpl_parameter);

        free(time_string);
        time_string = NULL;

        if (!copyright)
                goto error;

        char *content = lib->tools.readme(
                lib->config.project.name, 
                lib->config.project.description, 
                copyright);

        free(copyright);
        copyright = NULL;

        if (!content)
                goto error;

        if (!testrun_utils_create_file(
                       TESTRUN_FILE_README,
                       content,
                       project_root_path,
                       NULL))
                goto error;

        free(content);
        content = NULL;

        return true;

error:
        if (content)
                free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_changelog(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        char content[1000];
        memset(content, 0, 1000);

        char *time_string =  testrun_time_string(TESTRUN_SCOPE_MINUTE);

        if (!snprintf(content, 1000, "Project created at %s", time_string))
                goto error;


        if (!testrun_utils_create_file(
                       TESTRUN_FILE_CHANGELOG,
                       content,
                       project_root_path,
                       lib->config.path.docs))
                goto error;

        free(time_string);
        return true;

error:
        if (time_string)
                free(time_string);

        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_doxygen(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        if (!lib->config.path.doxygen)
                return false;

        char input[1000];
        memset(input, 0, 1000);

        if (!snprintf(input, 1000, "%s %s %s %s",
                TESTRUN_FILE_README, 
                lib->config.path.include, 
                lib->config.path.source,
                lib->config.path.tests))
                return false;

        char *content = lib->tools.doxygen(
                lib->config.project.name,
                lib->config.path.doxygen,
                TESTRUN_FILE_README,
                input);

        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       TESTRUN_FILE_DOXYGEN,
                       content,
                       project_root_path,
                       lib->config.path.doxygen))
                goto error;

        free(content);
        content = NULL;

        return true;

error:
        if (content)
                free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_copyright(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        char *time_string = testrun_time_string(TESTRUN_SCOPE_YEAR);

        char *content = lib->config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                time_string,
                                lib->config.copyright.owner,
                                lib->config.copyright.note,
                                NULL,
                                lib->config.indent.standard,
                                true,
                                lib->config.copyright.gpl_parameter);

        free(time_string);
        time_string = NULL;

        if (!content)
                goto error;

        if (!testrun_utils_create_file(
                       TESTRUN_FILE_COPYRIGHT,
                       content,
                       project_root_path,
                       lib->config.path.copyright))
                goto error;

        free(content);
        content = NULL;

        if (!lib->config.copyright.copyright.generate_full_text_licence)
                return true;

        content = lib->config.copyright.copyright.generate_full_text_licence(
                        lib->config.copyright.gpl_parameter);

        if (!content)
                return true;

        if (!testrun_utils_create_file(
                       TESTRUN_FILE_COPYRIGHT_FULL,
                       content,
                       project_root_path,
                       lib->config.path.copyright))
                goto error;

        free(content);
        content = NULL;

        return true;

error:
        if (time_string)
                free(time_string);
        if (content)
                free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_service(
        const char* project_root_path, const testrun_lib *lib){

        if (!project_root_path || !lib)
                return false;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        // create service file

        if (!snprintf(path, PATH_MAX, "%s/%s",
                TESTRUN_DEFAULT_INSTALL_PATH, 
                lib->config.project.name))
                return false;

        char *content = lib->tools.service_file(
                lib->config.project.name,
                path);

        if (!content)
                goto error;

        memset(path, 0, PATH_MAX);
        if (!snprintf(path, PATH_MAX, "%s%s",
                lib->config.project.name,
                TESTRUN_FILE_SERVICE_EXT))
                return false;

        if (!testrun_utils_create_file(
                       path,
                       content,
                       project_root_path,
                       lib->config.path.service))
                goto error;

        free(content);
        content = NULL;

        // create socket file

        content = lib->tools.socket_file(lib->config.project.name);
        if (!content)
                goto error;

        memset(path, 0, PATH_MAX);
        if (!snprintf(path, PATH_MAX, "%s%s",
                lib->config.project.name,
                TESTRUN_FILE_SOCKET_EXT))
                return false;

        if (!testrun_utils_create_file(
                       path,
                       content,
                       project_root_path,
                       lib->config.path.service))
                goto error;

        free(content);
        content = NULL;

        // create service readme

        content = 
        "In case of service configuration the following files will be copied to the system:"
        "(1) Everything under ./etc will be copied as is to /etc/ during installation."
        "(2) *.service and *.socket will be copied to /etc/systemd/system during installation.";

        if (!testrun_utils_create_file(
                       TESTRUN_FILE_README,
                       content,
                       project_root_path,
                       lib->config.path.service)){
                content = NULL;
                goto error;
        }

        return true;

error:
        if (content)
                free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_c_documentation_header(
        char *ptr,
        size_t size,
        const char *filename,
        const char *author,
        const char *ingroup,
        const char *brief){

        if (!ptr || !filename || size < 1)
                return false;

        if (!author)
                author = TESTRUN_TAG_AUTHOR;

        if (!ingroup)
                ingroup = TESTRUN_TAG_GROUP;

        if (!brief)
                brief = "...";

        char *date = testrun_time_string(TESTRUN_SCOPE_DAY);

        size_t result = snprintf(ptr, size,
                "%s"
                "        @file           %s\n"
                "        @author         %s\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        %s\n"
                "\n"
                "        @brief          %s\n"
                "\n"
                "%s\n",
                TESTRUN_DOCUMENTATION_PREFIX,
                filename,
                author,
                date,
                ingroup,
                brief,
                TESTRUN_DOCUMENTATION_SUFFIX);

        if (result == 0)
                return false;

        if (result >= size)
                return false;

        free(date);

        return true;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_c_header(
        const char* project_root_path, 
        const testrun_lib *lib, 
        const char *copyright_header,
        const char *name){

        if (!project_root_path || !lib || !copyright_header || !name)
                return false;

        char content[5000];
        memset(content, 0, 5000);

        char documentation_header[5000];
        memset(documentation_header, 0, 5000);

        char filename[5000];
        memset(filename, 0, 5000);

        if (!snprintf(filename, 5000, "%s%s",
                name,
                TESTRUN_SUFFIX_C_HEADER))
                goto error;

        if (!testrun_lib_create_c_documentation_header(documentation_header, 5000,
                filename,
                lib->config.copyright.author,
                NULL, 
                "Definition of "))
                goto error;


        if (!snprintf(content, 5000, "%s%s"
                "#ifndef %s_h\n"
                "#define %s_h\n"
                "\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      GENERIC FUNCTIONS\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "\n"
                "#endif /* %s_h */\n",
                copyright_header,
                documentation_header,
                name,
                name,
                name))
                goto error;

        

        if (!testrun_utils_create_file(
                       filename,
                       content,
                       project_root_path,
                       lib->config.path.include))
                goto error;

        return true;
error:
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_c_source(
        const char* project_root_path, 
        const testrun_lib *lib, 
        const char *copyright_header,
        const char *name){

        if (!project_root_path || !lib || !copyright_header || !name)
                return false;

        char content[5000];
        memset(content, 0, 5000);

        char documentation_header[5000];
        memset(documentation_header, 0, 5000);

        char filename[5000];
        memset(filename, 0, 5000);

        if (!snprintf(filename, 5000, "%s%s",
                name,
                TESTRUN_SUFFIX_C_SOURCE))
                goto error;
        if (!testrun_lib_create_c_documentation_header(documentation_header, 5000,
                filename,
                lib->config.copyright.author,
                NULL, 
                "Implementation of "))
                goto error;

        if (!snprintf(content, 5000, "%s%s"
                "#include \"%s%s/%s%s\"\n",
                copyright_header,
                documentation_header,
                lib->config.path.source_to_root,
                lib->config.path.include,
                name,
                TESTRUN_SUFFIX_C_HEADER))
                goto error;

        if (!testrun_utils_create_file(
                       filename,
                       content,
                       project_root_path,
                       lib->config.path.source))
                goto error;

        return true;
error:
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_c_unit_test(
        const char* project_root_path, 
        const testrun_lib *lib, 
        const char *copyright_header, 
        const char *name){

        if (!project_root_path || !lib || !copyright_header || !name)
                return false;

        char content[5000];
        memset(content, 0, 5000);

        char documentation_header[5000];
        memset(documentation_header, 0, 5000);

        char filename[5000];
        memset(filename, 0, 5000);

        char path[5000];
        memset(path, 0, 5000);

        if (!snprintf(path, 5000, "%s/%s",
                lib->config.path.tests,
                TESTRUN_FOLDER_UNIT_TESTS))
                goto error;

        if (!snprintf(filename, 5000, "%s%s%s",
                name,
                lib->config.test_suffix_source,
                TESTRUN_SUFFIX_C_SOURCE))
                goto error;

        if (!testrun_lib_create_c_documentation_header(documentation_header, 5000,
                filename,
                lib->config.copyright.author,
                NULL, 
                "Unit tests of "))
                goto error;

        if (!snprintf(content, 5000, "%s%s"
                "#include \"%s%s/testrun.h\"\n"
                "#include \"%s%s/%s%s\"\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST CASES                                                      #CASES\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "int test_case(){\n"
                "        testrun(1 == 1);\n"
                "\n"
                "        return testrun_log_success();\n"
                "}\n"
                "\n"
                "/*----------------------------------------------------------------------------*/\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST CLUSTER                                                    #CLUSTER\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "int all_tests() {\n"
                "\n"
                "        testrun_init();\n"
                "        testrun_test(test_case);\n"
                "\n"
                "        return testrun_counter;\n"
                "}\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST EXECUTION                                                  #EXEC\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "testrun_run(all_tests);\n",
                copyright_header,
                documentation_header,
                lib->config.path.unit_tests_to_root,
                lib->config.path.tools,
                lib->config.path.unit_tests_to_root,
                lib->config.path.source, 
                name,
                TESTRUN_SUFFIX_C_SOURCE
                ))
                goto error;

        if (!testrun_utils_create_file(
                       filename,
                       content,
                       project_root_path,
                       path))
                goto error;

        return true;
error:
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_testrun_scripts(const testrun_lib *lib){

        if (!testrun_lib_validate(lib))
                goto error;

        char project_path[PATH_MAX];
        memset(project_path, 0, PATH_MAX);

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        char path2[PATH_MAX];
        memset(path2, 0, PATH_MAX);

        if (!testrun_utils_generate_project_root_path(
                &lib->config, project_path, PATH_MAX))
                return false;

        char *content = NULL;

        content = lib->tools.testrun_header();
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       TESTRUN_TESTRUN_HEADER,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;


        free(content);
        content = lib->tools.testrun_header_openmp();
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       TESTRUN_TESTRUN_HEADER_OPENMP,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        free(content);
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", lib->config.path.tests, TESTRUN_FOLDER_UNIT_TESTS);
        content = lib->tools.testrun_simple_tests(
                TESTRUN_FOLDER_UNIT_TESTS,
                lib->config.project.name,
                lib->config.script.test_unit,
                lib->config.script.test_runner,
                lib->config.path.logfile,
                path,
                lib->config.path.tools);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.test_unit,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        if (!testrun_utils_chmod(
                        lib->config.script.test_unit,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        free(content);
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", lib->config.path.tests, TESTRUN_FOLDER_ACCEPTANCE_TESTS);
        content = lib->tools.testrun_simple_tests(
                TESTRUN_FOLDER_ACCEPTANCE_TESTS,
                lib->config.project.name,
                lib->config.script.test_unit,
                lib->config.script.test_runner,
                lib->config.path.logfile,
                path,
                lib->config.path.tools);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.test_acceptance,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        if (!testrun_utils_chmod(
                        lib->config.script.test_acceptance,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        free(content);
        content = lib->tools.testrun_runner(
                lib->config.project.name,
                lib->config.script.test_runner);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.test_runner,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        if (!testrun_utils_chmod(
                        lib->config.script.test_runner,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        free(content);
        content = lib->tools.testrun_loc(
                lib->config.project.name,
                lib->config.script.lines_of_code,
                lib->config.path.include,
                lib->config.path.source,
                lib->config.path.tests);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.lines_of_code,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        if (!testrun_utils_chmod(
                        lib->config.script.lines_of_code,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        free(content);
        content = lib->tools.testrun_simple_coverage(
                lib->config.project.name,
                lib->config.script.test_coverage,
                lib->config.test_prefix,
                lib->config.path.logfile,
                lib->config.path.source,
                lib->config.path.tests);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.test_coverage,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        if (!testrun_utils_chmod(
                        lib->config.script.test_coverage,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        free(content);
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/tests/%s", lib->config.path.build, TESTRUN_FOLDER_UNIT_TESTS);
        memset(path2, 0, PATH_MAX);
        snprintf(path2, PATH_MAX, "%s/%s", lib->config.path.tests, TESTRUN_FOLDER_UNIT_TESTS);
        content = lib->tools.testrun_gcov(
                lib->config.project.name,
                lib->config.script.test_gcov,
                lib->config.path.logfile,
                path,
                path2,
                lib->config.test_suffix_exec,
                lib->config.test_suffix_source);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.test_gcov,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;

        if (!testrun_utils_chmod(
                        lib->config.script.test_gcov,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        free(content);
        content = lib->tools.testrun_gprof(
                lib->config.project.name,
                lib->config.script.test_gprof,
                lib->config.path.logfile,
                path,
                lib->config.test_suffix_exec);
        if (!content)
                return false;

        if (!testrun_utils_create_file(
                       lib->config.script.test_gprof,
                       content,
                       project_path,
                       lib->config.path.tools))
                goto error;
        free(content);

        if (!testrun_utils_chmod(
                        lib->config.script.test_gprof,
                        project_path,
                        lib->config.path.tools,
                        0751))
                goto error;

        return true;

error:
        if (content)
                free(content);
        return false;

}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project_files(const testrun_lib *lib){

        if (!lib)
                return false;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        if (!testrun_utils_generate_project_root_path(
                &lib->config, path, PATH_MAX))
                return false;

        if (!testrun_lib_create_makefiles(path, lib))
                return false;

        if (!testrun_lib_create_gitignore(path, lib))
                return false;

        if (!testrun_lib_create_readme(path, lib))
                return false;

        if (!testrun_lib_create_changelog(path, lib))
                return false;

        if (!testrun_lib_create_copyright(path, lib))
                return false;

        if (!testrun_lib_create_testrun_scripts(lib))
                return false;

        if (lib->config.path.doxygen)
                if (!testrun_lib_create_doxygen(path, lib))
                        return false;

        if (lib->config.path.service)
                if (!testrun_lib_create_service(path, lib))
                        return false;

        return lib->create_module_files((testrun_lib*) lib, lib->config.project.name);

}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_module_files(
        const testrun_lib *lib,
        const char *module_name){

        if (!lib || !module_name)
                return false;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        if (!testrun_utils_generate_project_root_path(
                &lib->config, path, PATH_MAX))
                return false;

        fprintf(stdout, "\n\nPATH %s\n", path);

        char *time_string = testrun_time_string(TESTRUN_SCOPE_YEAR);

        char *copyright_string = lib->config.copyright.copyright.generate_header_string(
                                TESTRUN_COPYRIGHT_PREFIX,
                                COPYRIGHT_DEFAULT_INTRO,
                                time_string,
                                lib->config.copyright.owner,
                                lib->config.copyright.note,
                                TESTRUN_COPYRIGHT_SUFFIX,
                                lib->config.indent.c,
                                true,
                                lib->config.copyright.gpl_parameter);

        free(time_string);

        if (!copyright_string)
                goto error;

        if (!testrun_lib_create_c_header(
                path, 
                lib, 
                copyright_string,
                module_name))
                goto error;

        if (!testrun_lib_create_c_source(
                path, 
                lib, 
                copyright_string,
                module_name))
                goto error;

        if (!testrun_lib_create_c_unit_test(
                path, 
                lib, 
                copyright_string,
                module_name))
                goto error;

        free(copyright_string);
        return true;
error:
        if (copyright_string)
                free(copyright_string);
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project(const testrun_lib *lib){

        if (!testrun_lib_validate(lib))
                goto error;

        fprintf(stdout, "START creating a new project %s\n",
                lib->config.project.name);

        if (!lib->create_project_paths(lib))
                goto error;

        if (!lib->create_project_files(lib))
                goto error;

        return true;
error:
        log_error("Failed to create a new project.");
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_module(const testrun_lib *lib, const char* module_name){

        if (!testrun_lib_validate(lib))
                goto error;

        testrun_lib copy = *lib;
        bool result = false;

        char *project_path = lib->search_project_path(lib->config.project.path);
        if (!project_path){
                log_error("Failed to identify a project root path from starting PATH %s", lib->config.project.path);
                goto error;
        }

        copy.config.project.name = basename(project_path);
        copy.config.project.path = dirname(project_path);

        result = lib->create_module_files(&copy, module_name);
        free(project_path);

        return result;

error:
        log_error("Failed to create a new module.");
        return false;
}