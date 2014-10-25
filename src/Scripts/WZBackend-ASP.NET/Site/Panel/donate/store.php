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
		</br>
		<a href="logout.php">Logout</a></br></br></br></br>
		<font>You currently have <font style="color:red"><?= $_SESSION["user"]->{"userdata2"}->{"GamePoints"} ?></font>  GC!</font> </br></br>
		<table>
		<tr><td>Character name : </td><?php
			for($i = 0; $i < count($_SESSION["user"]->{"userchars"});$i++){
				?>
					<td><?=$_SESSION["user"]->{"userchars"}[$i]->{"Gamertag"}?></td>
					
				<?php		
			}
		?></tr>
		<tr><td>Killed Zombies   : <?php
			for($i = 0; $i < count($_SESSION["user"]->{"userchars"});$i++){
				?>
					<td><?=$_SESSION["user"]->{"userchars"}[$i]->{"Stat00"}?></td>
				
				<?php		
			}
		?></td></tr>
		<tr><td>Killed Survivors : <?php
			for($i = 0; $i < count($_SESSION["user"]->{"userchars"});$i++){
				?>
					<td><?=$_SESSION["user"]->{"userchars"}[$i]->{"Stat01"}?></td>
					
				<?php		
			}
		?></td></tr>
		<tr><td>Killed Bandits   : <?php
			for($i = 0; $i < count($_SESSION["user"]->{"userchars"});$i++){
				?>
					<td><?=$_SESSION["user"]->{"userchars"}[$i]->{"Stat02"}?></td>
					
				<?php		
			}
		?></td></tr>
		
		</table>
		<form action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">

<input type="hidden" name="cmd" value="_s-xclick">

<input type="hidden" name="hosted_button_id" value="9H4GQLT3R8QC8">

<input type="hidden" name="custom" value="<?=$_SESSION["user"]->{"userdata"}->{"CustomerID"}?>">

<input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">

<img alt="" border="0" src="https://www.paypalobjects.com/en_US/i/scr/pixel.gif" width="1" height="1">

</form> 


Money to GC converter</br>
<input type="text" id="money">
<button onclick="alert((document.getElementById('money').value / 5) * 2500)">Convert</button>

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