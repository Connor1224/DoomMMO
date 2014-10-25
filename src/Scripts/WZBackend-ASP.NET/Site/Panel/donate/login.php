<html>
<head>
<title>DOOMMMO Login page</title>
<link rel="stylesheet" type="text/css" href="styles/style.css">
</head>
<body>

<div>

<center>
<img src="images/logo.png"></br>
<?php
ob_start();
session_start();
$salt = "g5sc4gs1fz0h";

		
		
if(!isset($_SESSION["user"])){
	if(isset($_POST["log"])){
		if(isset($_POST["email"]) && isset($_POST["pass"])){
			$ch = curl_init();
			curl_setopt($ch, CURLOPT_URL,"http://192.99.20.149/api/php/api_WebLogin.php");
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS,
			"email=".$_POST["email"]."&pass=".$_POST["pass"]);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			$server_output =  curl_exec($ch);
			///echo $server_output;
			$server_output_json = json_decode($server_output);
			//echo $server_output;
			$_SESSION["email"] = $_POST["email"];
			$_SESSION["pass"] = $_POST["pass"];
			
			if($server_output_json ->{"status"} == "ok"){
				$_SESSION["user"] = $server_output_json;
				echo "Succesfully logged in";
				echo '<meta http-equiv="refresh" content="3;URL=store.php">';
			}else{
				echo "Incorrect email or password";
			}/* 
			$sql = "Select * from Accounts where email=(?) and MD5Password=(?)";// 
			
			$params = array($_POST["email"], md5($salt.$_POST["pass"]));	//
			$stmt = sqlsrv_query($conn, $sql, $params);
			if(sqlsrv_has_rows($stmt)){
				$customerID=  sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
				$customerID=$customerID["CustomerID"];
				echo "Succesfully logged in";
				$_SESSION["user"] = $customerID;
				echo '<meta http-equiv="refresh" content="3;URL=store.php">';
			} */
		/*	while($s = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC)){
				echo $s["email"];
	
			}*/
		}else{
			die("You must enter your email and password");
		}
		
	
	}
}else{
	header("location: store.php");
}
?>
</br>Use your in-game credentials
<form method="post" action="login.php">
	<table>
		<tr>
		<td>Email:</td><td>&nbsp;<input type="text" name="email"/></td>
		
		</tr>
		<tr>
		<td>Password:</td><td>&nbsp;<input type="password" name="pass"/></td>

		</tr>
		<tr>
		<td><input type="submit" name="log" value="Log In"/></td>

		</tr>
	</table>
</form>
</center>
</div>
<div style="position:absolute; bottom:0; left:0;"> 
<img src="images/bg.png">
</div>
</body>



</html>