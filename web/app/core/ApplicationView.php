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
	
	final class ApplicationView {
		private $application;
		private $path;
		private $variables;
		
		public function __construct(&$application, $path) {
			$this->application = $application;
			$this->path = $path;
			$this->variables = array();
		}
		
		public function __set($variable, $value) {
			$this->variables[$variable] = $value;
		}
		
		public function render() {
			if(!is_readable($this->path))
				throw new Exception('Cannot find template file!', 500);
		
			foreach($this->variables as $variable => $value)
				$$variable = $value;

			ob_start();
			
			require($this->path);
			
			$this->application->outputContent = ob_get_clean();
		}
		
		// string l(string $languageStringName [, string $argument [, string $... ]])
		// returns a formatted language string, like sprintf()
		private function l($languageStringName) {
			if(!isset($this->application->languageStrings[$languageStringName]))
				return '%' . $languageStringName . '%';
			$functionArguments = func_get_args();
			return call_user_func_array('sprintf', array_merge(
				array($this->application->languageStrings[$languageStringName]),
				array_slice($functionArguments, 1)
			));
		}
	}