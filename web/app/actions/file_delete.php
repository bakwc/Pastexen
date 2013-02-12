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
	
	final class ApplicationAction_file_delete extends ApplicationAction {
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
				throw new ApplicationException('Cannot delete file which belongs to a different user.', 403);
			
			// render the html
			$view = new ApplicationView($this->application, $this->application->path . '/views/file_delete.php');
			$view->user = $user;
			$view->file = $file;
			$view->render();
		}
	}