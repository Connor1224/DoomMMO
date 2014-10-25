<?php
ob_start();
session_start();

if(isset($_SESSION["user"])){
	unset($_SESSION["user"]);
	header("location: login.php");
}else{
header("location: login.php");
}
?>