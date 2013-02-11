<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013 powder96 <https://github.com/powder96>
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
	
	if(!defined('APPLICATION_ENTRY_POINT')) {
		echo 'Access denied.';
		exit();
	}
	
	require_once(dirname(__FILE__) . '/User.php');
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key.php');
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/Hash.php');
//	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/SortedSet.php');
	
	final class ApplicationModel_File extends ApplicationModel {
		private $id = null;				// file id
		private $type = null;			// file type (image / text)
		private $name = null;			// file name, given by the user
		private $extension = null;		// file extension, given by the user
		private $path = null;			// location of the file in the filesystem
		private $time = null;			// upload time
		private $description = null;	// description, given by the user
		private $uploader = null;		// uuid of the uploader
	
		public function __toString() {
			return 'File(id: ' . $this->id . ', type: "' . $this->type . '", name: "' . $this->name . '", extension: "' . $this->extension . '", path: "' . $this->path . '", time: ' . $this->time . ', description: "' . $this->description .'", uploader: "' . $this->uploader . '")';
		}
	
		public function setId($id) {
			if(!ctype_digit($id) || $id <= 0)
				throw new Exception('Id must be an integer greater than 0.');
			$this->id = $id;
		}
		
		public function getId() {
			if($this->id === null)
				throw new Exception('Id is not defined.');
			return $this->id;
		}
		
		public function setType($type) {
			if(empty($type))
				throw new Exception('Type cannot be an empty string.');
			$this->type = $type;
		}
		
		public function getType() {
			if($this->type === null)
				throw new Exception('Type is not defined.');
			return $this->type;
		}
		
		public function setName($name) {
			if(empty($name))
				throw new Exception('Name cannot be an empty string.');
			$this->name = $name;
		}
		
		public function getName() {
			if($this->name === null)
				throw new Exception('Name is not defined.');
			return $this->name;
		}
		
		public function setExtension($extension) {
			if(empty($extension))
				throw new Exception('Extension cannot be an empty string.');
			$this->extension = $extension;
		}
		
		public function getExtension() {
			if($this->extension === null)
				throw new Exception('Extension is not defined.');
			return $this->extension;
		}
		
		public function setPath($path) {
			if(empty($path))
				throw new Exception('Path cannot be an empty string.');
			$this->path = $path;
		}
		
		public function getPath() {
			if($this->path === null)
				throw new Exception('Path is not defined.');
			return $this->path;
		}
	
		public function setTime($time) {
			if(!ctype_digit($time))
				throw new Exception('Timestamp must be an integer.');
			$this->time = $time;
		}
		
		public function getTime() {
			if($this->time === null)
				throw new Exception('Timestamp is not defined.');
			return $this->time;
		}
		
		public function setDescription($description) {
			if(strlen($description) > 20000)
				throw new Exception('Description must be shorter than 20000 characters.');
			$this->description = $description;
		}
		
		public function getDescription() {
			if($this->description === null) // empty description is considered defined
				throw new Exception('Description is not defined.');
			return $this->description;
		}
		
		public function setUploader($uuid) {
			if(!ApplicationModel_User::validateUuid($uuid))
				throw new Exception('Uploader UUID is invalid.');
			$this->uploader = $uuid;
		}
		
		public function getUploader() {
			if($this->uploader === null)
				throw new Exception('Uploader UUID is not defined.');
			return $this->uploader;
		}

		public function load() {
			if($this->id === null && $this->path !== null) {
				$filePathKey = new Rediska_Key('file_path_' . $this->path);
				if($filePathKey->getValue() === null)
					throw new Exception('File with path ' . $this->path . ' does not exist in the database.');
				$this->id = (int)$filePathKey->getValue();
			}
			
			if($this->id !== null) {
				if(!$this->application->rediska->exists('file_' . $this->id))
					throw new Exception('File with id ' . $this->id . ' does not exist in the database.');
				
				$fileKeyHash = new Rediska_Key_Hash('file_' . $this->id);
				$this->type = $fileKeyHash->type;
				$this->name = $fileKeyHash->name;
				$this->extension = $fileKeyHash->extension;
				$this->path = $fileKeyHash->path;
				$this->time = $fileKeyHash->timestamp;
				$this->description = $fileKeyHash->description;
				$this->uploader = $fileKeyHash->uuid;
			}
			
			else
				throw new Exception('Not enough information was given to load the file\'s information.');
		}
		
		public function save() {
			// id lookup key
			$filePathKey = new Rediska_Key('file_path_' . $this->path);
			
			// if id is known - we will be editing file's information
			if($this->id !== null) {
				// file with selected id must exist
				if(!$this->application->rediska->exists('file_' . $this->id))
					throw new Exception('File with id ' . $this->id . ' does not exist in the database.');
			}
			
			// if id is unknown - we will be creating a new file
			else {
				// file's path must not be taken by any other file
				if($filePathKey->getValue() !== null)
					throw new Exception('File with path ' . $this->path . ' already exists in the database.');
				
				// get the id of the new file
				$this->id = $this->incrementRedisCounter('records_count');
			}
			
			$fileKeyHash = new Rediska_Key_Hash('file_' . $this->id);
			$this->type = $fileKeyHash->type;
			$this->name = $fileKeyHash->name;
			$this->extension = $fileKeyHash->extension;
			$this->path = $fileKeyHash->path;
			$this->time = $fileKeyHash->timestamp;
			$this->description = $fileKeyHash->description;
			$this->uploader = $fileKeyHash->uuid;
			
			$filePathKey = new Rediska_Key('file_path_' . $this->path);
			$filePathKey->setValue($this->id);
		}
	}