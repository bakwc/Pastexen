<?php

class Sources{
	
	public $charset = 'UTF-8';
	public $source_file;
	public $source_file_with_path;
	public $syntax = '';
	private $dir = './s/';
	public $file_not_found = false;

	public function __construct($file, $syntax=false){
		$file = pathinfo(Sources::encode($file));
		$this->source_file = $file['basename'];
		$this->source_file_with_path = $this->dir.$file['basename'];
		//$this->syntax = $this->encode($syntax);
		$this->syntax = $this->check_syntax();
	}

	/**
   * Метод подтягивания файла по имени
   * 
   * @param string $name имя файла
   * @return string
   */
	public function renderFile(){
		if(!empty($this->source_file_with_path)){
			return $this->getFileByName();
		}else{
			return $this->notfound();
		}
	}

   /**
   * Метод подтягивания файла по имени
   * 
   * @param string $name имя файла
   * @return string
   */
	protected function getFileByName(){
		if(file_exists($this->source_file_with_path)){
			return Sources::encode(file_get_contents($this->source_file_with_path));
		}else{
			return $this->notfound();
		}
	}

	/**
   * Метод для отдачи файла
   */
	public function download(){
		if(!empty($this->source_file_with_path) && file_exists($this->source_file_with_path)){
			header('Content-type: application/force-download');
	  		header('Content-Disposition: attachment; filename="'.$this->source_file.'"');
	  		header('Content-Length: '.filesize($this->source_file_with_path).'');
  		}else{
  			header('location: /');
  		}
	}

	/**
   * Метод для отдачи файла
   */
	public function raw(){
		if(!empty($this->source_file) && file_exists($this->source_file_with_path)){
			echo $this->getFileByName($this->source_file);
		}else{
			header('location: /');
		}
	}

	/**
   * Метод определяет нужный синтакцис по разширению
   * 
   * @param string $file имя файла
   * @return string
   */
	public function check_syntax() 
	{ 	
		$ext = pathinfo($this->source_file,PATHINFO_EXTENSION);

		// array(ext,lang);
		$lang = array(
			'php'=>'php',
			'cpp'=>'cpp',
			'c'=>'cpp',
			'h'=>'cpp',
			'py'=>'python',
			'java'=>'java',
			'm'=>'objectivec',
			'l'=>'objectivec',
			'pl'=>'perl',
			'xml'=>'xml',
			'html'=>'xml',
			'js'=>'javascript',
			'css'=>'css',
			'json'=>'json',
			'as'=>'actionscript',
			'vb'=>'vbscript',
			'http'=>'http',
			'cs'=>'cs',
			'd'=>'d',
			'sql'=>'sql',
			'st'=>'smalltalk',
			'lisp'=>'lisp',
			'cl'=>'lisp',
			'ini'=>'ini',
			'conf'=>'apache',
			'sh'=>'bash',
			'bat'=>'dos',
			'cmake'=>'cmake',
			'hs'=>'haskell',
			'lhs'=>'haskell',
		);
		// доделать

		if(!empty($ext)){
	    	return $lang[$ext]; 
	 	}else{
	 		return $this->syntax;
	 	}
	}

   /**
   * Метод очисти входных данных
   * 
   * @param string $text строка
   * @return string
   */
	public static function encode($text) 
	{ 
	    return htmlspecialchars($text,ENT_QUOTES); 
	}

   /**
   * Метод декодирования всех тегов
   * 
   * @param string $text строка
   * @return string
   */
	public function decode($text) 
	{ 
	    return htmlspecialchars_decode($text,ENT_QUOTES); 
	}

	protected function notfound(){
		$this->file_not_found = true;	
		return 'Файл не выбран или не сущетсвует';
	}
}

