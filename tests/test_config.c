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
** test_config.c
**
**        Created on: Jun 05, 2011
**   Orignial Author: fargie_s
**
*/

#include <CUnit/CUnit.h>
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "cunit_helper.h"
#include "rme-rule-manager.h"
#include "rme-config-loader.h"
#include "logger.h"

static RmeRuleManager *manager;
#define TEST_FILE "/tmp/config_test.cfg"

static int setUp()
{
    g_type_init();
    log_init();

    manager = rme_rule_manager_new();
    g_object_add_weak_pointer(G_OBJECT(manager), (gpointer *) &manager);

    return CUE_SUCCESS;
}

static int tearDown()
{
    g_object_unref(manager);
    unlink(TEST_FILE);
    return CUE_SUCCESS;
}

static void rules_counter(RmeRule *rule, int *count)
{
    (*count)++;
}

static void simple()
{
    int count = 0;

    FILE *fd = fopen(TEST_FILE, "w");
    CU_ASSERT_FATAL(fd != NULL);
    fputs("\n"
"# this is a comment after a blank line\n"
"  # this is another comment\n"
"\n"
"# IP         LOCAL_PORT   EXTERNAL_PORT  PROTO <comment>\n"
"127.0.0.1    80           80             TCP   # this is a comment\n"
"127.0.0.2    81           81             UDP ",
        fd);
    fclose(fd);

    RmeRule *ref_rule, *mgr_rule;

    RmeConfigLoader *loader = rme_config_loader_new(manager);
    CU_ASSERT_PTR_NOT_NULL(loader);

    CU_ASSERT_EQUAL(rme_config_loader_load(loader, TEST_FILE), 0);
    CU_ASSERT_EQUAL(rme_config_loader_synerr(loader), 0);
    g_object_unref(loader);

    rme_rule_manager_foreach(manager, (RmeRuleCallback) rules_counter,
            (gpointer) &count);
    CU_ASSERT_EQUAL(count, 2);

    ref_rule = rme_rule_new("127.0.0.1", 80, 80, RME_PROTO_TCP, NULL);

    mgr_rule = rme_rule_manager_get(manager, rme_rule_get_signature(ref_rule));
    CU_ASSERT_PTR_NOT_NULL(mgr_rule);
    g_object_unref(mgr_rule);
    g_object_unref(ref_rule);

    ref_rule =  rme_rule_new("127.0.0.2", 81, 81, RME_PROTO_UDP, NULL);
    mgr_rule = rme_rule_manager_get(manager, rme_rule_get_signature(ref_rule));
    CU_ASSERT_PTR_NOT_NULL(mgr_rule);
    g_object_unref(mgr_rule);
    g_object_unref(ref_rule);
}

static void empty()
{
    int count = 0;
    CU_ASSERT_EQUAL_FATAL(truncate(TEST_FILE, 0), 0);

    g_object_unref(manager);
    CU_ASSERT_PTR_NULL_FATAL(manager);
    manager = rme_rule_manager_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(manager);

    RmeConfigLoader *loader = rme_config_loader_new(manager);
    CU_ASSERT_PTR_NOT_NULL(loader);

    CU_ASSERT_EQUAL(rme_config_loader_load(loader, TEST_FILE), 0);
    CU_ASSERT_EQUAL(rme_config_loader_synerr(loader), 0);
    g_object_unref(loader);

    rme_rule_manager_foreach(manager, (RmeRuleCallback) rules_counter,
            (gpointer) &count);
    CU_ASSERT_EQUAL(count, 0);
}

static void error()
{
    int count = 0;

    FILE *fd = fopen(TEST_FILE, "w");
    CU_ASSERT_FATAL(fd != NULL);
    fputs("syntax error\n", fd);
    fclose(fd);

    RmeRule *ref_rule, *mgr_rule;

    RmeConfigLoader *loader = rme_config_loader_new(manager);
    CU_ASSERT_PTR_NOT_NULL(loader);

    CU_ASSERT_NOT_EQUAL(rme_config_loader_load(loader, TEST_FILE), 0);
    CU_ASSERT_EQUAL(rme_config_loader_synerr(loader), 1);
    g_object_unref(loader);

    rme_rule_manager_foreach(manager, (RmeRuleCallback) rules_counter,
            (gpointer) &count);
    CU_ASSERT_EQUAL(count, 0);
}

static void error_recover()
{
    int count = 0;

    FILE *fd = fopen(TEST_FILE, "w");
    CU_ASSERT_FATAL(fd != NULL);
    fputs("syntax error\n"
"b\n\n"
"error\n"
"32 32 32\n"
"127.0.0.1    80                          TCP   # this is a comment\n"
"127.0.0.2    81           81             UDP error here ", fd);
    fclose(fd);

    RmeRule *ref_rule, *mgr_rule;

    RmeConfigLoader *loader = rme_config_loader_new(manager);
    CU_ASSERT_PTR_NOT_NULL(loader);

    CU_ASSERT_NOT_EQUAL(rme_config_loader_load(loader, TEST_FILE), 0);
    CU_ASSERT_EQUAL(rme_config_loader_synerr(loader), 5);
    g_object_unref(loader);

    rme_rule_manager_foreach(manager, (RmeRuleCallback) rules_counter,
            (gpointer) &count);
    CU_ASSERT_EQUAL(count, 2);

    ref_rule = rme_rule_new("127.0.0.1", 80, 80, RME_PROTO_TCP, NULL);

    mgr_rule = rme_rule_manager_get(manager, rme_rule_get_signature(ref_rule));
    CU_ASSERT_PTR_NOT_NULL(mgr_rule);
    g_object_unref(mgr_rule);
    g_object_unref(ref_rule);

    ref_rule =  rme_rule_new("127.0.0.2", 81, 81, RME_PROTO_UDP, NULL);
    mgr_rule = rme_rule_manager_get(manager, rme_rule_get_signature(ref_rule));
    CU_ASSERT_PTR_NOT_NULL(mgr_rule);
    g_object_unref(mgr_rule);
    g_object_unref(ref_rule);
}

SUITE_BEGIN(test_config, setUp, tearDown)
    REGISTER_TEST(simple)
    REGISTER_TEST(empty)
    REGISTER_TEST(error)
    REGISTER_TEST(error_recover)
SUITE_END()

