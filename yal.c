/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
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

/* $Id: header 321634 2012-01-01 13:15:04Z felipe $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_yal.h"
#include "yal.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"


zend_class_entry *yal_acl_role_ce;
zend_class_entry *yal_acl_resource_ce;
zend_class_entry *yal_acl_adapter_ce;
zend_class_entry *yal_acl_exception_ce;
zend_class_entry *yal_acl_adapter_memory_ce;
zend_class_entry *yal_acl_adapterinterface_ce;
zend_class_entry *yal_acl_roleinterface_ce;
zend_class_entry *yal_acl_resourceinterface_ce;

/* If you declare any globals in php_yal.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(yal)
*/

/* True global resources - no need for thread safety here */
static int le_yal;

/* {{{ yal_functions[]
 *
 * Every user visible function must have an entry in yal_functions[].
 */
const zend_function_entry yal_functions[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ yal_module_entry
 */
zend_module_entry yal_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"yal",
	yal_functions,
	PHP_MINIT(yal),
	PHP_MSHUTDOWN(yal),
	PHP_RINIT(yal),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(yal),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(yal),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_YAL
ZEND_GET_MODULE(yal)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("yal.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_yal_globals, yal_globals)
    STD_PHP_INI_ENTRY("yal.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_yal_globals, yal_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_yal_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_yal_init_globals(zend_yal_globals *yal_globals)
{
	yal_globals->global_value = 0;
	yal_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(yal)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	ZEND_MODULE_STARTUP_N(yal_acl)(INIT_FUNC_ARGS_PASSTHRU);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(yal)
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
PHP_RINIT_FUNCTION(yal)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(yal)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(yal)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yal support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_yal_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_yal_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "yal", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
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
