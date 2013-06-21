
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

extern zend_class_entry *yal_acl_adapter_ce;

YAL_INIT_CLASS(Yal_Acl_Adapter);

PHP_METHOD(Yal_Acl_Adapter, setEventsManager);
PHP_METHOD(Yal_Acl_Adapter, getEventsManager);
PHP_METHOD(Yal_Acl_Adapter, setDefaultAction);
PHP_METHOD(Yal_Acl_Adapter, getDefaultAction);
PHP_METHOD(Yal_Acl_Adapter, getActiveRole);
PHP_METHOD(Yal_Acl_Adapter, getActiveResource);
PHP_METHOD(Yal_Acl_Adapter, getActiveAccess);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yal_acl_adapter_seteventsmanager, 0, 0, 1)
	ZEND_ARG_INFO(0, eventsManager)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yal_acl_adapter_setdefaultaction, 0, 0, 1)
	ZEND_ARG_INFO(0, defaultAccess)
ZEND_END_ARG_INFO()

YAL_INIT_FUNCS(yal_acl_adapter_method_entry){
	PHP_ME(Yal_Acl_Adapter, setEventsManager, arginfo_yal_acl_adapter_seteventsmanager, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Adapter, getEventsManager, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Adapter, setDefaultAction, arginfo_yal_acl_adapter_setdefaultaction, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Adapter, getDefaultAction, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Adapter, getActiveRole, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Adapter, getActiveResource, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME(Yal_Acl_Adapter, getActiveAccess, NULL, ZEND_ACC_PUBLIC) 
	PHP_FE_END
};

