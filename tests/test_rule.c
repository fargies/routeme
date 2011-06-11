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
** test_rule.c
**
**        Created on: Jun 03, 2011
**   Orignial Author: fargie_s
**
*/

#include <CUnit/CUnit.h>
#include <glib.h>
#include <string.h>

#include "rme-rule.h"
#include "cunit_helper.h"

static int setup() {
    g_type_init();
    log_init();

    return CUE_SUCCESS;
}

static void test_accessors()
{
    RmeRule *rule = rme_rule_new("*", 1234,
            1235, RME_PROTO_TCP, "test");

    CU_ASSERT_PTR_NOT_NULL(rule);

    const gchar *ip = rme_rule_get_local_ip(rule);
    CU_ASSERT_STRING_EQUAL(ip, "*");

    guint local_port = rme_rule_get_local_port(rule);
    CU_ASSERT_EQUAL(local_port, 1234);

    guint external_port = rme_rule_get_external_port(rule);
    CU_ASSERT_EQUAL(external_port, 1235);

    RmeProto protocol = rme_rule_get_protocol(rule);
    CU_ASSERT_EQUAL(protocol, RME_PROTO_TCP);

    const gchar *description = rme_rule_get_description(rule);
    CU_ASSERT_STRING_EQUAL(description, "test");

    g_object_unref(rule);
}

static void test_signature() {
    RmeRule *rule = rme_rule_new("*", 1234,
            1235, RME_PROTO_TCP, "test1");
    gchar *sig1;
    const gchar *sig2;

    sig1 = strdup(rme_rule_get_signature(rule));
    g_object_unref(rule);

    rule = rme_rule_new("*", 1234,
            1235, RME_PROTO_TCP, "test2");
    sig2 = rme_rule_get_signature(rule);
    CU_ASSERT_STRING_EQUAL(sig1, sig2);
    g_object_unref(rule);

    rule = rme_rule_new("mon-dns", 1234,
            1235, RME_PROTO_TCP, "test2");
    sig2 = rme_rule_get_signature(rule);
    CU_ASSERT_STRING_NOT_EQUAL(sig1, sig2);
    g_object_unref(rule);

    rule = rme_rule_new("*", 1235,
            1235, RME_PROTO_TCP, "test2");
    sig2 = rme_rule_get_signature(rule);
    CU_ASSERT_STRING_NOT_EQUAL(sig1, sig2);
    g_object_unref(rule);

    rule = rme_rule_new("*", 1234,
            1234, RME_PROTO_TCP, "test2");
    sig2 = rme_rule_get_signature(rule);
    CU_ASSERT_STRING_NOT_EQUAL(sig1, sig2);
    g_object_unref(rule);

    rule = rme_rule_new("*", 1234,
            1235, RME_PROTO_UDP, "test2");
    sig2 = rme_rule_get_signature(rule);
    CU_ASSERT_STRING_NOT_EQUAL(sig1, sig2);
    g_object_unref(rule);

    g_free(sig1);
}

SUITE_BEGIN(test_rule, setup, NULL)
    REGISTER_TEST(test_accessors)
    REGISTER_TEST(test_signature)
SUITE_END()

