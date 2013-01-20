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

	define('APP_ROOT_DIR', dirname(__FILE__));

	require_once(APP_ROOT_DIR . '/models/Source.php');
	
	$pageUrl = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
	
	if(!isset($_GET['file'])) {
		header('Location: /');
		echo 'Bad request.';
		exit();
	}
	
	try {
		$source = new Source(htmlspecialchars($_GET['file']), APP_ROOT_DIR . '/s');
	}
	catch(Exception $e) {
		header('Location: /');
		echo 'Not found.';
		exit();
	}
	
	if(isset($_GET['download'])) {
		header('Content-type: application/force-download');
		header('Content-Disposition: attachment; filename="' . $source->name . '"');
		header('Content-Length: ' . filesize($source->path));
		readfile($source->path);
		exit();
	}
	
	if(isset($_GET['raw'])) {
		header('Content-Type:text/html; charset=utf-8');
		echo '<pre>' . htmlspecialchars($source->getData()) . '</pre>';
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
		<link href="css/style.min.css" rel="stylesheet" />
		<?php
			if(isset($_GET['light']))
				$codeStylesheet = 'xcode.css';
			else
				$codeStylesheet = 'night.css';
		?>
		<link rel="stylesheet" type="text/css" href="css/<?php echo $codeStylesheet; ?>" />
		<link rel="shortcut icon" href="favicon.png" />
		<!--[if gte IE 9]>
			<style type="text/css">
				.gradient {
					filter: none;
				}
			</style>
		<![endif]-->
		<!--[if lt IE 9]>
			<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
		<![endif]-->
		<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
		<script type="text/javascript" src="//vk.com/js/api/openapi.js?60"></script>
		<script type="text/javascript">VK.init({apiId: 3196338, onlyWidgets: true});</script>
		<link type="text/css" href="css/jquery.jscrollpane.css" rel="stylesheet" media="all" />
	</head>

	<body id="codepage" data-spy="scroll" data-target=".bs-docs-sidebar">
		<div class="navbg">
			<div class="container">
				<div class="code-page-head">
					<div class="pull-left">
						<a href="/"><img src="img/logo_medium_beta.png"></a>
						<ul class="code-page-nav">
							<li><a href="/">Main</a></li>
							<li class="hide"><a href="/">Contact</a></li>
						</ul>
					</div>
					<div class="pull-right">
						<ul class="social">
							<li><a href="http://fb.com/pastexen" class="fb">FB</a></li>
							<li><a href="http://vk.com/pastexen" class="vk">BK</a></li>
						</ul>
					</div>
				</div>
			</div>
		</div>
		
		<div class="container bread hide">
			<ul class="breadcrumb">
				<li><a href="#">Home</a> <span class="divider">/</span></li>
				<li><a href="#">Source</a> <span class="divider">/</span></li>
				<li class="active">Filename</li>
			</ul>
		</div>
		
		<br />
		
		<div class="container">
			<div class="span12 content">
				<div class="pull-left">
					<a href="<?php echo htmlspecialchars($pageUrl); ?>&download" class="btn btn-primary">Download</a>
					<a href="<?php echo htmlspecialchars($pageUrl); ?>&raw" class="btn">Raw</a>
				</div>
				<div class="pull-right share">
					<div id="vk_like" class="vk_like"></div>
					<script type="text/javascript">VK.Widgets.Like("vk_like", {type: "mini"});</script>
					<div class="fb-like" data-href="http://pastexen.com/source/s" data-send="false" data-layout="button_count" data-width="450" data-show-faces="false" data-font="trebuchet ms"></div>
					<g:plusone></g:plusone>
				</div>
				<div class="clearfix"></div>
				
				<br />
				
				<div class="syn"><pre class="blockcode"><code class="<?php echo $source->getType(); ?> lineselect"><?php echo htmlspecialchars($source->getData()); ?></code></pre></div>
			</div>
		</div>

		<div id="fb-root"></div>
		
		<script type="text/javascript" src="js/app.js"></script>
		<script type="text/javascript" src="js/jquery.jscrollpane.min.js"></script>
		<script type="text/javascript" src="js/jquery.mousewheel.js"></script>
		<script type="text/javascript" src="js/highlight.js"></script>
		<script type="text/javascropt" src="js/mwheelIntent.js"></script>
		<script type="text/javascript">
			hljs.tabReplace = '    ';
			hljs.showPages = true;
			hljs.initHighlightingOnLoad();

			var pres = document.getElementsByClassName('blockcode'), pre, code, lines, pl, cl;
			for (pl = pres.length, p = 0; p < pl; p++) {
				pre  = pres[p];
				code = pre.children[0];
				code.innerHTML = code.innerHTML.split("\n").map(function(line) {
					return '<span class="line">' + line + '</span>';
				}).join("\n");
				lines = code.getElementsByClassName('line');
				for (cl = lines.length, l = 0; l < cl; l++) {
					lines[l].style.width = pre.scrollWidth + 'px';
				}
			}
			
			(function() {
			var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
			po.src = 'https://apis.google.com/js/plusone.js';
			var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
			})();
			(function(d, s, id) {
			var js, fjs = d.getElementsByTagName(s)[0];
			if (d.getElementById(id)) return;
			js = d.createElement(s); js.id = id;
			js.src = "//connect.facebook.net/en_US/all.js#xfbml=1&appId=124238757728309";
			fjs.parentNode.insertBefore(js, fjs);
			}(document, 'script', 'facebook-jssdk'));
		</script>
	</body>
</html>