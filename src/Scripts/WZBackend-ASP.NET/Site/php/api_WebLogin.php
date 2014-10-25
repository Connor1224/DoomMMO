<?php
			include_once "dbinfo.inc.php";
			$salt = "g5sc4gs1fz0h";
			$sql = "Select * from Accounts where email=(?) and MD5Password=(?)";// 

			$params = array($_POST["email"], md5($salt.$_POST["pass"]));	
			$stmt = sqlsrv_query($conn, $sql, $params);

			if(sqlsrv_has_rows($stmt)){

				$accountinfo=  sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
				$sql = "Select * from UsersData where CustomerID=(?)";// 

				$params = array($accountinfo["CustomerID"]);	
				$stmt = sqlsrv_query($conn, $sql, $params);
				$userdata = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
				$sql = "Select * from UsersChars where CustomerID=(?)";// 

				$params = array($accountinfo["CustomerID"]);	
				$stmt = sqlsrv_query($conn, $sql, $params);
				$userschars = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
				$response = array("status" =>"ok","userdata" => $accountinfo, "userdata2" => $userdata, "userchars" =>$userschars);
				echo json_encode($response);

			} else{
				echo json_encode(array("status" =>"nope"));
				echo "asdasd";
			}

?>