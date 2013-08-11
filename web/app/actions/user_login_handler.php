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
	
	final class ApplicationAction_user_login_handler extends ApplicationAction {
		public function run() {
			$success = true;
			$use_uuid = false;
			$register_redirect = false;
			$uuid = '';
			$login = '';
			$password = '';
			
			if(!isset($this->application->parameters['login']))
				$success = false;
			else {
				$login = $this->application->parameters['login'];
				if(!ApplicationModel_User::validateLogin($login))
					$success = false;
			}
			
			if(!isset($this->application->parameters['password']))
				$success = false;
			else
				$password = $this->application->parameters['password'];
			
			if (!$success) {
				if (isset($this->application->parameters['uuid'])) {
					$uuid = $this->application->parameters['uuid'];
					if (ApplicationModel_User::validateUuid($uuid)) {
						$use_uuid = true;
					}
				}
			}
			
			if($success) {
				$user = new ApplicationModel_User($this->application);
				try {
					$user->setLogin($login);
					$user->load();
				}
				catch(ApplicationModelException_User $e) {
					$success = false;
				}
			}
			
			if ($use_uuid) {
				$success = true;
				$user = new ApplicationModel_User($this->application);
				try {
					$user->setUuid($uuid);
					$user->load();
				}
				catch(ApplicationModelException_User $e) {
					$success = false;
					$register_redirect = true;
				}
			}
			
			if($success)
				if(!$use_uuid && $user->makePasswordHash($password) != $user->getPasswordHash())
					$success = false;

			if($success) {
				$_SESSION['authorized_user_id']    = $user->getId();
				$_SESSION['authorized_user_login'] = $user->getLogin();
				
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /account.php';
				$this->application->outputContent = '';
			}
			elseif ($register_redirect) {
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /register.php?uuid=' . $uuid;
				$this->application->outputContent = '';
			}
			else {
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /login.php?unsuccessful&login=' . urlencode($login);
				$this->application->outputContent = '';
			}
		}
	}