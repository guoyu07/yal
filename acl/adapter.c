
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
#include "yal.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/object.h"

/**
 * Yal\Acl\Adapter
 *
 * Adapter for Yal\Acl adapters
 */


/**
 * Yal\Acl\Adapter initializer
 */
YAL_INIT_CLASS(Yal_Acl_Adapter){

	YAL_REGISTER_CLASS(Yal\\Acl, Adapter, acl_adapter, yal_acl_adapter_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(yal_acl_adapter_ce, SL("_eventsManager"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(yal_acl_adapter_ce, SL("_defaultAccess"), 1, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yal_acl_adapter_ce, SL("_accessGranted"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_ce, SL("_activeRole"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_ce, SL("_activeResource"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_ce, SL("_activeAccess"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(yal_acl_adapter_ce TSRMLS_CC, 1, yal_events_eventsawareinterface_ce);

	return SUCCESS;
}

/**
 * Sets the events manager
 *
 * @param Yal\Events\ManagerInterface $eventsManager
 */
PHP_METHOD(Yal_Acl_Adapter, setEventsManager){

	zval *events_manager;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &events_manager) == FAILURE) {
		RETURN_NULL();
	}

	yal_update_property_this(this_ptr, SL("_eventsManager"), events_manager TSRMLS_CC);
	
}

/**
 * Returns the internal event manager
 *
 * @return Yal\Events\ManagerInterface
 */
PHP_METHOD(Yal_Acl_Adapter, getEventsManager){


	RETURN_MEMBER(this_ptr, "_eventsManager");
}

/**
 * Sets the default access level (Yal\Acl::ALLOW or Yal\Acl::DENY)
 *
 * @param int $defaultAccess
 */
PHP_METHOD(Yal_Acl_Adapter, setDefaultAction){

	zval *default_access;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &default_access) == FAILURE) {
		RETURN_NULL();
	}

	yal_update_property_this(this_ptr, SL("_defaultAccess"), default_access TSRMLS_CC);
	
}

/**
 * Returns the default ACL access level
 *
 * @return int
 */
PHP_METHOD(Yal_Acl_Adapter, getDefaultAction){


	RETURN_MEMBER(this_ptr, "_defaultAccess");
}

/**
 * Returns the role which the list is checking if it's allowed to certain resource/access
 *
 * @return string
 */
PHP_METHOD(Yal_Acl_Adapter, getActiveRole){


	RETURN_MEMBER(this_ptr, "_activeRole");
}

/**
 * Returns the resource which the list is checking if some role can access it
 *
 * @return string
 */
PHP_METHOD(Yal_Acl_Adapter, getActiveResource){


	RETURN_MEMBER(this_ptr, "_activeResource");
}

/**
 * Returns the access which the list is checking if some role can access it
 *
 * @return string
 */
PHP_METHOD(Yal_Acl_Adapter, getActiveAccess){


	RETURN_MEMBER(this_ptr, "_activeAccess");
}

