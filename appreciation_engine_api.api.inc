<?php 

/**
 * Interface for retrieving data from the Appreciation Engine API
 * @author Malcolm Poindexter <malcolm.poindexter@umusic.com>
 * @see http://theappreciationengine.com/ae/developers/
 *
 */
class AppreciationEngineAPI {
	
	//API Endpoint Constants 
	const FEED_API = "https://theappreciationengine.com/api/feed";
	const TOP_API = "https://theappreciationengine.com/api/top";
	const CHECK_API = "https://theappreciationengine.com/api/check";
	const INFO_API = "https://theappreciationengine.com/api/info";
	const USER_API = "https://theappreciationengine.com/api/user";
	const RECOMMENDED_CONTENT_API = "http://theappreciationengine.com/api/recommend_content";
	
	public static $ENDPOINTS = array("feed", "top", "user" );
	public static $SUPPORTED_FILTERS = array(
		"feed" => array("clientID"=>'', "campaign"=>'', "country"=>'', "region"=>'', "services"=>'', "keyword"=>'', "period"=>'', "period_end"=>'', "start_date"=>'', "end_date"=>'', "limit"=>''),
		"top" => array("clientID"=>'', "campaign"=>'', "country"=>'', "region"=>'', "services"=>'', "period"=>'', "period_end"=>'', "start_date"=>'', "end_date"=>'', "limit"=>''),
		"user" => array("clientID"=>'', "campaign"=>'', "partnerCode"=>'', "partnerId"=>'', "id"=>'', "services"=>'', "period"=>'', "period_end"=>'', "threshold"=>'', "limit"=>'', "showposition"=>'') //email lookup not supported
	);
	
	private $API_KEY; //the API key is set at the class level & added to each request.
	
	//constructor
	public function AppreciationEngineAPI($apiKey) {
		
		$this->API_KEY = $apiKey;
		
	}
	//@TODO Create, Update, Delete ?
	private function doHttpRequest($url, $post_data = NULL, $method = 'GET', $headers=NULL, $returnHeaders = false) {
		$post_data['apiKey'] = $this->API_KEY;//Add the api key
		$ch = curl_init();
		$options = array(
				CURLOPT_URL => $url,
				CURLOPT_RETURNTRANSFER => TRUE,
				CURLOPT_HEADER => FALSE,
				CURLOPT_SSL_VERIFYPEER => FALSE,
				CURLOPT_CUSTOMREQUEST => $method
		);
	
		if ($returnHeaders) {
			$options[CURLOPT_HEADER] = TRUE;
		}
	
		if ($headers) {
			$options[CURLOPT_HTTPHEADER] = array($headers);
		}
	
		if (isset($post_data)){
			if($method == "POST") {
				//Debug::show($post_data);
				$options[CURLOPT_POST] = TRUE;
				$options[CURLOPT_POSTFIELDS] = $post_data;
			} else {
				$options[CURLOPT_URL] = $url .'?'. http_build_query($post_data,'','&');
			}
		}
		
		curl_setopt_array($ch, $options);
		$response = curl_exec($ch);
		$http_status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
		$last_api_call = $url;
		
		curl_close($ch);
	
		//#Pseudo-code - entities
		/* Load - entity_id = feed_{{http_build_query($post_data)}}
		$ae_entity_feed = entity_metadata_wrapper("appreciation_engine_feed", $entity_id);//pull from cache / DB / api
		return $ae_entity_feed->response->value();
		//return $ae_entity_feed->response->raw();
		//note: before re-insert make sure request succeeds.
		*/		
		
		/* Save
		$ae_entity = $entity_create("appreciation_engine_feed", (object) $response_json_decoded);
		$ae_entity->save();
		//$ae_wrapper = entity_metadata_wrapper("appreciation_engine_feed", $ae_entity);
		//$ae_wrapper->save();
		*/
		
		/* Delete - entity_id = feed_{{query}}
		 $ae_entity_feed = entity_metadata_wrapper("appreciation_engine_feed", $entity_id);//pull from cache / DB / api
		$ae_entity_feed->delete();
		*/
		
		return $response;
	}
	
	private function request($url, $post_data, $useCache = true, $cacheDuration = 30){
		//if $useCache ==true && cache hit
			//return cached response
		//else request, cache_set, return
			return $this->doHttpRequest($url, $post_data);
	}
	
	public function feed($filters){
		//Process the filters
		$response = $this->request(AppreciationEngineAPI::FEED_API, $filters);
		$json = json_decode($response);
		return $json;
	}
	
	public function top($filters){
		$response = $this->request(AppreciationEngineAPI::TOP_API, $filters);
		$json = json_decode($response);
		return $json;
	}
	
	public function user($filters){
		$response = $this->request(AppreciationEngineAPI::USER_API, $filters);
		$json = json_decode($response);
		return $json;
	}
	
	public function check($user_id, $task_id, $period){
		//@TODO taskId
	}
	
	public function info($client_id){
		
	}
	
	public function recommend_content(){
		//@todo threshold 1+
	}
}

?>
