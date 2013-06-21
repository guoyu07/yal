
/*
  +------------------------------------------------------------------------+
  | yal Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 yal Team (http://www.yalphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@yalphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@yalphp.com>                      |
  |          Eduar Carvajal <eduar@yalphp.com>                         |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_yal.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"

/*
 * Memory Frames/Virtual Symbol Scopes
 *------------------------------------
 *
 * yal uses memory frames to track the variables used whithin a method
 * in order to free them or reduce their reference count accordingly before
 * exit the method in execution.
 *
 * The whole memory frame is an open double-linked list which start is an
 * allocated empty frame that points to the real first frame. The start
 * memory frame is globally accesed using yal_GLOBAL(start_frame)
 *
 * Not all methods must grown/restore the yal_memory_entry.
 */

/**
 * Initializes/Reinitializes a variable
 */
inline void yal_init_nvar(zval **var TSRMLS_DC) {
    if (*var) {
        if (Z_REFCOUNT_PP(var) > 1) {
            Z_DELREF_PP(var);
            ALLOC_ZVAL(*var);
            Z_SET_REFCOUNT_PP(var, 1);
            Z_UNSET_ISREF_PP(var);
        } else {
            zval_ptr_dtor(var);
            YAL_ALLOC_ZVAL(*var);
        }
    } else {
        yal_memory_alloc(var TSRMLS_CC);
    }
}

/**
 * Copy/Write variables caring of reference counting
 */
inline void yal_cpy_wrt(zval **dest, zval *var TSRMLS_DC) {

    if (*dest) {
        if (Z_REFCOUNT_PP(dest) > 0) {
            zval_ptr_dtor(dest);
        }
    } else {
        yal_memory_observe(dest TSRMLS_CC);
    }

    Z_ADDREF_P(var);
    *dest = var;
}

/**
 * Copy/Write variables caring of reference counting also duplicating the origin ctor
 */
inline void yal_cpy_wrt_ctor(zval **dest, zval *var TSRMLS_DC) {

    if (*dest) {
        if (Z_REFCOUNT_PP(dest) > 0) {
            zval_ptr_dtor(dest);
        }
    } else {
        yal_memory_observe(dest TSRMLS_CC);
    }

    Z_ADDREF_P(var);
    *dest = var;
    zval_copy_ctor(*dest);
    Z_SET_REFCOUNT_PP(dest, 1);
    Z_UNSET_ISREF_PP(dest);
}

/**
 * Adds a memory frame in the current executed method
 */
int YAL_FASTCALL yal_memory_grow_stack(TSRMLS_D) {

    yal_memory_entry *entry, *start;

    if (!YAL_GLOBAL(start_memory)) {
        start = (yal_memory_entry *) emalloc(sizeof(yal_memory_entry));
        start->pointer = -1;
        start->prev = NULL;
        start->next = NULL;
        YAL_GLOBAL(start_memory) = start;
        YAL_GLOBAL(active_memory) = start;
    }

    entry = (yal_memory_entry *) emalloc(sizeof(yal_memory_entry));
    entry->pointer = -1;
    entry->prev = YAL_GLOBAL(active_memory);
    YAL_GLOBAL(active_memory)->next = entry;
    YAL_GLOBAL(active_memory) = entry;

    return SUCCESS;
}

/**
 * Finishes the current memory stack by releasing allocated memory
 */
int YAL_FASTCALL yal_memory_restore_stack(TSRMLS_D) {

    register int i;
    yal_memory_entry *prev, *active_memory = YAL_GLOBAL(active_memory);
    yal_symbol_table *active_symbol_table;

    if (active_memory == NULL) {
        return FAILURE;
    }

    if (YAL_GLOBAL(active_symbol_table)) {
        active_symbol_table = YAL_GLOBAL(active_symbol_table);
        if (active_symbol_table->scope == active_memory) {
            zend_hash_destroy(EG(active_symbol_table));
            FREE_HASHTABLE(EG(active_symbol_table));
            EG(active_symbol_table) = active_symbol_table->symbol_table;
            YAL_GLOBAL(active_symbol_table) = active_symbol_table->prev;
            efree(active_symbol_table);
        }
    }

    if (active_memory->pointer > -1) {

        for (i = active_memory->pointer; i >= 0; i--) {

            if (active_memory->addresses[i] == NULL) {
                continue;
            }

            if (*active_memory->addresses[i] == NULL) {
                continue;
            }

            if ((Z_REFCOUNT_PP(active_memory->addresses[i]) - 1) == 0) {
                zval_ptr_dtor(active_memory->addresses[i]);
                active_memory->addresses[i] = NULL;
            } else {
                Z_DELREF_PP(active_memory->addresses[i]);
                if (Z_REFCOUNT_PP(active_memory->addresses[i]) == 1) {
                    active_memory->addresses[i] = NULL;
                }
            }
        }
    }

    prev = active_memory->prev;
    efree(YAL_GLOBAL(active_memory));
    YAL_GLOBAL(active_memory) = prev;
    if (prev != NULL) {
        YAL_GLOBAL(active_memory)->next = NULL;
        if (YAL_GLOBAL(active_memory) == YAL_GLOBAL(start_memory)) {
            efree(YAL_GLOBAL(active_memory));
            YAL_GLOBAL(start_memory) = NULL;
            YAL_GLOBAL(active_memory) = NULL;
        }
    } else {
        YAL_GLOBAL(start_memory) = NULL;
        YAL_GLOBAL(active_memory) = NULL;
    }

    return SUCCESS;
}

/**
 * Finishes memory stack when PHP throws a fatal error
 */
int YAL_FASTCALL yal_clean_shutdown_stack(TSRMLS_D) {

    #if !ZEND_DEBUG && PHP_VERSION_ID <= 50400

    yal_memory_entry *prev, *active_memory = YAL_GLOBAL(active_memory);

    while (active_memory != NULL) {

        prev = active_memory->prev;
        efree(active_memory);
        active_memory = prev;
        if (prev != NULL) {
            active_memory->next = NULL;
            if (active_memory == YAL_GLOBAL(start_memory)) {
                efree(active_memory);
                YAL_GLOBAL(start_memory) = NULL;
                active_memory = NULL;
            }
        } else {
            YAL_GLOBAL(start_memory) = NULL;
            active_memory = NULL;
        }

    }

    #endif

    YAL_GLOBAL(start_memory) = NULL;
    YAL_GLOBAL(active_memory) = NULL;

    return SUCCESS;
}

/**
 * Observes a memory pointer to release its memory at the end of the request
 */
int YAL_FASTCALL yal_memory_observe(zval **var TSRMLS_DC) {

    yal_memory_entry *active_memory = YAL(active_memory);

    active_memory->pointer++;

    #ifndef YAL_RELEASE
    if (active_memory->pointer >= (YAL_MAX_MEMORY_STACK - 1)) {
        fprintf(stderr, "ERROR: Yal memory stack is too small %d\n", YAL_MAX_MEMORY_STACK);
        return FAILURE;
    }
    #endif

    active_memory->addresses[active_memory->pointer] = var;
    active_memory->addresses[active_memory->pointer + 1] = NULL;

    return SUCCESS;
}

/**
 * Observe a variable and allocates memory for it
 */
int YAL_FASTCALL yal_memory_alloc(zval **var TSRMLS_DC) {

    yal_memory_entry *active_memory = YAL_GLOBAL(active_memory);

    active_memory->pointer++;

    #ifndef YAL_RELEASE
    if (active_memory->pointer >= (YAL_MAX_MEMORY_STACK - 1)) {
        fprintf(stderr, "ERROR: Yal memory stack is too small %d\n", YAL_MAX_MEMORY_STACK);
        return FAILURE;
    }
    #endif

    active_memory->addresses[active_memory->pointer] = var;
    active_memory->addresses[active_memory->pointer + 1] = NULL;

    ALLOC_ZVAL(*var);
    INIT_PZVAL(*var);
    ZVAL_NULL(*var);

    return SUCCESS;
}

/**
 * Removes a memory pointer from the active memory pool
 */
int YAL_FASTCALL yal_memory_remove(zval **var TSRMLS_DC) {
    zval_ptr_dtor(var);
    *var = NULL;
    return SUCCESS;
}

/**
 * Cleans the yal memory stack recursivery
 */
int YAL_FASTCALL yal_clean_restore_stack(TSRMLS_D) {
    while (YAL_GLOBAL(active_memory) != NULL) {
        yal_memory_restore_stack(TSRMLS_C);
    }
    return SUCCESS;
}

/**
 * Copies a variable only if its refcount is greater than 1
 */
void YAL_FASTCALL yal_copy_ctor(zval *destiny, zval *origin) {
    if (Z_REFCOUNT_P(origin) > 1) {
        zval_copy_ctor(destiny);
    } else {
        ZVAL_NULL(origin);
    }
}

/**
 * Creates virtual symbol tables dynamically
 */
void yal_create_symbol_table(TSRMLS_D) {

    yal_symbol_table *entry;
    HashTable *symbol_table;

    #ifndef YAL_RELEASE
    if (!yal_GLOBAL(active_memory)) {
        fprintf(stderr, "ERROR: Trying to create a virtual symbol table without a memory frame");
        return;
    }
    #endif

    entry = (yal_symbol_table *) emalloc(sizeof(yal_symbol_table));
    entry->scope = YAL_GLOBAL(active_memory);
    entry->symbol_table = EG(active_symbol_table);
    entry->prev = YAL_GLOBAL(active_symbol_table);
    YAL_GLOBAL(active_symbol_table) = entry;

    ALLOC_HASHTABLE(symbol_table);
    zend_hash_init(symbol_table, 0, NULL, ZVAL_PTR_DTOR, 0);
    EG(active_symbol_table) = symbol_table;
}

/**
 * Restores all the virtual symbol tables
 */
void yal_clean_symbol_tables(TSRMLS_D) {

    /*unsigned int i;

    if (yal_GLOBAL(symbol_tables)) {
        for (i = yal_GLOBAL(number_symbol_tables); i > 0; i--) {
            EG(active_symbol_table) = yal_GLOBAL(symbol_tables)[i - 1];
        }
        efree(yal_GLOBAL(symbol_tables));
        yal_GLOBAL(symbol_tables) = NULL;
    }*/
}

/**
 * Exports symbols to the active symbol table
 */
int yal_set_symbol(zval *key_name, zval *value TSRMLS_DC) {

    if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
    }

    if (EG(active_symbol_table)) {
        if (Z_TYPE_P(key_name) == IS_STRING) {
            Z_ADDREF_P(value);
            zend_hash_update(EG(active_symbol_table), Z_STRVAL_P(key_name), Z_STRLEN_P(key_name) + 1, &value, sizeof(zval *), NULL);
            if (EG(exception)) {
                return FAILURE;
            }
        }
    }

    return SUCCESS;
}

/**
 * Exports a string symbol to the active symbol table
 */
int yal_set_symbol_str(char *key_name, unsigned int key_length, zval *value TSRMLS_DC) {

    if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
    }

    if (&EG(symbol_table)) {
        Z_ADDREF_P(value);
        zend_hash_update(&EG(symbol_table), key_name, key_length, &value, sizeof(zval *), NULL);
        if (EG(exception)) {
            return FAILURE;
        }
    }

    return SUCCESS;
}