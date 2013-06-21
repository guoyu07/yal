
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

#define YAL_CONCAT_SV(result, op1, op2) \
	 yal_concat_sv(&result, op1, strlen(op1), op2, 0 TSRMLS_CC);
#define YAL_SCONCAT_SV(result, op1, op2) \
	 yal_concat_sv(&result, op1, strlen(op1), op2, 1 TSRMLS_CC);

#define YAL_CONCAT_SVS(result, op1, op2, op3) \
	 yal_concat_svs(&result, op1, strlen(op1), op2, op3, strlen(op3), 0 TSRMLS_CC);
#define YAL_SCONCAT_SVS(result, op1, op2, op3) \
	 yal_concat_svs(&result, op1, strlen(op1), op2, op3, strlen(op3), 1 TSRMLS_CC);

#define YAL_CONCAT_SVSV(result, op1, op2, op3, op4) \
	 yal_concat_svsv(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, 0 TSRMLS_CC);
#define YAL_SCONCAT_SVSV(result, op1, op2, op3, op4) \
	 yal_concat_svsv(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, 1 TSRMLS_CC);

#define YAL_CONCAT_SVSVS(result, op1, op2, op3, op4, op5) \
	 yal_concat_svsvs(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, strlen(op5), 0 TSRMLS_CC);
#define YAL_SCONCAT_SVSVS(result, op1, op2, op3, op4, op5) \
	 yal_concat_svsvs(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, strlen(op5), 1 TSRMLS_CC);

#define YAL_CONCAT_SVSVSV(result, op1, op2, op3, op4, op5, op6) \
	 yal_concat_svsvsv(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, strlen(op5), op6, 0 TSRMLS_CC);
#define YAL_SCONCAT_SVSVSV(result, op1, op2, op3, op4, op5, op6) \
	 yal_concat_svsvsv(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, strlen(op5), op6, 1 TSRMLS_CC);

#define YAL_CONCAT_SVSVSVS(result, op1, op2, op3, op4, op5, op6, op7) \
	 yal_concat_svsvsvs(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, strlen(op5), op6, op7, strlen(op7), 0 TSRMLS_CC);
#define YAL_SCONCAT_SVSVSVS(result, op1, op2, op3, op4, op5, op6, op7) \
	 yal_concat_svsvsvs(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, strlen(op5), op6, op7, strlen(op7), 1 TSRMLS_CC);

#define YAL_CONCAT_SVSVV(result, op1, op2, op3, op4, op5) \
	 yal_concat_svsvv(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, 0 TSRMLS_CC);
#define YAL_SCONCAT_SVSVV(result, op1, op2, op3, op4, op5) \
	 yal_concat_svsvv(&result, op1, strlen(op1), op2, op3, strlen(op3), op4, op5, 1 TSRMLS_CC);

#define YAL_CONCAT_SVV(result, op1, op2, op3) \
	 yal_concat_svv(&result, op1, strlen(op1), op2, op3, 0 TSRMLS_CC);
#define YAL_SCONCAT_SVV(result, op1, op2, op3) \
	 yal_concat_svv(&result, op1, strlen(op1), op2, op3, 1 TSRMLS_CC);

#define YAL_CONCAT_SVVS(result, op1, op2, op3, op4) \
	 yal_concat_svvs(&result, op1, strlen(op1), op2, op3, op4, strlen(op4), 0 TSRMLS_CC);
#define YAL_SCONCAT_SVVS(result, op1, op2, op3, op4) \
	 yal_concat_svvs(&result, op1, strlen(op1), op2, op3, op4, strlen(op4), 1 TSRMLS_CC);

#define YAL_CONCAT_VS(result, op1, op2) \
	 yal_concat_vs(&result, op1, op2, strlen(op2), 0 TSRMLS_CC);
#define YAL_SCONCAT_VS(result, op1, op2) \
	 yal_concat_vs(&result, op1, op2, strlen(op2), 1 TSRMLS_CC);

#define YAL_CONCAT_VSV(result, op1, op2, op3) \
	 yal_concat_vsv(&result, op1, op2, strlen(op2), op3, 0 TSRMLS_CC);
#define YAL_SCONCAT_VSV(result, op1, op2, op3) \
	 yal_concat_vsv(&result, op1, op2, strlen(op2), op3, 1 TSRMLS_CC);

#define YAL_CONCAT_VSVS(result, op1, op2, op3, op4) \
	 yal_concat_vsvs(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), 0 TSRMLS_CC);
#define YAL_SCONCAT_VSVS(result, op1, op2, op3, op4) \
	 yal_concat_vsvs(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), 1 TSRMLS_CC);

#define YAL_CONCAT_VSVSV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vsvsv(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), op5, 0 TSRMLS_CC);
#define YAL_SCONCAT_VSVSV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vsvsv(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), op5, 1 TSRMLS_CC);

#define YAL_CONCAT_VSVSVS(result, op1, op2, op3, op4, op5, op6) \
	 yal_concat_vsvsvs(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), op5, op6, strlen(op6), 0 TSRMLS_CC);
#define YAL_SCONCAT_VSVSVS(result, op1, op2, op3, op4, op5, op6) \
	 yal_concat_vsvsvs(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), op5, op6, strlen(op6), 1 TSRMLS_CC);

#define YAL_CONCAT_VSVSVSV(result, op1, op2, op3, op4, op5, op6, op7) \
	 yal_concat_vsvsvsv(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), op5, op6, strlen(op6), op7, 0 TSRMLS_CC);
#define YAL_SCONCAT_VSVSVSV(result, op1, op2, op3, op4, op5, op6, op7) \
	 yal_concat_vsvsvsv(&result, op1, op2, strlen(op2), op3, op4, strlen(op4), op5, op6, strlen(op6), op7, 1 TSRMLS_CC);

#define YAL_CONCAT_VSVV(result, op1, op2, op3, op4) \
	 yal_concat_vsvv(&result, op1, op2, strlen(op2), op3, op4, 0 TSRMLS_CC);
#define YAL_SCONCAT_VSVV(result, op1, op2, op3, op4) \
	 yal_concat_vsvv(&result, op1, op2, strlen(op2), op3, op4, 1 TSRMLS_CC);

#define YAL_CONCAT_VSVVV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vsvvv(&result, op1, op2, strlen(op2), op3, op4, op5, 0 TSRMLS_CC);
#define YAL_SCONCAT_VSVVV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vsvvv(&result, op1, op2, strlen(op2), op3, op4, op5, 1 TSRMLS_CC);

#define YAL_CONCAT_VV(result, op1, op2) \
	 yal_concat_vv(&result, op1, op2, 0 TSRMLS_CC);
#define YAL_SCONCAT_VV(result, op1, op2) \
	 yal_concat_vv(&result, op1, op2, 1 TSRMLS_CC);

#define YAL_CONCAT_VVS(result, op1, op2, op3) \
	 yal_concat_vvs(&result, op1, op2, op3, strlen(op3), 0 TSRMLS_CC);
#define YAL_SCONCAT_VVS(result, op1, op2, op3) \
	 yal_concat_vvs(&result, op1, op2, op3, strlen(op3), 1 TSRMLS_CC);

#define YAL_CONCAT_VVSV(result, op1, op2, op3, op4) \
	 yal_concat_vvsv(&result, op1, op2, op3, strlen(op3), op4, 0 TSRMLS_CC);
#define YAL_SCONCAT_VVSV(result, op1, op2, op3, op4) \
	 yal_concat_vvsv(&result, op1, op2, op3, strlen(op3), op4, 1 TSRMLS_CC);

#define YAL_CONCAT_VVV(result, op1, op2, op3) \
	 yal_concat_vvv(&result, op1, op2, op3, 0 TSRMLS_CC);
#define YAL_SCONCAT_VVV(result, op1, op2, op3) \
	 yal_concat_vvv(&result, op1, op2, op3, 1 TSRMLS_CC);

#define YAL_CONCAT_VVVSV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vvvsv(&result, op1, op2, op3, op4, strlen(op4), op5, 0 TSRMLS_CC);
#define YAL_SCONCAT_VVVSV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vvvsv(&result, op1, op2, op3, op4, strlen(op4), op5, 1 TSRMLS_CC);

#define YAL_CONCAT_VVVV(result, op1, op2, op3, op4) \
	 yal_concat_vvvv(&result, op1, op2, op3, op4, 0 TSRMLS_CC);
#define YAL_SCONCAT_VVVV(result, op1, op2, op3, op4) \
	 yal_concat_vvvv(&result, op1, op2, op3, op4, 1 TSRMLS_CC);

#define YAL_CONCAT_VVVVSVV(result, op1, op2, op3, op4, op5, op6, op7) \
	 yal_concat_vvvvsvv(&result, op1, op2, op3, op4, op5, strlen(op5), op6, op7, 0 TSRMLS_CC);
#define YAL_SCONCAT_VVVVSVV(result, op1, op2, op3, op4, op5, op6, op7) \
	 yal_concat_vvvvsvv(&result, op1, op2, op3, op4, op5, strlen(op5), op6, op7, 1 TSRMLS_CC);

#define YAL_CONCAT_VVVVV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vvvvv(&result, op1, op2, op3, op4, op5, 0 TSRMLS_CC);
#define Yal_SCONCAT_VVVVV(result, op1, op2, op3, op4, op5) \
	 yal_concat_vvvvv(&result, op1, op2, op3, op4, op5, 1 TSRMLS_CC);


extern void yal_concat_sv(zval **result, char *op1, zend_uint op1_len, zval *op2, int self_var TSRMLS_DC);
extern void yal_concat_svs(zval **result, char *op1, zend_uint op1_len, zval *op2, char *op3, zend_uint op3_len, int self_var TSRMLS_DC);
extern void yal_concat_svsv(zval **result, char *op1, zend_uint op1_len, zval *op2, char *op3, zend_uint op3_len, zval *op4, int self_var TSRMLS_DC);
extern void yal_concat_svsvs(zval **result, char *op1, zend_uint op1_len, zval *op2, char *op3, zend_uint op3_len, zval *op4, char *op5, zend_uint op5_len, int self_var TSRMLS_DC);
extern void yal_concat_svsvsv(zval **result, char *op1, zend_uint op1_len, zval *op2, char *op3, zend_uint op3_len, zval *op4, char *op5, zend_uint op5_len, zval *op6, int self_var TSRMLS_DC);
extern void yal_concat_svsvsvs(zval **result, char *op1, zend_uint op1_len, zval *op2, char *op3, zend_uint op3_len, zval *op4, char *op5, zend_uint op5_len, zval *op6, char *op7, zend_uint op7_len, int self_var TSRMLS_DC);
extern void yal_concat_svsvv(zval **result, char *op1, zend_uint op1_len, zval *op2, char *op3, zend_uint op3_len, zval *op4, zval *op5, int self_var TSRMLS_DC);
extern void yal_concat_svv(zval **result, char *op1, zend_uint op1_len, zval *op2, zval *op3, int self_var TSRMLS_DC);
extern void yal_concat_svvs(zval **result, char *op1, zend_uint op1_len, zval *op2, zval *op3, char *op4, zend_uint op4_len, int self_var TSRMLS_DC);
extern void yal_concat_vs(zval **result, zval *op1, char *op2, zend_uint op2_len, int self_var TSRMLS_DC);
extern void yal_concat_vsv(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, int self_var TSRMLS_DC);
extern void yal_concat_vsvs(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, char *op4, zend_uint op4_len, int self_var TSRMLS_DC);
extern void yal_concat_vsvsv(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, char *op4, zend_uint op4_len, zval *op5, int self_var TSRMLS_DC);
extern void yal_concat_vsvsvs(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, char *op4, zend_uint op4_len, zval *op5, char *op6, zend_uint op6_len, int self_var TSRMLS_DC);
extern void yal_concat_vsvsvsv(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, char *op4, zend_uint op4_len, zval *op5, char *op6, zend_uint op6_len, zval *op7, int self_var TSRMLS_DC);
extern void yal_concat_vsvv(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, zval *op4, int self_var TSRMLS_DC);
extern void yal_concat_vsvvv(zval **result, zval *op1, char *op2, zend_uint op2_len, zval *op3, zval *op4, zval *op5, int self_var TSRMLS_DC);
extern void yal_concat_vv(zval **result, zval *op1, zval *op2, int self_var TSRMLS_DC);
extern void yal_concat_vvs(zval **result, zval *op1, zval *op2, char *op3, zend_uint op3_len, int self_var TSRMLS_DC);
extern void yal_concat_vvsv(zval **result, zval *op1, zval *op2, char *op3, zend_uint op3_len, zval *op4, int self_var TSRMLS_DC);
extern void yal_concat_vvv(zval **result, zval *op1, zval *op2, zval *op3, int self_var TSRMLS_DC);
extern void yal_concat_vvvsv(zval **result, zval *op1, zval *op2, zval *op3, char *op4, zend_uint op4_len, zval *op5, int self_var TSRMLS_DC);
extern void yal_concat_vvvv(zval **result, zval *op1, zval *op2, zval *op3, zval *op4, int self_var TSRMLS_DC);
extern void yal_concat_vvvvsvv(zval **result, zval *op1, zval *op2, zval *op3, zval *op4, char *op5, zend_uint op5_len, zval *op6, zval *op7, int self_var TSRMLS_DC);
extern void yal_concat_vvvvv(zval **result, zval *op1, zval *op2, zval *op3, zval *op4, zval *op5, int self_var TSRMLS_DC);
