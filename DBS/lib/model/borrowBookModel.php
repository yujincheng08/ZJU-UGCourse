<?php
class borrowBookModel extends model
{
	public function __construct()
	{
		$this->set('active','Borrow');
		$this->set('tip','Borrow books with provided book ID for the provided card ID.');
	}

	public function content()
	{
		require_once(VIEWPATH.'borrowBook.php');
	}
}

?>
