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

#include "config.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <glib.h>
#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif

#include "logger.h"
#include "rme-rule-manager.h"
#include "rme-config-loader.h"
#include "rme-controlpoint.h"

static GMainLoop *main_loop = NULL;

void sighdlr(int sig)
{
    log_notice("[Main]: signal caught (%s)",
            g_strsignal(sig));
    if (main_loop)
        g_main_loop_quit(main_loop);
}

int main(int argc, char **argv)
{
#ifdef HAVE_GETOPT_LONG
    int c;
    struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"config", 1, 0, 'c'},
        {0, 0, 0, 0}
    };
#endif
    const char *config_file = CONFIG_FILE;

    log_init();
    g_type_init();
    if (g_thread_get_initialized() == FALSE)
        g_thread_init(NULL);

#ifdef HAVE_GETOPT_LONG
    while ((c = getopt_long(argc, argv, "hc:", long_options, NULL)) != -1) {
        switch (c) {
            case 'h':
                fprintf(stderr, "Usage: %s [test name]\n", argv[0]);
                fputs("\nOptions:\n", stderr);
                fputs("  -h, --help           Show this help message and exit\n", stderr);
                fputs("  -c, --config         Set config file (default: " CONFIG_FILE ")\n", stderr);
                exit(EXIT_SUCCESS);
            case 'c':
                config_file = optarg;
        }
    }
#endif

    RmeRuleManager *manager = rme_rule_manager_new();

    RmeConfigLoader *loader = rme_config_loader_new(manager);

    if (rme_config_loader_load(loader, config_file) != 0)
    {
        log_critical("[Main]: some errors in configuration file %s",
                config_file);
    }
    g_object_unref(loader);

    RmeControlPoint *cp = rme_controlpoint_new();

    main_loop = g_main_loop_new(NULL, TRUE);

    signal(SIGTERM, sighdlr);
    signal(SIGINT, sighdlr);

    g_main_loop_run(main_loop);

    g_object_unref(cp);
    g_object_unref(manager);

    return 0;
}

