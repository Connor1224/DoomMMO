<?php

ob_start();

session_start();

 if(isset($_SESSION["user"])){
 	header("location: store.php");
 }else
 {
 	header("location: login.php");
 }