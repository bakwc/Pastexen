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
	
	final class ApplicationAction_file_source_raw extends ApplicationAction {
		public function run() {
			if(!isset($this->application->parameters['file']))
				throw new ApplicationException('File identifier is missing.', 400);
			if(!ApplicationModel_File::validateSystemName($this->application->parameters['file']))
				throw new ApplicationException('System name of the file is invalid.', 400);
			$systemName = $this->application->parameters['file'];
			
			$file = new ApplicationModel_File($this->application);
			$file->setType(ApplicationModel_File::TYPE_SOURCE);
			$file->setSystemName($systemName);
			try {
				$file->load();
			}
			catch(ApplicationModelException_File $e) {
				if(!is_file($file->getPath()))
					throw new ApplicationException('File is not found.', 404);
			}
			
			$this->application->outputHeaders[] = 'Content-Type:text/plain; charset=utf-8';
			$this->application->outputContent = file_get_contents($file->getPath());
		}
	}