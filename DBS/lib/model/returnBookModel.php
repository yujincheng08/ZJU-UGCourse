<?php
class returnBookModel extends model
{
	public function __construct()
	{
		$this->set('active','Return');
		$this->set('tip','Return books of the provided card ID.');
	}

	public function content()
	{
		require_once(VIEWPATH.'returnBook.php');
	}
}

?>
