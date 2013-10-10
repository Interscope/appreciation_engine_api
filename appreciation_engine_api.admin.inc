<?php
/**
 * @file appreciation_engine_api.admin
 * @author Malcolm Poindexter <malcolm.poindexter@umusic.com>
 * Admin forms, settings, and permissions
 */

/**
 * Implements hook_help()
 */
function iga_common_help($path, $arg){

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
function appreciation_engine_api_admin_access($op, $type = NULL, $account = NULL) {
	return user_access('administer appreciation_engine settings', $account);
}

/**
 * Implements hook_menu()
 */
function appreciation_engine_api_common_menu() {
	$items = array();

	$items['admin/config/appreciation_engine'] = array(
			'title' => 'Appreciation Engine',
			'description' => 'Settings related to Appreciation Engine.',
			'position' => 'right',
			'weight' => 0,
			'page callback' => 'system_admin_menu_block_page',
			'access arguments' => array('access administration pages', 'administer appreciation_engine settings'),
			'file' => 'system.admin.inc',
	);

	return $items;
}

/**
 * Implements hook_admin()
 * Defines configuration form for the Appreciation Engine Module.
 */
function appreciation_engine_api_admin() {
	$form = array();

	$form['appreciation_engine_api_key'] = array(
			'#type' => 'password',//textfield
			'#default_value' => variable_get('appreciation_engine_api_key'),
			'#title' => t('API Key'),
			'#description' => t("API Key used for Appreciation Engine APIs."),
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
	
	$form['appreciation_engine_api_cache'] = array(
			'#type' => 'fieldset',
			'#title' => t('Caching Settings'),
			'#weight' => 2,
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#tree' => TRUE,
	);
	
	$form['appreciation_engine_api_cache']['appreciation_engine_api_cache_requestsize'] = array(
			'#type' => 'textfield',
			'#default_value' => 1000,//MAX supported value - allows local paging
			'#title' => t('API Key'),
			'#description' => t("The number of results to pull from the API endpoint per cache miss. Requests will then page locally. MAX = 1000"),
			'#required' => FALSE,
			'#weight' => 1,
	);
	
	$form['appreciation_engine_api_cache']['appreciation_engine_api_cache_feed'] = array(
			'#type' => 'textfield',
			'#title' => t('Activity Feed Cache Duration'),
			'#default_value' => 60,
			'#description' => t("The time in seconds to cache responses from the /feed api."),
			'#required' => FALSE,
			'#weight' => 1,
	);
	
	$form['appreciation_engine_api_cache']['appreciation_engine_api_cache_top'] = array(
			'#type' => 'textfield',
			'#title' => t('Leaderboard Cache Duration'),
			'#default_value' => 60,
			'#description' => t("The time in seconds to cache responses from the /top api."),
			'#required' => FALSE,
			'#weight' => 1,
	);

	return system_settings_form($form);
}
