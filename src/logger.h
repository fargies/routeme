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
** logger.h
**
**        Created on: Jun 05, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "config.h"

#ifndef PACKAGE_NAME
#error "PACKAGE_NAME must be defined"
#endif

#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL LOG_LEVEL_NOTICE
#endif

int _log_init(const char *ident);

#define log_init() _log_init(PACKAGE_NAME)
void log_exit();

void log_debug(const char *format, ...);
void log_info(const char *format, ...);
void log_notice(const char *format, ...);
void log_warning(const char *format, ...);
void log_error(const char *format, ...);
void log_critical(const char *format, ...);
void log_alert(const char *format, ...);
void log_emerg(const char *format, ...);

#endif

