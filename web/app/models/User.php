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
	
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key.php');
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/Hash.php');
	require_once(dirname(__FILE__) . '/../lib/Rediska/Rediska/Key/SortedSet.php');
	
	final class ApplicationModel_User extends ApplicationModel {
		private $id = null;				// user id
		private $login = null;			// user's login
		private $loginOld = null;		// user's login as it is stored in the database
		private $passwordHash = null;	// user's password hash
		private $uuids = array();		// UUIDs of user's clients
	
		/**
		 * Converts this object to string. Used only for debugging.
		 */
		public function __toString() {
			$uuidsString = '';
			foreach($this->uuids as $time => $uuid)
				$uuidsString .= $uuid . '/' . $time . ', ';
			return 'User(id: ' . $this->id . ', login: "' . $this->login . '", passwordHash: "' . $this->passwordHash . '", uuids: {' . $uuidsString . '})';
		}
	
		/**
		 * Sets the id. If it is invalid, throws an exception.
		 */
		public function setId($id) {
			if(!is_int($id) || $id <= 0)
				throw new Exception('Id must be an integer greater than 0.');
			$this->id = $id;
		}
		
		/**
		 * Returns the id. If id is not known, throws an exception.
		 */
		public function getId() {
			if($this->id === null)
				throw new Exception('Id is not defined.');
			return $this->id;
		}
		
		/**
		 * Sets the login. If the login is invalid, throws an exception.
		 */
		public function setLogin($login) {
			if(!self::validateLogin($login))
				throw new Exception('Login is invalid.');
			$this->login = strtolower($login);
		}
		
		/**
		 * Returns the login. If it is not known, throws an exception.
		 */
		public function getLogin() {
			if($this->login === null)
				throw new Exception('Login is not defined.');
			return $this->login;
		}
		
		/**
		 * Checks whether the login is valid. Returns false, if it is not.
		 */
		public static function validateLogin($login) {
			return strlen($login) >= 4 && strlen($login) <= 25 && self::validateAlphanumeric($login);
		}
		
		/**
		 * Sets the password hash. Throws an exception if it is not a valid md5 hash.
		 */
		public function setPasswordHash($hash) {
			if(!self::validateMd5Hash($hash))
				throw new Exception('Password hash is invalid.');
			$this->passwordHash = $hash;
		}
		
		/**
		 * Returns the password hash. Throws an exception if the hash is not known.
		 */
		public function getPasswordHash() {
			if($this->passwordHash === null)
				throw new Exception('Password hash is not defined.');
			return $this->passwordHash;
		}
		
		/**
		 * Makes hash from the original password.
		 */
		public function makePasswordHash($password) {
			return md5(md5($password) . $this->application->config['user_password_salt']);
		}
		
		/**
		 * Adds a client's UUID to the list of user's clients. UUIDs and their timestamps must be unique for one user.
		 * You cannot add two same UUIDs or two different UUIDs with same timestamps to one user. Throws an exception
		 * if the uuid or its timestamp is invalid.
		 */
		public function addUuid($uuid, $time) {
			if(!self::validateUuid($uuid))
				throw new Exception('UUID is invalid.');
			if(!is_int($time))
				throw new Exception('Timestamp must be an integer.');
		
			// if there is the same uuid, we must remove it
			$sameUuidTime = array_search($uuid, $this->uuids);
			if($time !== false)
				unset($this->uuids[$sameUuidTime]);
			
			// if there is an uuid with the same time, it will be replaced
			$this->uuids[$time] = $uuid;
		}
		
		/**
		 * Returns a list of user's clients UUIDs. Throws an exception if there are no UUIDs for the user.
		 * The list is an associative array where the keys are timestamps of the moment when UUID was attached
		 * to the user and values are the UUID strings.
		 */
		public function getUuids() {
			if(empty($this->uuids))
				throw new Exception('No uuids are defined.');
			return $this->uuids;
		}
		
		/**
		 * Checks whether the client UUID is valid. Returns false, if it is not.
		 */
		public static function validateUuid($uuid) {
			return strlen($uuid) == 48 && self::validateHex($uuid);
		}
		
		/**
		 * Loads the user's information from the database. The user's login or id must be set before this
		 * function is called. If the id is unknown, but the login is known, this function will try to find the
		 * user's id first. Then, it will try to load the information for this user from the database. If an
		 * error occurs, an exception will be thrown.
		 */
		public function load() {
			// id is unknown, but the login is known - use id lookup key to get the id of the user with selected login
			if($this->id === null && $this->login !== null) {
				$userLoginKey = new Rediska_Key('user_login_' . $this->login);
				if($userLoginKey->getValue() === null)
					throw new Exception('User with login ' . $this->login . ' does not exist in the database.');
				$this->id = (int)$userLoginKey->getValue();
			}
			
			// if the id is known - load user's information from the database
			if($this->id !== null) {
				// user with selected id must exist
				if(!$this->application->rediska->exists('user_' . $this->id))
					throw new Exception('User with id ' . $this->id . ' does not exist in the database.');
				
				// load user's login and password hash
				$userKeyHash = new Rediska_Key_Hash('user_' . $this->id);
				$this->login = $userKeyHash->login;
				$this->loginOld = $this->login;
				$this->passwordHash = $userKeyHash->password;
				
				// load user's uuids
				$this->uuids = array();
				$uuidsKeySet = new Rediska_Key_SortedSet('user_' . $this->id . '_uuids');
				foreach($uuidsKeySet->toArray(true) as $uuid)
					$this->addUuid(substr($uuid->value, strlen('uuid_')), (int)$uuid->score);
			}
			
			// if the id is unknown, we cannot do anything
			else
				throw new Exception('Not enough information was given to load the user data.');
		}
		
		/**
		 * Saves user's information into the database. If user's id is not known, it will try to create
		 * a new user. If user's id is known, it will try to edit information in database to make it identical
		 * to information in this class. If you want to change user's login, load user's information before doing
		 * so. If an error occurs, this function will throw an exception.
		 */
		public function save() {
			// id lookup key
			$userLoginKey = new Rediska_Key('user_login_' . $this->login);
		
			// id is known - we are going to edit user's information
			if($this->id !== null) {
				// user with selected id must exist
				if(!$this->application->rediska->exists('user_' . $this->id))
					throw new Exception('User with id ' . $this->id . ' does not exist in the database.');
			}
			
			// id is unknown - we are going to create a new user
			else {
				// new login must not be taken by someone else
				if($userLoginKey->getValue() !== null)
					throw new Exception('User with login ' . $this->login . ' already exists in the database.');
				
				// get the id for the new user
				$this->id = $this->incrementRedisCounter('users_count');
			}
			
			// if user's login needs to be changed
			if($this->loginOld !== null && $this->loginOld != $this->login) {
				// new login must not be taken by someone else
				if($userLoginKey->getValue() !== null)
					throw new Exception('User with login ' . $this->login . ' already exists in the database.');
				
				// remove old id lookup key
				$userLoginOldKey = new Rediska_Key('user_login_' . $this->loginOld);
				$userLoginOldKey->delete();
			}
			
			// save user's login and password hash
			$userKeyHash = new Rediska_Key_Hash('user_' . $this->id);
			$userKeyHash->login = $this->login;
			$this->loginOld = $this->login;
			$userKeyHash->password = $this->passwordHash;

			// save id lookup key
			$userLoginKey->setValue($this->id);
			
			// reset uuids for this user
			$uuidsKeySet = new Rediska_Key_SortedSet('user_' . $this->id . '_uuids');
			foreach($uuidsKeySet as $uuid)
				$uuidsKeySet->remove($uuid); // Note: do not cut off 'uuid_' prefix here.
			foreach($this->uuids as $time => $uuid)
				$uuidsKeySet[$time] = 'uuid_' . $uuid;
		}
	}