<?php
	$db_apikey = "ACOR4823G%sjYU*@476xnDvYaK@!56";
	function db_connect()
	{
		$db_user   = "wz_api_user";
		$db_pass   = "5vj5K9BznB9K3K83Df6x7kUs9X7x3W";
		$db_dbname = "WarZ";

		$db_serverName     = "localhost,1444";
		$db_connectionInfo = array(
			"UID" => $db_user,
			"PWD" => $db_pass,
			"Database" => $db_dbname,
			"CharacterSet" => "UTF-8"
			//"ReturnDatesAsStrings" => true
			);
		$conn = sqlsrv_connect($db_serverName, $db_connectionInfo);

		if(! $conn)
		{
			//echo "Connection could not be established.\n";
			die( print_r( sqlsrv_errors(), true));
			exit();
		}

		return $conn;
	}
	
	function db_exec($conn, $tsql, $params)
	{
		$stmt  = sqlsrv_query($conn, $tsql, $params);
		if(! $stmt)
		{
			echo "exec failed.\n";
			die( print_r( sqlsrv_errors(), true));
		}

		$member = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
		return $member;
	}

	$conn = db_connect();
?>