<?php
class model
{
	protected $data = array
		(
		'header' => array
			(
			'Home' => './',
			'Borrow' => 'borrowBook',
			'Return' => 'returnBook',
			'Card Manage' => 'card',
			'Search' => 'search',
			'Import' => 'import'
			),
		'tip' => 'Welcome to YU\'s library!'
		);

	public function set($key, $value)
	{
		$this->data[$key] = $value;
	}

	public function display()
	{
		$data = $this->data;
		if($data['user']||ACTION=='search'||ACTION=='index')
		{
			require(VIEWPATH . 'header.php');
			$this->content($data);
		}
		else
		{
			$data['tip']='You have to log in first.';
			require(VIEWPATH . 'header.php');
			require_once(VIEWPATH . 'login.php');
		}
		require(VIEWPATH . 'tail.php');
	}

}
?>
