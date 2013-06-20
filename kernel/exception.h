
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

/** Exceptions */
#define YAL_THROW_EXCEPTION_STR(class_entry, message) yal_throw_exception_string(class_entry, message, strlen(message) TSRMLS_CC);
#define YAL_THROW_EXCEPTION_ZVAL(class_entry, message) yal_throw_exception_zval(class_entry, message TSRMLS_CC);

/** Throw Exceptions */
extern void yal_throw_exception(zval *object TSRMLS_DC);
extern void yal_throw_exception_string(zend_class_entry *ce, char *message, zend_uint message_len TSRMLS_DC);
extern void yal_throw_exception_zval(zend_class_entry *ce, zval *message TSRMLS_DC);
extern void yal_throw_exception_internal(zval *exception TSRMLS_DC);

/** Catch Exceptions */
extern void yal_try_execute(zval *success, zval *return_value, zval *call_object, zval *params, zval **exception TSRMLS_DC);
