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
	
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/Hash.php');
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/SortedSet.php');
	
	final class ApplicationModel_User extends ApplicationModel {
		private $id = null;
		private $login = null;
		private $passwordHash = null;
		private $uuids = array();
	
		public function __toString() {
			$uuidsString = '';
			foreach($this->uuids as $time => $uuid)
				$uuidsString .= $uuid . '/' . $time . ', ';
			return 'User(id: ' . $this->id . ', login: "' . $this->login . '", passwordHash: "' . $this->passwordHash . '", uuids: {' . $uuidsString . '})';
		}
	
		public function setId($id) {
			if(!is_numeric($id) || $id <= 0)
				throw new Exception('Id must be an integer greater than 0.');
			$this->id = $id;
		}
		
		public function getId() {
			if($this->id === null)
				throw new Exception('Id is not defined.');
			return $this->id;
		}
		
		public function setLogin($login) {
			if(!self::validateLogin($login))
				throw new Exception('Login is invalid.');
			$this->login = $login;
		}
		
		public function getLogin() {
			if($this->login === null)
				throw new Exception('Login is not defined.');
			return $this->login;
		}
		
		public static function validateLogin($login) {
			return strlen($login) >= 6 && strlen($login) <= 25 && self::validateAlphanumeric($login);
		}
		
		public function setPasswordHash($hash) {
			if(!self::validatePasswordHash($hash))
				throw new Exception('Password hash is invalid.');
			$this->passwordHash = $hash;
		}
		
		public function getPasswordHash() {
			if($this->passwordHash === null)
				throw new Exception('Password hash is not defined.');
			return $this->passwordHash;
		}
		
		public function makePasswordHash($password) {
			return md5(md5($password) . $this->application->config['user_password_salt']);
		}
		
		public static function validatePasswordHash($hash) {
			return strlen($hash) == 32 && self::validateHex($hash);
		}
		
		public function addUuid($uuid, $time) {
			if(!self::validateUuid($uuid))
				throw new Exception('Uuid is invalid.');
		
			// if there is the same uuid, we must remove it
			$sameUuidTime = array_search($uuid, $this->uuids);
			if($time !== false)
				unset($this->uuids[$sameUuidTime]);
			
			// if there is an uuid with the same time, it will be replaced
			$this->uuids[$time] = $uuid;
		}
		
		public function getUuids() {
			if(empty($this->uuids))
				throw new Exception('No uuids are defined.');
			return $this->uuids;
		}
		
		public static function validateUuid($uuid) {
			return strlen($uuid) == 48 && self::validateHex($uuid);
		}
		
		public function load() {
			if($this->id === null && $this->login !== null) {
				$userLoginKey = new Rediska_Key('user_login_' . $this->login);
				if($userLoginKey->getValue() === null)
					throw new Exception('User with login ' . $this->login . ' does not exist in the database.');
				$this->id = (int)$userLoginKey->getValue();
			}
			if($this->id !== null) {
				if(!$this->application->rediska->exists('user_' . $this->id))
					throw new Exception('User with id ' . $this->id . ' does not exist in the database.');
				
				$userKeyHash = new Rediska_Key_Hash('user_' . $this->id);
				$this->login = $userKeyHash->login;
				$this->passwordHash = $userKeyHash->password;
				
				$this->uuids = array();
				$uuidsKeySet = new Rediska_Key_SortedSet('user_' . $this->id . '_uuids');
				foreach($uuidsKeySet->toArray(true) as $uuid)
					$this->addUuid(substr($uuid->value, strlen('uuid_')), $uuid->score);
			}
			else
				throw new Exception('Not enough information was given to load the user data.');
		}
		
		public function save() {
			if($this->id !== null) { // id is known - edit existing user
				if(!$this->application->rediska->exists('user_' . $this->id))
					throw new Exception('User with id ' . $this->id . ' does not exist in the database.');
			}
			else { // id is unknown - create a new user
				$userLoginKey = new Rediska_Key('user_login_' . $this->login);
				if($userLoginKey->getValue() !== null)
					throw new Exception('User with login ' . $this->login . ' already exists in the database.');
				
				$this->id = $this->incrementRedisCounter('users_count');
			}
			
			$userKeyHash = new Rediska_Key_Hash('user_' . $this->id);
			$userKeyHash->login = $this->login;
			$userKeyHash->password = $this->passwordHash;
			
			$userLoginKey = new Rediska_Key('user_login_' . $this->login);
			$userLoginKey->setValue($this->id);
			
			$uuidsKeySet = new Rediska_Key_SortedSet('user_' . $this->id . '_uuids');
			foreach($uuidsKeySet as $uuid)
				$uuidsKeySet->remove($uuid);
			foreach($this->uuids as $time => $uuid)
				$uuidsKeySet[$time] = 'uuid_' . $uuid;
		}
	}