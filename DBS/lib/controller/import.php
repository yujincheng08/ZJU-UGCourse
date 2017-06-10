<?php
class import extends controller
{
	public function __construct()
	{
		parent::__construct(__CLASS__);
		$this->setTitle('import');
	}

	private function handle($key)
	{
		return isset($_POST[$key])?
			$_POST[$key] : NULL;
	}
	
	public function ajax()
	{
		$success = false;
		if(isset($_POST['id'])&&isset($_POST['stock']))
		{
			$result = $this->lib->storeBook
				(
					$_POST['id'],
					$this->handle('category'),
					$this->handle('title'),
					$this->handle('publish'),
					$this->handle('year'),
					$this->handle('author'),
					(double)$this->handle('price'),
					(int)$_POST['stock']
				);
			$success= $result[0];
		}
		else if(isset($_FILES['file']) && $_FILES['file']['error']==0)
		{
			$result = $this->lib->storeBooks($_FILES['file']);
			$success = $result[0];
		}
		$json = array('success' => $success);
		if(isset($result[1]))
			$json['error'] = $result[1];
		else if(!$success)
			$json['error'] = 'error parameter received.';
		echo json_encode($json);
	}
}

?>


