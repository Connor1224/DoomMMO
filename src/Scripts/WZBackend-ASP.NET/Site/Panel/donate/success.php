	<?php

ob_start();

session_start();

 if(isset($_SESSION["user"])){


 	?><html>

		<head>

			<title>DOOM MMO STORE</title>
			<link rel="stylesheet" type="text/css" href="styles/style.css">
		</head>

		<body>
		<center>
<?php
 	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,"http://192.99.20.149/api/php/api_WebLogin.php");
	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS,
	"email=".$_SESSION["email"]."&pass=".$_SESSION["pass"]);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	$server_output =  curl_exec($ch);
	///echo $server_output;
	
	$server_output_json = json_decode($server_output);
	
	$_SESSION["user"] = $server_output_json;
?>



<div>
		<a href="store.php">Go Back</a></br></br></br></br>
		<font style="color:green">Success! <?=isset($_GET["mc_gross"]) ? "You just donated ".$_GET["mc_gross"]. "$": ""?></font>

</center>
</div>
<div style="position:absolute; bottom:0; left:0;"> 
<img src="images/bg.png" >
</div>

</body>

</html>

<?php

}else{

echo "You Must Login!";

}

 ?>