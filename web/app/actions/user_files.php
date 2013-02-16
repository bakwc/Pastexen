<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013 powder96 <https://github.com/powder96>
	 * Copyright (C) 2013 bakwc <https://github.com/bakwc>
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
	
	final class ApplicationAction_user_files extends ApplicationAction {
		public function run() {
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
			
			// get the list of files for this user
			$files = array();
			$userUuids = $user->getUuids();
			foreach($userUuids as $time => $uuid) {
				// go through every file, this uuid has
				$userUuidFileIds = ApplicationModel_File::getIdsForUploader($this->application, $uuid);
				foreach($userUuidFileIds as $fileId) {
					try {
						// load file
						$file = new ApplicationModel_File($this->application);
						$file->setId($fileId);
						$file->load();
					
						// put it into the list of user's files
						$files[] = $file;
					}
					catch(ApplicationModelException_File $e) {
						// skip this file
						
						// temp:
						throw new ApplicationException($e->getMessage());
					}
				}
			}
			
			// render the html
			$view = new ApplicationView($this->application, $this->application->path . '/views/user_files.php');
			$view->user = $user;
			$view->files = $files;
			$view->render();
		}
	}