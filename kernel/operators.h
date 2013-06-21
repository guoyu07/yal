
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

/** Strict comparing */
#define YAL_IS_LONG(op1, op2) yal_compare_strict_long(op1, op2 TSRMLS_CC)
#define YAL_IS_STRING(op1, op2) yal_compare_strict_string(op1, op2, strlen(op2))

/** strict boolean comparison */
#define YAL_IS_FALSE(var) Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var)
#define YAL_IS_TRUE(var) Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var)

#define YAL_IS_NOT_FALSE(var) Z_TYPE_P(var) != IS_BOOL || (Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var))
#define YAL_IS_NOT_TRUE(var) Z_TYPE_P(var) != IS_BOOL || (Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var))

/** SQL null empty **/
#define YAL_IS_EMPTY(var) Z_TYPE_P(var) == IS_NULL || (Z_TYPE_P(var) == IS_STRING && !Z_STRLEN_P(var))
#define YAL_IS_NOT_EMPTY(var) !(Z_TYPE_P(var) == IS_NULL || (Z_TYPE_P(var) == IS_STRING && !Z_STRLEN_P(var)))

/** Is scalar */
#define YAL_IS_SCALAR(var) Z_TYPE_P(var) == IS_NULL || Z_TYPE_P(var) == IS_ARRAY || Z_TYPE_P(var) == IS_OBJECT || Z_TYPE_P(var) == IS_RESOURCE
#define YAL_IS_NOT_SCALAR(var) !(Z_TYPE_P(var) == IS_NULL || Z_TYPE_P(var) == IS_ARRAY || Z_TYPE_P(var) == IS_OBJECT || Z_TYPE_P(var) == IS_RESOURCE)

/** Equals/Identical */
#define YAL_IS_EQUAL(op1, op2) yal_is_equal(op1, op2 TSRMLS_CC)
#define YAL_IS_IDENTICAL(op1, op2) yal_is_identical(op1, op2 TSRMLS_CC)

/** Operator functions */
extern int yal_add_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
extern int yal_and_function(zval *result, zval *left, zval *right);

extern void yal_concat_self(zval **left, zval *right TSRMLS_DC);
extern void yal_concat_self_str(zval **left, char *right, int right_length TSRMLS_DC);

/** Strict comparing */
extern int yal_compare_strict_string(zval *op1, char *op2, int op2_length);
extern int yal_compare_strict_long(zval *op1, long op2 TSRMLS_DC);

extern int yal_is_smaller_strict_long(zval *op1, long op2 TSRMLS_DC);
extern int yal_is_smaller_or_equal_strict_long(zval *op1, long op2 TSRMLS_DC);

extern void yal_cast(zval *result, zval *var, zend_uint type);
extern long yal_get_intval(zval *op);
extern int yal_is_numeric(zval *op);

extern int yal_is_equal(zval *op1, zval *op2 TSRMLS_DC);
extern int yal_is_identical(zval *op1, zval *op2 TSRMLS_DC);