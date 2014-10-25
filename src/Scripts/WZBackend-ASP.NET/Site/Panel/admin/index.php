<?php
ob_start();
session_start();
include_once "../../php/dbinfo.inc.php";
include_once "totalpayment.php"
?>
<html>
<head>
	<title>DoomMMO Admin Panels</title>
	<link rel="stylesheet" href="//code.jquery.com/ui/1.11.0/themes/smoothness/jquery-ui.css">
    <script src="//code.jquery.com/jquery-1.10.2.js"></script>
    <script src="//code.jquery.com/ui/1.11.0/jquery-ui.js"></script>
	<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/angularjs/1.2.20/angular.min.js"></script>
	<script>
	function chageGc(CustomerID, Gamertag){
		var amount = prompt("Enter the amount of GC that you want to add to : " + Gamertag, "0");
		if (amount != null) {
		    window.location = "http://192.99.20.149/panel/index.php?customerID=" + CustomerID + "&amount=" + amount;
		}
	}
	function goTo(value, Where, state, message){
		
		if(state == 0){

			 if (confirm(message)) {
			 	window.location = "http://192.99.20.149/panel/" + Where;
			 }
		}else{
			$( "#dialog" ).html('<form action="index.php" method="get"><input type="text" placeholder="Reason" name="reason"><input type="hidden" name="admin_banuser" value="'+ Where +'"><input type="submit" value="Ban user"></form> ');
			$( "#dialog" ).dialog({ autoOpen: false });
			$( "#dialog" ).dialog( "open" );
		}
	}
	function tempban(aasdas){
		$("#dialog").html('<form action="index.php" method="get"><input type="text" placeholder="Reason" name="reason"></br><input type="text" placeholder="Time in hours" name="time"><input type="hidden" name="admin_tempban" value="'+ aasdas +'"><input type="submit" value="Ban user"></form> ');
		$("#dialog").dialog({autoOpen: false});
		$("#dialog").dialog("open");
	}
	function search(){
		$("#dialogs").html('<form action="index.php" method="get"><select name="searchfor"><option value="gamertag">Gamertag</option><option value="email">Email</option><option value="ip">Ip Address</option></select><input type="text" name="val" placeholder="Type in what to search"><input type="submit" value="Search"></form>');
		$("#dialogs").dialog({autoOpen: false});
		$("#dialogs").dialog("open");
	}
	</script>
	<style>
ul li{
  display: inline;
}

	</style>
</head>
<body ng-app ng-init="tab = 'characters'">
<?php
/*
ng-app
ng-init
ng-class
ng-click
ng-switch
ng-switch-when


*/
	if(isset($_POST["Submit"])){			
			$email = (isset($_POST["email"])) ? htmlspecialchars($_POST["email"]) : "";
			$pass = (isset($_POST["password"])) ? htmlspecialchars($_POST["password"]) : "";
			$badchars = array(";", "'", "\"", "*", "DROP", "SELECT", "UPDATE", "DELETE", "-"); 
   
		    foreach($_POST as $value) 
		    { 
		      if(in_array($value, $badchars)) 
		      { 
		        die("SQL Injection Detected\n<br />\nIP: ".$_SERVER['REMOTE_ADDR']); 
		      }
		      else 
		      { 
		        $check = preg_split("//", $value, -1, PREG_SPLIT_OFFSET_CAPTURE); 
		        foreach($check as $char) 
		        { 
		          if(in_array($char, $badchars)) 
		          { 
		            die("SQL Injection Detected\n<br />\nIP: ".$_SERVER['REMOTE_ADDR']); 
		         }
				 }
				 }
				 }
			if(strlen($email) == 0) die("You Must Type In Your Email!");
			if(strlen($pass) == 0) die("You Must Type In Your Password!");
			$salt = "g5sc4gs1fz0h";
			$sql = "Select * from Accounts where email=(?) and MD5Password=(?)";// 
			$isDev = 1;
			$params = array($email, md5($salt.$pass));	
			$stmt = sqlsrv_query($conn, $sql, $params);
			if( $stmt === false ) {
		     	die(print_r( sqlsrv_errors(), true));
			}else{
				if(sqlsrv_has_rows($stmt)){
					$ret = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
					if($isDev == 1) 
					{
						if($ret["IsDeveloper"] != "1")	$err = "Error: you must be developer!";
					}
				}else{
					$err =  "Error: no user found!";
				}
			}
			if(isset($err)) die("asd".$err);
			$_SESSION["user"] = $ret;
	}
	if(isset($_SESSION["user"])){
		if(!isset($_GET["hwid_ban"]) && !isset($_GET["charlogs"]) && !isset($_GET["ip_ban"]) && !isset($_GET["un_ip_ban"]) && !isset($_GET["un_hwid_ban"])&& !isset($_GET["customerID"])&& !isset($_GET["amount"]) && !isset($_GET["admin_banuser"]) && !isset($_GET["un_admin_banuser"]) && !isset($_GET["admin_tempban"])){
					$charactersPerPage = 250;
					$que = "";
					if(isset($_GET["page"])){
						$asddd = "";
						$gamertsag = isset($_GET["searchfor"]) ? htmlspecialchars($_GET["searchfor"]): "";	
								
						switch ($gamertsag) {
							case 'gamertag':
								$asddd = "Gamertag";
								
								break;
							case 'email':
								$asddd = "email";
								break;
							case 'ip':
								$asddd = "lastloginIP";
								break;
							default:
								$asddd = "Gamertag";
								break;
						}
						$page = (int)$_GET["page"];
						$page = $page * $charactersPerPage;
						$que = "Order by ".$asddd."
								OFFSET ".$page." ROWS 
							     FETCH NEXT ".$charactersPerPage." ROWS ONLY ";
					}else{
						$asddd = "";
						$gamertsag = isset($_GET["searchfor"]) ? htmlspecialchars($_GET["searchfor"]): "";	
								
						switch ($gamertsag) {
							case 'gamertag':
								$asddd = "Gamertag";
								
								break;
							case 'email':
								$asddd = "email";
								break;
							case 'ip':
								$asddd = "lastloginIP";
								break;
							default:
								$asddd = "Gamertag";
								break;
						}
						$que = "Order by ".$asddd."
								OFFSET 0  ROWS 
							     FETCH NEXT ".$charactersPerPage." ROWS ONLY ";

					}
					$sql = "Select Count(*) as count from UsersChars";
					
					
					$gamertag = isset($_GET["searchfor"]) ? htmlspecialchars($_GET["searchfor"]): "";
					$vals = isset($_GET["val"]) ? htmlspecialchars($_GET["val"]): "";
					$boolacc = false;
					switch ($gamertag) {
						case 'gamertag':

							$sql = strlen($vals) > 0 ? $sql." where Gamertag LIKE '%".$vals."%'" : $sql;
							break;
						case 'email':
							$boolacc = true;
							$sql = "Select Count(*) from Accounts";
							$sql = strlen($vals) > 0 ? $sql." where email LIKE '%".$vals."%'" : $sql;
							break;
						case 'ip':
							$boolacc = true;
							$sql = "Select Count(*) from Accounts";
							$sql = strlen($vals) > 0 ? $sql." where lastloginIP LIKE '%".$vals."%'" : $sql;
							break;
						default:
							# code...
							break;
					}
					
					$p = isset($_GET["page"]) ? (int)htmlspecialchars($_GET["page"]) : 1;
					$params = array();	
					
					$stmt = sqlsrv_query($conn, $sql, $params);
					$count = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC);
					$count = $count["count"];
					$countt = ceil($count / $charactersPerPage);
					
					
		?>

			List of the users </br> Total money raised:  <?php echo getPayments(); ?></br>Current Money we have :  <?php echo getPayments(true); ?> </br> Players online: <?php echo getServerInfo()["playersonline"]; ?> </br> Servers online : <?php echo getServerInfo()["serversonline"]; ?> </br>
			Total Characters : <?php echo $count; ?> 

			<div id="dialog" title="Ban account"><form method="get" action="index.php">

			</form></div>
			<div id="dialogs" title="Search Account"><form method="get" action="index.php">

			</form></div>
			<div id="tab">
				<ul class="tab">
					<li ng-class="{active: tab == 'characters'}">
						<a href="#" ng-click="tab = 'characters'"><img src="images/characters.png"></a>
					</li>
					<li ng-class="{active: tab == 'hwid banned players'}">
						<a href="#" ng-click="tab = 'hwid banned players'"><img src="images/bannedplayers.png"></a>
					</li>
					
					
				</ul>
				<div ng-switch="tab">
					<div class="tab-content" ng-switch-when="characters">
						<div>
						<center><button onclick="search();">Search</button></center>
			<center>
				<table border="1">
					<tr><td>Customer Id</td><td>Character Name</td><td>IP Address</td><td>Email Address</td><td>Experience</td><td>Character Count</td><td>Char Log</td><td>GC</td><td>BAN / UnBan</td><td>Change GC</td><td>Temp ban</td></tr>
				<?php
					$sql = "Select * from UsersChars";
					
					
					$gamertag = isset($_GET["searchfor"]) ? htmlspecialchars($_GET["searchfor"]): "";
					$vals = isset($_GET["val"]) ? htmlspecialchars($_GET["val"]): "";
					$boolacc = false;
					switch ($gamertag) {
						case 'gamertag':

							$sql = strlen($vals) > 0 ? $sql." where Gamertag LIKE '%".$vals."%'" : $sql;
							break;
						case 'email':
							$boolacc = true;
							$sql = "Select * from Accounts INNER JOIN UsersChars
													ON Accounts.CustomerID=UsersChars.CustomerID";
							$sql = strlen($vals) > 0 ? $sql." where email LIKE '%".$vals."%'" : $sql;
							break;
						case 'ip':
							$boolacc = true;
							$sql = "Select * from Accounts INNER JOIN UsersChars
													ON Accounts.CustomerID=UsersChars.CustomerID";
							$sql = strlen($vals) > 0 ? $sql." where lastloginIP LIKE '%".$vals."%'" : $sql;
							break;
						default:
							# code...
							break;
					}
					$sql .= " ".$que;
					$params = array();	
					$stmt = sqlsrv_query($conn, $sql, $params);
					if( $stmt === false ) {
						die(print_r( sqlsrv_errors(), true));
					}else{
						while($each_Row = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC)){
							
								$sql1 = "Select * from UsersData where CustomerID='".$each_Row["CustomerID"]."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn, $sql1, $params1);
								$userDatas = sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC);
								$sql2 = "Select * from Accounts where CustomerID='".$each_Row["CustomerID"]."'";
								$params2 = array();
								$stmt2 = sqlsrv_query($conn, $sql2, $params2);
								$accountData = sqlsrv_fetch_array($stmt2, SQLSRV_FETCH_ASSOC);
								$sql3 = "Select * from UsersChars where CustomerID='".$each_Row["CustomerID"]."'";
								$params3 = array();
								$stmt3 = sqlsrv_query($conn, $sql3, $params3);
								$accountCharCount = 0;
								while($ffff = sqlsrv_fetch_array($stmt3, SQLSRV_FETCH_ASSOC)){
									$accountCharCount++;
								}
								$ip = $accountData["lastloginIP"];
								$sql4 = "Select * from BannedIP where ip='".$ip."'";
								$params4 = array();
								$stmt4 = sqlsrv_query($conn,$sql4,$params4);
								$echo_ip_ban = "index.php?admin_banuser=".$each_Row["CustomerID"];
								$ask_ip_ban = "Are you sure you want to ban".$each_Row["Gamertag"]."?";
								$text_ip_ban = "Ban";
								if(sqlsrv_has_rows($stmt4)){
									$echo_ip_ban = "index.php?un_admin_banuser=".$each_Row["CustomerID"];
									$ask_ip_ban = "Are you sure you want to remove the ban of".$each_Row["Gamertag"]."?";
									$text_ip_ban = "Remove Ban";
								}								
								$sql1 = "Select * from HWID_Log where CustomerID='".$accountData["CustomerID"]."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn,$sql1,$params1);
								$hwidd =  sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC)["HardwareID"];
								$sql1 = "Select * from HWID_Ban where HardwareID='".$hwidd."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn,$sql1,$params1);
								$text_hwid_ban = "Hwid Ban";
								$echo_hwid_ban = "index.php?hwid_ban=".$each_Row["CustomerID"];
								$ask_hwid_ban = "Are you sure you want to HWID Ban ".$each_Row["Gamertag"]."?";
								if(sqlsrv_has_rows($stmt1)){
									$echo_ip_ban = "index.php?un_admin_banuser=".$each_Row["CustomerID"];
									$ask_ip_ban = "Are you sure you want to remove the ban of ".$each_Row["Gamertag"]."?";
									$text_ip_ban = "Remove Ban";
								}							
									?>
									<tr><td><?php echo $each_Row["CustomerID"]; ?></td><td><?php echo $each_Row["Gamertag"]; ?></td><td><?php echo $accountData["lastloginIP"];?></td><td><?php echo $accountData["email"];?></td><td><?php echo $each_Row["XP"]; ?></td><td><?php echo $accountCharCount; ?></td><td><a href="index.php?charlogs=<?php echo  $each_Row["Gamertag"]; ?>">Char logs</a></td><td><?php echo $userDatas["GamePoints"]; ?></td><td><button onclick="goTo('reason','<?php if($text_ip_ban == "Remove Ban"){ echo $echo_ip_ban;}else{ echo $each_Row["CustomerID"]; } ?>', <?php if($text_ip_ban == "Remove Ban"){ echo "0";}else{ echo "1"; } ?>, '<?php echo $ask_ip_ban;?>');"><?php echo $text_ip_ban;?></button></td><td><a href="#" onclick="chageGc(<?php echo $each_Row["CustomerID"]; ?>,'<?php echo $each_Row["Gamertag"]; ?>');">Change GC</a></td>

											<?php if($text_ip_ban != "Remove Ban"){ 
													?>
													<td><button onclick="tempban('<?php echo $each_Row["CustomerID"]; ?>');">Temp ban</button></td>
											<?php 
										    }
									?>
									</tr>
								<?php
							
						}
					}
				?>
				</table>
			</center>
					</div>
					<center>
				<?php
					for($df = 1; $df <= $countt; $df ++){
						if($p == $df){
							echo "&nbsp;".$p."&nbsp;";
						}else{
							$extension = "";
							foreach($_GET as $name => $value){
								if($name != "page"){
									$extension.="&".$name."=".$value;
								}
							}
							echo "&nbsp;<a href=index.php?page=".$df.$extension.">".$df."</a>&nbsp;";
						}
					}

				?>
			</center>
				</div>
					<div class="tab-content" ng-switch-when="hwid banned players">
						<center>
						
						<table border="1">
							<tr><td>CustomerID</td><td>Email</td><td>Un Ban</td><td>Reason</td><td>Banned by</td><td>Ban time</td><td>Duration</td></tr>
							<?php
								$sql = "Select * from DBG_BanLog order by RecordID desc";
								$params = array();
								$stmt = sqlsrv_query($conn,$sql,$params);								
								while($each_row = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC)){
									$CustomerID = $each_row["CustomerID"];
									$sql1 = "Select * from Accounts where CustomerID=(?)";
									$params1 = array($CustomerID);
									$stmt1 = sqlsrv_query($conn, $sql1, $params1);
									$accountInfo = sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC);
									$echo_ip_ban = "index.php?un_admin_banuser=".$accountInfo["CustomerID"];
									$ask_ip_ban = "Are you sure you want to remove the ban of".$accountInfo["email"]."?";
									$text_ip_ban = "Remove Ban";
									?>
										<tr><td><?php echo (strlen($accountInfo["CustomerID"]) > 0 ? $accountInfo["CustomerID"] : "Deleted Account"); ?></td><td><?php echo (strlen($accountInfo["email"]) > 0 ? $accountInfo["email"] : "Deleted email"); ?></td><td><a href="<?php echo $echo_ip_ban;?>" onclick="return confirm('<?php echo $ask_ip_ban;?>');"><?php echo $text_ip_ban;?></a></td><td><?php echo $each_row["BanReason"]; ?></td><td><?php echo $each_row["Banned_by"]; ?></td><td><?php echo $each_row["BanTime"]->format('Y/m/d H:i:s'); ?></td><td><?php if($each_row["BanDuration"] == "0"){ echo "Permanent";}else{ echo $each_row["BanDuration"]." Hours";} ?></td></tr>
									<?php

								}
								
								
							?>

						</table>
					</center>
					</div>
					
					<div class="tab-content" ng-switch-when="Accounts">
						Accounts
					</div>
					
				</div>
			</br>
			
			</div>
			<?php
		}else{
			if(!isset($_GET["ip_ban"]) && !isset($_GET["hwid_ban"]) && isset($_GET["charlogs"])){
			?>
				Charlogs : 
				<table border="1">
					<tr><td>ID</td><td>Char Message</td><td>Time</td></tr>
					<?php
						$sql7 = "Select * from CharsLog where GamerTag='".htmlspecialchars($_GET["charlogs"])."'";
						$params7 = array();
						$stmt7 = sqlsrv_query($conn, $sql7, $params7);
						while($each_row = sqlsrv_fetch_array($stmt7, SQLSRV_FETCH_ASSOC)){
							?>
								<tr><td><?php echo $each_row["id"]; ?></td><td><?php echo $each_row["CharMessage"]; ?></td><td><?php echo $each_row["time"]; ?></td></tr>
							<?php
						}
					?>
				</table>

			<?php
		}else{
							if(isset($_GET["hwid_ban"])){
								$sql1 = "Select * from HWID_Log where CustomerID='".htmlspecialchars($_GET["hwid_ban"])."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn, $sql1, $params1);
								$hwid = sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC)["HardwareID"];
								$sql1 = "INSERT INTO HWID_Ban
							           (HardwareID
							           ,Reason)
							     	   VALUES
				                      ((?),(?))";
				           		$params1 = array($hwid,"Banned from panel : ".$_SESSION["user"]["email"]);
				           		$stmt1 = sqlsrv_query($conn,$sql1,$params1);
				           		$sql1 = "Select * from LoginSessions where CustomerID='".htmlspecialchars($_GET["hwid_ban"])."'";
				           		$params1 = array();
				           		$stmt1 = sqlsrv_query($conn, $sql1, $params1);
				           		if(sqlsrv_has_rows($stmt1)){
				           			$sql1 = "Delete from LoginSessions where CustomerID='".htmlspecialchars($_GET["hwid_ban"])."'";
				           			$params1 = array();
				           			$stmt1 = sqlsrv_query($conn, $sql1, $params1);
				           		}

				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully hwid banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
							}
							if(isset($_GET["ip_ban"])){
								$sql1 = "Select * from Accounts where CustomerID='".htmlspecialchars($_GET["ip_ban"])."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn, $sql1, $params1);
								$hwid = sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC)["lastloginIP"];
								$sql1 = "INSERT INTO BannedIP
							           (cid
							           ,ip
							           ,reason
							           ,bannedby,date)
							     VALUES
							           (1000000,(?),(?),1000000,2013-01-01)";
				           		$params1 = array($hwid,"Banned from panel : ".$_SESSION["user"]["email"]);
				           		$stmt1 = sqlsrv_query($conn,$sql1,$params1);
				           		$sql1 = "Select * from LoginSessions where CustomerID='".htmlspecialchars($_GET["ip_ban"])."'";
				           		$params1 = array();
				           		$stmt1 = sqlsrv_query($conn, $sql1, $params1);
				           		if(sqlsrv_has_rows($stmt1)){
				           			$sql1 = "Delete from LoginSessions where CustomerID='".htmlspecialchars($_GET["ip_ban"])."'";
				           			$params1 = array();
				           			$stmt1 = sqlsrv_query($conn, $sql1, $params1);
				           		}

				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully ip banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
								
							}
							if(isset($_GET["account_ban"])){
								$sql1 = "Update Accounts set AccountStatus=202 where CustomerID=(?)";
				           		$params1 = array(htmlspecialchars($_GET["account_ban"]));
				           		$stmt1 = sqlsrv_query($conn,$sql1,$params1);
				           		$sql1 = "Select * from LoginSessions where CustomerID='".htmlspecialchars($_GET["account_ban"])."'";
				           		$params1 = array();
				           		$stmt1 = sqlsrv_query($conn, $sql1, $params1);
				           		if(sqlsrv_has_rows($stmt1)){
				           			$sql1 = "Delete from LoginSessions where CustomerID='".htmlspecialchars($_GET["account_ban"])."'";
				           			$params1 = array();
				           			$stmt1 = sqlsrv_query($conn, $sql1, $params1);
				           		}
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully account is  banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
								
							}
							if(isset($_GET["admin_tempban"]) && isset($_GET["time"]) && isset($_GET["reason"])){
								$sql1 = "{call ADMIN_TempBan( ?, ?, ?, ?)}";
								$reason = htmlspecialchars($_GET["reason"]);
								$amount = htmlspecialchars($_GET["time"]);
								$customerID = htmlspecialchars($_GET["admin_tempban"]);
								$bannedby = $_SESSION["user"]["email"];
				           		$params = array( 
				                 array($customerID, SQLSRV_PARAM_IN),
				                 array($amount, SQLSRV_PARAM_IN),
				                 array($reason, SQLSRV_PARAM_IN),
				                 array($bannedby, SQLSRV_PARAM_IN)
				                );

				           		$stmt1 = sqlsrv_query($conn,$sql1,$params);
				           		
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully account is  banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
							}
							if(isset($_GET["admin_banuser"]) && isset($_GET["reason"])){
								$sql1 = "{call ADMIN_BanUser( ?, ?, ?)}";
								$reason = htmlspecialchars($_GET["reason"]);
								$customerID = htmlspecialchars($_GET["admin_banuser"]);
								$bannedby = $_SESSION["user"]["email"];
				           		$params = array( 
				                 array($customerID, SQLSRV_PARAM_IN),
				                 array($reason, SQLSRV_PARAM_IN),
				                 array($bannedby, SQLSRV_PARAM_IN)
				               );

				           		$stmt1 = sqlsrv_query($conn,$sql1,$params);
				           		
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully account is  banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
							}
							if(isset($_GET["un_admin_banuser"])){
								$sql1 = "{call ADMIN_UNBanUser( ?, ?)}";
								$customerID = htmlspecialchars($_GET["un_admin_banuser"]);
								$bannedby = $_SESSION["user"]["email"];
				           		$params = array( 
				                 array($customerID, SQLSRV_PARAM_IN),
				                  array($bannedby, SQLSRV_PARAM_IN)
				               );

				           		$stmt1 = sqlsrv_query($conn,$sql1,$params);
				           		
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully account is  unbanned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
							}
							if(isset($_GET["un_ip_ban"])){
								$sql1 = "Select * from Accounts where CustomerID='".htmlspecialchars($_GET["un_ip_ban"])."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn, $sql1, $params1);
								$hwid = sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC)["lastloginIP"];
								$sql1 = "Delete from BannedIP where ip=(?)";
				           		$params1 = array($hwid);
				           		$stmt1 = sqlsrv_query($conn,$sql1,$params1);
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo  'successfully un banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
								
							}
							if(isset($_GET["un_hwid_ban"])){
								$sql1 = "Select * from HWID_Log where CustomerID='".htmlspecialchars($_GET["un_hwid_ban"])."'";
								$params1 = array();
								$stmt1 = sqlsrv_query($conn, $sql1, $params1);
								$hwid = sqlsrv_fetch_array($stmt1, SQLSRV_FETCH_ASSOC)["HardwareID"];
								$sql1 = "Delete from HWID_Ban where HardwareID=(?)";
				           		$params1 = array($hwid);
				           		$stmt1 = sqlsrv_query($conn,$sql1,$params1);
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'successfully un banned! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
							}
							if(isset($_GET["amount"]) && isset($_GET["customerID"])){
								$sql1 = "{call ADMIN_GiveGC( ?, ?, ?)}";
								$reason = "";
								$amount = htmlspecialchars($_GET["amount"]);
								$customerID = htmlspecialchars($_GET["customerID"]);
				           		$params = array( 
				                 array($customerID, SQLSRV_PARAM_IN),
				                 array($amount, SQLSRV_PARAM_IN),
				                 array($reason, SQLSRV_PARAM_IN)
				                );

				           		$stmt1 = sqlsrv_query($conn,$sql1,$params);
				           		
				           		if($stmt1 === false){
				           			die(print_r( sqlsrv_errors(), true));
				           		}else{
				           			echo 'Gc changed! Directing to main page in 3 sec... <meta http-equiv="refresh" content="3;URL=index.php">';
				           		}
								
							}


		}
		}
	}else{
		
		?>
			<form action="index.php" method="post">
				<table>
					<tr><td><input type="text" name="email" placeholder="In Game Email"></td></tr>
					<tr><td><input type="password" name="password" placeholder="In Game Password"></td></tr>
					<tr><td><input type="Submit" name="Submit" value="Login"></td></tr>
				</table>
			</form>


		<?php
	}



?>
</body>
</html>