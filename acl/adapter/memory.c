
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
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/operators.h"

/**
 * Yal\Acl\Adapter\Memory
 *
 * Manages ACL lists in memory
 *
 *<code>
 *
 *	$acl = new Yal\Acl\Adapter\Memory();
 *
 *	$acl->setDefaultAction(Yal\Acl::DENY);
 *
 *	//Register roles
 *	$roles = array(
 *		'users' => new Yal\Acl\Role('Users'),
 *		'guests' => new Yal\Acl\Role('Guests')
 *	);
 *	foreach ($roles as $role) {
 *		$acl->addRole($role);
 *	}
 *
 *	//Private area resources
 *  $privateResources = array(
 *		'companies' => array('index', 'search', 'new', 'edit', 'save', 'create', 'delete'),
 *		'products' => array('index', 'search', 'new', 'edit', 'save', 'create', 'delete'),
 *		'invoices' => array('index', 'profile')
 *	);
 *	foreach ($privateResources as $resource => $actions) {
 *		$acl->addResource(new Yal\Acl\Resource($resource), $actions);
 *	}
 *
 *	//Public area resources
 *	$publicResources = array(
 *		'index' => array('index'),
 *		'about' => array('index'),
 *		'session' => array('index', 'register', 'start', 'end'),
 *		'contact' => array('index', 'send')
 *	);
 *  foreach ($publicResources as $resource => $actions) {
 *		$acl->addResource(new Yal\Acl\Resource($resource), $actions);
 *	}
 *
 *  //Grant access to public areas to both users and guests
 *	foreach ($roles as $role){
 *		foreach ($publicResources as $resource => $actions) {
 *			$acl->allow($role->getName(), $resource, '*');
 *		}
 *	}
 *
 *	//Grant access to private area to role Users
 *  foreach ($privateResources as $resource => $actions) {
 * 		foreach ($actions as $action) {
 *			$acl->allow('Users', $resource, $action);
 *		}
 *	}
 *
 *</code>
 */


/**
 * Yal\Acl\Adapter\Memory initializer
 */
YAL_INIT_CLASS(Yal_Acl_Adapter_Memory){

	YAL_REGISTER_CLASS_EX(Yal\\Acl\\Adapter, Memory, acl_adapter_memory, "yal\\acl\\adapter", yal_acl_adapter_memory_method_entry, 0);

	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_rolesNames"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_roles"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_resources"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_access"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_roleInherits"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_resourcesNames"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yal_acl_adapter_memory_ce, SL("_accessList"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(yal_acl_adapter_memory_ce TSRMLS_CC, 1, yal_acl_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Yal\Acl\Adapter\Memory constructor
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, __construct){

	zval *a0 = NULL, *a1 = NULL, *a2 = NULL;

	YAL_MM_GROW();

	yal_update_property_empty_array(yal_acl_adapter_memory_ce, this_ptr, SL("_access") TSRMLS_CC);
	
	YAL_INIT_VAR(a0);
	array_init_size(a0, 1);
	add_assoc_bool_ex(a0, SS("*"), 1);
	zend_update_property(yal_acl_adapter_memory_ce, this_ptr, SL("_resourcesNames"), a0 TSRMLS_CC);
	
	YAL_INIT_VAR(a1);
	array_init_size(a1, 1);
	
	YAL_INIT_VAR(a2);
	array_init_size(a2, 1);
	add_assoc_bool_ex(a2, SS("*"), 1);
	yal_array_update_string(&a1, SL("*"), &a2, PH_COPY | PH_SEPARATE TSRMLS_CC);
	zend_update_property(yal_acl_adapter_memory_ce, this_ptr, SL("_accessList"), a1 TSRMLS_CC);
	

	YAL_MM_RESTORE();
}

/**
 * Adds a role to the ACL list. Second parameter allows inheriting access data from other existing role
 *
 * Example:
 * <code>
 * 	$acl->addRole(new Yal\Acl\Role('administrator'), 'consultant');
 * 	$acl->addRole('administrator', 'consultant');
 * </code>
 *
 * @param  Yal\Acl\RoleInterface $role
 * @param  array|string $accessInherits
 * @return boolean
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, addRole){

	zval *role, *access_inherits = NULL, *role_name = NULL, *object = NULL;
	zval *roles_names, *exists, *default_access;
	zval *_access, *success;

	YAL_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &role, &access_inherits) == FAILURE) {
		RETURN_MM_NULL();
	}

	if (!access_inherits) {
		YAL_INIT_VAR(access_inherits);
	}
	
	if (Z_TYPE_P(role) == IS_OBJECT) {
		YAL_INIT_VAR(role_name);
		YAL_CALL_METHOD(role_name, role, "getname");
		YAL_CPY_WRT(object, role);
	} else {
		YAL_CPY_WRT(role_name, role);
	
		YAL_INIT_VAR(object);
		object_init_ex(object, yal_acl_role_ce);
		YAL_CALL_METHOD_PARAMS_1_NORETURN(object, "__construct", role);
	
	}
	
	YAL_OBS_VAR(roles_names);
	yal_read_property_this(&roles_names, this_ptr, SL("_rolesNames"), PH_NOISY_CC);
	if (yal_array_isset(roles_names, role_name)) {
		RETURN_MM_FALSE;
	}
	
	YAL_INIT_VAR(exists);
	ZVAL_BOOL(exists, 1);
	yal_update_property_array_append(this_ptr, SL("_roles"), object TSRMLS_CC);
	yal_update_property_array(this_ptr, SL("_rolesNames"), role_name, exists TSRMLS_CC);
	
	YAL_OBS_VAR(default_access);
	yal_read_property_this(&default_access, this_ptr, SL("_defaultAccess"), PH_NOISY_CC);
	
	YAL_OBS_VAR(_access);
	yal_read_property_this(&_access, this_ptr, SL("_access"), PH_NOISY_CC);
	yal_array_update_zval_string_string_multi_3(&_access, role_name, SL("*"), SL("*"), &default_access, 0 TSRMLS_CC);
	if (Z_TYPE_P(access_inherits) != IS_NULL) {
		YAL_INIT_VAR(success);
		YAL_CALL_METHOD_PARAMS_2(success, this_ptr, "addinherit", role_name, access_inherits);
		RETURN_CCTOR(success);
	}
	
	RETURN_MM_TRUE;
}

/**
 * Do a role inherit from another existing role
 *
 * @param string $roleName
 * @param string $roleToInherit
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, addInherit){

	zval *role_name, *role_to_inherit, *roles_names;
	zval *exception_message = NULL, *role_inherit_name = NULL;
	zval *roles_inherits, *empty_arr, *_roleInherits;

	YAL_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &role_name, &role_to_inherit) == FAILURE) {
		RETURN_MM_NULL();
	}

	YAL_OBS_VAR(roles_names);
	yal_read_property_this(&roles_names, this_ptr, SL("_rolesNames"), PH_NOISY_CC);
	if (!Yal_array_isset(roles_names, role_name)) {
		Yal_INIT_VAR(exception_message);
		Yal_CONCAT_SVS(exception_message, "Role '", role_name, "' does not exist in the role list");
		Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
		return;
	}
	
	if (Z_TYPE_P(role_to_inherit) == IS_OBJECT) {
		Yal_INIT_VAR(role_inherit_name);
		Yal_CALL_METHOD(role_inherit_name, role_to_inherit, "getname");
	} else {
		Yal_CPY_WRT(role_inherit_name, role_to_inherit);
	}
	
	/** 
	 * Check if the role to inherit is valid
	 */
	if (!Yal_array_isset(roles_names, role_inherit_name)) {
		Yal_INIT_NVAR(exception_message);
		Yal_CONCAT_SVS(exception_message, "Role '", role_inherit_name, "' (to inherit) does not exist in the role list");
		Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
		return;
	}
	
	if (Yal_IS_EQUAL(role_inherit_name, role_name)) {
		RETURN_MM_FALSE;
	}
	
	Yal_OBS_VAR(roles_inherits);
	Yal_read_property_this(&roles_inherits, this_ptr, SL("_roleInherits"), PH_NOISY_CC);
	if (!Yal_array_isset(roles_inherits, role_name)) {
		Yal_INIT_VAR(empty_arr);
		array_init(empty_arr);
		Yal_update_property_array(this_ptr, SL("_roleInherits"), role_name, empty_arr TSRMLS_CC);
	}
	
	Yal_OBS_VAR(_roleInherits);
	Yal_read_property_this(&_roleInherits, this_ptr, SL("_roleInherits"), PH_NOISY_CC);
	Yal_array_update_append_multi_2(&_roleInherits, role_name, role_inherit_name, 0 TSRMLS_CC);
	Yal_update_property_this(this_ptr, SL("_roleInherits"), _roleInherits TSRMLS_CC);
	
	/** 
	 * Re-build the access list with its inherited roles
	 */
	Yal_CALL_METHOD_NORETURN(this_ptr, "_rebuildaccesslist");
	RETURN_MM_TRUE;
}

/**
 * Check whether role exist in the roles list
 *
 * @param  string $roleName
 * @return boolean
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, isRole){

	zval *role_name, *roles_names, *is_role = NULL;
	zval *r0 = NULL;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &role_name) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_OBS_VAR(roles_names);
	Yal_read_property_this(&roles_names, this_ptr, SL("_rolesNames"), PH_NOISY_CC);
	
	Yal_INIT_VAR(r0);
	ZVAL_BOOL(r0, Yal_array_isset(roles_names, role_name));
	Yal_CPY_WRT(is_role, r0);
	RETURN_NCTOR(is_role);
}

/**
 * Check whether resource exist in the resources list
 *
 * @param  string $resourceName
 * @return boolean
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, isResource){

	zval *resource_name, *resources_names, *is_resource = NULL;
	zval *r0 = NULL;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &resource_name) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_OBS_VAR(resources_names);
	Yal_read_property_this(&resources_names, this_ptr, SL("_resourcesNames"), PH_NOISY_CC);
	
	Yal_INIT_VAR(r0);
	ZVAL_BOOL(r0, Yal_array_isset(resources_names, resource_name));
	Yal_CPY_WRT(is_resource, r0);
	RETURN_NCTOR(is_resource);
}

/**
 * Adds a resource to the ACL list
 *
 * Access names can be a particular action, by example
 * search, update, delete, etc or a list of them
 *
 * Example:
 * <code>
 * //Add a resource to the the list allowing access to an action
 * $acl->addResource(new Yal\Acl\Resource('customers'), 'search');
 * $acl->addResource('customers', 'search');
 *
 * //Add a resource  with an access list
 * $acl->addResource(new Yal\Acl\Resource('customers'), array('create', 'search'));
 * $acl->addResource('customers', array('create', 'search'));
 * </code>
 *
 * @param   Yal\Acl\Resource $resource
 * @param   array $accessList
 * @return  boolean
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, addResource){

	zval *resource, *access_list = NULL, *resource_name = NULL;
	zval *object = NULL, *resources_names, *exists, *empty_arr;
	zval *status;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &resource, &access_list) == FAILURE) {
		RETURN_MM_NULL();
	}

	if (!access_list) {
		Yal_INIT_VAR(access_list);
	}
	
	if (Z_TYPE_P(resource) == IS_OBJECT) {
		Yal_INIT_VAR(resource_name);
		Yal_CALL_METHOD(resource_name, resource, "getname");
		Yal_CPY_WRT(object, resource);
	} else {
		Yal_CPY_WRT(resource_name, resource);
	
		Yal_INIT_VAR(object);
		object_init_ex(object, Yal_acl_resource_ce);
		Yal_CALL_METHOD_PARAMS_1_NORETURN(object, "__construct", resource_name);
	
	}
	
	Yal_OBS_VAR(resources_names);
	Yal_read_property_this(&resources_names, this_ptr, SL("_resourcesNames"), PH_NOISY_CC);
	if (!Yal_array_isset(resources_names, resource_name)) {
		Yal_INIT_VAR(exists);
		ZVAL_BOOL(exists, 1);
		Yal_update_property_array_append(this_ptr, SL("_resources"), object TSRMLS_CC);
	
		Yal_INIT_VAR(empty_arr);
		array_init(empty_arr);
		Yal_update_property_array(this_ptr, SL("_accessList"), resource_name, empty_arr TSRMLS_CC);
		Yal_update_property_array(this_ptr, SL("_resourcesNames"), resource_name, exists TSRMLS_CC);
	}
	
	Yal_INIT_VAR(status);
	Yal_CALL_METHOD_PARAMS_2(status, this_ptr, "addresourceaccess", resource_name, access_list);
	
	RETURN_CCTOR(status);
}

/**
 * Adds access to resources
 *
 * @param string $resourceName
 * @param mixed $accessList
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, addResourceAccess){

	zval *resource_name, *access_list, *resources_names;
	zval *exception_message, *exists, *access_name = NULL;
	zval *internal_access_list = NULL, *_accessList = NULL;
	zval *r0 = NULL, *r1 = NULL;
	zval *t0 = NULL, *t1 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &resource_name, &access_list) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_OBS_VAR(resources_names);
	Yal_read_property_this(&resources_names, this_ptr, SL("_resourcesNames"), PH_NOISY_CC);
	if (!Yal_array_isset(resources_names, resource_name)) {
		Yal_INIT_VAR(exception_message);
		Yal_CONCAT_SVS(exception_message, "Resource '", resource_name, "' does not exist in ACL");
		Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
		return;
	}
	
	Yal_INIT_VAR(exists);
	ZVAL_BOOL(exists, 1);
	if (Z_TYPE_P(access_list) == IS_ARRAY) { 
	
		if (!Yal_is_iterable(access_list, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
			Yal_GET_FOREACH_VALUE(access_name);
	
			Yal_OBS_NVAR(internal_access_list);
			Yal_read_property_this(&internal_access_list, this_ptr, SL("_accessList"), PH_NOISY_CC);
	
			Yal_OBS_NVAR(r0);
			Yal_array_fetch(&r0, internal_access_list, resource_name, PH_NOISY_CC);
			if (!Yal_array_isset(r0, access_name)) {
				Yal_OBS_NVAR(_accessList);
				Yal_read_property_this(&_accessList, this_ptr, SL("_accessList"), PH_NOISY_CC);
				Yal_array_update_multi_2(&_accessList, resource_name, access_name, &exists, 0 TSRMLS_CC);
				Yal_update_property_this(this_ptr, SL("_accessList"), _accessList TSRMLS_CC);
			}
	
			zend_hash_move_forward_ex(ah0, &hp0);
		}
	
	} else {
		if (Z_TYPE_P(access_list) == IS_STRING) {
	
			Yal_OBS_VAR(t0);
			Yal_read_property_this(&t0, this_ptr, SL("_accessList"), PH_NOISY_CC);
			Yal_OBS_VAR(r1);
			Yal_array_fetch(&r1, t0, resource_name, PH_NOISY_CC);
			if (!Yal_array_isset(r1, access_list)) {
				Yal_OBS_VAR(t1);
				Yal_read_property_this(&t1, this_ptr, SL("_accessList"), PH_NOISY_CC);
				Yal_array_update_multi_2(&t1, resource_name, access_list, &exists, 0 TSRMLS_CC);
				Yal_update_property_this(this_ptr, SL("_accessList"), t1 TSRMLS_CC);
			}
		}
	}
	
	RETURN_MM_TRUE;
}

/**
 * Removes an access from a resource
 *
 * @param string $resourceName
 * @param mixed $accessList
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, dropResourceAccess){

	zval *resource_name, *access_list, *access_name = NULL;
	zval *t0 = NULL, *t1 = NULL;
	zval *r0 = NULL, *r1 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &resource_name, &access_list) == FAILURE) {
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(access_list) == IS_ARRAY) { 
	
		if (!Yal_is_iterable(access_list, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
			Yal_GET_FOREACH_VALUE(access_name);
	
			Yal_OBS_NVAR(t0);
			Yal_read_property_this(&t0, this_ptr, SL("_accessList"), PH_NOISY_CC);
			Yal_OBS_NVAR(r0);
			Yal_array_fetch(&r0, t0, resource_name, PH_NOISY_CC);
			Yal_array_unset(&r0, access_name, PH_SEPARATE);
	
			zend_hash_move_forward_ex(ah0, &hp0);
		}
	
	} else {
		if (zend_is_true(access_list)) {
			Yal_OBS_VAR(t1);
			Yal_read_property_this(&t1, this_ptr, SL("_accessList"), PH_NOISY_CC);
			Yal_OBS_VAR(r1);
			Yal_array_fetch(&r1, t1, resource_name, PH_NOISY_CC);
			Yal_array_unset(&r1, access_list, PH_SEPARATE);
		}
	}
	Yal_CALL_METHOD_NORETURN(this_ptr, "_rebuildaccesslist");
	
	Yal_MM_RESTORE();
}

/**
 * Checks if a role has access to a resource
 *
 * @param string $roleName
 * @param string $resourceName
 * @param string $access
 * @param string $action
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, _allowOrDeny){

	zval *role_name, *resource_name, *access, *action;
	zval *roles_names, *exception_message = NULL, *resources_names;
	zval *default_access, *access_list, *access_name = NULL;
	zval *empty_arr = NULL, *_access = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL;
	zval *t0 = NULL, *t1 = NULL, *t2 = NULL, *t3 = NULL, *t4 = NULL, *t5 = NULL, *t6 = NULL;
	zval *t7 = NULL, *t8 = NULL, *t9 = NULL;
	HashTable *ah0, *ah1;
	HashPosition hp0, hp1;
	zval **hd;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzz", &role_name, &resource_name, &access, &action) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_OBS_VAR(roles_names);
	Yal_read_property_this(&roles_names, this_ptr, SL("_rolesNames"), PH_NOISY_CC);
	if (!Yal_array_isset(roles_names, role_name)) {
		Yal_INIT_VAR(exception_message);
		Yal_CONCAT_SVS(exception_message, "Role \"", role_name, "\" does not exist in ACL");
		Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
		return;
	}
	
	Yal_OBS_VAR(resources_names);
	Yal_read_property_this(&resources_names, this_ptr, SL("_resourcesNames"), PH_NOISY_CC);
	if (!Yal_array_isset(resources_names, resource_name)) {
		Yal_INIT_NVAR(exception_message);
		Yal_CONCAT_SVS(exception_message, "Resource \"", resource_name, "\" does not exist in ACL");
		Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
		return;
	}
	
	Yal_OBS_VAR(default_access);
	Yal_read_property_this(&default_access, this_ptr, SL("_defaultAccess"), PH_NOISY_CC);
	if (Z_TYPE_P(access) == IS_ARRAY) { 
	
		Yal_OBS_VAR(access_list);
		Yal_read_property_this(&access_list, this_ptr, SL("_accessList"), PH_NOISY_CC);
	
		if (!Yal_is_iterable(access, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
			Yal_GET_FOREACH_VALUE(access_name);
	
			Yal_OBS_NVAR(r0);
			Yal_array_fetch(&r0, access_list, resource_name, PH_NOISY_CC);
			if (!Yal_array_isset(r0, access_name)) {
				Yal_INIT_NVAR(exception_message);
				Yal_CONCAT_SVSVS(exception_message, "Acccess '", access_name, "' does not exist in resource '", resource_name, "' in ACL");
				Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
				return;
			}
	
			zend_hash_move_forward_ex(ah0, &hp0);
		}
	
	
		if (!Yal_is_iterable(access, &ah1, &hp1, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) == SUCCESS) {
	
			Yal_GET_FOREACH_VALUE(access_name);
	
			Yal_OBS_NVAR(t0);
			Yal_read_property_this(&t0, this_ptr, SL("_access"), PH_NOISY_CC);
			Yal_OBS_NVAR(r1);
			Yal_array_fetch(&r1, t0, role_name, PH_NOISY_CC);
			if (!Yal_array_isset(r1, resource_name)) {
				Yal_INIT_NVAR(empty_arr);
				array_init(empty_arr);
	
				Yal_OBS_NVAR(_access);
				Yal_read_property_this(&_access, this_ptr, SL("_access"), PH_NOISY_CC);
				Yal_array_update_multi_2(&_access, role_name, resource_name, &empty_arr, 0 TSRMLS_CC);
				Yal_update_property_this(this_ptr, SL("_access"), _access TSRMLS_CC);
			}
	
			Yal_OBS_NVAR(t1);
			Yal_read_property_this(&t1, this_ptr, SL("_access"), PH_NOISY_CC);
			Yal_array_update_zval_zval_zval_multi_3(&t1, role_name, resource_name, access_name, &action, 0 TSRMLS_CC);
	
			Yal_OBS_NVAR(t2);
			Yal_read_property_this(&t2, this_ptr, SL("_access"), PH_NOISY_CC);
	
			Yal_OBS_NVAR(r2);
			Yal_array_fetch(&r2, t2, role_name, PH_NOISY_CC);
	
			Yal_OBS_NVAR(r3);
			Yal_array_fetch(&r3, r2, resource_name, PH_NOISY_CC);
			if (!Yal_array_isset_string(r3, SS("*"))) {
				Yal_OBS_NVAR(t3);
				Yal_read_property_this(&t3, this_ptr, SL("_access"), PH_NOISY_CC);
				Yal_array_update_string_zval_zval_multi_3(&t3, role_name, resource_name, SL("*"), &default_access, 0 TSRMLS_CC);
			}
	
			zend_hash_move_forward_ex(ah1, &hp1);
		}
	
	} else {
		if (!Yal_IS_STRING(access, "*")) {
	
			Yal_OBS_VAR(t4);
			Yal_read_property_this(&t4, this_ptr, SL("_accessList"), PH_NOISY_CC);
			Yal_OBS_VAR(r4);
			Yal_array_fetch(&r4, t4, resource_name, PH_NOISY_CC);
			if (!Yal_array_isset(r4, access)) {
				Yal_INIT_NVAR(exception_message);
				Yal_CONCAT_SVSVS(exception_message, "Acccess '", access, "' does not exist in resource '", resource_name, "' in ACL");
				Yal_THROW_EXCEPTION_ZVAL(Yal_acl_exception_ce, exception_message);
				return;
			}
		}
	
		Yal_OBS_VAR(t5);
		Yal_read_property_this(&t5, this_ptr, SL("_access"), PH_NOISY_CC);
	
		Yal_OBS_VAR(r5);
		Yal_array_fetch(&r5, t5, role_name, PH_NOISY_CC);
		if (!Yal_array_isset(r5, resource_name)) {
			Yal_INIT_NVAR(empty_arr);
			array_init(empty_arr);
	
			Yal_OBS_VAR(t6);
			Yal_read_property_this(&t6, this_ptr, SL("_access"), PH_NOISY_CC);
			Yal_array_update_multi_2(&t6, role_name, resource_name, &empty_arr, 0 TSRMLS_CC);
			Yal_update_property_this(this_ptr, SL("_access"), t6 TSRMLS_CC);
		}
	
		Yal_OBS_VAR(t7);
		Yal_read_property_this(&t7, this_ptr, SL("_access"), PH_NOISY_CC);
	
		Yal_OBS_VAR(r6);
		Yal_array_fetch(&r6, t7, role_name, PH_NOISY_CC);
	
		Yal_OBS_VAR(r7);
		Yal_array_fetch(&r7, r6, resource_name, PH_NOISY_CC);
		if (!Yal_array_isset_string(r7, SS("*"))) {
			Yal_OBS_VAR(t8);
			Yal_read_property_this(&t8, this_ptr, SL("_access"), PH_NOISY_CC);
			Yal_array_update_string_zval_zval_multi_3(&t8, role_name, resource_name, SL("*"), &default_access, 0 TSRMLS_CC);
		}
	
		Yal_OBS_VAR(t9);
		Yal_read_property_this(&t9, this_ptr, SL("_access"), PH_NOISY_CC);
		Yal_array_update_zval_zval_zval_multi_3(&t9, role_name, resource_name, access, &action, 0 TSRMLS_CC);
	}
	
	Yal_CALL_METHOD_NORETURN(this_ptr, "_rebuildaccesslist");
	
	Yal_MM_RESTORE();
}

/**
 * Allow access to a role on a resource
 *
 * You can use '*' as wildcard
 *
 * Example:
 * <code>
 * //Allow access to guests to search on customers
 * $acl->allow('guests', 'customers', 'search');
 *
 * //Allow access to guests to search or create on customers
 * $acl->allow('guests', 'customers', array('search', 'create'));
 *
 * //Allow access to any role to browse on products
 * $acl->allow('*', 'products', 'browse');
 *
 * //Allow access to any role to browse on any resource
 * $acl->allow('*', '*', 'browse');
 * </code>
 *
 * @param string $roleName
 * @param string $resourceName
 * @param mixed $access
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, allow){

	zval *role_name, *resource_name, *access, *action;
	zval *status;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &role_name, &resource_name, &access) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_INIT_VAR(action);
	ZVAL_LONG(action, 1);
	
	Yal_INIT_VAR(status);
	Yal_CALL_METHOD_PARAMS_4(status, this_ptr, "_allowordeny", role_name, resource_name, access, action);
	RETURN_CCTOR(status);
}

/**
 * Deny access to a role on a resource
 *
 * You can use '*' as wildcard
 *
 * Example:
 * <code>
 * //Deny access to guests to search on customers
 * $acl->deny('guests', 'customers', 'search');
 *
 * //Deny access to guests to search or create on customers
 * $acl->deny('guests', 'customers', array('search', 'create'));
 *
 * //Deny access to any role to browse on products
 * $acl->deny('*', 'products', 'browse');
 *
 * //Deny access to any role to browse on any resource
 * $acl->deny('*', '*', 'browse');
 * </code>
 *
 * @param string $roleName
 * @param string $resourceName
 * @param mixed $access
 * @return boolean
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, deny){

	zval *role_name, *resource_name, *access, *action;
	zval *status;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &role_name, &resource_name, &access) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_INIT_VAR(action);
	ZVAL_LONG(action, 0);
	
	Yal_INIT_VAR(status);
	Yal_CALL_METHOD_PARAMS_4(status, this_ptr, "_allowordeny", role_name, resource_name, access, action);
	RETURN_CCTOR(status);
}

/**
 * Check whether a role is allowed to access an action from a resource
 *
 * <code>
 * //Does andres have access to the customers resource to create?
 * $acl->isAllowed('andres', 'Products', 'create');
 *
 * //Do guests have access to any resource to edit?
 * $acl->isAllowed('guests', '*', 'edit');
 * </code>
 *
 * @param  string $role
 * @param  string $resource
 * @param  string $access
 * @return boolean
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, isAllowed){

	zval *role, *resource, *access, *events_manager;
	zval *event_name = NULL, *status, *default_access, *roles_names;
	zval *have_access = NULL, *access_roles, *resource_access = NULL;
	zval *resource_name = NULL;
	zval *t0 = NULL;
	HashTable *ah0, *ah1;
	HashPosition hp0, hp1;
	zval **hd;

	Yal_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &role, &resource, &access) == FAILURE) {
		RETURN_MM_NULL();
	}

	Yal_update_property_this(this_ptr, SL("_activeRole"), role TSRMLS_CC);
	Yal_update_property_this(this_ptr, SL("_activeResource"), resource TSRMLS_CC);
	Yal_update_property_this(this_ptr, SL("_activeAccess"), access TSRMLS_CC);
	
	Yal_OBS_VAR(events_manager);
	Yal_read_property_this(&events_manager, this_ptr, SL("_eventsManager"), PH_NOISY_CC);
	if (Z_TYPE_P(events_manager) == IS_OBJECT) {
	
		Yal_INIT_VAR(event_name);
		ZVAL_STRING(event_name, "acl:beforeCheckAccess", 1);
	
		Yal_INIT_VAR(status);
		Yal_CALL_METHOD_PARAMS_2(status, events_manager, "fire", event_name, this_ptr);
		if (Yal_IS_FALSE(status)) {
			RETURN_CCTOR(status);
		}
	}
	
	Yal_OBS_VAR(default_access);
	Yal_read_property_this(&default_access, this_ptr, SL("_defaultAccess"), PH_NOISY_CC);
	
	/** 
	 * Check if the role exists
	 */
	Yal_OBS_VAR(roles_names);
	Yal_read_property_this(&roles_names, this_ptr, SL("_rolesNames"), PH_NOISY_CC);
	if (!Yal_array_isset(roles_names, role)) {
		RETURN_CCTOR(default_access);
	}
	
	Yal_INIT_VAR(have_access);
	
	Yal_OBS_VAR(t0);
	Yal_read_property_this(&t0, this_ptr, SL("_access"), PH_NOISY_CC);
	
	Yal_OBS_VAR(access_roles);
	Yal_array_fetch(&access_roles, t0, role, PH_NOISY_CC);
	if (Z_TYPE_P(access_roles) == IS_ARRAY) { 
	
		if (!Yal_is_iterable(access_roles, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
			Yal_GET_FOREACH_KEY(resource_name, ah0, hp0);
			Yal_GET_FOREACH_VALUE(resource_access);
	
			if (Yal_IS_EQUAL(resource_name, resource)) {
				if (Yal_array_isset(resource_access, access)) {
					Yal_OBS_NVAR(have_access);
					Yal_array_fetch(&have_access, resource_access, access, PH_NOISY_CC);
					break;
				}
	
				Yal_OBS_NVAR(have_access);
				Yal_array_fetch_string(&have_access, resource_access, SL("*"), PH_NOISY_CC);
				break;
			}
	
			zend_hash_move_forward_ex(ah0, &hp0);
		}
	
	}
	
	if (Z_TYPE_P(have_access) == IS_NULL) {
		if (Z_TYPE_P(access_roles) == IS_ARRAY) { 
	
			if (!Yal_is_iterable(access_roles, &ah1, &hp1, 0, 0 TSRMLS_CC)) {
				return;
			}
	
			while (zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) == SUCCESS) {
	
				Yal_GET_FOREACH_KEY(resource_name, ah1, hp1);
				Yal_GET_FOREACH_VALUE(resource_access);
	
				if (Yal_array_isset_string(resource_access, SS("*"))) {
					if (Yal_array_isset(resource_access, access)) {
						Yal_OBS_NVAR(have_access);
						Yal_array_fetch(&have_access, resource_access, access, PH_NOISY_CC);
						break;
					}
	
					Yal_OBS_NVAR(have_access);
					Yal_array_fetch_string(&have_access, resource_access, SL("*"), PH_NOISY_CC);
					break;
				}
	
				zend_hash_move_forward_ex(ah1, &hp1);
			}
	
		}
	}
	
	Yal_update_property_this(this_ptr, SL("_accessGranted"), have_access TSRMLS_CC);
	if (Z_TYPE_P(events_manager) == IS_OBJECT) {
		Yal_INIT_NVAR(event_name);
		ZVAL_STRING(event_name, "acl:afterCheckAccess", 1);
		Yal_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", event_name, this_ptr);
	}
	
	if (Z_TYPE_P(have_access) == IS_NULL) {
		Yal_MM_RESTORE();
		RETURN_LONG(0);
	}
	
	
	RETURN_CCTOR(have_access);
}

/**
 * Return an array with every role registered in the list
 *
 * @return Yal\Acl\Role[]
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, getRoles){


	RETURN_MEMBER(this_ptr, "_roles");
}

/**
 * Return an array with every resource registered in the list
 *
 * @return Yal\Acl\Resource[]
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, getResources){


	RETURN_MEMBER(this_ptr, "_resources");
}

/**
 * Rebuild the list of access from the inherit lists
 *
 */
PHP_METHOD(Yal_Acl_Adapter_Memory, _rebuildAccessList){

	zval *roles, *number_roles, *pow_roles, *two, *middle_roles;
	zval *middle, *roles_names, *roles_inherits;
	zval *changed = NULL, *i, *internal_access = NULL, *one = NULL, *role_name = NULL;
	zval *role_inherit = NULL, *inherit_internal = NULL, *access = NULL;
	zval *resource_name = NULL, *value = NULL, *name = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL;
	HashTable *ah0, *ah1, *ah2, *ah3;
	HashPosition hp0, hp1, hp2, hp3;
	zval **hd;

	Yal_MM_GROW();

	Yal_OBS_VAR(roles);
	Yal_read_property_this(&roles, this_ptr, SL("_roles"), PH_NOISY_CC);
	
	Yal_INIT_VAR(number_roles);
	Yal_fast_count(number_roles, roles TSRMLS_CC);
	
	Yal_INIT_VAR(pow_roles);
	Z_LVAL_P(pow_roles) = Z_LVAL_P(number_roles) * Z_LVAL_P(number_roles);
	
	Yal_INIT_VAR(two);
	ZVAL_LONG(two, 2);
	
	Yal_INIT_VAR(middle_roles);
	div_function(middle_roles, pow_roles, two TSRMLS_CC);
	
	Yal_INIT_VAR(middle);
	Yal_CALL_FUNC_PARAMS_1(middle, "ceil", middle_roles);
	
	Yal_OBS_VAR(roles_names);
	Yal_read_property_this(&roles_names, this_ptr, SL("_rolesNames"), PH_NOISY_CC);
	
	Yal_OBS_VAR(roles_inherits);
	Yal_read_property_this(&roles_inherits, this_ptr, SL("_roleInherits"), PH_NOISY_CC);
	
	Yal_INIT_VAR(changed);
	ZVAL_BOOL(changed, 1);
	
	Yal_INIT_VAR(i);
	ZVAL_LONG(i, 0);
	while (1) {
	
		Yal_INIT_NVAR(r0);
		is_smaller_or_equal_function(r0, i, middle TSRMLS_CC);
		if (!zend_is_true(r0)) {
			break;
		}
		Yal_OBS_NVAR(internal_access);
		Yal_read_property_this(&internal_access, this_ptr, SL("_access"), PH_NOISY_CC);
		if (Z_TYPE_P(internal_access) != IS_ARRAY) { 
			goto ph_cycle_incr_0;
		}
	
	
		if (!Yal_is_iterable(roles_names, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
			Yal_GET_FOREACH_KEY(role_name, ah0, hp0);
			Yal_GET_FOREACH_VALUE(one);
	
			if (Yal_array_isset(roles_inherits, role_name)) {
	
				Yal_OBS_NVAR(r1);
				Yal_array_fetch(&r1, roles_inherits, role_name, PH_NOISY_CC);
	
				if (!Yal_is_iterable(r1, &ah1, &hp1, 0, 0 TSRMLS_CC)) {
					return;
				}
	
				while (zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) == SUCCESS) {
	
					Yal_GET_FOREACH_VALUE(role_inherit);
	
					if (Yal_array_isset(internal_access, role_inherit)) {
	
						Yal_OBS_NVAR(inherit_internal);
						Yal_array_fetch(&inherit_internal, internal_access, role_inherit, PH_NOISY_CC);
	
						if (!Yal_is_iterable(inherit_internal, &ah2, &hp2, 0, 0 TSRMLS_CC)) {
							return;
						}
	
						while (zend_hash_get_current_data_ex(ah2, (void**) &hd, &hp2) == SUCCESS) {
	
							Yal_GET_FOREACH_KEY(resource_name, ah2, hp2);
							Yal_GET_FOREACH_VALUE(access);
	
	
							if (!Yal_is_iterable(access, &ah3, &hp3, 0, 0 TSRMLS_CC)) {
								return;
							}
	
							while (zend_hash_get_current_data_ex(ah3, (void**) &hd, &hp3) == SUCCESS) {
	
								Yal_GET_FOREACH_KEY(name, ah3, hp3);
								Yal_GET_FOREACH_VALUE(value);
	
								if (Yal_array_isset(internal_access, role_name)) {
	
									Yal_OBS_NVAR(r2);
									Yal_array_fetch(&r2, internal_access, role_name, PH_NOISY_CC);
									if (Yal_array_isset(r2, resource_name)) {
	
										Yal_OBS_NVAR(r3);
										Yal_array_fetch(&r3, internal_access, role_name, PH_NOISY_CC);
										Yal_OBS_NVAR(r4);
										Yal_array_fetch(&r4, r3, resource_name, PH_NOISY_CC);
										if (Yal_array_isset(r4, name)) {
											zend_hash_move_forward_ex(ah3, &hp3);
											continue;
										}
									}
								}
								Yal_array_update_zval_zval_zval_multi_3(&internal_access, role_name, resource_name, name, &value, 0 TSRMLS_CC);
	
								Yal_INIT_NVAR(changed);
								ZVAL_BOOL(changed, 1);
	
								zend_hash_move_forward_ex(ah3, &hp3);
							}
	
	
							zend_hash_move_forward_ex(ah2, &hp2);
						}
	
					}
	
					zend_hash_move_forward_ex(ah1, &hp1);
				}
	
			}
	
			zend_hash_move_forward_ex(ah0, &hp0);
		}
	
		if (zend_is_true(changed)) {
			Yal_update_property_this(this_ptr, SL("_access"), internal_access TSRMLS_CC);
		}
		ph_cycle_incr_0:
		Yal_SEPARATE(i);
		increment_function(i);
	}
	
	Yal_MM_RESTORE();
}

