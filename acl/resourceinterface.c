
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
#include "yal.h"

#include "kernel/main.h"

/**
 * Yal\Acl\ResourceInterface initializer
 */
YAL_INIT_CLASS(Yal_Acl_ResourceInterface){

	YAL_REGISTER_INTERFACE(Yal\\Acl, ResourceInterface, acl_resourceinterface, yal_acl_resourceinterface_method_entry);

	return SUCCESS;
}

/**
 * Yal\Acl\ResourceInterface constructor
 *
 * @param string $name
 * @param string $description
 */
YAL_DOC_METHOD(Yal_Acl_ResourceInterface, __construct);

/**
 * Returns the resource name
 *
 * @return string
 */
YAL_DOC_METHOD(Yal_Acl_ResourceInterface, getName);

/**
 * Returns resource description
 *
 * @return string
 */
YAL_DOC_METHOD(Yal_Acl_ResourceInterface, getDescription);

/**
 * Magic method __toString
 *
 * @return string
 */
YAL_DOC_METHOD(Yal_Acl_ResourceInterface, __toString);

