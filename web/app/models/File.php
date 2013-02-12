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
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/SortedSet.php');
	require_once(dirname(__FILE__) . '/../lib/ProgrammingLanguageDetector/ProgrammingLanguageDetector.php');
	
	final class ApplicationModel_File extends ApplicationModel {
		// file types
		const TYPE_IMAGE  = 0; // image
		const TYPE_SOURCE = 1; // source code
	
		private $id = null;				// file id
		private $type = null;			// file type (self::TYPE_* constant)
		private $name = null;			// file name, given by the user
		private $extension = null;		// file extension, given by the user
		private $systemName = null;		// name of the file in the filesystem
		private $systemNameOld = null;	// name of the file in the filesystem as it is stored in the database
		private $time = null;			// upload time
		private $description = null;	// description, given by the user
		private $uploader = null;		// uuid of the uploader
		private $uploaderOld = null;	// uuid of the uploader as it is stored in the database
	
		/**
		 * Converts this object to string. Used only for debugging.
		 */
		public function __toString() {
			return 'File(id: ' . $this->id . ', type: "' . $this->type . '", name: "' . $this->name . '", extension: "' . $this->extension . '", systemName: "' . $this->systemName . '", time: ' . $this->time . ', description: "' . $this->description .'", uploader: "' . $this->uploader . '")';
		}
		
		/**
		 * Sets the id. If it is invalid, throws an exception.
		 */
		public function setId($id) {
			if(!is_int($id) || $id <= 0)
				throw new Exception('Id must be an integer greater than 0.');
			$this->id = $id;
		}
		
		/**
		 * Returns the id. If id is not known, throws an exception.
		 */
		public function getId() {
			if($this->id === null)
				throw new Exception('Id is not defined.');
			return $this->id;
		}
		
		/**
		 * Sets the file type. If it is invalid, throws an exception.
		 */
		public function setType($type) {
			if($type != self::TYPE_IMAGE && $type != self::TYPE_SOURCE)
				throw new Exception('File type is invalid.');
			$this->type = $type;
		}
		
		/**
		 * Returns the file type. If it is not known, throws an exception.
		 */
		public function getType() {
			if($this->type === null)
				throw new Exception('Type is not defined.');
			return $this->type;
		}
		
		/**
		 * Sets the filename. If it is empty, throws an exception.
		 */
		public function setName($name) {
			if(empty($name))
				throw new Exception('Name cannot be an empty string.');
			$this->name = $name;
		}
		
		/**
		 * Returns the filename. If it is not known, throws an exception.
		 */
		public function getName() {
			if($this->name === null)
				throw new Exception('Name is not defined.');
			return $this->name;
		}
		
		/**
		 * Sets the extension. If it is an empty string, throws an exception.
		 */
		public function setExtension($extension) {
			if(empty($extension))
				throw new Exception('Extension cannot be an empty string.');
			$this->extension = $extension;
		}
		
		/**
		 * Returns the extension. If it is not known, throws an exception.
		 */
		public function getExtension() {
			if($this->extension === null)
				throw new Exception('Extension is not defined.');
			return $this->extension;
		}
		
		/**
		 * Sets the name of the file in the filesystem. If it is invalid, an exception will be thrown.
		 */
		public function setSystemName($systemName) {
			if(!self::validateSystemName($systemName))
				throw new Exception('System name is invalid.');
			$this->systemName = $systemName;
		}
		
		/**
		 * Returns the name of the file in the filesystem. If it is not known, throws an exception.
		 */
		public function getSystemName() {
			if($this->systemName === null)
				throw new Exception('System name is not defined.');
			return $this->systemName;
		}
		
		/**
		 * Checks whether the system name of the file is valid. Returns false, if it is not.
		 */
		public static function validateSystemName($systemName) {
			return strlen($systemName) >= 2 && strlen($systemName) <= 25 && self::validateAlphanumeric(str_replace(array('.', '-'), '', $systemName));
		}
		
		/**
		 * Sets the time of the file upload. If it is not a valid timestamp, throws an exception.
		 */
		public function setTime($time) {
			if(!is_int($time))
				throw new Exception('Upload timestamp must be an integer.');
			$this->time = $time;
		}
		
		/**
		 * Returns the time of the file upload. If it is not known, throws an exception.
		 */
		public function getTime() {
			if($this->time === null)
				throw new Exception('Timestamp is not defined.');
			return $this->time;
		}
		
		/**
		 * Sets the description of the file. If it is longer than 20k symbols, throws an exception.
		 * Note: the description _can_ be an empty string.
		 */
		public function setDescription($description) {
			if(strlen($description) > 20000)
				throw new Exception('Description must be shorter than 20000 characters.');
			$this->description = $description;
		}
		
		/**
		 * Returns the description of the file. If it is unknown, throws an exception.
		 */
		public function getDescription() {
			if($this->description === null)
				throw new Exception('Description is not defined.');
			return $this->description;
		}
		
		/**
		 * Sets the uploader's UUID. If it is not a valid uuid, throws an exception.
		 */
		public function setUploader($uuid) {
			if(!ApplicationModel_User::validateUuid($uuid))
				throw new Exception('Uploader UUID is invalid.');
			$this->uploader = $uuid;
		}
		
		/**
		 * Returns the uploader's UUID. If it is not known, throws an exception.
		 */
		public function getUploader() {
			if($this->uploader === null)
				throw new Exception('Uploader UUID is not defined.');
			return $this->uploader;
		}

		/**
		 * Returns path to the file in the filesystem. Throws an exception, if the system name is not known.
		 */
		public function getPath() {
			switch($this->getType()) {
				case self::TYPE_IMAGE:  return $this->application->config['file_image_dir']  . '/' . $this->getSystemName(); break;
				case self::TYPE_SOURCE: return $this->application->config['file_source_dir'] . '/' . $this->getSystemName(); break;
			}
		}
		
		/**
		 * Returns url for this file. If the type of it or the system name is not known,
		 * throws an exception.
		 */
		public function getUrl() {
			switch($this->getType()) {
				case self::TYPE_IMAGE:  $linkTemplate = $this->application->config['file_image_link']; break;
				case self::TYPE_SOURCE: $linkTemplate = $this->application->config['file_source_link']; break;
			}
			return sprintf($linkTemplate, $this->getSystemName());
		}
		
		/**
		 * Returns the name of the programming language in which the source should have been written. Throws an
		 * exception if the file's type is not a source code or if the file cannot be found.
		 */
		public function getProgrammingLanguage() {
			$filePath = $this->getPath();
			if(!is_file($filePath))
				throw new Exception('Cannot access file ' . $filePath . '.');
			$sourceText = substr(file_get_contents($filePath), 0, 20000);
			
			$languages = array(
				// file extension => programming language
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
				'txt'	=> 'text',
				'dat'	=> 'text'
			);
			
			if(isset($languages[$this->extension]))
				return $languages[$this->extension];
			
			$systemExtension = strtolower(pathinfo($this->systemName, PATHINFO_EXTENSION));
			if(isset($languages[$systemExtension]))
				return $languages[$systemExtension];
			
			$detector = new ProgrammingLanguageDetector();
			$detector->importKnowledgeBase(file_get_contents(dirname(__FILE__) . '/../lib/ProgrammingLanguageDetector/knowledge_base.dat'));
			return $detector->detect($sourceText);
		}
		
		/**
		 * Returns gd image which can be used as a thumbnail for the file. Throws an exception if the requested
		 * thumbnail is too big, too small, or if a file does not exist.
		 */
		public function getThumbnail($size = 64) {
			if($size < $this->application->config['file_thumbnail_min_size'])
				throw new Exception('The thumbnail is too small.');
			
			if($size > $this->application->config['file_thumbnail_max_size'])
				throw new Exception('The thumbnail is too big.');
		
			$filePath = $this->getPath();
			
			if(!is_file($filePath))
				throw new Exception('Cannot access file ' . $filePath . '.');
			
			switch($this->getType()) {
				case self::TYPE_IMAGE:
					$pictureImageGd = imageCreateFromPNG($filePath);
					$pictureWidth  = imageSx($pictureImageGd);
					$pictureHeight = imageSy($pictureImageGd);
					
					$scale = $size / max($pictureWidth, $pictureHeight);
					$pictureNewWidth  = ceil($pictureWidth  * $scale);
					$pictureNewHeight = ceil($pictureHeight * $scale);
					
					if($pictureWidth > $pictureHeight) {
						$pictureNewX = 0;
						$pictureNewY = ($size - $pictureNewHeight) / 2;
					}
					else {
						$pictureNewX = ($size - $pictureNewWidth ) / 2;
						$pictureNewY = 0;
					}
					
					$imageGd = imageCreateTrueColor($size, $size);
					$bgColor = $this->application->config['file_image_thumbnail_bgcolor'];
					$bgColorGd = imageColorResolve($imageGd, $bgColor[0], $bgColor[1], $bgColor[2]);
					imageFilledRectangle($imageGd, 0, 0, $size - 1, $size - 1, $bgColorGd);
					
					imageCopyResampled($imageGd, $pictureImageGd, $pictureNewX, $pictureNewY, 0, 0, $pictureNewWidth, $pictureNewHeight, $pictureWidth, $pictureHeight);
					
					imageDestroy($pictureImageGd);
					
					return $imageGd;
				
				case self::TYPE_SOURCE:
					$codeImageGd = imageCreate($size * 2, $size * 2); // Note: truecolor image is not necessary here.
					$codeBgColor = $this->application->config['file_source_thumbnail_bgcolor'];
					$codeFgColor = $this->application->config['file_source_thumbnail_fgcolor'];
					$codeBgColorGd = imageColorResolve($codeImageGd, $codeBgColor[0], $codeBgColor[1], $codeBgColor[2]);
					$codeFgColorGd = imageColorResolve($codeImageGd, $codeFgColor[0], $codeFgColor[1], $codeFgColor[2]);

					$sourceText = substr(file_get_contents($filePath), 0, 20000);
					$sourceText = str_replace("\t", '    ', $sourceText);
					$sourceText = str_replace("\r", '', $sourceText);
					
					$fontGd = 4; // gd fonts 2 and 4 work the best
					$x = 0;
					$y = 0;
					for($i = 0, $sourceSz = strlen($sourceText); $i < $sourceSz; ++$i) {
						if($sourceText{$i} == "\n") {
							$x = 0;
							$y += imageFontHeight($fontGd);
						}
						elseif($sourceText{$i} >= ' ' && $sourceText{$i} <= '~') {
							imageString($codeImageGd, $fontGd, $x, $y, $sourceText{$i}, $codeFgColorGd);
							$x += imageFontWidth($fontGd);
						}
					}
					
					$imageGd = imageCreateTrueColor($size, $size);
					imageCopyResampled($imageGd, $codeImageGd, 0, 0, 0, 0, $size, $size, imageSx($codeImageGd), imageSy($codeImageGd));
					
					imageDestroy($codeImageGd);
					
					return $imageGd;
			}
		}
		
		/**
		 * Loads file's information from the database. The file's id or system name must be set before this function
		 * is called. If the id is unknown, but the system name is known, this function will try to find the file's
		 * id first. Then it will try to load the information for the file from the database. If an error occurs, an
		 * exception will be thrown.
		 */
		public function load() {
			// if the id is unknown, but the system name is - use id lookup key to get the id of the file.
			if($this->id === null && $this->systemName !== null) {
				$fileSysNameKey = new Rediska_Key('file_path_' . $this->systemName);
				if($fileSysNameKey->getValue() === null)
					throw new Exception('File with system name ' . $this->systemName . ' does not exist in the database.');
				$this->id = (int)$fileSysNameKey->getValue();
			}
			
			// if the id is known, load the information from the database
			if($this->id !== null) {
				if(!$this->application->rediska->exists('file_' . $this->id))
					throw new Exception('File with id ' . $this->id . ' does not exist in the database.');
				
				// load some information from the database
				$fileKeyHash = new Rediska_Key_Hash('file_' . $this->id);
				$this->name = $fileKeyHash->name;
				$this->extension = $fileKeyHash->extension;
				$this->systemName = $fileKeyHash->path;
				$this->systemNameOld = $this->systemName;
				$this->time = (int)$fileKeyHash->timestamp;
				$this->description = $fileKeyHash->description;
				$this->uploader = substr($fileKeyHash->uuid, strlen('uuid_'));
				$this->uploaderOld = $this->uploader;
				
				// get file's type
				switch(strtolower($fileKeyHash->type)) {
					case 'image': $this->type = self::TYPE_IMAGE; break;
					case 'text':  $this->type = self::TYPE_SOURCE; break;
					default:
						throw new Exception('Unknown file type.');
				}
			}
			
			// if the id is unknown, we cannot do anything
			else
				throw new Exception('Not enough information was given to load the file\'s information.');
		}
		
		/**
		 * Saves file's information into the database. If file's id is not known, it will try to create a new file.
		 * If file's id is known, it will try to edit information in the database to make it identical to information
		 * in this class. If you want to change file's system name or uploader's uuid, load file's information before
		 * doing that. If an error occurs, this function will throw an exception.
		 */
		public function save() {
			// id lookup key
			$fileSysNameKey = new Rediska_Key('file_path_' . $this->systemName);
			
			// if id is known - we will be editing file's information
			if($this->id !== null) {
				// file with selected id must exist
				if(!$this->application->rediska->exists('file_' . $this->id))
					throw new Exception('File with id ' . $this->id . ' does not exist in the database.');
			}
			
			// if id is unknown - we will be creating a new file
			else {
				// file's system name must not be taken by any other file
				if($fileSysNameKey->getValue() !== null)
					throw new Exception('File with system name ' . $this->systemName . ' already exists in the database.');
				
				// get the id of the new file
				$this->id = $this->incrementRedisCounter('records_count');
			}
			
			// if file's system name needs to be changed
			if($this->systemNameOld !== null && $this->systemNameOld != $this->systemName) {
				// new system name must not be taken by another file
				if($fileSysNameKey->getValue() !== null)
					throw new Exception('File with system name ' . $this->systemName . ' already exists in the database.');
				
				// remove old id lookup key
				$fileSysNameOldKey = new Rediska_Key('file_path_' . $this->systemNameOld);
				$fileSysNameOldKey->delete();
			}
			
			// if uploader's uuid needs to be changed
			if($this->uploaderOld !== null && $this->uploaderOld != $this->uploader) {
				// remove file from the list of files for the old uuid
				$filesOldUuidKeySet = new Rediska_Key_SortedSet('uuid_' . $this->uploaderOld);
				$filesOldUuidKeySet->remove('file_' . $this->id);
			}
			
			// save file's information
			$fileKeyHash = new Rediska_Key_Hash('file_' . $this->id);
			$fileKeyHash->name = $this->name;
			$fileKeyHash->extension = $this->extension;
			$fileKeyHash->path = $this->systemName;
			$this->systemNameOld = $this->systemName;
			$fileKeyHash->timestamp = $this->time;
			$fileKeyHash->description = $this->description;
			$fileKeyHash->uuid = 'uuid_' . $this->uploader;
			$this->uploaderOld = $this->uploader;
			
			// save file's type
 			switch($this->type) {
				case self::TYPE_IMAGE:  $fileKeyHash->type = 'image'; break;
				case self::TYPE_SOURCE: $fileKeyHash->type = 'text' ; break;
			}
			
			// save id lookup key
			$fileSysNameKey->setValue($this->id);
			
			// reset uploader's uuid
			$filesUuidKeySet = new Rediska_Key_SortedSet('uuid_' . $this->uploader);
			$filesUuidKeySet->remove('file_' . $this->id);
			$filesUuidKeySet[$this->time] = 'file_' . $this->id;
		}
		
		/**
		 * Returns ids of all files which have a specified uploader's UUID. Throws an exception if UUID is invalid or
		 * if there are no files for selected UUID.
		 */
		public static function getIdsForUploader(&$application, $uuid) {
			// uuid must be valid
			if(!ApplicationModel_User::validateUuid($uuid))
				throw new Exception('Uploader UUID is invalid.');
			
			// get id of every file with a selected uuid
			$fileIds = array();
			$filesKeySet = new Rediska_Key_SortedSet('uuid_' . $uuid);
			foreach($filesKeySet as $fileId)
				$fileIds[] = (int)substr($fileId, strlen('file_'));
			
			// if our list is empty - throw an exception
			if(empty($fileIds))
				throw new Exception('There are no files with UUID ' . $uuid . ' in the database.');
			
			return $fileIds;
		}
	}