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
?>
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Pastexen - <?php echo $pageTitle; ?></title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<meta name="description" content="" />
		<meta name="author" content="" />
		<link rel="shortcut icon" href="/app/static/images/favicon.png" />
		<!--[if lt IE 9]>
			<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
		<![endif]-->
		<?php
			if(isset($jsIncludes))
				foreach($jsIncludes as $js)
					echo '<script src="' . $js . '"></script>';
		?>
		<link href="/app/static/styles/twitter-bootstrap-2.2.2.css" rel="stylesheet" />
		<?php
			if(isset($cssIncludes))
				foreach($cssIncludes as $css)
					echo '<link href="' . $css . '" rel="stylesheet" />';
		?>
		<link href="/app/static/styles/app-common.css" rel="stylesheet" />
		<link href="/app/static/styles/app-navbar.css" rel="stylesheet" />
	</head>
	<body <?php if(isset($bodyTagParameters)) echo $bodyTagParameters; ?>>
		<div class="navbar navbar-fixed-top">
			<div class="navbar-inner">
				<div class="container">
					<a class="brand" href="/">Pastexen</a>
					<ul class="nav">
						<li><a href="/"><?php echo $this->l('menu_index'); ?></a></li>
					</ul>
					<p class="navbar-text pull-right"><?php echo $this->l('we_on_fb_and_vk_linked', '<a href="http://www.facebook.com/groups/310112359099842/">', '</a>', '<a href="http://vk.com/pastexen">', '</a>'); ?>.</p>
				</div>
			</div>
		</div>
		
		<div class="container">