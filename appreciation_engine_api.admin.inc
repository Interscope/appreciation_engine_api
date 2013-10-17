<?php
/**
 * @file appreciation_engine_api.admin
 * @author Malcolm Poindexter <malcolm.poindexter@umusic.com>
 * Admin forms, settings, and permissions
 */

/**
 * Implements hook_help()
 */
function appreciation_engine_api_common_help($path, $arg){
	//return 'Help text goes here.';/** @todo add help text */
}

/**
 * Implements hook_permission().
 */
function appreciation_engine_api_permission() {
	$permissions['administer appreciation_engine settings'] = array(
		'title' => t('Administer Appreciation Engine settings.'),
	);
	return $permissions;
}

/**
 * Access callback for the entity API.
 */
function appreciation_engine_api_admin_access($op, $entity = NULL, $account = NULL, $entity_type) {
	return user_access('administer appreciation_engine settings', $account);
}

/**
 * Implements hook_menu()
 */
function appreciation_engine_api_menu() {
	$items = array();
	
	$items['admin/config/services/appreciation_engine'] = array(// /settings
			'title' => 'Appreciation Engine',
			'description' => 'Settings for appreciation_engine modules',
			'page callback' => 'drupal_get_form',
			'page arguments' => array('appreciation_engine_api_admin'),
			'access arguments' => array('administer appreciation_engine settings'),//, 'administer appreciation_engine settings'
			'type' => MENU_NORMAL_ITEM,
	);
	
	//TODO Add a clear cache button.
	$items['admin/config/services/appreciation_engine/top/entities'] = array(
			'title' => 'Top API Data',
			'description' => 'Form to manage top api entities stored in the Database.',
			'page callback' => 'drupal_get_form',
			'page arguments' => array('appreciation_engine_api_admin'),
			'access arguments' => array('administer appreciation_engine settings'),
			'type' => MENU_LOCAL_TASK
	);
	
	$items['admin/config/services/appreciation_engine/feed/entities'] = array(
			'title' => 'Feed API Data',
			'description' => 'Form to manage feed api entities stored in the Database.',
			'page callback' => 'drupal_get_form',
			'page arguments' => array('appreciation_engine_api_admin'),
			'access arguments' => array('administer appreciation_engine settings'),
			'type' => MENU_LOCAL_TASK
	);
	
	$items['admin/config/services/appreciation_engine/user/entities'] = array(
			'title' => 'User API Data',
			'description' => 'Form to manage user api entities stored in the Database.',
			'page callback' => 'drupal_get_form',
			'page arguments' => array('appreciation_engine_api_admin'),
			'access arguments' => array('administer appreciation_engine settings'),
			'type' => MENU_LOCAL_TASK
	);
	
	return $items;
}

/**
 * Implements hook_admin()
 * Defines configuration form for the Appreciation Engine Module.
 */
function appreciation_engine_api_admin() {
	$form = array();
	
	$form['#submit'][] = 'appreciation_engine_api_admin_submit';//Register submit callback 
	
	//# API Settings
	$form['appreciation_engine_api'] = array(
			'#type' => 'fieldset',
			'#title' => t('API Settings'),
			'#weight' => 0,
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#tree' => TRUE,
	);
	
	$ae_api = variable_get('appreciation_engine_api');
	
	$form['appreciation_engine_api']['apiKey'] = array(
			'#type' => 'password',//textfield
			'#title' => t('API Key'),
			'#description' => t("API Key used for Appreciation Engine access. (Will only update if entered manually)"),
			'#required' => FALSE,
			'#weight' => 1,
	);
	
	//## Caching Settings
	$form['appreciation_engine_api']['caching'] = array(
			'#type' => 'fieldset',
			'#title' => t('Caching'),
			'#weight' => 2,
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#tree' => TRUE,
	);
	
	$ae_caching = getValueOrDefault($ae_api, 'caching');
	$form['appreciation_engine_api']['caching']['feed'] = array(
			'#type' => 'textfield',
			'#title' => t('Activity Feed Cache Duration'),
			'#default_value' => getValueOrDefault($ae_caching,'feed', 60),
			'#description' => t("The time in seconds to cache responses from the /feed api."),
			'#required' => FALSE,
			'#weight' => 1,
	);
	
	$form['appreciation_engine_api']['caching']['top'] = array(
			'#type' => 'textfield',
			'#title' => t('Leaderboard Cache Duration'),
			'#default_value' => getValueOrDefault($ae_caching,'top', 60),
			'#description' => t("The time in seconds to cache responses from the /top api."),
			'#required' => FALSE,
			'#weight' => 2,
	);
	
	$form['appreciation_engine_api']['caching']['user'] = array(
			'#type' => 'textfield',
			'#title' => t('User Cache Duration'),
			'#default_value' => getValueOrDefault($ae_caching,'user', 60),
			'#description' => t("The time in seconds to cache responses from the /user api."),
			'#required' => FALSE,
			'#weight' => 3,
	);
	
	$form['appreciation_engine_api']['caching']['buffersize'] = array(// TODO support paging buffer.
			'#type' => 'textfield',
			'#default_value' => getValueOrDefault($ae_caching,'buffersize',60),//MAX supported value - allows local paging
			'#title' => t('Request Buffer Size'),
			'#description' => t("The number of results to pull from the API endpoint per cache miss. Requests will then page locally. MAX = 1000"),
			'#required' => FALSE,
			'#weight' => 4,
	);
	
	//@TODO implement item entities?
	/*$form['appreciation_engine_api_store_items'] = array(
	 '#type' => 'checkbox',
			'#default_value' => 0,
			'#title' => t('Store API Items'),
			'#description' => t("By default API responses are stored as entities, but not individual response items."),
			'#required' => FALSE,
			'#weight' => 1,
	);*/
	
	return system_settings_form($form);
}

/**
 * Implements hook_form_submit
 */
function appreciation_engine_api_admin_submit($form, &$form_state) {
	if($form['#id'] == 'appreciation-engine-api-admin'){
		$ae_api = variable_get('appreciation_engine_api');
		
		//if form
		if($form_state['values']['appreciation_engine_api']['apiKey'] ===''){//If the api Key isn't provided, use the saved value.
			$form_state['values']['appreciation_engine_api']['apiKey'] = getValueOrDefault($ae_api,'apiKey','');
		}
	}
}
