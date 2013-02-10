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
	
	abstract class ApplicationModel {
		protected $application; // instance of Application class
		
		/**
		 * Creates a model object.
		 */
		public function __construct(&$application) {
			$this->application = $application;
		}
		
		/**
		 * Increments the value of the key in Redis database and returns it after the increment.
		 * If the racing condition occurs, it will try to increment the value again. If it happens
		 * five times, an exception will be thrown.
		 */
		protected function incrementRedisCounter($name, $amount = 1) {
			// try to do an increment five times
			for($i = 0; $i < 5; ++$i) {
				// start transaction and watch
				$transaction = $this->application->rediska->transaction();
				$transaction->watch($name);

				// get the current value, increment it, and then store it
				$value = $this->application->rediska->get($name);
				$value += $amount;
				$transaction->set($name, $value);
				
				// execute transaction
				try {
					$transaction->execute();
				}
				catch(Rediska_Transaction_AbortedException $e) {
					// try to do increment again
					continue;
				}
				
				// success, return new counter's value
				return $value;
			}
			
			// we tried many times with no success
			throw new Exception('Cannot perform increment.');
		}
		
		/**
		 * Checks whether a string contains only alphanumberic characters. Returns false if it is not.
		 */
		protected static function validateAlphanumeric($string) {
			return strspn($string, '0123456789abcdefghijklmnopqrstuvwxyz') === strlen(strtolower($string));
		}
		
		/**
		 * Checks whether a string is a valid HEX number. Returns false if it is not.
		 */
		protected static function validateHex($string) {
			return strspn($string, '0123456789AaBbCcDdEeFf') === strlen($string);
		}
		
		/**
		 * Checks whether a string is a valid Md5 hash. Returns false if it is not.
		 */
		protected static function validateMd5Hash($string) {
			return strlen($string) == 32 && self::validateHex($string);
		}
	}