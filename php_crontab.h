/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_CRONTAB_H
#define PHP_CRONTAB_H

#include <time.h>
#include <sys/time.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include <signal.h>

#include "php.h"
#include "php_ini.h"
#include "php_globals.h"
#include "php_main.h"
#include "zend_interfaces.h"

extern zend_module_entry crontab_module_entry;
#define phpext_crontab_ptr &crontab_module_entry

#define PHP_CRONTAB_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_CRONTAB_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_CRONTAB_API __attribute__ ((visibility("default")))
#else
#	define PHP_CRONTAB_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/**
 * busybox-1.21.0/include/platform.h
 */
/* Useful for defeating gcc's alignment of "char message[]"-like data */
#if !defined(__s390__)
    /* on s390[x], non-word-aligned data accesses require larger code */
# define ALIGN1 __attribute__((aligned(1)))
# define ALIGN2 __attribute__((aligned(2)))
# define ALIGN4 __attribute__((aligned(4)))
#else
/* Arches which MUST have 2 or 4 byte alignment for everything are here */
# define ALIGN1
# define ALIGN2
# define ALIGN4
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(crontab)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(crontab)
*/

/* Always refer to the globals in your function as CRONTAB_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define CRONTAB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(crontab, v)

#if defined(ZTS) && defined(COMPILE_DL_CRONTAB)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_CRONTAB_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
