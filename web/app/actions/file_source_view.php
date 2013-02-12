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
			if(!isset($this->application->parameters['file']))
				throw new Exception('File identifier is missing.', 400);
			
			if(!ApplicationModel_File::validateSystemName($this->application->parameters['file']))
				throw new Exception('System name of the file is invalid.', 400);
			
			$file = new ApplicationModel_File($this->application);
			$file->setSystemName($this->application->parameters['file']);
			try {
				$file->load();
			}
			catch(ApplicationModelException_File $e) {
				$file->setType(ApplicationModel_File::TYPE_SOURCE);
				if(!is_file($file->getPath()))
					throw new Exception('File is not found.', 404);
				$file->setName(basename($file->getSystemName(), '.' . pathinfo($file->getSystemName(), PATHINFO_EXTENSION)));
				$file->setExtension('auto');
				$file->setTime(time());
				$file->setDescription('');
				$file->setUploader('000000000000000000000000000000000000000000000000');
			}
			
			if($file->getType() != ApplicationModel_File::TYPE_SOURCE)
				throw new Exception('Incorrect file type.', 403);
			
			try {
				$owner = new ApplicationModel_User($this->application);
				$owner->setId(ApplicationModel_User::getIdForUuid($this->application, $file->getUploader()));
				$owner->load();
			}
			catch(ApplicationModelException_User $e) {
				$owner = null;
			}
			
			$view = new ApplicationView($this->application, $this->application->path . '/views/file_source_view.php');
			
			$view->url = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
			$view->owner = $owner;
			$view->file = $file;
			$view->fileData = file_get_contents($file->getPath());

			$view->isDarkColorScheme = true;
			if(isset($this->application->parameters['light']))
				$view->isDarkColorScheme = false;
			
			$view->render();
		}
	}