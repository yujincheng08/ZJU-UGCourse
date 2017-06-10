<?php
class searchModel extends model
{
	public function __construct()
	{
		$this->set('active','Search');
		$this->set('tip','Search books in stock. Leave empty to ignore some attributes');
	}
	public function content()
	{
		require_once(VIEWPATH.'search.php');
	}
}

