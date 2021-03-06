
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

/** Main macros */
#define Y_DEBUG 0

#define Y_NOISY 0
#define Y_SILENT 1

#define Y_NOISY_CC  Y_NOISY TSRMLS_CC
#define Y_SILENT_CC Y_SILENT TSRMLS_CC

#define Y_SEPARATE 256
#define Y_COPY 1024
#define Y_CTOR 4096

#define Y_FETCH_CLASS_SILENT (zend_bool) ZEND_FETCH_CLASS_SILENT TSRMLS_CC

#define SL(str) ZEND_STRL(str)
#define SS(str) ZEND_STRS(str)

/** SPL dependencies */
#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
extern ZEND_API zend_class_entry *zend_ce_iterator;
extern ZEND_API zend_class_entry *zend_ce_arrayaccess;
extern ZEND_API zend_class_entry *zend_ce_serializable;
extern PHPAPI zend_class_entry *spl_ce_RuntimeException;
extern PHPAPI zend_class_entry *spl_ce_Countable;
extern PHPAPI zend_class_entry *spl_ce_SeekableIterator;
#endif

/* Startup functions */
extern void php_yal_init_globals(zend_yal_globals *yal_globals TSRMLS_DC);
extern zend_class_entry *yal_register_internal_interface_ex(zend_class_entry *orig_class_entry, char *parent_name TSRMLS_DC);

/* Globals functions */
extern int yal_init_global(char *global, unsigned int global_length TSRMLS_DC);
extern int yal_get_global(zval **arr, char *global, unsigned int global_length TSRMLS_DC);
extern int yal_get_global_by_index(char *global, char *index, zval *result TSRMLS_DC);

extern int yal_is_callable(zval *var TSRMLS_DC);
extern int yal_function_exists_ex(char *method_name, unsigned int method_len TSRMLS_DC);
extern int yal_function_quick_exists_ex(char *method_name, unsigned int method_len, unsigned long key TSRMLS_DC);

/* Count */
extern void yal_fast_count(zval *result, zval *array TSRMLS_DC);
extern int yal_fast_count_ev(zval *array TSRMLS_DC);

/* Utils functions */
extern void yal_inherit_not_found(char *class_name, char *inherit_name);
extern int yal_is_iterable(zval *arr, HashTable **arr_hash, HashPosition *hash_position, int duplicate, int reverse TSRMLS_DC);

/* Fetch Parameters */
extern int yal_fetch_parameters(int num_args TSRMLS_DC, int required_args, int optional_args, ...);

/* Compatibility with PHP 5.3 */
#ifndef ZVAL_COPY_VALUE
 #define ZVAL_COPY_VALUE(z, v)\
  (z)->value = (v)->value;\
  Z_TYPE_P(z) = Z_TYPE_P(v);
#endif

#ifndef INIT_PZVAL_COPY
 #define INIT_PZVAL_COPY(z, v) ZVAL_COPY_VALUE(z, v);\
  Z_SET_REFCOUNT_P(z, 1);\
  Z_UNSET_ISREF_P(z);
#endif

/** Symbols */
#define YAL_READ_SYMBOL(var, auxarr, name) if (EG(active_symbol_table)){ \
	if (zend_hash_find(EG(active_symbol_table), name, sizeof(name), (void **)  &auxarr) == SUCCESS) { \
			var = *auxarr; \
		} else { \
			ZVAL_NULL(var); \
		} \
	} else { \
		ZVAL_NULL(var); \
	}

/**
 * Return zval checking if it's needed to ctor
 */
#define RETURN_CCTOR(var) { \
		*(return_value) = *(var); \
		if (Z_TYPE_P(var) > IS_BOOL) { \
			yal_copy_ctor(return_value, var); \
		} \
		INIT_PZVAL(return_value) \
	} \
	YAL_MM_RESTORE(); \
	return;

/**
 * Return zval checking if it's needed to ctor, without restoring the memory stack
 */
#define RETURN_CCTORW(var) { \
		*(return_value) = *(var); \
		if (Z_TYPE_P(var) > IS_BOOL) { \
			yal_copy_ctor(return_value, var); \
		} \
		INIT_PZVAL(return_value) \
	} \
	return;

/**
 * Return zval with always ctor
 */
#define RETURN_CTOR(var) { \
		RETVAL_ZVAL(var, 1, 0); \
	} \
	YAL_MM_RESTORE(); \
	return;

/**
 * Return zval with always ctor, without restoring the memory stack
 */
#define RETURN_CTORW(var) { \
		RETVAL_ZVAL(var, 1, 0); \
	} \
	return;

/**
 * Return this pointer
 */
#define RETURN_THIS() { \
		RETVAL_ZVAL(this_ptr, 1, 0); \
	} \
	YAL_MM_RESTORE(); \
	return;

/**
 * Return zval with always ctor, without restoring the memory stack
 */
#define RETURN_THISW() \
	RETURN_ZVAL(this_ptr, 1, 0);

/**
 * Returns variables without ctor
 */
#define RETURN_NCTOR(var) { \
		*(return_value) = *(var); \
		INIT_PZVAL(return_value) \
	} \
	YAL_MM_RESTORE(); \
	return;

/**
 * Returns variables without ctor, without restoring the memory stack
 */
#define RETURN_NCTORW(var) { \
		*(return_value) = *(var); \
		INIT_PZVAL(return_value) \
	} \
	return;

/**
 * Check for ctor on the same return_value
 */
#define RETURN_SCTOR() \
	if (Z_TYPE_P(return_value) > IS_BOOL) { \
		zval_copy_ctor(return_value); \
	} \
	YAL_MM_RESTORE(); \
	return;

/**
 * Check for ctor on the same return_value, without restoring the memory stack
 */
#define RETURN_SCTORW() \
	if (Z_TYPE_P(return_value) > IS_BOOL) { \
		zval_copy_ctor(return_value); \
	} \
	return;

/**
 * Returns a zval in an object member
 */
#define RETURN_MEMBER(object, member_name) \
 	yal_return_property(return_value, object, SL(member_name) TSRMLS_CC); \
	return;

/**
 * Returns a zval in an object member (quick)
 */
#define RETURN_MEMBER_QUICK(object, member_name, key) \
 	yal_return_property_quick(return_value, object, SL(member_name), key TSRMLS_CC); \
	return;

/** Return null restoring memory frame */
#define RETURN_MM_NULL() YAL_MM_RESTORE(); RETURN_NULL();

/** Return bool restoring memory frame */
#define RETURN_MM_FALSE YAL_MM_RESTORE(); RETURN_FALSE;
#define RETURN_MM_TRUE YAL_MM_RESTORE(); RETURN_TRUE;

/** Return string restoring memory frame */
#define RETURN_MM_STRING(str) RETURN_STRING(str); YAL_MM_RESTORE();

#ifndef IS_INTERNED
#define IS_INTERNED(key) 0
#endif

/** Foreach */
#define YAL_GET_FOREACH_KEY(var, hash, hash_pointer) \
	{\
		int hash_type; \
		char *hash_index; \
		uint hash_index_len; \
		ulong hash_num; \
		 \
		YAL_INIT_NVAR(var); \
		hash_type = zend_hash_get_current_key_ex(hash, &hash_index, &hash_index_len, &hash_num, 0, &hash_pointer); \
		if (hash_type == HASH_KEY_IS_STRING) { \
			if (IS_INTERNED(hash_index)) { \
				ZVAL_STRINGL(var, hash_index, hash_index_len - 1, 0); \
			} else { \
				ZVAL_STRINGL(var, hash_index, hash_index_len - 1, 1); \
			} \
		} else { \
			if (hash_type == HASH_KEY_IS_LONG) { \
				ZVAL_LONG(var, hash_num); \
			}\
		}\
	}

#define YAL_GET_FOREACH_VALUE(var) \
	YAL_OBSERVE_VAR(var); \
	var = *hd; \
	Z_ADDREF_P(var);

/** class/interface registering */
#define YAL_REGISTER_CLASS(ns, class_name, name, methods, flags) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		yal_ ##name## _ce = zend_register_internal_class(&ce TSRMLS_CC); \
		yal_ ##name## _ce->ce_flags |= flags;  \
	}

#define YAL_REGISTER_CLASS_EX(ns, class_name, name, parent, methods, flags) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		yal_ ##name## _ce = zend_register_internal_class_ex(&ce, NULL, parent TSRMLS_CC); \
		if (!yal_ ##name## _ce) { \
			yal_inherit_not_found(parent, ZEND_NS_NAME(#ns, #class_name)); \
			return FAILURE;	\
		}  \
		yal_ ##name## _ce->ce_flags |= flags;  \
	}

#define YAL_REGISTER_INTERFACE(ns, classname, name, methods) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #classname, methods); \
		yal_ ##name## _ce = zend_register_internal_interface(&ce TSRMLS_CC); \
	}

#define YAL_REGISTER_INTERFACE_EX(ns, classname, name, parent, methods) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #classname, methods); \
		yal_ ##name## _ce = Yal_register_internal_interface_ex(&ce, parent TSRMLS_CC); \
		if (!yal_ ##name## _ce) { \
			fprintf(stderr, "Can't register interface with parent: %s", parent); \
			return FAILURE;	\
		}  \
	}

/** Method declaration for API generation */
#define YAL_DOC_METHOD(class_name, method)

/** Low overhead parse/fetch parameters */
#define yal_fetch_params(memory_grow, required_params, optional_params, ...) \
	if (yal_fetch_parameters(ZEND_NUM_ARGS() TSRMLS_CC, required_params, optional_params, __VA_ARGS__) == FAILURE) { \
		if (memory_grow) { \
			RETURN_MM_NULL(); \
		} else { \
			RETURN_NULL(); \
		} \
	}
