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
			// file id must be defined and valid
			if(!isset($this->application->parameters['file']))
				throw new ApplicationException('File identifier is missing.', 400);
			$fileId = (int)$this->application->parameters['file'];
			if(!ApplicationModel_File::validateId($fileId))
				throw new ApplicationException('Id of the file is invalid.', 400);
			
			// load file's information
			try {
				$file = new ApplicationModel_File($this->application);
				$file->setId($fileId);
				$file->load();
			}
			catch(ApplicationModelException_File $e) {
				throw new ApplicationException('File is not found.', 404);
			}
			
			// determine size of the thumbnail
			$minSize = $this->application->config['file_thumbnail_min_size'];
			$maxSize = $this->application->config['file_thumbnail_max_size'];
			if(!isset($this->application->parameters['size']))
				$size = $maxSize;
			else
				$size = min(max($minSize, (int)$this->application->parameters['size']), $maxSize);
			
			// create a thumbnail
			try {
				$imageGd = $file->getThumbnail($size);
			}
			catch(ApplicationModelException_File $e) {
				throw new ApplicationException('An error occured.', 500);
			}
			
			// return it to the user
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