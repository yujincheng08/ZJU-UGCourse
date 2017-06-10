<?php
class loginModel extends model
{
	public function __construct()
	{
		$this->set('tip','You have to login in first');
		$this->set('active','Home');
	}
	protected function content($data)
	{
		require_once(VIEWPATH.'login.php');
	}
}
