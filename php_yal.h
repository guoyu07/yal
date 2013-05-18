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

#ifndef PHP_YAL_H
#define PHP_YAL_H 1

#define PHP_YAL_VERSION "1.1.0"
#define PHP_YAL_EXTNAME "yal"

#define YAL_MAX_MEMORY_STACK 48

/** Memory frame */
typedef struct _yal_memory_entry {
  int pointer;
  zval **addresses[YAL_MAX_MEMORY_STACK];
  struct _yal_memory_entry *prev;
  struct _yal_memory_entry *next;
} yal_memory_entry;

/** Virtual Symbol Table */
typedef struct _yal_symbol_table {
  struct _yal_memory_entry *scope;
  HashTable *symbol_table;
  struct _yal_symbol_table *prev;
} yal_symbol_table;

/** ORM options */
typedef struct _yal_orm_options {
  zend_bool events;
  zend_bool virtual_foreign_keys;
  zend_bool column_renaming;
  zend_bool not_null_validations;
  zend_bool exception_on_failed_save;
  HashTable *parser_cache;
} yal_orm_options;

/** DB options */
typedef struct _yal_db_options {
  zend_bool escape_identifiers;
} yal_db_options;

ZEND_BEGIN_MODULE_GLOBALS(yal)

  /** Memory */
  yal_memory_entry *start_memory;
  yal_memory_entry *active_memory;

  /** Virtual Symbol Tables */
  yal_symbol_table *active_symbol_table;

  /** Function cache */
  HashTable *function_cache;

  /** Max recursion control */
  unsigned int recursive_lock;

  /** Stats */
#ifndef YAL_RELEASE
  unsigned int yal_stack_stats;
  unsigned int yal_number_grows;
#endif

  /** ORM */
  yal_orm_options orm;

  /** DB */
  yal_db_options db;

ZEND_END_MODULE_GLOBALS(yal)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(yal)

#ifdef ZTS
  #define YAL_GLOBAL(v) TSRMG(yal_globals_id, zend_yal_globals *, v)
#else
  #define YAL_GLOBAL(v) (yal_globals.v)
#endif

#ifdef ZTS
  #define YAL_VGLOBAL ((zend_yal_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(yal_globals_id)])
#else
  #define YAL_VGLOBAL &(yal_globals)
#endif

extern zend_module_entry yal_module_entry;
#define phpext_yal_ptr &yal_module_entry

#endif

#if PHP_VERSION_ID >= 50400
  #define YAL_INIT_FUNCS(class_functions) static const zend_function_entry class_functions[] =
#else
  #define YAL_INIT_FUNCS(class_functions) static const function_entry class_functions[] =
#endif

#ifndef PHP_FE_END
  #define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

/** Define FASTCALL */
#if defined(__GNUC__) && ZEND_GCC_VERSION >= 3004 && defined(__i386__)
# define YAL_FASTCALL __attribute__((fastcall))
#elif defined(_MSC_VER) && defined(_M_IX86)
# define YAL_FASTCALL __fastcall
#else
# define YAL_FASTCALL
#endif

#define YAL_INIT_CLASS(name) \
  int yal_ ##name## _init(INIT_FUNC_ARGS)

#define YAL_INIT(name) \
  if (yal_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { \
    return FAILURE; \
  }

#if PHP_VERSION_ID >= 50400
#define YAL_EXPERIMENTAL_FCALL 0
#else
#define YAL_EXPERIMENTAL_FCALL 1
#endif

/** Macros for branch prediction */
#if defined(__GNUC__) && ZEND_GCC_VERSION >= 3004 && defined(__i386__)
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#else
#define likely(x)       x
#define unlikely(x)     x
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
