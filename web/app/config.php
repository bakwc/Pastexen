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
	
	$applicationConfig = array(
		'database_host'					=> '127.0.0.1',
		'database_port'					=> 6379,
		'database_password'				=> null,
		
		'language_default'				=> 'ru',
		
		'download_link_windows'			=> 'https://github.com/bakwc/Pastexen/raw/master/builds/pastexen_v2_win32_installer.exe',
		'download_link_other'			=> 'https://github.com/bakwc/Pastexen/tree/master/builds',
		'download_link_source'			=> 'https://github.com/bakwc/Pastexen/',
		
		'user_password_salt'			=> 'pastexen',
		'user_files_pagination_range'	=> 5,
		'user_files_per_page'			=> 20,
		
		'file_image_link'				=> 'http://pastexen.com/i/%s',
		'file_source_link'				=> 'http://pastexen.com/code.php?file=%s',
		'file_thumbnail_link'			=> 'http://pastexen.com/thmb/%s.png',
                'file_image_dir'                                => '/pastexen/images',
                'file_source_dir'                               => '/pastexen/sources',

		'file_extensions'				=> array(
			// file extension => programming language
			'php'   => 'php',
			'cpp'   => 'cpp',
			'c'     => 'cpp',
			'h'     => 'cpp',
			'py'    => 'python',
			'java'  => 'java',
			'm'     => 'objectivec',
			'l'     => 'objectivec',
			'pl'    => 'perl',
			'xml'   => 'xml',
			'html'  => 'xml',
			'js'    => 'javascript',
			'css'   => 'css',
			'json'  => 'json',
			'as'    => 'actionscript',
			'vb'    => 'vbscript',
			'http'  => 'http',
			'cs'    => 'cs',
			'd'     => 'd',
			'sql'   => 'sql',
			'st'    => 'smalltalk',
			'lisp'  => 'lisp',
			'cl'    => 'lisp',
			'ini'   => 'ini',
			'conf'  => 'apache',
			'sh'    => 'bash',
			'bat'   => 'dos',
			'cmake' => 'cmake',
			'hs'    => 'haskell',
			'lhs'   => 'haskell',
			'txt'	=> 'text',
			'dat'	=> 'text',
			'auto'  => 'auto'
		)
	);
