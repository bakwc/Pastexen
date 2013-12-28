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
		'date_format'						=> 'd.m.Y H:i:s',
	
		'menu_index'						=> 'Главная',
		'action_download'					=> 'Скачать',
		'action_open_raw'					=> 'Необработанный текст',
		'action_edit'						=> 'Редактировать',
		'action_save'						=> 'Сохранить',
		'action_delete'						=> 'Удалить',
		'action_cancel'						=> 'Отмена',
		'action_continue'					=> 'Продолжить',
		'action_login'						=> 'Вход',
		'action_logout'						=> 'Выход',
		'action_register'					=> 'Регистрация',
		'action_switch_to_dark'				=> 'Темный режим',
		'action_switch_to_light'			=> 'Светлый режим',
		'we_on_fb_and_vk_linked'			=> 'Мы в %sFacebook%s и %sВконтакте%s',
		'hero_unit'							=> 'Удобный способ передать картинку или часть кода.',
		'slogan_line1'						=> 'А что Вы делаете, когда надо быстро показать',
		'slogan_line2'						=> 'картинку или часть кода',
		'download_windows'					=> 'Для Windows',
		'download_other'					=> 'Другие сборки',
		'download_source'					=> 'Исходный код',
		'our_team'							=> 'Наша команда',
		'feature_screenshot_header' 		=> 'Делитесь скриншотами одним нажатием клавиши',
		'feature_screenshot_text'			=> 'Все снятые изображения сохраняются на сервере и создается ссылка. Остается лишь нажать Ctrl+V.',
		'feature_screenshot_picture_title'	=> 'Публикация скриншота',
		'feature_code_header' 				=> 'Копируйте код и отправляйте его уже с подсветкой синтаксиса',
		'feature_code_text' 				=> 'Весь загруженный код сохраняется на сервере и создается ссылка. Остается лишь нажать Ctrl+V.',
		'feature_code_picture_title'		=> 'Публикация исходного кода',
		'account_login_error'				=> 'Неверный логин или пароль.',
		'account_register_title'			=> 'Регистрация клиента',
		'account_register_description'		=> 'Если у Вас уже есть аккаунт, Вы можете ввести свой логин и пароль, чтобы добавить все файлы из нового клиента в Ваш аккаунт. Если у Вас нет аккаунта - выберите логин и пароль, чтобы его создать.',
		'congratulations'					=> 'Поздравляем!',
		'account_attached_successfully'		=> 'Вы успешно привязали Ваш клиент к Вашему аккаунту!',
		'account_registered_successfully'	=> 'Теперь у Вас есть персональный аккаунт, который Вы можете использовать для управления Вашими скриншотами и фрагментами кода!',
		'account_register_errors_occured'	=> 'При регистрации или привязке аккаунта возникли следующие проблемы:',
		'error_login_bad'					=> 'Выбран некорректный логин. Убедитесь, что Ваш логин содержит от 2 до 25 букв латинского алфавита и цифр.',
        'error_password_bad'                => 'Выбран некорректный пароль. Убедитесь, что Ваш пароль содержит от 4 до 25 символов.',
        'error_password_wrong'				=> 'Вы ввели неправильный для этого аккаунта пароль. Пожалуйста попробуйте еще раз.',
		'error_uuid_taken'					=>	'К сожалению, этот клиент уже был привязан к другому аккаунту.',
		'field_login'						=> 'Логин',
		'field_login_description'			=> 'Должен содержать от 4 до 25 букв латинского алфавита и цифр.',
		'field_password'					=> 'Пароль',
		'field_hide_password'					=> 'Скрыть пароль',
		'field_filename'					=> 'Имя',
		'field_extension'					=> 'Расширение',
		'field_description'					=> 'Описание',
		'my_files'							=> 'Мои файлы',
		'file_delete'						=> 'Удаление файла',
		'file_delete_confirmation'			=> 'Вы действительно уверены, что хотите удалить этот файл?',
		'file_edit'							=> 'Редактирование информации о файле',
		'file_edit_errors_occured'			=> 'Возникли следующие проблемы во время редактирования информации о файле:',
		'error_filename_bad'				=> 'Неправильное имя файла. Убедитесь, что оно содержит от 1 до 25 символов.',
		'error_extension_bad'				=> 'Запрещенное расширение файла.',
		'error_description_bad'				=> 'Описание файла слишком длинное. Убедитесь, что оно короче 20000 символов.'
	);
