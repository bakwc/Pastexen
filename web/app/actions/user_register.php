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
	
	final class ApplicationAction_user_register extends ApplicationAction {
		public function run() {
			$uuid = '';
			$uuidBad = false;
			if(!isset($this->application->parameters['uuid']))
				$uuidBad = true;
			else {
				$uuid = $this->application->parameters['uuid'];
				if(!ApplicationModel_User::validateUuid($uuid))
					$uuidBad = true;
			}
			if($uuidBad) {
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /';
				$this->application->outputContent = '';
				return;
			}
		
			$view = new ApplicationView($this->application, $this->application->path . '/views/user_register.php');
			$view->uuid = $uuid;
			
			if(isset($this->application->parameters['login']))
				$view->login = $this->application->parameters['login'];
			elseif(isset($_SESSION['authorized_user_login']))
				$view->login = $_SESSION['authorized_user_login'];
			else
				$view->login = '';
			
			$view->render();
		}
	}