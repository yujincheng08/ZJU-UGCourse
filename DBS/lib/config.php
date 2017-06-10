<?php

//Configuration for the system

//PostgreSQL settings
define("PGHOST",'localhost');
define("PGUSER",'db2017');
define("PGPASSWD",'DB2017');
define("PGDB",'postgres');
define("PGPORT",'5432');

define("VIEWPATH", './view/');
define("CTRLPATH", './controller/');
define("MODELPATH", './model/');
define("ACTION", $_REQUEST['action'] ? $_REQUEST['action'] : 'index');

define("SITENAME", 'YU\'s Library');

$action = ACTION;

//Require base class
require_once(CTRLPATH.'controller.php');
require_once(MODELPATH.'model.php');

session_start();

