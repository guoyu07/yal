
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

extern zend_class_entry *yal_acl_resourceinterface_ce;

YAL_INIT_CLASS(Yal_Acl_ResourceInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yal_acl_resourceinterface___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, description)
ZEND_END_ARG_INFO()

YAL_INIT_FUNCS(yal_acl_resourceinterface_method_entry){
	PHP_ABSTRACT_ME(Yal_Acl_ResourceInterface, __construct, arginfo_yal_acl_resourceinterface___construct)
	PHP_ABSTRACT_ME(Yal_Acl_ResourceInterface, getName, NULL)
	PHP_ABSTRACT_ME(Yal_Acl_ResourceInterface, getDescription, NULL)
	PHP_ABSTRACT_ME(Yal_Acl_ResourceInterface, __toString, NULL)
	PHP_FE_END
};

