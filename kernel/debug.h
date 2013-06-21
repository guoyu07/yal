
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

#ifndef YAL_RELEASE

#define PHV(v) yal_vdump(v)
#define PHPR(v) yal_print_r(v)

typedef struct _yal_debug_entry {
	char *class_name;
	char *method_name;
	int lineno;
	struct _yal_debug_entry *prev;
	struct _yal_debug_entry *next;
} yal_debug_entry;

extern int yal_start_debug();
extern int yal_stop_debug();

extern int yal_print_r(zval *userval TSRMLS_DC);
extern int yal_vdump(zval *uservar TSRMLS_DC);
extern int yal_debug_assign(char *name, zval *value TSRMLS_DC);
extern int yal_vpdump(const zval **uservar TSRMLS_DC);
extern int yal_dump_ce(zend_class_entry *ce TSRMLS_DC);
extern int yal_class_debug(zval *val TSRMLS_DC);

extern int yal_debug_backtrace_internal();
extern int yal_debug_str(char *what, char *message);
extern int yal_debug_long(char *what, uint vlong);
extern int yal_debug_screen(char *message);

extern int yal_step_over(char *message);
extern int yal_step_into(char *message);
extern int yal_step_out(char *message);

extern int yal_step_into_entry(char *class_name, char *method_name, int lineno);
extern int yal_step_out_entry();

extern int yal_debug_method_call(zval *obj, char *method_name TSRMLS_DC);
extern int yal_debug_vdump(char *preffix, zval *value TSRMLS_DC);
extern int yal_debug_param(zval *param TSRMLS_DC);

extern int yal_error_space();
extern int yal_debug_space();

extern FILE *yal_log;
extern int yal_debug_trace;
extern yal_debug_entry *start;
extern yal_debug_entry *active;

#endif
