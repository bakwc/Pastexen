<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013  powder96 <https://github.com/powder96>
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
?>
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Pastexen</title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<meta name="description" content="" />
		<meta name="author" content="" />
		<link rel="shortcut icon" href="/app/static/images/favicon.png" />
		<link href="/app/static/styles/twitter-bootstrap-2.2.2.css" rel="stylesheet" />
		<link href="/app/static/styles/app-common.css" rel="stylesheet" />
	</head>
	<body>
		<div class="container">
			<img src="/app/static/images/logo_beta.png" />
			
			<hr />
			
			<div class="hero-unit">
				<h1>Удобный способ передать картинку или часть кода.</h1>
			</div>
			
			<hr />

			<div class="download-section">
				<h1>Скачать:</h1>
				<div class="btn-group">
					<?php
						$supportedDownloads = array(
							'windows'	=> 'Windows',
							'debian'	=> 'Debian',
							'source'	=> 'Source'
						);
						foreach($supportedDownloads as $configName => $downloadName) {
							if(isset($this->application->config['download_link_' . $configName]))
								echo '<a class="btn btn-large btn-warning" href="' . $this->application->config['download_link_' . $configName] . '">' . $downloadName . '</a>';
						}
					?>
				</div>
			</div>
			
			<hr />
			
			<div class="row">
				<div class="span6">
					<div class="media">
						<img class="pull-left" src="/app/static/images/features-code.png" />
						<div class="media-body">
							<h4 class="media-heading">Делитесь скриншотами одним нажатием клавиши</h4>
							<p>Все снятые изображения сохраняются на сервере и создается ссылка. Вам остается лишь нажать Ctrl+V.</p>
						</div>
					</div>
				</div>
				<div class="span6">
					<div class="media">
						<img class="pull-left" src="/app/static/images/features-screenshot.png" />
						<div class="media-body">
							<h4 class="media-heading">Копируйте код и отправляйте его с подсветкой синтаксиса</h4>
							<p>Весь загруженный код сохраняется на сервере и создается ссылка. Вам остается лишь нажать Ctrl+V.</p>
						</div>
					</div>
				</div>
			</div>
			
		</div>
	</body>
</html>