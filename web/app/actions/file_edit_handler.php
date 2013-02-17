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
	
	require_once(dirname(__FILE__) . '/../models/User.php');
	require_once(dirname(__FILE__) . '/../models/File.php');
	
	final class ApplicationAction_file_edit_handler extends ApplicationAction {
		public function run() {
			// file id must be defined and valid
			if(!isset($this->application->parameters['file']))
				throw new ApplicationException('File identifier is missing.', 400);
			$fileId = (int)$this->application->parameters['file'];
			if(!ApplicationModel_File::validateId($fileId))
				throw new ApplicationException('Id of the file is invalid.', 400);
		
			// user must be authorized
			if(!isset($_SESSION['authorized_user_id'])) {
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /login.php';
				$this->application->outputContent = '';
				return;
			}
			
			// filename must be present and valid
			$name = '';
			$nameBad = false;
			if(!isset($this->application->parameters['name']))
				$nameBad = true;
			else {
				$name = $this->application->parameters['name'];
				if(!ApplicationModel_File::validateName($name))
					$nameBad = true;
			}
			
			// description must be valid
			$description = '';
			$descriptionBad = false;
			if(isset($this->application->parameters['description'])) {
				$description = $this->application->parameters['description'];
				if(!ApplicationModel_File::validateDescription($description))
					$descriptionBad = true;
			}
			
			// load user's information
			$user = new ApplicationModel_User($this->application);
			try {
				$user->setId($_SESSION['authorized_user_id']);
				$user->load();
			}
			catch(ApplicationModelException_User $e) {
                throw new ApplicationException('Cannot load user.', 500);
			}
			
			// load file's information
			$file = new ApplicationModel_File($this->application);
			try {
				$file->setId($fileId);
				$file->load();
			}
			catch(ApplicationModelException_File $e) {
                throw new ApplicationException('File is not found.', 404);
			}
			
			// ignore extension for images
			if($file->getType() == ApplicationModel_File::TYPE_IMAGE) {
				$extension = $file->getExtension();
				$extensionBad = false;
			}

			// for other files - extension must be present and valid			
			else {
				$extension = '';
				$extensionBad = false;
				if(!isset($this->application->parameters['extension']))
					$extensionBad = true;
				else {
					$extension = $this->application->parameters['extension'];
					if(!$file->validateExtension($extension))
						$extensionBad = true;
				}
			}
			
			// load file owner's information
			try {
				$owner = new ApplicationModel_User($this->application);
				$owner->setId(ApplicationModel_User::getIdForUuid($this->application, $file->getUploader()));
				$owner->load();
			}
			catch(ApplicationModelException_User $e) {
				throw new ApplicationException('Cannot load file\'s owner.', 500);
			};
			
			// authorized user must be the owner of the file
			if($user->getId() != $owner->getId())
				throw new ApplicationException('Cannot edit file which belongs to a different user.', 403);
			
			// check if everything is fine
			$success = !$nameBad && !$extensionBad && !$descriptionBad;
			
			// edit the file
			if($success) {
				try {
					$file->setName($name);
					$file->setExtension($extension);
					$file->setDescription($description);
					$file->save();
				}
				catch(ApplicationModelException_File $e) {
					throw new ApplicationException('Cannot save file\'s information.', 500);
				}
				
				// redirect user back to his account
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /account.php';
				$this->application->outputContent = '';
				return;
			}
			
			// render the html with errors
			$view = new ApplicationView($this->application, $this->application->path . '/views/file_edit_handler.php');
			$view->success = $success;
			$view->filenameBad = $nameBad;
			$view->extensionBad = $extensionBad;
			$view->descriptionBad = $descriptionBad;
			$view->user = $user;
			$view->file = $file;
			$view->name = $name;
			$view->extension = $extension;
			$view->description = $description;
			$view->render();
		}
	}