
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

/** Fast char position */
extern int yal_memnstr(zval *haystack, zval *needle TSRMLS_DC);
extern int yal_memnstr_str(zval *haystack, char *needle, unsigned int needle_length TSRMLS_DC);

/** Function replacement */
extern void yal_fast_strlen(zval *return_value, zval *str);
extern void yal_fast_strtolower(zval *return_value, zval *str);
extern void yal_fast_join(zval *result, zval *glue, zval *pieces TSRMLS_DC);
extern void yal_fast_join_str(zval *result, char *glue, unsigned int glue_length, zval *pieces TSRMLS_DC);
extern void yal_fast_explode(zval *result, zval *delimiter, zval *str TSRMLS_DC);
extern void yal_fast_strpos(zval *return_value, zval *haystack, zval *needle TSRMLS_DC);
extern void yal_fast_strpos_str(zval *return_value, zval *haystack, char *needle, unsigned int needle_length TSRMLS_DC);
extern void yal_fast_stripos_str(zval *return_value, zval *haystack, char *needle, unsigned int needle_length TSRMLS_DC);
extern void yal_fast_str_replace(zval *return_value, zval *search, zval *replace, zval *subject TSRMLS_DC);

/** Camelize/Uncamelize */
extern void yal_camelize(zval *return_value, zval *str TSRMLS_DC);
extern void yal_uncamelize(zval *return_value, zval *str TSRMLS_DC);

/** Extract named parameters */
extern void yal_extract_named_params(zval *return_value, zval *str, zval *matches);
extern void yal_replace_paths(zval *return_value, zval *pattern, zval *paths, zval *uri TSRMLS_DC);

/** Starts/Ends with */
extern int yal_start_with(zval *str, zval *compared, zval *ignore_case);
extern int yal_start_with_str(zval *str, char *compared, unsigned int compared_length);
extern int yal_start_with_str_str(char *str, unsigned int str_length, char *compared, unsigned int compared_length);
extern int yal_end_with(zval *str, zval *compared, zval *ignore_case);
extern int yal_end_with_str(zval *str, char *compared, unsigned int compared_length);

/** Random string */
extern void yal_random_string(zval *return_value, zval *type, zval *length TSRMLS_DC);

/* Strips extra slashes */
extern void yal_remove_extra_slashes(zval *return_value, zval *str);

/** Generates a unique key for an array/object */
extern void yal_unique_key(zval *return_value, zval *prefix, zval *value TSRMLS_DC);

/** ssprintf */
extern int yal_spprintf(char **message, int max_len, char *format, ...);

/* Substr */
void yal_substr(zval *return_value, zval *str, unsigned long from, unsigned long length TSRMLS_DC);

/** EOL */
zval *yal_eol(int eol TSRMLS_DC);

/** Preg-Match */
void yal_preg_match(zval *return_value, zval *regex, zval *subject, zval *matches TSRMLS_DC);
