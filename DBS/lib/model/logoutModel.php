<?php
class logoutModel extends model
{
	public function __construct()
	{
		$this->set('active','Home');
	}
	protected function content($data)
	{
		require_once(VIEWPATH.'logout.php');
	}
}
