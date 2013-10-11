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
	drupal_set_message($account);
	return user_access('administer appreciation_engine settings', $account);
}

/**
 * Implements hook_menu()
 */
function appreciation_engine_api_menu() {
	$items = array();

	/*$items['admin/config/appreciation_engine'] = array(
			'title' => 'Appreciation Engine',
			'description' => 'Settings related to Appreciation Engine.',
			'position' => 'right',
			'weight' => 0,
			'page callback' => 'system_admin_menu_block_page',
			'access arguments' => array('access administration pages'),//, 'administer appreciation_engine settings'
			'file' => 'system.admin.inc',
	);*/
	
	$items['admin/config/services/appreciation_engine'] = array(// /settings
			'title' => 'Appreciation Engine',
			'description' => 'Settings for appreciation_engine modules',
			'page callback' => 'drupal_get_form',
			'page arguments' => array('appreciation_engine_api_admin'),
			'access arguments' => array('access administration pages', 'administer appreciation_engine settings'),
			'type' => MENU_NORMAL_ITEM,
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
			'#description' => t("API Key used for Appreciation Engine access."),
			'#required' => FALSE,
			'#weight' => 1,
	);
	
	//@TODO implement item entities.
	/*$form['appreciation_engine_api_store_items'] = array(
			'#type' => 'checkbox',
			'#default_value' => 0,
			'#title' => t('Store API Items'),
			'#description' => t("By default API responses are stored as entities, but not individual response items."),
			'#required' => FALSE,
			'#weight' => 1,
	);*/
	
	//## Caching Settings
	$form['appreciation_engine_api']['caching'] = array(
			'#type' => 'fieldset',
			'#title' => t('Caching'),
			'#weight' => 2,
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#tree' => TRUE,
	);
	
	$form['appreciation_engine_api']['caching']['feed'] = array(
			'#type' => 'textfield',
			'#title' => t('Activity Feed Cache Duration'),
			'#default_value' => 60,
			'#description' => t("The time in seconds to cache responses from the /feed api."),
			'#required' => FALSE,
			'#weight' => 1,
	);
	
	$form['appreciation_engine_api']['caching']['top'] = array(
			'#type' => 'textfield',
			'#title' => t('Leaderboard Cache Duration'),
			'#default_value' => 60,
			'#description' => t("The time in seconds to cache responses from the /top api."),
			'#required' => FALSE,
			'#weight' => 2,
	);
	
	$form['appreciation_engine_api']['caching']['user'] = array(
			'#type' => 'textfield',
			'#title' => t('User Cache Duration'),
			'#default_value' => 60,
			'#description' => t("The time in seconds to cache responses from the /user api."),
			'#required' => FALSE,
			'#weight' => 3,
	);
	
	$form['appreciation_engine_api']['caching']['buffersize'] = array(// TODO support paging buffer.
			'#type' => 'textfield',
			'#default_value' => 1000,//MAX supported value - allows local paging
			'#title' => t('Request Buffer Size'),
			'#description' => t("The number of results to pull from the API endpoint per cache miss. Requests will then page locally. MAX = 1000"),
			'#required' => FALSE,
			'#weight' => 4,
	);
	
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
