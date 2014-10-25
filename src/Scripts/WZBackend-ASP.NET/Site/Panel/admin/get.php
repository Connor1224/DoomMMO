
<?php
include_once "dbinfo.inc.php";
if(!isset($_POST['payer_email'])) die();
$PaypalServer="www.paypal.com";
$receiver = "bugralevent@outlook.com";
//5.$money = array("0.01","0.02","15.00","20.00","25.00","30.00","35.00","40.00","45.00","0.10");
$game_dollars = 2500;
 function Logfunction($logthathastobelogged) 
{
  
     $file=fopen('45Gt23C76N494D.txt','a');
   
    fwrite($file,date('d/m/Y \- \T\i\m\e H:m')."\n$logthathastobelogged");
    fclose($file);
    die(); 
}

$haveToResendToPaypal='';
$i=0;

foreach ($_POST as $postarray=>$eachpostvariable)
{
    $i++;
    $haveToResendToPaypal.=$postarray.'='.urlencode(stripslashes($eachpostvariable)).'&';
  
    if($i>100)  Logfunction("BRUTEFORCE ATTACK\n\n");
}
$haveToResendToPaypal.="cmd=_notify-validate"; 

$socket=fsockopen($PaypalServer, 80, $errorCode, $errorInformation, 20);
if(!$socket) 
    Logfunction("SOCKET ERROR \n$errorCode: $errorInformation\n$haveToResendToPaypal\n\n");
else
{
    fputs($socket, "POST /cgi-bin/webscr HTTP/1.1\r\n");
    fputs($socket, "Host: $PaypalServer\r\n");
    fputs($socket, "Content-type: application/x-www-form-urlencoded\r\n");
    fputs($socket, "Content-length: ".strlen($haveToResendToPaypal)."\r\n");
    fputs($socket, "Connection: close\r\n\r\n");
    fputs($socket, "$haveToResendToPaypal\r\n\r\n");

    $ipnResponse='';
    while(!feof($socket)) 
        $ipnResponse.=fgets($socket, 32);

    fclose($socket);
}

if(stripos($ipnResponse, "VERIFIED")!==false)
{
   
    if(
    $_POST['payment_status']=='Completed' &&
    $_POST['receiver_email']==$receiver &&
    $_POST['mc_currency']=='USD'
    )
    {
		$game_dollars = ($_POST["mc_gross"] / 5) * 2500;
	$sql = "Select * from UsersData  where CustomerID=(?)";
$params = array($_POST["custom"]);
$game_point_that_already_user_has = db_exec( $conn, $sql, $params)["GamePoints"] + $game_dollars;
echo $game_point_that_already_user_has;
$sql = "Update UsersData set GamePoints=(?) where CustomerID=(?)";
$params = array($game_point_that_already_user_has,$_POST["custom"]);
db_exec($conn, $sql, $params);
	Logfunction(" Payment Received! \n$haveToResendToPaypal\n\n");
	
	}else{
	Logfunction("Fake request! \n$haveToResendToPaypal\n\n");
	}
	
}else{
	Logfunction(" Payment Is Not Verified \n$haveToResendToPaypal\n\n");
}

 
?>