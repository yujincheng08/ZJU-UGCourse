<?php
class logout extends controller
{
	public function __construct()
	{
		parent::__construct(__CLASS__);
		$this->setTitle('logout');
	}

	public function show()
	{
		$result = $this->lib->logout();
		if($result)
			$this->model->set('tip','Successfully logged out!'.
					' Jumpping to homepage in 5 seconds.');
		else
			$this->model->set('tip','Failed to log out! Have you logged in?');

		parent::show();
	}
}
?>
