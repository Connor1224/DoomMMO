<?php

include_once "../../php/dbinfo.inc.php";
function doLogin($email, $pass, $isDev = 0){
	$salt = "g5sc4gs1fz0h";
	$sql = "Select * from Accounts where email=(?) and MD5Password=(?)";// 

	$params = array($email, md5($salt.$pass));	
	$stmt = sqlsrv_query($conn, $sql, $params);
	if( $stmt === false ) {
     	die(print_r( sqlsrv_errors(), true));
	}else{
		//print_r(sqlsrv_has_rows($stmt));
		if(sqlsrv_has_rows($stmt)){
			$ret = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
			if($isDev == 0) return $ret;
			else{
				if($ret["IsDeveloper"] == "1")	return $ret;
				else return "Error: you must be developer!";
			}
		}else{
			return "Error: no user found!";
		}
	}
}
function checkError($err){
	if(substr($err, 0, 6) == "Error:") die($err);
	return 1;
}
?>