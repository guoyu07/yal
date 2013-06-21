
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

extern zend_class_entry *yal_acl_roleinterface_ce;

Yal_INIT_CLASS(Yal_Acl_RoleInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yal_acl_roleinterface___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, description)
ZEND_END_ARG_INFO()

Yal_INIT_FUNCS(yal_acl_roleinterface_method_entry){
	PHP_ABSTRACT_ME(Yal_Acl_RoleInterface, __construct, arginfo_yal_acl_roleinterface___construct)
	PHP_ABSTRACT_ME(Yal_Acl_RoleInterface, getName, NULL)
	PHP_ABSTRACT_ME(Yal_Acl_RoleInterface, getDescription, NULL)
	PHP_ABSTRACT_ME(Yal_Acl_RoleInterface, __toString, NULL)
	PHP_FE_END
};

