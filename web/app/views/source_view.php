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
		<title>Pastexen - <?php echo $source->name; ?></title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<meta name="description" content="" />
		<meta name="author" content="" />
		<link rel="shortcut icon" href="/app/static/images/favicon.png" />
		<!--[if lt IE 9]>
			<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
		<![endif]-->
		<script src="/app/static/javascript/google-prettify.js"></script>
		<link href="/app/static/styles/twitter-bootstrap-2.2.2.css" rel="stylesheet" />
		<link href="/app/static/styles/prettify-tomorrow<?php echo $isDarkColorScheme ? '-night-bright' : ''; ?>.css" rel="stylesheet" />
		<link href="/app/static/styles/app-common.css" rel="stylesheet" />
		<link href="/app/static/styles/app-navbar.css" rel="stylesheet" />
	</head>
	<body onload="prettyPrint()">
		<div id="fb-root"></div>
		
		<div class="navbar navbar-fixed-top">
			<div class="navbar-inner">
				<div class="container">
					<a class="brand" href="/">Pastexen</a>
					<ul class="nav">
						<li><a href="/">Home</a></li>
					</ul>
					<p class="navbar-text pull-right">Pastexen on <a href="http://www.facebook.com/groups/310112359099842/">Facebook</a> and <a href="http://vk.com/pastexen">Vkontakte</a>.</p>
				</div>
			</div>
		</div>
		
		<div class="container">
			<div class="btn-group pull-left">
				<a class="btn btn-success" href="<?php echo htmlspecialchars($url); ?>&download">Download</a>
				<a class="btn" href="<?php echo htmlspecialchars($url); ?>&raw">Raw</a>
				<?php
					if($isDarkColorScheme)
						echo '<a class="btn" href="' . htmlspecialchars($url) . '&light">Light mode</a>';
					else
						echo '<a class="btn" href="' . htmlspecialchars(ApplicationUtils::urlStripParameter($url, 'light')) . '">Dark mode</a>';
				?>
			</div>
			<div class="pull-right social">
				<div id="vk_like" class="vkontakte-like-button"></div>
				<script type="text/javascript" src="//vk.com/js/api/openapi.js?60"></script>
				<script type="text/javascript">
					VK.init({apiId: 3196338, onlyWidgets: true});
					VK.Widgets.Like("vk_like", {type: "mini"});
				</script>
				
				<div class="fb-like" data-href="<?php echo htmlspecialchars($url); ?>" data-send="false" data-layout="button_count" data-width="450" data-show-faces="false" data-font="trebuchet ms"></div>
				<script type="text/javascript">
					(function(d, s, id) {
					var js, fjs = d.getElementsByTagName(s)[0];
					if (d.getElementById(id)) return;
					js = d.createElement(s); js.id = id;
					js.src = "//connect.facebook.net/en_US/all.js#xfbml=1&appId=124238757728309";
					fjs.parentNode.insertBefore(js, fjs);
					}(document, 'script', 'facebook-jssdk'));
				</script>
				
				<g:plusone></g:plusone>
				<script type="text/javascript">
					(function() {
					var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
					po.src = 'https://apis.google.com/js/plusone.js';
					var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
					})();
				</script>
				
				<a href="https://twitter.com/share" class="twitter-share-button" data-via="pastexen" data-lang="en">Tweet</a>
				<script type="text/javascript">
					!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");
				</script>
			</div>
			<div class="clearfix"></div>
			
			<hr />
		
			<pre class="prettyprint <?php echo $isDarkColorScheme ? 'dark' : 'light'; ?> linenums lang-<?php echo $source->getType(); ?>"><?php echo htmlspecialchars($source->getData()); ?></pre>
		</div>
	</body>
</html>