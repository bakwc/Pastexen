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
		const ERROR_INVALID_ID				= 10001;
		const ERROR_INVALID_LOGIN			= 10002;
		const ERROR_INVALID_PASSWORD_HASH	= 10003;
		const ERROR_INVALID_UUID			= 10004;
		const ERROR_INVALID_UUID_TIMESTAMP	= 10005;
		const ERROR_UNDEFINED_ID			= 10101;
		const ERROR_UNDEFINED_LOGIN			= 10102;
		const ERROR_UNDEFINED_PASSWORD_HASH	= 10103;
		const ERROR_NOTFOUND_ID				= 10201;
		const ERROR_NOTFOUND_LOGIN			= 10202;
		const ERROR_NOTFOUND_UUID			= 10203;
		const ERROR_TAKEN_LOGIN				= 10301;
		const ERROR_TAKEN_UUID				= 10302;
	
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
			return 'User(' .
				'id: ' . $this->id . ', ' .
				'login: "' . $this->login . '", ' .
				'passwordHash: "' . $this->passwordHash . '", ' .
				'uuids: {' . $uuidsString . '}' .
			')';
		}
	
		/**
		 * Sets the id. If it is invalid, throws an exception with code self::ERROR_INVALID_ID.
		 */
		public function setId($id) {
			if(!is_int($id) || $id <= 0)
				throw new ApplicationModelException_User('Id must be an integer greater than 0.',
					self::ERROR_INVALID_ID);
			$this->id = $id;
		}
		
		/**
		 * Returns the id. If id is not known, throws an exception with code self::ERROR_UNDEFINED_ID.
		 */
		public function getId() {
			if($this->id === null)
				throw new ApplicationModelException_User('Id is not defined.', self::ERROR_UNDEFINED_ID);
			return $this->id;
		}
		
		/**
		 * Sets the login. If the login is invalid, throws an exception with code self::ERROR_INVALID_LOGIN.
		 */
		public function setLogin($login) {
			if(!self::validateLogin($login))
				throw new ApplicationModelException_User('Login is invalid.', self::ERROR_INVALID_LOGIN);
			$this->login = strtolower($login);
		}
		
		/**
		 * Returns the login. If it is not known, throws an exception with code self::ERROR_UNDEFINED_LOGIN.
		 */
		public function getLogin() {
			if($this->login === null)
				throw new ApplicationModelException_User('Login is not defined.', self::ERROR_UNDEFINED_LOGIN);
			return $this->login;
		}
		
		/**
		 * Checks whether the login is valid. Returns false, if it is not.
		 */
		public static function validateLogin($login) {
			return strlen($login) >= 4 && strlen($login) <= 25 && self::validateAlphanumeric($login);
		}
		
		/**
		 * Sets the password hash. Throws an exception with code self::ERROR_INVALID_PASSWORD_HASH if it is not a
		 * valid md5 hash.
		 */
		public function setPasswordHash($hash) {
			if(!self::validateMd5Hash($hash))
				throw new ApplicationModelException_User('Password hash is invalid.',
					self::ERROR_INVALID_PASSWORD_HASH);
			$this->passwordHash = $hash;
		}
		
		/**
		 * Returns the password hash. Throws an exception with code self::ERROR_UNDEFINED_PASSWORD_HASH if the hash
		 * is not known.
		 */
		public function getPasswordHash() {
			if($this->passwordHash === null)
				throw new ApplicationModelException_User('Password hash is not defined.',
					self::ERROR_UNDEFINED_PASSWORD_HASH);
			return $this->passwordHash;
		}
		
		/**
		 * Makes hash from the original password.
		 */
		public function makePasswordHash($password) {
			return md5(md5($password) . $this->application->config['user_password_salt']);
		}
		
		/**
		 * Adds a client's UUID to the list of user's clients. UUIDs and their timestamps must be unique for one
		 * user. You cannot add two same UUIDs or two different UUIDs with same timestamps to one user. Throws an
		 * exception with code self::ERROR_INVALID_UUID if the uuid is invalid. It also throws an exception with
		 * code self::ERROR_INVALID_UUID_TIMESTAMP if the timestamp of the uuid is invalid.
		 */
		public function addUuid($uuid, $time) {
			if(!self::validateUuid($uuid))
				throw new ApplicationModelException_User('UUID is invalid.', self::ERROR_INVALID_UUID);
			if(!is_int($time))
				throw new ApplicationModelException_User('Timestamp must be an integer.',
					self::ERROR_INVALID_UUID_TIMESTAMP);
		
			// if there is the same uuid, we must remove it
			$sameUuidTime = array_search($uuid, $this->uuids);
			if($time !== false)
				unset($this->uuids[$sameUuidTime]);
			
			// if there is an uuid with the same time, it will be replaced
			$this->uuids[$time] = $uuid;
		}
		
		/**
		 * Returns a list of user's clients UUIDs. Returns an empty array if there are no UUIDs for the user. The
		 * list is an associative array where the keys are timestamps of the moment when UUID was attached to the
		 * user and values are the UUID strings.
		 */
		public function getUuids() {
			return $this->uuids;
		}
		
		/**
		 * Checks whether the client UUID is valid. Returns false, if it is not.
		 */
		public static function validateUuid($uuid) {
			return strlen($uuid) == 48 && self::validateHex($uuid);
		}
		
		/**
		 * Loads the user's information from the database. The user's login or id must be set before this function
		 * is called. If the id is unknown, but the login is known, this function will try to find the user's id
		 * first. Then, it will try to load the information for this user from the database. If user with selected
		 * id or login is not found an exception with code self::ERROR_NOTFOUND_ID or self::ERROR_NOTFOUND_LOGIN
		 * will be thrown. An exception with code self::ERROR_UNDEFINED_ID will be throw if the id is not known.
		 */
		public function load() {
			// id is unknown, but the login is known - use id lookup key to get the id of the user
			if($this->id === null && $this->login !== null) {
				$userLoginKey = new Rediska_Key('user_login_' . $this->login);
				if($userLoginKey->getValue() === null)
					throw new ApplicationModelException_User(
						'User with login ' . $this->login . ' does not exist in the database.',
						self::ERROR_NOTFOUND_LOGIN);
				$this->id = (int)$userLoginKey->getValue();
			}
			
			// if the id is known - load user's information from the database
			if($this->id !== null) {
				// user with selected id must exist
				if(!$this->application->rediska->exists('user_' . $this->id))
					throw new ApplicationModelException_User(
						'User with id ' . $this->id . ' does not exist in the database.',
						self::ERROR_NOTFOUND_ID);
				
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
				throw new ApplicationModelException_User('Not enough information was given to load the user data.',
					self::ERROR_UNDEFINED_ID);
		}
		
		/**
		 * Saves user's information into the database. If user's id is not known, it will try to create a new user.
		 * If user's id is known, it will try to edit information in database to make it identical to information in
		 * this class. If you want to change user's login, load user's information before doing so. If the user with
		 * selected id does not exist, it will throw an exception with code self::ERROR_NOTFOUND_ID. If the function
		 * tries to create a user with taken login, an exception with code self::ERROR_TAKEN_LOGIN will be thrown.
		 * If the user tries to take somebody else's UUID, an exception with code self::ERROR_TAKEN_UUID will be
		 * thrown.
		 */
		public function save() {
			// id lookup key
			$userLoginKey = new Rediska_Key('user_login_' . $this->login);
		
			// id is known - we are going to edit user's information
			if($this->id !== null) {
				// user with selected id must exist
				if(!$this->application->rediska->exists('user_' . $this->id))
					throw new ApplicationModelException_User(
						'User with id ' . $this->id . ' does not exist in the database.', self::ERROR_NOTFOUND_ID);
			}
			
			// id is unknown - we are going to create a new user
			else {
				// new login must not be taken by someone else
				if($userLoginKey->getValue() !== null)
					throw new ApplicationModelException_User(
						'User with login ' . $this->login . ' already exists in the database.',
						self::ERROR_TAKEN_LOGIN);
			}
			
			// make sure that the user does not want to take somebody else's uuid
			foreach($this->uuids as $time => $uuid) {
				$userUuidKey = new Rediska_Key('user_uuid_' . $uuid);
				if($userUuidKey->getValue() !== null && $userUuidKey->getValue() != $this->id)
					throw new ApplicationModelException_User(
						'UUID ' . $uuid . ' is taken by somebody else.', self::ERROR_TAKEN_UUID);
			}
			
			// if we are creating a new user - get the new id for him
			if($this->id === null)
				$this->id = $this->incrementRedisCounter('users_count');
			
			// if user's login needs to be changed (for new users, this will not be active)
			if($this->loginOld !== null && $this->loginOld != $this->login) {
				// new login must not be taken by someone else
				if($userLoginKey->getValue() !== null)
					throw new ApplicationModelException_User(
						'User with login ' . $this->login . ' already exists in the database.',
						self::ERROR_TAKEN_LOGIN);
				
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
			
			// for every old uuid of the user:
			$uuidsKeySet = new Rediska_Key_SortedSet('user_' . $this->id . '_uuids');
			foreach($uuidsKeySet as $uuid) {
				// cut off uuid_ prefix
				$uuid = substr($uuid, strlen('uuid_'));
			
				// remove old id lookup key
				$userUuidOldKey = new Rediska_Key('user_uuid_' . $uuid);
				$userUuidOldKey->delete();
			
				// remove uuid from the list
				$uuidsKeySet->remove('uuid_' . $uuid);
			}
			
			// for every new uuid for this user:
			foreach($this->uuids as $time => $uuid) {
				// create a new id lookup key
				$userUuidKey = new Rediska_Key('user_uuid_' . $uuid);
				$userUuidKey->setValue($this->id);
			
				// add it to the list
				$uuidsKeySet[$time] = 'uuid_' . $uuid;
			}
		}
		
		/**
		 * Deletes user from the database. Unimplemented.
		 */
		public function delete() {
		}
		
		/**
		 * Returns the id of the user to which the selected UUID belongs. If selected UUID does not exist, an
		 * exception with code self::ERROR_NOTFOUND_UUID will be thrown.
		 */
		public static function getIdForUuid(&$application, $uuid) {
			// uuid must be valid
			if(!ApplicationModel_User::validateUuid($uuid))
				throw new ApplicationModelException_File('UUID is invalid.', self::ERROR_INVALID_UUID);
			
			// use id lookup key
			$userUuidKey = new Rediska_Key('user_uuid_' . $uuid);
			if($userUuidKey->getValue() === null)
				throw new ApplicationModelException_File('UUID is invalid.', self::ERROR_NOTFOUND_UUID);
			return $userUuidKey->getValue();
		}
	}
	
	final class ApplicationModelException_User extends ApplicationModelException {
	}