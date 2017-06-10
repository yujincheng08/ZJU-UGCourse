<?php
class indexModel extends model
{
	public function __construct()
	{
		$this->set('active','Home');
	}
	public function content()
	{
		require_once(VIEWPATH.'index.php');
	}
}
