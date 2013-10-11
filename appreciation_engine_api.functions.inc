<?php

if(!function_exists("getIfSet")){
	function getIfSet(&$var) {
		if (isset($var)) {
			return $var;
		}
		return NULL;
	}
}

if(!function_exists("getValueOrDefault")){
	function getValueOrDefault($array, $key, $default = NULL)//@TODO move to iga_common functions.php
	{
		if(!isset($array)){ return NULL; }
		return isset($array[$key]) ? $array[$key] : $default;
	}
}
