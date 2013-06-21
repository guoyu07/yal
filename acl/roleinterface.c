
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
#include "php_Yal.h"
#include "yal.h"

#include "kernel/main.h"

/**
 * Yal\Acl\RoleInterface initializer
 */
Yal_INIT_CLASS(Yal_Acl_RoleInterface){

	Yal_REGISTER_INTERFACE(Yal\\Acl, RoleInterface, acl_roleinterface, Yal_acl_roleinterface_method_entry);

	return SUCCESS;
}

/**
 * Yal\Acl\Role constructor
 *
 * @param string $name
 * @param string $description
 */
Yal_DOC_METHOD(Yal_Acl_RoleInterface, __construct);

/**
 * Returns the role name
 *
 * @return string
 */
Yal_DOC_METHOD(Yal_Acl_RoleInterface, getName);

/**
 * Returns role description
 *
 * @return string
 */
Yal_DOC_METHOD(Yal_Acl_RoleInterface, getDescription);

/**
 * Magic method __toString
 *
 * @return string
 */
Yal_DOC_METHOD(Yal_Acl_RoleInterface, __toString);

