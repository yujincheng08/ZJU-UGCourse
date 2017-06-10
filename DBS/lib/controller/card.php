<?php
class card extends controller
{
	public function __construct()
	{
		parent::__construct(__CLASS__);
		$this->setTitle('Card Management');
	}

	public function ajax()
	{
		if(isset($_POST['id']))
			$status = $this->lib->deleteCard($_POST['id']);
		else if(isset($_POST['name']) && isset($_POST['unit']) && isset($_POST['class']))
			$status = $this->lib->newCard($_POST['name'],$_POST['unit'],$_POST['class']);
		$result = $this->lib->listCard();
		$result['success'] = $result['success'] && $status['success'];
		$result['status'] = $status['success'];
		if(!$status['success'])
			$result['error'] = $status['error'];
		echo json_encode($result);
	}
	
	public function show()
	{
		$cards = $this->lib->listCard();
		if(isset($cards['error']))
			$this->model->set('error',$cards['error']);
		else
		{
			$this->model->set('card',$cards['data']);
			$this->model->set('cardHead',$cards['field']);
		}

		parent::show();
	}
}
?>
