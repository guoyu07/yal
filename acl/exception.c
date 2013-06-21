
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

/**
 * Yal\Acl\Exception
 *
 * Class for exceptions thrown by Yal\Acl
 */


/**
 * Yal\Acl\Exception initializer
 */
YAL_INIT_CLASS(Yal_Acl_Exception){

	YAL_REGISTER_CLASS_EX(Yal\\Acl, Exception, acl_exception, "yal\\exception", NULL, 0);

	return SUCCESS;
}

