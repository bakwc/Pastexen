<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013  powder96 <https://github.com/powder96>
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
	
	require_once(dirname(__FILE__) . '/../models/Source.php');
	
	final class ApplicationAction_source_raw extends ApplicationAction {
		public function run() {
			if(!isset($this->application->parameters['file']))
				throw new Exception('File identifier is missing.', 400);
			
			try {
				$source = new ApplicationModel_Source($this->application->parameters['file'], $this->application->config['sources_dir']);
			}
			catch(Exception $e) {
				throw new Exception('File was not found.', 404);
			}
			
			$this->application->outputHeaders = array(
				'Content-Type:text/plain; charset=utf-8'
			);
			$this->application->outputContent = $source->getData();
		}
	}