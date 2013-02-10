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
			$login = '';
			$password = '';
			$uuidBad = false;
			$loginBad = false;
			$passwordWrong = false;
			$registerUser = false;	
		
			if(!isset($this->application->parameters['uuid']))
				$uuidBad = true;
			else {
				$uuid = $this->application->parameters['uuid'];
				if(!ApplicationModel_User::validateUuid($uuid))
					$uuidBad = true;
			}
			
			if(!isset($this->application->parameters['login']))
				$loginBad = true;
			else {
				$login = $this->application->parameters['login'];
				if(!ApplicationModel_User::validateLogin($login))
					$loginBad = true;
			}
			
			if(isset($this->application->parameters['password']))
				$password = $this->application->parameters['password'];
			
			$success = !$uuidBad && !$loginBad;
			
			if($success) {
				$user = new ApplicationModel_User($this->application);
				try {
					$user->setLogin($login);
					$user->load();
				}
				catch(Exception $e) {
					$registerUser = true;
				}
				
				if($registerUser) {
					$user->setPasswordHash($user->makePasswordHash($password));
					$user->save();
				}
				else // user exists; check password
					if($user->makePasswordHash($password) != $user->getPasswordHash())
						$passwordWrong = true;
				
				$success = !$passwordWrong;
			}
			
			if($success) {
				try {
					$_SESSION['authorized_user_id']    = $user->getId();
					$_SESSION['authorized_user_login'] = $user->getLogin();
				
					$user->addUuid($uuid, time());
					$user->save();
				}
				catch(Exception $e) {
					$success = false;
				}
			}
			
			$view = new ApplicationView($this->application, $this->application->path . '/views/user_register_handler.php');
			$view->success = $success;
			$view->registered = $registerUser;
			$view->uuid = $uuid;
			$view->uuidBad = $uuidBad;
			$view->login = $login;
			$view->loginBad = $loginBad;
			$view->passwordWrong = $passwordWrong;
			$view->render();
		}
	}