/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_crontab.h"

/* If you declare any globals in php_crontab.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(crontab)
*/

/* True global resources - no need for thread safety here */
static int le_crontab;
extern sapi_module_struct sapi_module;

zend_class_entry *crontab_ce;

static const char weeksAry[] ALIGN1 =
    "sun""mon""tue""wed""thu""fri""sat"
    /* "Sun""Mon""Tue""Wed""Thu""Fri""Sat" */
;

static const char monthsAry[] ALIGN1 =
    "jan""feb""mar""apr""may""jun""jul""aug""sep""oct""nov""dec"
    /* "Jan""Feb""Mar""Apr""May""Jun""Jul""Aug""Sep""Oct""Nov""Dec" */
;

typedef struct crontab_s crontab_t;
struct crontab_s {
	char                        weeks[7];           /* 0-6, beginning sunday */
	char                        months[12];         /* 0-11 */
	char                        hours[24];          /* 0-23 */
	char                        days[32];           /* 1-31 */
	char                        minutes[60];        /* 0-59 */

    char                        *name;
    zval                        *callbak;

    crontab_t                   *next;
};

static crontab_t *crontab_head;
static crontab_t *crontab_last;

static int parse_line(char *str, crontab_t *cron TSRMLS_DC);
static int parse_field(char *ary, int modvalue, int off, const char *names, char *ptr TSRMLS_DC);


/* {{{ crontab_functions[]
 *
 * Every user visible function must have an entry in crontab_functions[].
 */
const zend_function_entry crontab_functions[] = {
    PHP_FE_END    /* Must be the last line in crontab_functions[] */
};
/* }}} */

/* {{{ crontab_module_entry
 */
zend_module_entry crontab_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "crontab",
    crontab_functions,
    PHP_MINIT(crontab),
    PHP_MSHUTDOWN(crontab),
    PHP_RINIT(crontab),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(crontab),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(crontab),
#if ZEND_MODULE_API_NO >= 20010901
    "0.1", /* Replace with version number for your extension */
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CRONTAB
ZEND_GET_MODULE(crontab)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("crontab.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_crontab_globals, crontab_globals)
    STD_PHP_INI_ENTRY("crontab.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_crontab_globals, crontab_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_crontab_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_crontab_init_globals(zend_crontab_globals *crontab_globals)
{
    crontab_globals->global_value = 0;
    crontab_globals->global_string = NULL;
}
*/
/* }}} */

PHP_METHOD(crontab_ce, __construct) {
    //only cli env
    if (strcasecmp("cli", sapi_module.name) != 0)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "crontab must run at cli environment.");
        RETURN_FALSE;
    }
}

PHP_METHOD(crontab_ce, __destruct) {
}

PHP_METHOD(crontab_ce, add) {
    zval *argv1, *argv2;
    int n, l, i;
    zval **val;
    char *key;
    int idx;
    crontab_t *cb;

    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &argv1, &argv2) == SUCCESS ){
        // if array
        if (Z_TYPE_P(argv1) == IS_ARRAY) {
            // foreach argv1
            l = zend_hash_num_elements(Z_ARRVAL_P(argv1));
            zend_hash_internal_pointer_reset(Z_ARRVAL_P(argv1));
            for(i = 0; i < l; i++) {
                if(zend_hash_get_current_key(Z_ARRVAL_P(argv1), &key, &idx, 0) == HASH_KEY_IS_STRING) {
                    // is al
                    cb = emalloc(sizeof(crontab_t));

                    n = parse_line(key, cb TSRMLS_CC);
                    cb->next = crontab_last;
                    crontab_last = cb;
                    //cb->callback = 
                    //zend_hash_get_current_data(Z_ARRVAL_P(white), (void**)&val);

                } else {
                    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid args(array key must be string).");
                }
            } // for i
            
        } else {// IS_ARRAY

                    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid args(array key must be string 1).");
        }
    } else {

                    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid argsarray key must be string x %d).", ZEND_NUM_ARGS());
    }
}

PHP_METHOD(crontab_ce, run) {

}

PHP_METHOD(crontab_ce, info) {

}

zend_function_entry crontab_methods[] = {
    PHP_ME(crontab_ce, __construct, 			NULL, 	ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)	//	final protected __construct
    PHP_ME(crontab_ce, __destruct, 			NULL, 	ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)	//	final public __destruct
    PHP_ME(crontab_ce, add,      			NULL, 	ZEND_ACC_PUBLIC )	//	static public final getInstance
    PHP_ME(crontab_ce, run,      			NULL, 	ZEND_ACC_PUBLIC )	//	static public final getInstance
    PHP_ME(crontab_ce, info,      			NULL, 	ZEND_ACC_PUBLIC )	//	static public final getInstance
    {NULL, NULL, NULL}
};

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(crontab)
{
    /* If you have INI entries, uncomment these lines 
    REGISTER_INI_ENTRIES();
    */

    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Crontab", crontab_methods);
    crontab_ce = zend_register_internal_class(&ce TSRMLS_CC);

    crontab_head = emalloc(sizeof(crontab_t));
    crontab_last = crontab_head;

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(crontab)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(crontab)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(crontab)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(crontab)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "crontab support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */



static int parse_line(char *str, crontab_t *cron TSRMLS_DC) {
    int n, i;
    char *ptr;
    char *tokens[6];
    int weekUsed = 0;
    int daysUsed = 0;

    n = strlen(str);
    i = 0;
    ptr = str;
    do {
        if(*str == NULL || *str == '\0' || i >= 5) break;
        tokens[i++] = str;

        str += strspn(str, "1234567890,/*-");
        if(*str != '\0') {
            *str = '\0';
        }
        if(str - ptr < n) {
            str++;
            str = strpbrk(str, "1234567890,/*-");
        }
    }while(1);

    parse_field(cron->minutes, 60, 0, NULL, tokens[0] TSRMLS_CC);
    parse_field(cron->hours, 24, 0, NULL, tokens[1] TSRMLS_CC);
    parse_field(cron->days, 32, 0, NULL, tokens[2] TSRMLS_CC);
    parse_field(cron->months, 12, 0, monthsAry, tokens[3] TSRMLS_CC);
    parse_field(cron->weeks, 7, 0, weeksAry, tokens[4] TSRMLS_CC);

    for (i = 0; i < 7; ++i) {
        if (cron->weeks[i] == 0) {
            weekUsed = 1;
            break;
        }
    }

    for (i = 0; i < 32; ++i) {
        if (cron->days[i] == 0) {
            daysUsed = 1;
            break;
        }
    }

    if (weekUsed != daysUsed) {
        if (weekUsed) {
            memset(cron->days, 0, sizeof(cron->days));
        } else {
            // daysUsed 
            memset(cron->weeks, 0, sizeof(cron->weeks));
        }   
    }

    return 1;
}

static int parse_field(char *ary, int modvalue, int off, const char *names, char *ptr TSRMLS_DC) {
    char *base = ptr;
    int n1 = -1;
    int n2 = -1;
    int skip = 0;
    char *endp;

    // this can't happen due to config_read()
    /*if (base == NULL)
        return;*/

    while (1) {
        skip = 0;

        /* Handle numeric digit or symbol or '*' */
        if (*ptr == '*') {
            n1 = 0;  /* everything will be filled */
            n2 = modvalue - 1;
            skip = 1;
            ++ptr;
        } else if (isdigit(*ptr)) {
            if (n1 < 0) {
                n1 = strtol(ptr, &endp, 10) + off;
            } else {
                n2 = strtol(ptr, &endp, 10) + off;
            }
            ptr = endp; /* gcc likes temp var for &endp */
            skip = 1;
        } else if (names) {
            int i;

            for (i = 0; names[i]; i += 3) {
                /* was using strncmp before... */
                if (strncasecmp(ptr, &names[i], 3) == 0) {
                    ptr += 3;
                    if (n1 < 0) {
                        n1 = i / 3;
                    } else {
                        n2 = i / 3;
                    }
                    skip = 1;
                    break;
                }
            }
        }

        /* handle optional range '-' */
        if (skip == 0) {
            return 0;
        }
        if (*ptr == '-' && n2 < 0) {
            ++ptr;
            continue;
        }

        /*
         * collapse single-value ranges, handle skipmark, and fill
         * in the character array appropriately.
         */
        if (n2 < 0) {
            n2 = n1;
        }
        if (*ptr == '/') {
            char *endp;
            skip = strtol(ptr + 1, &endp, 10);
            ptr = endp; /* gcc likes temp var for &endp */
        }

        /*
         * fill array, using a failsafe is the easiest way to prevent
         * an endless loop
         */
        {
            int s0 = 1;
            int failsafe = 1024;

            --n1;
            do {
                n1 = (n1 + 1) % modvalue;

                if (--s0 == 0) {
                    ary[n1 % modvalue] = 1;
                    s0 = skip;
                }
                if (--failsafe == 0) {
                    return 0;
                }
            } while (n1 != n2);
        }
        if (*ptr != ',') {
            break;
        }
        ++ptr;
        n1 = -1;
        n2 = -1;
    }

    if (*ptr) {
        return 0;
    }

    return 1;
}

/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
