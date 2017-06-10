<?php
class importModel extends model
{
	public function __construct()
	{
		$this->set('tip','Select file containing books\' information to be import.');
		$this->set('active','Import');
	}
	protected function content($data)
	{
		require_once(VIEWPATH.'import.php');
	}
}
