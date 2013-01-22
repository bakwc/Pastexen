<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013  powder96 <https://github.com/powder96>
	 *
	 * This program is free software: you can redistribute it and/or modify
	 * it under the terms of the GNU General Public License as published by
	 * the Free Software Foundation, either version 3 of the License, or
	 * (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
	 */
	
	class Source {
		private $codeTypes = array(
			// file extension => code type
			'php'   => 'php',
			'cpp'   => 'cpp',
			'c'     => 'cpp',
			'h'     => 'cpp',
			'py'    => 'python',
			'java'  => 'java',
			'm'     => 'objectivec',
			'l'     => 'objectivec',
			'pl'    => 'perl',
			'xml'   => 'xml',
			'html'  => 'xml',
			'js'    => 'javascript',
			'css'   => 'css',
			'json'  => 'json',
			'as'    => 'actionscript',
			'vb'    => 'vbscript',
			'http'  => 'http',
			'cs'    => 'cs',
			'd'     => 'd',
			'sql'   => 'sql',
			'st'    => 'smalltalk',
			'lisp'  => 'lisp',
			'cl'    => 'lisp',
			'ini'   => 'ini',
			'conf'  => 'apache',
			'sh'    => 'bash',
			'bat'   => 'dos',
			'cmake' => 'cmake',
			'hs'    => 'haskell',
			'lhs'   => 'haskell',
		);
		
		public $name; // name of the file
		public $path; // path to the file

		/**
		 * Creates the Source object.
		 * @param string $file File's name.
		 * @param string $storage Path to the folder where all files are stored.
		 */
		public function __construct($file, $storage) {
			$this->name = basename($file);
			$this->path = $storage . '/' . $this->name;
		
			if(!is_file($this->path))
				throw new Exception('File does not exist!');
		}
		
		/**
		 * @return string Type of the source code.
		 */
		public function getType() {
			$extension = strtolower(pathinfo($this->name, PATHINFO_EXTENSION));
			if(empty($extension) || !in_array($extension, $this->codeTypes))
				return 'undefined'; // unknown type
			return $this->codeTypes[$extension];
		}
		
		/**
		 * @return string Contents of the file.
		 */
		public function getData() {
			return file_get_contents($this->path);
		}
	}