<?php
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
		<link href="/app/static/styles/old/style.min.css" rel="stylesheet">
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
							<img src="/app/static/images/logo_beta.png" width="240" height="45">
							<ul class="social">
								<li><a href="http://www.facebook.com/groups/310112359099842/" class="fb">FB</a></li>
								<li><a href="http://vk.com/pastexen" class="vk">BK</a></li>
							</ul>
						</div>
					</div>
					<div class="span12 slogan">
						<span class="frst">А что Вы делаете, когда надо быстро показать</span>
						<br>
						<span class="tw">картинку или часть кода</span>
						<div class="sheme"></div>
					</div>
					<div class="span12 download-section">
						<span class="down-text">Скачать</span>
						<div class="btns row">
							<a href="https://github.com/bakwc/Pastexen/raw/master/builds/pastexen_0.2_win32_installer.exe" class="win">Для Windows</a>
							<span class="vertsper"></span>
							<a href="https://github.com/bakwc/Pastexen/tree/master/builds" class="macos">Other builds</a>
						</div>
						<a href="https://github.com/bakwc/Pastexen/" class="src">Sources (github)</a> <a href="Oo.php" class="src">Our team</a>
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
							<img src="/app/static/images/features-screenshot.png" width="140" height="115">
						</div>
						<div class="span4 descr">
							<span class="btext">Делитесь скриншотами одним <br/>нажатием клавиши</span>
							<br/>
							<span class="ltext">Все снятые изображения сохраняются на сервере и создается ссылка. Остается лишь нажать Ctrl+V</span>
						</div>
					</div>
					<div class="block">
						<div class="span2 pict" style="float:left">
							<img src="/app/static/images/features-code.png" width="140" height="115">
						</div>
						<div class="span4 descr">
							<span style="font-size:16px;font-weight: bold;">Копируйте код и отправляйте его <br/>уже с подсветкой синтаксиса</span>
							<br/>
							<span style="font-size:14px;">Весь загруженный код сохраняется на сервере и создается ссылка. Остается лишь нажать Ctrl+V</span>
						</div>
					</div>
				</div>
			</div>
		</div>
	</body>
</html>