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
	
	$pageTitle = $source->name;
	$jsIncludes = array('/app/static/javascript/google-prettify.js');
	$cssIncludes = array('/app/static/styles/prettify-tomorrow' . ($isDarkColorScheme ? '-night-bright' : '') . '.css');
	$bodyTagParameters = 'onload="prettyPrint()"';
	require(dirname(__FILE__) . '/includes/header.php');
?>

<div id="fb-root"></div>

<div class="btn-group pull-left">
	<a class="btn btn-success" href="<?php echo htmlspecialchars($url); ?>&download"><?php echo $this->l('action_download'); ?></a>
	<a class="btn" href="<?php echo htmlspecialchars($url); ?>&raw"><?php echo $this->l('action_open_raw'); ?></a>
	<?php
		if($isDarkColorScheme)
			echo '<a class="btn" href="' . htmlspecialchars($url) . '&light">' . $this->l('action_switch_to_light') . '</a>';
		else
			echo '<a class="btn" href="' . htmlspecialchars(ApplicationUtils::urlStripParameter($url, 'light')) . '">' . $this->l('action_switch_to_dark') . '</a>';
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

<?php
	require(dirname(__FILE__) . '/includes/footer.php');
?>