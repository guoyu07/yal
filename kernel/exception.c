
/*
  +------------------------------------------------------------------------+
  | Yal Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Yal Team (http://www.Yalphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@Yalphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@Yalphp.com>                      |
  |          Eduar Carvajal <eduar@Yalphp.com>                         |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_yal.h"
#include "php_main.h"
#include "ext/standard/php_string.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"

#include "Zend/zend_exceptions.h"

/**
 * Throws an zval object as exception
 */
void yal_throw_exception(zval *object TSRMLS_DC){
	Z_ADDREF_P(object);
	zend_throw_exception_object(object TSRMLS_CC);
	yal_memory_restore_stack(TSRMLS_C);
}

/**
 * Throws a exception with a single string parameter
 */
void yal_throw_exception_string(zend_class_entry *ce, char *message, zend_uint message_len TSRMLS_DC){

	zval *object, *msg;

	ALLOC_INIT_ZVAL(object);
	object_init_ex(object, ce);

	YAL_INIT_VAR(msg);
	ZVAL_STRINGL(msg, message, message_len, 1);

	YAL_CALL_METHOD_PARAMS_1_NORETURN(object, "__construct", msg);

	zend_throw_exception_object(object TSRMLS_CC);

	yal_memory_restore_stack(TSRMLS_C);
}

/**
 * Throws a exception with a single zval parameter
 */
void yal_throw_exception_zval(zend_class_entry *ce, zval *message TSRMLS_DC){

	zval *object;

	ALLOC_INIT_ZVAL(object);
	object_init_ex(object, ce);

	YAL_CALL_METHOD_PARAMS_1_NORETURN(object, "__construct", message);

	zend_throw_exception_object(object TSRMLS_CC);

	yal_memory_restore_stack(TSRMLS_C);
}

/**
 * Latest version of zend_throw_exception_internal
 */
void yal_throw_exception_internal(zval *exception TSRMLS_DC) {

	if (exception != NULL) {
		zval *previous = EG(exception);
		zend_exception_set_previous(exception, EG(exception) TSRMLS_CC);
		EG(exception) = exception;
		if (previous) {
			return;
		}
	}
	if (!EG(current_execute_data)) {
		if(EG(exception)) {
			zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
		}
		zend_error(E_ERROR, "Exception thrown without a stack frame");
	}

	if (zend_throw_exception_hook) {
    	zend_throw_exception_hook(exception TSRMLS_CC);
	}

	if (EG(current_execute_data)->opline == NULL ||
    	(EG(current_execute_data)->opline+1)->opcode == ZEND_HANDLE_EXCEPTION) {
		/* no need to rethrow the exception */
		return;
	}
	EG(opline_before_exception) = EG(current_execute_data)->opline;
	EG(current_execute_data)->opline = EG(exception_op);

}

/*void yal_try_execute(zval *success, zval *return_value, zval *call_object, zval *params, zval **exception TSRMLS_DC){

	zval *fn = NULL;
	int status = FAILURE;
	zval *func_params[] = { call_object, params };

	YAL_ALLOC_ZVAL(fn);
	ZVAL_STRING(fn, "call_user_func_array", 0);

	status = yal_call_user_function(CG(function_table), NULL, fn, return_value, 2, func_params TSRMLS_CC);
	if (status == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Call to undefined function call_user_func_array()");
	}

	ZVAL_NULL(fn);
	zval_ptr_dtor(&fn);

	if (status == SUCCESS) {
		zend_exception_restore(TSRMLS_C);
		if (EG(exception)) {
			zval_ptr_dtor(exception);
			*exception = EG(exception);
			EG(exception) = NULL;
			EG(current_execute_data)->opline->opcode = 40;
			ZVAL_BOOL(success, 0);
		} else {
			ZVAL_BOOL(success, 1);
		}
	} else {
		ZVAL_BOOL(success, 0);
	}

}*/