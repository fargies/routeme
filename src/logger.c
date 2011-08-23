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
** logger.c
**
**        Created on: Jun 05, 2011
**   Orignial Author: fargie_s
**
*/

#include <syslog.h>
#include <stdarg.h>
#include "logger.h"

int _log_init(const char *ident)
{
    openlog(ident, LOG_PERROR | LOG_PID, LOG_DAEMON);
    return 0;
}

void log_exit()
{
    closelog();
}

void log_debug(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_DEBUG, format, ap);
    va_end(ap);
}

void log_info(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_INFO, format, ap);
    va_end(ap);
}

void log_notice(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_NOTICE, format, ap);
    va_end(ap);
}

void log_warning(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_WARNING, format, ap);
    va_end(ap);
}

void log_error(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_ERR, format, ap);
    va_end(ap);
}

void log_critical(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_CRIT, format, ap);
    va_end(ap);
}

void log_alert(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_ALERT, format, ap);
    va_end(ap);
}

void log_emerg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_EMERG, format, ap);
    va_end(ap);
}

