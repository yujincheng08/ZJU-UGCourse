<?php
class returnBook extends controller
{
	public function __construct()
	{
		parent::__construct(__CLASS__);
		$this->setTitle('Return Books');
	}

	public function ajax()
	{
		if(@$_POST['cid'] && @$_POST['bid'] && $_POST['borrow'])
		{
			$status = $this->lib->returnBook($_POST['bid'],$_POST['cid'],$_POST['borrow']);
			$result = $this->lib->listBorrow($_POST['cid']);
			$result['success'] = $result['success'] && $status['success'];
			$result['status'] = $status['success'];
			if(!$status['success'])
				$result['error'] = $status['error'];
			echo json_encode($result);
		}
		else if(@$_POST['cid'])
		{
			$result = $this->lib->listBorrow($_POST['cid']);
			echo json_encode($result);
		}
		else
			echo json_encode(array('success' => false));
	}

}
?>
