<?php
class controller
{
	protected $lib;
	protected $model;
	protected $userName;

	protected function __construct($name)
	{
		require_once('./controller/library.php');
		$this->lib = new Library();
		$name = $name.'Model';
		require_once (MODELPATH . $name . '.php');
		$this->userName = $this->lib->nowUser();
		$this->model = new $name;
		$this->model->set('user', $this->userName);
	}
	protected function setTitle($title)
	{
		$this->model->set('title',SITENAME.' - '. $title);
	}

	public function preajax()
	{
		if(ACTION=='login'||ACTION=='search'||$this->userName)
			$this->ajax();
		else
		{
			$json=array
				(
				 'success' => false,
				 'error' => 'You have to log in again.'
				);
			echo json_encode($json);
		}
	}

	public function show()
	{
		$this->model->display();
	}

};

?>
