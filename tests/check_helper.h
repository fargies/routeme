/*
** Copyright (C) 2011 Fargier Sylvain <fargier.sylvain@free.fr>
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
** check_helper.h
**
**        Created on: Jun 03, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef __CHECK_HELPER_H__
#define __CHECK_HELPER_H__

#include <sys/queue.h>

LIST_HEAD(test_list, test);
typedef struct {
    void (*)() test;
    const char *name;
    LIST_ENTRY(suite) entries;
} suite;

#define xstr(s) str(s)
#define str(s) #s

#define TEST_SUITE_CREATE(name) \
    extern SRunner *srunner; \
    void name ## _creator() __attribute__ ((constructor, used)) { \
        Suite *suite = suite_create(xstr(name)); \
        TCase *tcase;

#define TEST_SUITE_END() \
        TEST_SUITE_REGISTER(runner, suite); \
    }

#define TEST_CASE_CREATE(name) \
    tcase = tcase_create(xstr(name));

#define TEST_ADD(test) \
    tcase_add_test(tcase, test);

#define TEST_CASE_END() \
    suite_add_tcase(suite, tcase);

#define TEST_SUITE_REGISTER(runner, suite) \
do { \
    Suite *suite ## _creator();\
    if (runner == NULL) \
        runner = srunner_create(suite ## _creator()); \
    else \
        srunner_add_suite(runner, suite ## _creator()); \
} while(0);

#endif

