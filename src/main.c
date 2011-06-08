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
** main.c
**
**        Created on: Jun 02, 2011
**   Orignial Author: fargie_s
**
*/

#include <stdio.h>
#include "rme-rule.h"

int main() {
    g_type_init();
    RmeRule *rule = rme_rule_new("127.0.0.1", 1234, 1235, RME_PROTO_UDP, "test");
    printf("%s\n", rme_rule_get_local_ip(rule));
    return 0;
}

