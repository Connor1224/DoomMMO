<?php

function startsWith($haystack, $needle)
	{
	    return $needle === "" || strpos($haystack, $needle) === 0;
	}
function getPayments($b){
	$lines = file('45Gt23C76N494D.txt');
	
	$total = 0;
	foreach ($lines as $line_num => $line) {
	   if(startsWith($line,"mc_gross")){
	   		$andmark_pos = strrpos($line,"&");
	   		$money = substr($line, strlen("mc_gross="), $andmark_pos - strlen("mc_gross="));
	   		$money= (float)$money;
	   		$total += $money;
	   }
	}
	if($b){
		$total -= 1780.663;
		return "$".$total;
	}
	return "$".$total;
}
function getServerInfo(){
	$lines = file('serverinfo.txt');
	$strpo = strrpos($lines[0],"/");
	$playersonline = substr($lines[0], 0, $strpo);
	
	$serversonline = substr($lines[1], strrpos($lines[1], ":") + 1, strlen($lines[1]) - strrpos($lines[1], ":") - 1);
	return array("playersonline" => $playersonline, "serversonline" => $serversonline);
	}
?>