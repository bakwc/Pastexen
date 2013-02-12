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
	
	final class ApplicationAction_file_thumbnail extends ApplicationAction {
		public function run() {
			if(!isset($this->application->parameters['file']))
				throw new Exception('System name of the file must be set.', 400);
			
			if(!ApplicationModel_File::validateSystemName($this->application->parameters['file']))
				throw new Exception('System name of the file is invalid.', 400);
			
			try {
				$file = new ApplicationModel_File($this->application);
				$file->setSystemName($this->application->parameters['file']);
				$file->load();
			}
			catch(Exception $e) {
				throw new Exception('File is not found.', 404);
			}
			
			$minSize = $this->application->config['file_thumbnail_min_size'];
			$maxSize = $this->application->config['file_thumbnail_max_size'];
			if(!isset($this->application->parameters['size']))
				$size = $maxSize;
			else
				$size = min(max($minSize, (int)$this->application->parameters['size']), $maxSize);
			
			try {
				$imageGd = $file->getThumbnail($size);
			}
			catch(Exception $e) {
				throw new Exception('An error occured.', 500);
			}
			
			$this->application->outputHeaders = array(
				'Expires: ' . gmdate('D, d M Y H:i:s', time() + (60 * 60 * 24 * 30 * 6)) . ' GMT', // 6 months
				'Last-Modified: ' . gmdate('D, d M Y H:i:s') . ' GMT',
				'Cache-Control: public',
				'Content-type: image/png'
			);
			ob_start();
			imagePNG($imageGd);
			$this->application->outputContent = ob_get_clean();
		}
	}