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
** cunit_helper.h
**
**        Created on: Jul 6, 2010
**            Author: fargie_s
**
*/


#ifndef CUNIT_HELPER_H_
#define CUNIT_HELPER_H_

#include <CUnit/CUnit.h>
#include <stdio.h>

typedef void (*test_func_t)(void);
void init_cunit();

#define SUITE_BEGIN(name, setup, teardown) \
    static __attribute__ ((constructor)) void ctor ##name(void) {\
        init_cunit(); \
        CU_pSuite suite = CU_add_suite(#name, setup, teardown); \
        if (NULL == suite) { \
            CU_cleanup_registry(); \
            fprintf(stderr, "%s\n", CU_get_error_msg()); \
            return; \
        }

#define REGISTER_TEST(test_func) \
        CU_add_test(suite, #test_func, test_func);

#define SUITE_END() \
    }

/* GLIB specific */
#define CU_ASSERT_GERROR(error) { \
  if (error) \
    printf("Error: %s\n", error->message); \
  CU_ASSERT_FATAL(error == NULL); \
}

#endif /* CUNIT_HELPER_H_ */
