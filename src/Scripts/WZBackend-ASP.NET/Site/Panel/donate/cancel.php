<?php

ob_start();

session_start();

 if(isset($_SESSION["user"])){
 	?>
	<html>

		<head>

			<title>DOOM MMO STORE</title>
			<link rel="stylesheet" type="text/css" href="styles/style.css">
		</head>

		<body>
<center>
<div>
		<a href="store.php">Go Back</a></br></br></br></br>
		<font style="color:red">You cancelled your payment!</font>

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