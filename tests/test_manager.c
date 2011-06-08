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
** test_manager.c
**
**        Created on: Jun 04, 2011
**   Orignial Author: fargie_s
**
*/

#include <CUnit/CUnit.h>
#include <glib.h>

#include "rme-manager.h"
#include "cunit_helper.h"

static int setup() {
    g_type_init();
    return CUE_SUCCESS;
}

static void test_singleton() {
    RmeManager *mgr1 = rme_manager_new();
    RmeManager *mgr2 = rme_manager_new();

    CU_ASSERT_PTR_NOT_NULL(mgr1);
    CU_ASSERT_PTR_EQUAL(mgr1, mgr2);
    g_object_unref(mgr1);
    g_object_unref(mgr2);
}

SUITE_BEGIN(test_manager, setup, NULL)
    REGISTER_TEST(test_singleton)
SUITE_END()

