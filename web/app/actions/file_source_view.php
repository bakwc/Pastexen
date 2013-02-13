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
	
	require_once(dirname(__FILE__) . '/../models/File.php');
	
	final class ApplicationAction_file_source_view extends ApplicationAction {
		public function run() {
			// system name must be defined and valid
			if(!isset($this->application->parameters['file']))
				throw new ApplicationException('System name of the file is missing.', 400);
			if(!ApplicationModel_File::validateSystemName($this->application->parameters['file']))
				throw new ApplicationException('System name of the file is invalid.', 400);
			$systemName = $this->application->parameters['file'];
			
			// load file's information
			$file = new ApplicationModel_File($this->application);
			$file->setSystemName($systemName);
			try {
				$file->load();
			}
			catch(ApplicationModelException_File $e) {
				// file does not exist in the database, but if the file exists in the filesystem, then fill the model with default data
				$file->setType(ApplicationModel_File::TYPE_SOURCE);
				$file->setTime(time());
				$file->setDescription('');
				$file->setUploader('000000000000000000000000000000000000000000000000');
				try {
					$file->setExtension(pathinfo($file->getSystemName(), PATHINFO_EXTENSION));
					$file->setName(basename($file->getSystemName(), '.' . $file->getExtension()));
				}
				catch(ApplicationModelException_File $e) {
					// we have got problems with file's name/extension
					$file->setExtension('txt');
					$file->setName('untitled');
				}
				
				// if the file does not exist both in db and in fs, this is a problem
				if(!is_file($file->getPath()))
					throw new ApplicationException('File is not found.', 404);
			}
			
			// this action works only for sources
			if($file->getType() != ApplicationModel_File::TYPE_SOURCE)
				throw new ApplicationException('This page supports only source code files.', 403);
			
			// load file's owner
			try {
				$fileUploader = $file->getUploader();
				$owner = new ApplicationModel_User($this->application);
				$owner->setId(ApplicationModel_User::getIdForUuid($this->application, $fileUploader));
				$owner->load();
			}
			catch(ApplicationModelException_File $e) {
				// file has no defined owner
				$owner = null;
			}
			catch(ApplicationModelException_User $e) {
				// file has the owner, but it cannot be loaded
				throw new ApplicationException('Cannot load the owner of the file.', 500);
			}
			
			// render html
			$view = new ApplicationView($this->application, $this->application->path . '/views/file_source_view.php');
			$view->url = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
			$view->owner = $owner;
			$view->file = $file;
			$view->fileData = file_get_contents($file->getPath());
			
			// choose the right color scheme
			$view->isDarkColorScheme = true;
			if(isset($this->application->parameters['light']))
				$view->isDarkColorScheme = false;
			
			// display html
			$view->render();
		}
	}