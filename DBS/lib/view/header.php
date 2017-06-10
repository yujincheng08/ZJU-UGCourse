<html>
	<head>
		<title><?php echo $data['title'];?></title>
		<script src='script/jquery.min.js'></script>
		<script src='script/library.js'></script>
		<link rel="stylesheet" type="text/css" href="style/library.css">
		<link rel="stylesheet" type="text/css" href="style/font-awesome.min.css">
	</head>
	<body>
	<div id='header'>
	<nav id='nav' class='clearfix center'>
	<div class='wp'>
	<ul id='menu'>
	<?php foreach($data['header'] as $header => $href) :?>
		<li class='<?=$data['active']==$header?'active':'noactive'?>'>
			<a href='<?=$href;?>'><?=$header;?></a>
		</li>
	<?php endforeach;?>
	</ul>
	<ul id='user'>
	<?php if($data['user']) :?>
		<li id='username'>Welcome: <?=$data['user'];?> </li>
		<li id='logout'>
		<a href='logout'><i class='logoutIcon fa fa-power-off'></i>Log out</a></li>
	<?php else :?>
		<li id='anonymous'>Welcome! Please </li>
		<li id='login'><a href='login'>Log in</a></li>
	<?php endif;?>
	</ul>
	</div>
	</nav>
	</div>
	<div id='main' class='center'>
	<div id='tip' class='tip center wp'>
		<span><?=$data['tip']?></span>
	</div>

