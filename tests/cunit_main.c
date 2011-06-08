/*
** Copyright (C) 2010 Fargier Sylvain <fargier.sylvain@free.fr>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** cunit_main.c
**
**        Created on: Jul 6, 2010
**            Author: fargie_s
**
*/

#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "cunit_helper.h"

#ifndef XML_TEST_REPORT
#define XML_TEST_REPORT "test-report.xml"
#endif

static unsigned char is_init = 0;

void init_cunit() {
    if (is_init == 0) {
        is_init = 1;
        /* initialize the CUnit test registry */
        if (CUE_SUCCESS != CU_initialize_registry()) {
            fprintf(stderr, "%s\n", CU_get_error_msg());
            return;
        }
    }
}

#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief print suites and tests list on output
 *
 * @param[in]  suite The first suite to print
 * @param[out] output The out FILE *
 */
static void printSuites(const CU_pSuite suite, FILE *output)
{
    unsigned int i;
    CU_pTest test;

    if (!suite) {
        fprintf(output, "Failed to print NULL suite");
        return;
    }
    fprintf(output, "%s\n", suite->pName);
    for (test = suite->pTest; test != NULL; test = test->pNext)
        fprintf(output, "%s::%s\n", suite->pName, test->pName);

    if (suite->pNext)
        printSuites(suite->pNext, output);

}

/**
 * @brief copies a suite from one registry to another
 *
 * @param[in] in_registry input registry
 * @param[in] suite_name the test suite name
 * @param[in] test_name the test name
 * @return
 *  - 0 when no test or suite have been copied
 *  - 1 if a suite/test has been copied
 *
 * @details set test to NULL to copy the whole suite
 */
static unsigned char copySuite(const CU_pTestRegistry in_registry,
        const char *suite_name,
        const char *test_name)
{
    unsigned char ret;
    CU_pSuite old_suite, new_suite;
    CU_pTest old_test;

    old_suite = CU_get_suite_by_name(suite_name, in_registry);

    if (!old_suite)
        return 0;

    new_suite = CU_get_suite_by_name(suite_name, CU_get_registry());
    if (!new_suite)
        new_suite = CU_add_suite(old_suite->pName, old_suite->pInitializeFunc,
                old_suite->pCleanupFunc);

    if (test_name) {
        old_test = CU_get_test_by_name(test_name, old_suite);

        if (old_test)
            CU_add_test(new_suite, old_test->pName, old_test->pTestFunc);
        else
            return 0;
    }
    else {
        for (old_test = old_suite->pTest; old_test; old_test = old_test->pNext)
            CU_add_test(new_suite, old_test->pName, old_test->pTestFunc);
    }
    return 1;
}

#endif

int main(int argc, char * const *argv)
{
    int ret = 0;
#ifdef HAVE_GETOPT_LONG
    int c;
    struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"list", 0, 0, 'l'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, "hl", long_options, NULL)) != -1) {
        switch (c) {
            case 'h':
                fprintf(stderr, "Usage: %s [test name]\n", argv[0]);
                fputs("\nOptions:\n", stderr);
                fputs("  -h, --help           Show this help message and exit\n", stderr);
                fputs("  -l, --list           List available tests and exit\n", stderr);
                exit(EXIT_SUCCESS);
            case 'l':
                fputs("Available test suites:\n", stdout);;
                printSuites(CU_get_registry()->pSuite, stdout);
                exit(EXIT_SUCCESS);
        }
    }

    if (optind < argc) {
        CU_pTestRegistry old_registry = CU_get_registry();
        CU_set_registry(CU_create_new_registry());

        while (optind < argc) {
            char *suite_name = strdup(argv[optind++]);
            char *test_name = strstr(suite_name, "::");

            if (test_name) {
                *test_name = '\0';
                test_name += 2;
            }

            if (copySuite(old_registry, suite_name, test_name) == 0) {
                fprintf(stderr, "Can't find test suite \"%s", suite_name);
                if (test_name)
                    fprintf(stderr, "::%s\"\n", test_name);
                else
                    fputs("\"\n", stderr);
                exit(EXIT_FAILURE);
            }

            free(suite_name);
        }
        CU_destroy_existing_registry(&old_registry);
    }
#endif

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_set_output_filename(XML_TEST_REPORT);
    CU_basic_run_tests();
    ret = (CU_get_number_of_tests_failed() == 0) ? 0 : 1;
    CU_cleanup_registry();

    return ret;
}

