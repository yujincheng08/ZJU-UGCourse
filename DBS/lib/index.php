<?php
require_once('config.php');
$actionPath = CTRLPATH . ACTION . '.php';
require $actionPath;
$controller = new $action;
if($_POST||$_FILES)
	$controller->preajax();
else
	$controller->show();
?>
