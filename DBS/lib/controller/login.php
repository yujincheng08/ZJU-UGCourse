<?php
class login extends controller
{
	public function __construct()
	{
		parent::__construct(__CLASS__);
		$this->setTitle('login');
	}

	public function ajax()
	{
		$success = false;
		if(isset($_POST['user']) && isset($_POST['password']))
			$success = $this->lib->login($_POST['user'],$_POST['password']);
		$result = array('success' => $success);
		if($success)
			$result['username'] = $this->lib->nowUser();
		else
			$result['error'] = 
				'Log in failed. Please check your user id and password again.';
		echo json_encode($result);
	}

	public function show()
	{
		$user = $this->lib->nowUser();
		if($user)
			$this->model->set('tip',
					$user.', you have been logged in.');
		parent::show();
	}
}
?>
