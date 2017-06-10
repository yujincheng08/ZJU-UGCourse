<?php
class cardModel extends model
{
	public function __construct()
	{
		$this->set('active','Card Manage');
		$this->set('tip','Insert a new card or remove an exsited card.');
	}

	public function content($data)
	{
		require_once(VIEWPATH.'card.php');
	}
}
?>
