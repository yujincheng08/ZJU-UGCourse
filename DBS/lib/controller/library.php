<?php
	class Library
	{
		private $host=PGHOST;
		private $port=PGPORT;
		private $dbname=PGDB;
		private $user=PGUSER;
		private $passwd=PGPASSWD;
		
		private $bookPrepare='INSERT INTO book VAlUES('.
				'$1,$2,$3,$4,$5,$6,$7,$8)';
		private $borrowPrepare='INSERT INTO record VALUES('.
				'$1,$2,now(),null,$3)';
		private $returnPrepare='UPDATE record SET return=now() '.
				'WHERE bid=$1 AND cid=$2 AND borrow=$3';
		private $newCardPrepare='INSERT INTO card'.
				'(name, unit, class) VALUES($1,$2,$3)';
		private $deleteCardPrepare='DELETE FROM card WHERE '.
				'id=$1';
		private $listCardPrepare='SELECT * FROM card order by id';
		private $queryBookPrepare='SELECT * FROM book ';
		private $listBorrowPrepare='SELECT bid,cid,borrow,brokerage,category,title,publish,year,author,price,stock FROM '.
				'record JOIN book ON record.bid = book.id '.
				'WHERE cid=$1 AND return IS NULL '.
				'ORDER BY title';
		private $loginPrepare='SELECT name FROM admin '.
				'WHERE id=$1 AND passwd=$2';
		private $pattern='/\(\s*(?<id>.+)\s*,\s*(?<category>.+)\s*,\s*(?<title>.+)\s*,\s*(?<publish>.+)\s*,\s*(?<year>\d+)\s*,\s*(?<author>.+)\s*,\s*(?<price>\d+(\.\d{1,2})?)\s*,\s*(?<stock>\d+)\s*\),?/';

		public $con;

		public function __construct()
		{
			$this->connect();
		}

		public function __destruct()
		{
			$this->close();
		}

		public function connect()
		{
			$this->con = @pg_connect("host=$this->host port=$this->port dbname=$this->dbname user=$this->user password=$this->passwd");
			if(!$this->con)
				die('Bad connection: '.pg_last_error());
		}

		public function close()
		{
			@pg_close($this->con);
		}

		public function borrowBook($bid,$cid)
		{
			$query =  @pg_query_params($this->con,$this->borrowPrepare,array($bid,$cid,$this->nowID()));
			if(!$query)
			{
				$result['success'] = false;
				$result['error'] = pg_last_error();
			}
			else
				$result['success'] = true;
			return $result;
		}

		public function returnBook($bid,$cid,$borrow)
		{
			$query =  @pg_query_params($this->con,$this->returnPrepare,
					array($bid,$cid,$borrow));
			if(!$query)
			{
				$result['success'] = false;
				$result['error'] = pg_last_error();
			}
			else
				$result['success'] = true;
			return $result;
		}


		public function storeBook($id,$category,$title,$publish,$year,$author,$price,$stock)
		{
			$result =  @pg_query_params($this->con,$this->bookPrepare,array($id,$category,$title,$publish,$year,$author,$price,$stock));
			if($result)
				return array(true);
			else
				return array(false,pg_last_error());
		}
		
		public function storeBooks($file)
		{
			if($file['error']>0 || !$file['tmp_name'])
				return array(false,$file['error']);
			$csv = fopen($file['tmp_name'],'r');
			if(!$csv)
				return array(false,'File not exists');
			pg_query($this->con,'BEGIN');
			while(!feof($csv))
			{
				$row = fgets($csv);
				if(!preg_match_all($this->pattern,$row,$match))
				{
					pg_query($this->con,'ROLLBACK');
					return array(false,"Error format: ".$row);
				}
				$result = $this->storeBook(
						$match['id'][0],$match['category'][0],
						$match['title'][0],$match['publish'][0],
						(int)$match['year'][0],$match['author'][0],
						(double)$match['price'][0],
						(int)$match['stock'][0]);
				if(!$result[0])
				{
					pg_query($this->con,'ROLLBACK');
					return array(false,$result[1]."\nOn Row: $row");
				}
			}
			pg_query($this->con,'COMMIT');
			return array(true);
		}

		public function newCard($name,$unit,$class)
		{
			$result =  @pg_query_params($this->con,$this->newCardPrepare,
					array($name,$unit,$class));
			if($result)
				return array('success' => true);
			else
				return array('success' => false,
						'error' => pg_last_error());
		}

		public function deleteCard($id)
		{
			$result =  @pg_query_params($this->con,$this->deleteCardPrepare,
					array((int)$id));
		if($result)
				return array('success' => true);
			else
				return array('success' => false,
						'error' => pg_last_error());
		}


		public function listCard()
		{
			$result = @pg_query($this->con,$this->listCardPrepare);
			if(!$result)
			{
				return array('success'=>false, 'error' => pg_last_error());
			}
			else
			{
				$field = array();
				$num = pg_num_fields($result);
				for($i=0;$i<$num;++$i)
					$field[]=pg_field_name($result,$i);
				return array('success'=>true,'field'=> $field,
						'data' => pg_fetch_all($result));
			}
		}

		private function addAttr($str,$count,$attr)
		{
			$str = $this->addAnd($str,$count);
			return $str." $attr=$$count";
		}

		private function addAnd($str,$count)
		{
			if($count > 1)
				return $str = "$str AND ";
			else
				return $str = "$str WHERE";
		}

		public function queryBook($id,$category,$title,$publish,$startYear,$endYear,$author,$startPrice,$endPrice)
		{
			$queryAttr = array();
			$count = 0;
			@$attrs=array($id,$category,$title,$publish,$author);
			$attrsName=array('id','category','title','publish','author');
			$queryStr = $this->queryBookPrepare;
			foreach($attrs as $key => $attr)
			{
				if($attr)
				{
					$queryStr = $this->addAttr($queryStr,++$count,$attrsName[$key]);
					$queryAttr[$count]=$attr;
				}
			}
			if($startYear)
			{
				$queryStr = $this->addAnd($queryStr,++$count);
				$queryStr = "$queryStr year>=$$count";
				$queryAttr[$count]=(int)$startYear;
			}
			if($endYear)
			{
				$queryStr = $this->addAnd($queryStr,++$count);
				$queryStr = "$queryStr year<=$$count";
				$queryAttr[$count]=(int)$endYear;
			}
			if($startPrice)
			{
				$queryStr = $this->addAnd($queryStr,++$count);
				$queryStr = "$queryStr price>=$$count";
				$queryAttr[$count]=(double)$startPrice;
			}
			if($endPrice)
			{
				$queryStr = $this->addAnd($queryStr,++$count);
				$queryStr = "$queryStr price<=$$count";
				$queryAttr[$count]=(double)$endPrice;
			}
			$queryStr = "$queryStr ORDER BY title";
			$result = pg_query_params($this->con,$queryStr,$queryAttr);
			if(!$result)
				return array('success' => false, 'error' =>pg_last_error());
			else
			{
				$field = array();
				$num = pg_num_fields($result);
				for($i=0;$i<$num;++$i)
					$field[]=pg_field_name($result,$i);
				return array('success' => true, 'field'=> $field,
						'data' => pg_fetch_all($result));
			}
		}

		public function listBorrow($id)
		{
			$result = @pg_query_params($this->con,$this->listBorrowPrepare,array((int)$id));
			if(!$result)
				return array('success' => false,
						'error' => pg_last_error());
			else
			{
				$field = array();
				$num = pg_num_fields($result);
				for($i=0;$i<$num;++$i)
					$field[]=pg_field_name($result,$i);
				return array('success' => true, 'field'=> $field,
						'data' => pg_fetch_all($result));
			}
		}

		public function login($id,$passwd)
		{
			$result = @pg_query_params($this->con,$this->loginPrepare,array($id,md5($passwd)));
			$name = pg_fetch_row($result);
			if($name)
			{
				$this->setSession($id,$name[0]);
				return true;
			}
			else
				return false;
		}

		public function logout()
		{
			if(isset($_SESSION['id']))
			{
				unset($_SESSION['id']);
				unset($_SESSION['name']);
				return true;
			}
			else
				return false;
		}

		public function nowUser()
		{
			if(isset($_SESSION['id']))
				return $_SESSION['name'];
			else
				return false;
		}

		private function nowID()
		{
			if(isset($_SESSION['id']))
				return $_SESSION['id'];
			else
				return false;
		}

		private function setSession($id,$name)
		{
			$_SESSION['name']=$name;
			$_SESSION['id']=$id;
		}

	};

?>
