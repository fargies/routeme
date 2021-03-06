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

#include "rme-rule-manager.h"
#include "cunit_helper.h"

static int setup() {
    g_type_init();
    log_init();

    return CUE_SUCCESS;
}

static void test_singleton() {
    RmeRuleManager *mgr1 = rme_rule_manager_new();
    RmeRuleManager *mgr2 = rme_rule_manager_new();

    CU_ASSERT_PTR_NOT_NULL(mgr1);
    CU_ASSERT_PTR_EQUAL(mgr1, mgr2);
    g_object_unref(mgr1);
    g_object_unref(mgr2);
}

static void sig_added(RmeRuleManager *mgr, RmeRule *rule, gpointer count)
{
    *((int *)count) += 1;
}

static void sig_removed(RmeRuleManager *mgr, RmeRule *rule, gpointer count)
{
    *((int *)count) -= 1;
}

static void test_signals() {
    int count = 0;

    RmeRuleManager *mgr = rme_rule_manager_new();
    RmeRule *rule = rme_rule_new("127.0.0.1", 80, 80, RME_PROTO_TCP, NULL);

    g_object_ref(rule);

    rme_rule_manager_connect__rule_added(mgr, sig_added, (gpointer) &count);
    rme_rule_manager_connect__rule_removed(mgr, sig_removed, (gpointer) &count);

    rme_rule_manager_add(mgr, rule);
    CU_ASSERT_EQUAL(count, 1);

    CU_ASSERT(rme_rule_manager_remove(mgr, rme_rule_get_signature(rule)) == TRUE);
    CU_ASSERT_EQUAL(count, 0);

    rme_rule_manager_add(mgr, rule);
    CU_ASSERT_EQUAL(count, 1);

    g_object_unref(mgr);
    CU_ASSERT_EQUAL(count, 0);

    g_object_unref(rule);
}

SUITE_BEGIN(test_manager, setup, NULL)
    REGISTER_TEST(test_singleton)
    REGISTER_TEST(test_signals)
SUITE_END()

