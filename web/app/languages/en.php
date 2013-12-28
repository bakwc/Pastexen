<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013 powder96 <https://github.com/powder96>
	 * Copyright (C) 2013 bakwc <https://github.com/bakwc>
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
	
	$this->languageStrings = array(
		'date_format'						=> 'n/j/Y g:iA',
	
		'menu_index'						=> 'Home',
		'action_download'					=> 'Download',
		'action_open_raw'					=> 'Raw',
		'action_edit'						=> 'Edit',
		'action_save'						=> 'Save',
		'action_delete'						=> 'Delete',
		'action_cancel'						=> 'Cancel',
		'action_continue'					=> 'Continue',
		'action_login'						=> 'Log In',
		'action_logout'						=> 'Log Out',
		'action_register'					=> 'Register',
		'action_switch_to_dark'				=> 'Dark mode',
		'action_switch_to_light'			=> 'Light mode',
		'we_on_fb_and_vk_linked'			=> 'Pastexen on %sFacebook%s and %sVkontakte%s',
		'hero_unit'							=> 'An easy way to send a picture or a part of code.',
		'slogan_line1'						=> 'What do you do when you need to rapidly show',
		'slogan_line2'						=> 'a picture or a part of code',
		'download_windows'					=> 'For Windows',
		'download_other'					=> 'Other builds',
		'download_source'					=> 'Source code',
		'our_team'							=> 'Our team',
		'feature_screenshot_header'			=> 'Share screenshots by pressing only one key',
		'feature_screenshot_text'			=> 'All taken screenshots are saved on the server and the url for it is created. The only thing you have to do is to press Ctrl+V.',
		'feature_screenshot_picture_title'	=> 'Publishing a screenshot',
		'feature_code_header' 				=> 'Copy code and send it with syntax highlighting',
		'feature_code_text' 				=> 'All uploaded code is saved on the server and the url for it is created. The only thing you have to do is to press Ctrl+V.',
		'feature_code_picture_title'		=> 'Publishing a part of code',
		'account_login_error'				=> 'A wrong login or password.',
		'account_register_title'			=> 'Client registration',
		'account_register_description'		=> 'If you already have an account, you can enter your login and password to add all your files from your client to your account. If you don\'t have one, then choose a login and password to create it.',
		'congratulations'					=> 'Congratulations!',
		'account_attached_successfully'		=> 'You have successfully attached your client to your account!',
		'account_registered_successfully'	=> 'Now you have your own personal account, which you can use to manage your screenshots and code pastes!',
		'account_register_errors_occured'	=> 'Some problems occured during the registration or the attachment of the account:',
		'error_login_bad'					=> 'Chosen login is invalid. Make sure that your login is longer than 2 and shorter than 25 characters and contains only letters and numbers.',
        'error_password_bad'                => 'Chosen password is invalid. Make sure that your password is logner than 4 and shorter than 25 characters.',
        'error_password_wrong'				=> 'You have entered an incorrect password for this account. Please try again.',
		'error_uuid_taken'					=> 'We are sorry, but this client is already attached to a different account.',
		'field_login'						=> 'Login',
		'field_login_description'			=> 'It must be longer than 4 and shorter than 25 characters and must contain only letters and numbers.',
		'field_password'					=> 'Password',
		'field_hide_password'				=> 'Hide password',
		'field_filename'					=> 'Name',
		'field_extension'					=> 'Extension',
		'field_description'					=> 'Description',
		'my_files'							=> 'My files',
		'file_delete'						=> 'Delete a file',
		'file_delete_confirmation'			=> 'Are you sure you want to delete this file?',
		'file_edit'							=> 'Edit file\'s information',
		'file_edit_errors_occured'			=> 'Some errors occured while editing file\'s information:',
		'error_filename_bad'				=> 'The name of the file is invalid. Make sure that it is between 1 and 25 characters long.',
		'error_extension_bad'				=> 'Disallowed file extension.',
		'error_description_bad'				=> 'The description is too long. Make sure that it contains less than 20000 characters.'
	);
