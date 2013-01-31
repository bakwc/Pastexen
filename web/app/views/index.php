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
			<img src="/app/static/images/logo_beta.png" alt="Pastexen" />
			<a class="pull-right" href="/Oo.php"><?php echo $this->l('our_team'); ?></a>
			
			<hr />
			
			<div class="hero-unit">
				<h1><?php echo $this->l('hero_unit'); ?></h1>
			</div>
			
			<hr />

			<div class="download-section">
				<h1><?php echo $this->l('action_download'); ?>:</h1>
				<div class="btn-group">
					<?php
						$supportedDownloads = array(
							'windows'	=> array(true,  $this->l('download_windows')),
							'other'		=> array(false, $this->l('download_other')),
							'source'	=> array(false, $this->l('download_source'))
						);
						foreach($supportedDownloads as $configName => $downloadParameters) {
							list($downloadImportance, $downloadName) = $downloadParameters;
							if(isset($this->application->config['download_link_' . $configName]))
								echo '<a class="btn btn-large' . ($downloadImportance ? ' btn-warning' : '') . '" href="' . $this->application->config['download_link_' . $configName] . '">' . $downloadName . '</a>';
						}
					?>
				</div>
			</div>
			
			<hr />
			
			<div class="row">
				<div class="span6">
					<div class="media">
						<img class="pull-left" src="/app/static/images/features-code.png" alt="<?php echo $this->l('feature_code_picture_title'); ?>" />
						<div class="media-body">
							<h4 class="media-heading"><?php echo $this->l('feature_code_header'); ?></h4>
							<p><?php echo $this->l('feature_code_text'); ?></p>
						</div>
					</div>
				</div>
				<div class="span6">
					<div class="media">
						<img class="pull-left" src="/app/static/images/features-screenshot.png" alt="<?php echo $this->l('feature_screenshot_picture_title'); ?>" />
						<div class="media-body">
							<h4 class="media-heading"><?php echo $this->l('feature_screenshot_header'); ?></h4>
							<p><?php echo $this->l('feature_screenshot_text'); ?></p>
						</div>
					</div>
				</div>
			</div>
		</div>
	</body>
</html>
