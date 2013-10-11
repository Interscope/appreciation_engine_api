<?php


class AppreciationEngineRemonteEntityAPIController extends RemoteEntityAPIDefaultController {
	
	/**
	 * Overridden to load entity by remote_id if not found in DB or cache.
	 *
	 * @see RemoteEntityAPIDefaultController#load($ids, $conditions)
	 */
	public function load($ids = array(), $conditions = array()) {//TODO EntityFieldQuery ? - https://api.drupal.org/api/drupal/includes%21entity.inc/class/EntityFieldQuery/7		
		$entities = parent::load($ids, $conditions);//only ever performing lookup by remote_id $ids will be local only
		//dsm($entities, 'CONTROLLER load');
		
		if ($entities == NULL) { // If there are no entities, load from API using ids
			foreach ($conditions as $field => $value) {
				if($field !== "remote_id"){ continue; }
				$remote_id = $value;

				$resource = clients_resource_get_for_component('remote_entity', $this->entityType);
				// Load the remote entity. This in turn:
				// - calls the connection to build a RemoteEntityQuery
				// - calls us to take care of doing the packing and the saving.
				$remote_entity = $resource->remote_entity_load($remote_id);
				//dsm($remote_entity, 'controller load fetched entity from remote!');

				// Replace the entity in the array, it is now stale.
				// Only return something if the entity exists: if nothing has come
				// back then the remote entity has vanished on the remote site.
				if (!empty($remote_entity)) {
					$entities[$remote_entity->eid] = $remote_entity;
				}
			}
		}
	
		return $entities;
	}
	// TODO public function set_expiry($entity, $expiry_time = NULL)	
}