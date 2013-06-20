
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_yal.h"
#include "kernel/debug.h"
#include "kernel/string.h"

#ifndef YAL_RELEASE

FILE *yal_log = NULL;
int yal_debug_trace = 0;
yal_debug_entry *start = NULL;
yal_debug_entry *active = NULL;

/**
 * Stars debug on file pipe
 */
int yal_start_debug(){
	if(!yal_log){
		/*//yal_log = fopen("/home/gutierrezandresfelipe/phalcon-debug.a", "w");
		yal_log = fopen("/tmp/phalcon-debug.a", "w");
		if(!yal_log){
			fprintf(stderr, "Can't open debug log\n");
		}*/
		yal_log = stderr;
	}
	return SUCCESS;
}

/**
 * Stops debug process
 */
int yal_stop_debug(){
	yal_debug_entry *ptr = active;
	yal_debug_entry *this_entry = NULL;
	while(ptr){
		this_entry = ptr;
		ptr = ptr->prev;
		efree(this_entry);
	}
	//fclose(yal_log);
	yal_log = NULL;
	return SUCCESS;
}

/**
 * Executes a print_r on an interal zval
 */
int yal_print_r(zval *userval TSRMLS_DC){
	zend_print_zval_r(userval, 0 TSRMLS_CC);
	return SUCCESS;
}

/**
 * Internal fast zval dump
 */
int yal_vdump(zval *uservar TSRMLS_DC){
	yal_start_debug();
    if(!uservar){
		fprintf(yal_log, "Null pointer\n");
		return SUCCESS;
	}
    switch(Z_TYPE_P(uservar)){
        case IS_NULL:
            fprintf(yal_log, "NULL \n");
            break;
        case IS_BOOL:
            fprintf(yal_log, "Boolean: %s\n", Z_LVAL_P(uservar) ? "TRUE" : "FALSE");
            break;
        case IS_LONG:
            fprintf(yal_log, "Long: %ld at %p, refcount=%d\n", Z_LVAL_P(uservar), uservar, Z_REFCOUNT_P(uservar));
            break;
        case IS_DOUBLE:
            fprintf(yal_log, "Double: %f\n", Z_DVAL_P(uservar));
            break;
        case IS_STRING:
			fprintf(yal_log, "String: %s(%d) at %p, refcount=%d\n", Z_STRVAL_P(uservar), Z_STRLEN_P(uservar), uservar, Z_REFCOUNT_P(uservar));
            break;
        case IS_RESOURCE:
            fprintf(yal_log, "Resource\n");
            break;
        case IS_ARRAY:
            fprintf(yal_log, "Array at %p, refcount=%d\n", uservar, Z_REFCOUNT_P(uservar));
            break;
        case IS_OBJECT:
            fprintf(yal_log, "Object <%s> at %p\n", Z_OBJCE_P(uservar)->name, uservar);
            break;
        default:
            fprintf(yal_log, "Unknown\n");
    }
    return SUCCESS;
}

int yal_dump_ce(zend_class_entry *ce TSRMLS_DC){
	char *message = emalloc(sizeof(char *)*120);
	if(ce){
		sprintf(message, "- ClassType => %d", ce->type);
		yal_step_over(message);
		if(ce->name){
			sprintf(message, "- ClassName => %s", ce->name);
			yal_step_over(message);
		} else {
			yal_step_over("- ClassName => NULL");
		}
	} else {
		yal_step_over("- NULL class entry :(");
	}
	return SUCCESS;
}

int yal_class_debug(zval *val TSRMLS_DC){
	char *message = emalloc(sizeof(char *)*120);
	zend_class_entry *ce;
	if(val){
		ce = Z_OBJCE_P(val);
		if(ce){
			sprintf(message, "- MemoryAddress => %p", val);
			yal_step_over(message);
			yal_dump_ce(ce TSRMLS_CC);
		} else {
			yal_step_over("- No class entry :(");
		}
	} else {
		yal_step_over("- this_ptr is null :(");
	}
	return SUCCESS;
}

/**
 * Append debug information to file
 */
int yal_debug_str(char *what, char *message){
	fprintf(yal_log, "%s", what);
	fprintf(yal_log, "%s", message);
	fprintf(yal_log, "\n");
	return SUCCESS;
}

int yal_debug_long(char *what, uint vlong){
	fprintf(yal_log, "%s", what);
	fprintf(yal_log, "%u", vlong);
	fprintf(yal_log, "\n");
	return SUCCESS;
}

int yal_debug_screen(char *message){
	yal_debug_space();
	fprintf(yal_log, "%s\n", message);
	return SUCCESS;
}

int yal_debug_method_call(zval *obj, char *method_name TSRMLS_DC){
	if(Z_TYPE_P(obj)==IS_OBJECT){
		yal_debug_space();
	} else {
		yal_error_space();
	}
	if(Z_TYPE_P(obj)==IS_OBJECT){
		fprintf(yal_log, "Calling method %s::%s on Object at %p\n", Z_OBJCE_P(obj)->name, method_name, obj);
	} else {
		fprintf(yal_log, "Calling method %s on non object :(\n", method_name);
	}
	return SUCCESS;
}

int yal_error_space(){
	int i;
	fprintf(yal_log, "[ERROR] ");
	for(i=0;i<yal_debug_trace;i++){
		fprintf(yal_log, " ");
	}
	return SUCCESS;
}

int yal_debug_space(){
	int i;
	fprintf(yal_log, "[DEBUG] ");
	for(i=0;i<yal_debug_trace;i++){
		fprintf(yal_log, " ");
	}
	return SUCCESS;
}

int yal_debug_param(zval *param TSRMLS_DC){
	yal_debug_space();
	fprintf(yal_log, "Push method Param > ");
	yal_vdump(param TSRMLS_CC);
	return SUCCESS;
}

int yal_debug_vdump(char *preffix, zval *value TSRMLS_DC){
	yal_debug_space();
	fprintf(yal_log, "%s", preffix);
	yal_vdump(value TSRMLS_CC);
	return SUCCESS;
}

int yal_debug_assign(char *name, zval *value TSRMLS_DC){
	yal_debug_space();
	fprintf(yal_log, "Assign on %s with ", name);
	yal_vdump(value TSRMLS_CC);
	return SUCCESS;
}

int yal_step_over(char *message){
	yal_debug_screen(message);
	return SUCCESS;
}

int yal_step_into(char *message){
	yal_debug_trace++;
	yal_debug_screen(message);
	return SUCCESS;
}

int yal_step_out(char *message){
	yal_debug_screen(message);
	yal_debug_trace--;
	return SUCCESS;
}

/**
 * Prints internal debug backtrace
 */
int yal_debug_backtrace_internal(){
	int step = 0;
	char *message;
	yal_debug_entry *ptr = active;
	while(ptr){
		yal_spprintf(&message, 0, "#%d %s::%s", step, ptr->class_name, ptr->method_name);
		yal_debug_screen(message);
		efree(message);
		ptr = ptr->prev;
		step++;
	}
	return SUCCESS;
}

/**
 * Appends a debug entry to internal execution scope
 */
int yal_step_into_entry(char *class_name, char *method_name, int lineno){

	char *message;
	yal_debug_entry *entry;

	if (!start) {
		start = (yal_debug_entry *) emalloc(sizeof(yal_debug_entry));
		start->class_name = "__main__";
		start->method_name = "__init__";
		start->lineno = 0;
		start->prev = NULL;
		start->next = NULL;
		active = start;
	}

	yal_spprintf(&message, 0, "Step Into %s::%s", class_name, method_name);
	yal_debug_screen(message);
	efree(message);

	entry = emalloc(sizeof(yal_debug_entry));
	entry->class_name = class_name;
	entry->method_name = method_name;
	entry->lineno = lineno;
	entry->prev = active;
	active->next = entry;
	active = entry;
	yal_debug_trace++;

	return SUCCESS;
}

/**
 * Steps out current stack
 */
int yal_step_out_entry(){

	char *message;
	yal_debug_entry *prev;
	if(active){

		yal_debug_trace--;

		yal_spprintf(&message, 0, "Step out %s::%s", active->class_name, active->method_name);
		yal_debug_screen(message);
		efree(message);

		prev = active->prev;
		efree(active);
		active = prev;

	} else {
		fprintf(yal_log, "Problem, stack?");
		return FAILURE;
	}
	return SUCCESS;
}

#endif
