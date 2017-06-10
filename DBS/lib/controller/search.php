<?php
class search extends controller
{
	public function __construct()
	{
		parent::__construct(__CLASS__);
		$this->setTitle('Search Books');
	}

	public function ajax()
	{
		$result = $this->lib->queryBook
				(
				 $_POST['id'],
				 $_POST['category'],
				 $_POST['title'],
				 $_POST['publish'],
				 $_POST['startYear'],
				 $_POST['endYear'],
				 $_POST['author'],
				 $_POST['startPrice'],
				 $_POST['endPrice']
				 );
		echo json_encode($result);
	}
}
?>
