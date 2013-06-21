
/*
  +------------------------------------------------------------------------+
  | Yal Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Yal Team (http://www.yalphp.com)       |
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
#include "yal.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/object.h"

/**
 * Yal\Acl\Role
 *
 * This class defines role entity and its description
 *
 */


/**
 * Yal\Acl\Role initializer
 */
YAL_INIT_CLASS(Yal_Acl_Role){

	YAL_REGISTER_CLASS(Yal\\Acl, Role, acl_role, yal_acl_role_method_entry, 0);

	zend_declare_property_null(yal_acl_role_ce, SL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_role_ce, SL("_description"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(yal_acl_role_ce TSRMLS_CC, 1, yal_acl_roleinterface_ce);

	return SUCCESS;
}

/**
 * Yal\Acl\Role description
 *
 * @param string $name
 * @param string $description
 */
PHP_METHOD(Yal_Acl_Role, __construct){

	zval *name, *description = NULL;

	YAL_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &name, &description) == FAILURE) {
		RETURN_MM_NULL();
	}

	if (!description) {
		YAL_INIT_VAR(description);
	}
	
	if (YAL_IS_STRING(name, "*")) {
		YAL_THROW_EXCEPTION_STR(yal_acl_exception_ce, "Role name cannot be \"*\"");
		return;
	}
	yal_update_property_this(this_ptr, SL("_name"), name TSRMLS_CC);
	if (Z_TYPE_P(description) != IS_NULL) {
		yal_update_property_this(this_ptr, SL("_description"), description TSRMLS_CC);
	}
	
	YAL_MM_RESTORE();
}

/**
 * Returns the role name
 *
 * @return string
 */
PHP_METHOD(Yal_Acl_Role, getName){


	RETURN_MEMBER(this_ptr, "_name");
}

/**
 * Returns role description
 *
 * @return string
 */
PHP_METHOD(Yal_Acl_Role, getDescription){


	RETURN_MEMBER(this_ptr, "_description");
}

/**
 * Magic method __toString
 *
 * @return string
 */
PHP_METHOD(Yal_Acl_Role, __toString){


	RETURN_MEMBER(this_ptr, "_name");
}

