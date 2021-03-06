<?php

/**
 * Select query for remote Appreciation Engine data.
 * @author Malcolm Poindexter <malcolm.poindexter@umusic.com>
 */
class AERemoteSelectQuery extends RemoteEntityQuery {
  // TODO: make vars protected once no longer developing.

  /**
   * Determines whether the query is RetrieveMultiple or Retrieve.
   *
   * The query is Multiple by default, until an ID condition causes it to be
   * single.
   */
  public $retrieve_multiple = TRUE;

  /**
   * An array of conditions on the query. These are grouped by the table they
   * are on.
   */
  public $conditions = array();

  /**
   * The fields to retrieve.
   */
  public $fields = array();

  /**
   * Relationships.
   */
  public $relationships = array();

  /**
   * The pager details.
   */
  public $pager = array(
    'limit' => 1000,//variable_get('appreciation_engine_api_cache_requestsize');
    'page'  => 0,
  );

  /**
   * Add a condition to the query.
   *
   * Based on EntityFieldQuery::entityCondition().
   *
   * @param $name
   *  The name of the entity property. We only support 'entity_id' for now.
   */
  function entityCondition($name, $value, $operator = NULL) {
    $field = $this->entity_info['remote entity keys']['remote id'];

    // This isn't used yet.
    $this->conditions[$this->remote_base][] = array(
      'field' => $field,
      'value' => $value,
      'operator' => $operator,
    );

    if ($name == 'entity_id' && (is_null($operator) || $operator == '=')) {
      $this->retrieve_multiple = FALSE;
    }
  }

  /**
   * Modified base query->execute. Appends api key to query and adds base-64 encoded query to response to store as primary key.
   *
   * @return
   *  Remote entity objects as retrieved from the remote connection.
   */
  function execute() {
  	$entity_id = $this->conditions[$this->remote_base][0]['value'];
  	$query = base64_decode($entity_id);
    // Make the request.
    $response = $this->connection->makeRequest('GET', $query, 
    	array('apiKey'=> getValueOrDefault( variable_get('appreciation_engine_api'),'apiKey') ));//add the API Key
    
    if(isset($response)){
    	$response->query_enc = $entity_id;
    	//$response->query = $entity_id;
    }
    if (!$this->retrieve_multiple) {
      $response = array($response);
    }

    return $response;
  }
}

/**
 * Leave unimplemented.
 */
class AERemoteInsertQuery extends RemoteEntityInsertQuery {
}

/**
 * Leave unimplemented.
 */
class AERemoteUpdateQuery extends RemoteEntityUpdateQuery {
}


