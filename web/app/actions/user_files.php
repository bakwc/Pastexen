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
                throw new ApplicationException($e->getMessage(), 500);
			}
			
			// get the page number
			if(isset($this->application->parameters['page']))
				$page = (int)$this->application->parameters['page'];
			else
				$page = 1;
			if($page <= 0)
				$page = 1;

			// build list of files, this user owns
			$userUuids = $user->getUuids();
			$userFiles = array();
			$fileLoadQueueTimestamps = array();
			$fileLoadQueue = array();
			foreach($userUuids as $time => $uuid) {
				$userUuidFileIds = ApplicationModel_File::getIdsForUploader($this->application, $uuid);
				$userFiles = array_merge($userFiles, $userUuidFileIds);
			}
			
			foreach($userFiles as $userFile) {
				$fileLoadQueue[] = (int)substr($userFile["value"], strlen('file_'));
				$fileLoadQueueTimestamps[] = (int)($userFile["score"]);
			}
			
			array_multisort($fileLoadQueueTimestamps, SORT_DESC, $fileLoadQueue, SORT_ASC);
			
			// make sure that the requested page number is not too high
			$totalPages = ceil(count($fileLoadQueue) / $this->application->config['user_files_per_page']);
			if($page > $totalPages)
				$page = 1;
			
			// cut off the part of the file list which we need to display on selected page
			$fileLoadQueue = array_slice($fileLoadQueue, ($page - 1) * $this->application->config['user_files_per_page'], $this->application->config['user_files_per_page']);
			
			// load file information for every file which we need to display on this page
			foreach($fileLoadQueue as $fileId) {
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
				}
			}
			
			// render the html
			$view = new ApplicationView($this->application, $this->application->path . '/views/user_files.php');
			$view->user = $user;
			$view->files = $files;
			$view->currentPage = $page;
			$view->totalPages = $totalPages;
			$view->render();
		}
	}