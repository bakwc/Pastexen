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
	require_once(dirname(__FILE__) . '/../models/File.php');
	
	final class ApplicationAction_init_test_db extends ApplicationAction {
		public function run() {
			// add users
			$users = array(
				array(
					'login' => 'derpyhooves',
					'password' => 'derpy',
					'uuids' => array(
						time() - (60 * 60 * 24 * 2) => 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
					),
				),
				array(
					'login' => 'pinkiepie',
					'password' => 'pinkie',
					'uuids' => array(
						time() - (60 * 60 * 24 * 3) => 'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb',
						time() - (60 * 60 * 24 * 7) => 'cccccccccccccccccccccccccccccccccccccccccccccccc'
					),
				),
			);
			foreach($users as $user) {
				$userModel = new ApplicationModel_User($this->application);
				$userModel->setLogin($user['login']);
				$userModel->setPasswordHash($userModel->makePasswordHash($user['password']));
				foreach($user['uuids'] as $time => $uuid)
					$userModel->addUuid($uuid, $time);
				$userModel->save();
			}
			
			// add files
			$files = array(
				array(
					'type' => ApplicationModel_File::TYPE_IMAGE,
					'name' => 'rainbowdash',
					'extension' => 'png',
					'systemName' => 'rd-bashful.png',
					'time' => time() - (60 * 60 * 24),
					'description' => 'Pastexen is now at least 20% cooler.',
					'uploader' => 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa',
				),
				array(
					'type' => ApplicationModel_File::TYPE_IMAGE,
					'name' => 'applejack',
					'extension' => 'png',
					'systemName' => 'aj-determined.png',
					'time' => time() - (60 * 60 * 24 * 2),
					'description' => 'Applejack seems to be angry.',
					'uploader' => 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa',
				),
				array(
					'type' => ApplicationModel_File::TYPE_IMAGE,
					'name' => 'twilightsparkle',
					'extension' => 'png',
					'systemName' => 'ts-crystal.png',
					'time' => time() - (60 * 60 * 24 * 7),
					'description' => 'This happened to Twilight in the second episode of the third season.',
					'uploader' => 'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb',
				),
				array(
					'type' => ApplicationModel_File::TYPE_IMAGE,
					'name' => 'fluttershy',
					'extension' => 'png',
					'systemName' => 'fs-ascending.png',
					'time' => time() - (60 * 60 * 24 * 30),
					'description' => 'Fluttershy rises up.',
					'uploader' => 'cccccccccccccccccccccccccccccccccccccccccccccccc',
				),
				array(
					'type' => ApplicationModel_File::TYPE_SOURCE,
					'name' => 'clipboard-docs',
					'extension' => 'txt',
					'systemName' => 'clipboard.txt',
					'time' => time() - (60 * 60 * 24 * 30),
					'description' => 'Something I have found on the internet.',
					'uploader' => 'cccccccccccccccccccccccccccccccccccccccccccccccc',
				),
				array(
					'type' => ApplicationModel_File::TYPE_SOURCE,
					'name' => 'pastexen',
					'extension' => 'cpp',
					'systemName' => 'pastexen-cpp.auto',
					'time' => time() - (60 * 60 * 24),
					'description' => 'Some code from pastexen.',
					'uploader' => 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa',
				),
				array(
					'type' => ApplicationModel_File::TYPE_SOURCE,
					'name' => 'pastexen',
					'extension' => 'css',
					'systemName' => 'pastexen-css.auto',
					'time' => time() - (60 * 60 * 24 * 2),
					'description' => 'Some code from pastexen frontend.',
					'uploader' => 'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb',
				),
				array(
					'type' => ApplicationModel_File::TYPE_SOURCE,
					'name' => 'google-prettify',
					'extension' => 'js',
					'systemName' => 'prettify-js.auto',
					'time' => time() - (60 * 60 * 24 * 7),
					'description' => 'Source code of the Google Prettify script.',
					'uploader' => 'cccccccccccccccccccccccccccccccccccccccccccccccc',
				),
				array(
					'type' => ApplicationModel_File::TYPE_SOURCE,
					'name' => 'test-code',
					'extension' => 'php',
					'systemName' => 'test.php',
					'time' => time() - (60 * 60 * 24 * 30),
					'description' => 'Php code for testing syntax highlighter.',
					'uploader' => 'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb',
				),
				array(
					'type' => ApplicationModel_File::TYPE_SOURCE,
					'name' => 'clock',
					'extension' => 'c',
					'systemName' => 'reactos-c.auto',
					'time' => time() - (60 * 60 * 24 * 7),
					'description' => 'Some code from ReactOS kernel.',
					'uploader' => 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa',
				),
			);
			foreach($files as $file) {
				$fileModel = new ApplicationModel_File($this->application);
				$fileModel->setType($file['type']);
				$fileModel->setName($file['name']);
				$fileModel->setExtension($file['extension']);
				$fileModel->setSystemName($file['systemName']);
				$fileModel->setTime($file['time']);
				$fileModel->setDescription($file['description']);
				$fileModel->setUploader($file['uploader']);
				$fileModel->save();
			}
		}
	}