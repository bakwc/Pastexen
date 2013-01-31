<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013  denis <denisavvakumov@gmail.com>
	 * Copyright (C) 2013  roman <dutchakdev.com>
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
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta name="description" content="">
		<meta name="author" content="">
		<link href="/app/static/styles/twitter-bootstrap-2.2.2.css" rel="stylesheet" />
		<link href="/app/static/styles/old/style.css" rel="stylesheet">
		<link rel="shortcut icon" href="/app/static/images/favicon.png">
		<!--[if gte IE 9]><style type="text/css"> .gradient{filter: none;}</style><![endif]-->
		<!--[if lt IE 9]><script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script><![endif]-->
	</head>
	<body data-spy="scroll" data-target=".bs-docs-sidebar">
		<div class="hbg">
			<div class="container intro">
				<div class="row">
					<div class="span12 head">
						<div class="logo">
							<img src="/app/static/images/logo_beta.png" width="240" height="45"  alt="Pastexen beta">
							<ul class="social">
								<li><a href="http://www.facebook.com/groups/310112359099842/" class="fb">FB</a></li>
								<li><a href="http://vk.com/pastexen" class="vk">BK</a></li>
							</ul>
						</div>
					</div>
					<div class="span12 slogan">
						<span class="frst"><?php echo $this->l('slogan_line1'); ?></span>
						<br>
						<span class="tw"><?php echo $this->l('slogan_line2'); ?></span>
						<div class="sheme"></div>
					</div>
					<div class="span12 download-section">
						<span class="down-text"><?php echo $this->l('action_download'); ?></span>
						<br />
						<div class="btn-group">
							<a class="btn btn-large btn-warning" href="<?php echo $this->application->config['download_link_windows']; ?>"><?php echo $this->l('download_windows'); ?></a>
							<a class="btn btn-large" href="<?php echo $this->application->config['download_link_other']; ?>"><?php echo $this->l('download_other'); ?></a>
						</div>
						<br />
						<a href="<?php echo $this->application->config['download_link_source']; ?>" class="download-section-sub"><?php echo $this->l('download_source'); ?></a>
						&nbsp;&nbsp;&nbsp;
						<a href="Oo.php" class="download-section-sub"><?php echo $this->l('our_team'); ?></a>
						<br/>
						<br/>
						<br/>
					</div>
				</div>
			</div>
		</div>
		<div class="str"></div>
		<div class="container">
			<div class="ping"></div>
		</div>
		<div class="container about">
			<div class="row">
				<div class="span12">
					<div class="block">
						<div class="span2 pict" style="float:left">
							<img src="/app/static/images/features-screenshot.png" width="140" height="115"  alt="Features">
						</div>
						<div class="span4 descr">
							<span class="btext"><?php echo $this->l('feature_screenshot_header'); ?></span>
							<br/>
							<span class="ltext"><?php echo $this->l('feature_screenshot_text'); ?></span>
						</div>
					</div>
					<div class="block">
						<div class="span2 pict" style="float:left">
							<img src="/app/static/images/features-code.png" width="140" height="115"  alt="Code">
						</div>
						<div class="span4 descr">
							<span class="btext"><?php echo $this->l('feature_code_header'); ?></span>
							<br/>
							<span class="ltext"><?php echo $this->l('feature_code_text'); ?></span>
						</div>
					</div>
				</div>
			</div>
		</div>
	</body>
</html>
