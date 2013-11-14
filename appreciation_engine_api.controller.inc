<?php

/**
 * Extends Remote Entity API Controller to load entities from the AE API if not already in DB / cache.
 * @author Malcolm Poindexter <malcolm.poindexter@umusic.com>
 */
class AppreciationEngineRemonteEntityAPIController extends RemoteEntityAPIDefaultController {
	
	/**
	 * Overridden to load entity by remote_id if not found in DB or cache.
	 *
	 * @see RemoteEntityAPIDefaultController#load($ids, $conditions)
	 */
	public function load($ids = array(), $conditions = array()) {//TODO EntityFieldQuery ? - https://api.drupal.org/api/drupal/includes%21entity.inc/class/EntityFieldQuery/7
		if(count($ids) > 0 && is_numeric($ids[0]) ){//cron loads by eid only. Does not support multiple.
			foreach ($ids as $eid) {
				$conditions['eid'] =  $eid;
			}
			$ids = FALSE;
		}
		
		$uids = array();//not the full query. For caching we can ignore some parameters 
		foreach ($ids as $i => $query_enc){
			$uids[$i] = $this->getUniqueIDfromQuery($query_enc);
		}
		
		$entities = parent::load($uids, $conditions);//only ever performing lookup by remote_id $ids will be local only
		
		// If there are no entities, load from API using ids
		if ( count($entities) === 0 && $this->bypass_remote_retrieve === FALSE) { // ... unless we're not in a normal load (save calls entity_load_unchanged to pull from DB only)
			foreach ($ids as $remote_id) {
				$resource = clients_resource_get_for_component('remote_entity', $this->entityType);
				// Load the remote entity using a custom Entity controller.
				$remote_entity = $resource->remote_entity_load($remote_id);

				// Add entity into collection indexed by base 64 encoded query (remote_id).
				if (!empty($remote_entity)) {
					$entities[$remote_id] = $remote_entity;
				}
			}
		}
	
		return $entities;
	}
	
	/**
	 * takes a base64 encoded query and strips out the non-unique params to compress the entity ID
	 * @param string $query_enc
	 */
	public function getUniqueIDfromQuery($query_enc){
		static $_CACHE_IGNORE_PARAMS;
		if(!isset($_CACHE_IGNORE_PARAMS)){
			$ae_caching = getValueOrDefault(variable_get('appreciation_engine_api'), 'caching');
			$_CACHE_IGNORE_PARAMS = explode(',', getValueOrDefault($ae_caching,'ignoreParams', "exclude_members,includeOneTime,showposition,partnerCode"));
		}
		
		//un-encode
		$query = base64_decode($query_enc);
		//strip params
		parse_str($query, $query_arr);
		foreach($_CACHE_IGNORE_PARAMS as $p){
			if(isset($query_arr[$p])){
				unset($query_arr[$p]);
			}
		}
		//re-encode
		return base64_encode(http_build_query($query_arr));
	}
	
	/**
	 * Get api endpoint cache durations from module config & call base.
	 */
	public function set_expiry($entity, $expiry_time = NULL) {
		// load caching configuration variables.
		$ae_api = variable_get('appreciation_engine_api');
		$api_cache = array(
			'appreciation_engine_api_feed' => getValueOrDefault(getValueOrDefault($ae_api,'caching'), 'feed', 0),
			'appreciation_engine_api_top' => getValueOrDefault(getValueOrDefault($ae_api,'caching'), 'top', 0),
			'appreciation_engine_api_user' => getValueOrDefault(getValueOrDefault($ae_api,'caching'), 'user', 0)
		);
		
		//update expiry time from config variables.
		$this->entityInfo['expiry']['expiry time'] = $api_cache[$this->entityType];
		
		// call base set_expiry
		parent::set_expiry($entity, $expiry_time);
	}
	
}