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
	
	final class ApplicationAction_user_register_handler extends ApplicationAction {
		public function run() {
			$uuid = '';
			$uuidBad = false;
			if(!isset($this->application->parameters['uuid']))
				$uuid = '';
			else {
				$uuid = $this->application->parameters['uuid'];
				if(!empty($uuid) && !ApplicationModel_User::validateUuid($uuid))
					$uuidBad = true;
			}
			if($uuidBad) { // invalid uuid is set
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /';
				$this->application->outputContent = '';
				return;
			}

			$login = '';
			$loginBad = false;
			if(!isset($this->application->parameters['login']))
				$loginBad = true;
			else {
				$login = $this->application->parameters['login'];
				if(!ApplicationModel_User::validateLogin($login))
					$loginBad = true;
			}
			
			$password = '';
			if(isset($this->application->parameters['password']))
				$password = $this->application->parameters['password'];
			
			// login must be valid
			$success = !$loginBad;
			
			$passwordWrong = false;
			$registerUser = false;
			if($success) {
				$user = new ApplicationModel_User($this->application);
				try {
					// try to load user with selected login 
					$user->setLogin($login);
					$user->load();
				}
				catch(ApplicationModelException_User $e) {
					// selected login does not exist - create a new user
					$registerUser = true;
				}
				
				// register a new user
				if($registerUser) {
					$user->setPasswordHash($user->makePasswordHash($password));
					$user->save();
				}
				
				// user already exists, then check password
				else {
					if($user->makePasswordHash($password) != $user->getPasswordHash())
						$passwordWrong = true;
					$success = !$passwordWrong;
				}
			}
			
			// if everything is ok (user has the correct password, etc)...
			$attachUser = false;
			$uuidTaken = false;
			if($success) {
				try {
					// authorize user
					$_SESSION['authorized_user_id']    = $user->getId();
					$_SESSION['authorized_user_login'] = $user->getLogin();
				
					// attach uuid if we have to
					if(!empty($uuid)) {
						$attachUser = true;
						$user->addUuid($uuid, time());
						$user->save();
					}
				}
				catch(ApplicationModelException_User $e) {
					if($e->getCode() == ApplicationModel_User::ERROR_TAKEN_UUID)
						$uuidTaken = true;
					$success = !$uuidTaken;
				}
			}
			
			// if we have only authorized the user
			if($success && !$registerUser && !$attachUser) {
				$this->application->outputHeaders[] = 'HTTP/1.1 302 Found';
				$this->application->outputHeaders[] = 'Location: /account.php';
				$this->application->outputContent = '';
			}
			
			$view = new ApplicationView($this->application, $this->application->path . '/views/user_register_handler.php');
			$view->success = $success;
			$view->registered = $registerUser;
			$view->uuid = $uuid;
			$view->uuidTaken = $uuidTaken;
			$view->login = $login;
			$view->loginBad = $loginBad;
			$view->passwordWrong = $passwordWrong;
			$view->render();
		}
	}