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
			if(!isset($jsIncludes))
				$jsIncludes = array();
			$jsIncludes = array_merge(array(
				'/app/static/javascript/jquery-1.7.1.min.js',
				'/app/static/javascript/bootstrap-transition.js',
				'/app/static/javascript/bootstrap-dropdown.js',
				'/app/static/javascript/chosen.jquery.min.js',
				'/app/static/javascript/app.js',
				), $jsIncludes);
			foreach($jsIncludes as $js)
				echo '<script src="' . $js . '"></script>';

			if(!isset($cssIncludes))
				$cssIncludes = array();
			$cssIncludes = array_merge(array(
				'/app/static/styles/twitter-bootstrap-2.2.2.css',
				'/app/static/styles/harvesthq-chosen-0.9.11.css'
				), $cssIncludes, array(
				'/app/static/styles/app-common.css',
				'/app/static/styles/app-navbar.css'
				));
			foreach($cssIncludes as $css)
				echo '<link href="' . $css . '" rel="stylesheet" />';
		?>
	</head>
	<body <?php if(isset($bodyTagParameters)) echo $bodyTagParameters; ?>>
		<div class="navbar navbar-fixed-top">
			<div class="navbar-inner">
				<div class="container">
					<a class="brand" href="/">Pastexen</a>
					<ul class="nav">
						<li><a href="/"><?php echo $this->l('menu_index'); ?></a></li>
					</ul>
					
					<?php if(isset($_SESSION['authorized_user_id'])) { ?>
						<div class="btn-group pull-right">
							<a class="btn btn-primary" href="/account.php"><i class="icon-user icon-white"></i> <?php echo $_SESSION['authorized_user_login']; ?></a>
							<a class="btn" title="<?php echo $this->l('action_logout'); ?>" href="/logout.php"><i class="icon-off"></i></a>
						</div>
					<?php } else { ?>
						<a class="btn btn-primary pull-right" href="/login.php"><?php echo $this->l('action_login'); ?></a>
					<?php } ?>
				</div>
			</div>
		</div>
		
		<div class="container">