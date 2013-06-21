
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

extern zend_class_entry *yal_acl_resource_ce;

YAL_INIT_CLASS(Yal_Acl_Resource);

PHP_METHOD(Yal_Acl_Resource, __construct);
PHP_METHOD(Yal_Acl_Resource, getName);
PHP_METHOD(Yal_Acl_Resource, getDescription);
PHP_METHOD(Yal_Acl_Resource, __toString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yal_acl_resource___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, description)
ZEND_END_ARG_INFO()

YAL_INIT_FUNCS(yal_acl_resource_method_entry){
	PHP_ME(Yal_Acl_Resource, __construct, arginfo_yal_acl_resource___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR) 
	PHP_ME(Yal_Acl_Resource, getName, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Resource, getDescription, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Resource, __toString, NULL, ZEND_ACC_PUBLIC) 
	PHP_FE_END
};

