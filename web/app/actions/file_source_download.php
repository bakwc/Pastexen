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
	
	final class ApplicationAction_file_source_download extends ApplicationAction {
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
			catch(Exception $e) {
				$file->setType(ApplicationModel_File::TYPE_SOURCE);
				if(!is_file($file->getPath()))
					throw new Exception('File is not found.', 404);
				$file->setName(basename($file->getSystemName(), '.' . pathinfo($file->getSystemName(), PATHINFO_EXTENSION)));
				$file->setExtension('auto');
			}
			
			if($file->getType() != ApplicationModel_File::TYPE_SOURCE)
				throw new Exception('Incorrect file type.', 403);
			
			$this->application->outputHeaders = array(
				'Content-type: application/force-download',
				'Content-Disposition: attachment; filename="' . $file->getName() . '.' . $file->getProgrammingLanguage() .  '"',
				'Content-Length: ' . filesize($file->getPath())
			);
			$this->application->outputContent = file_get_contents($file->getPath());
		}
	}