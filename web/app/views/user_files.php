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
	
	$pageTitle = $this->l('my_files');
	require(dirname(__FILE__) . '/includes/header.php');
?>

<h1><?php echo $this->l('my_files'); ?></h1>

<hr />

<?php foreach($files as $file) { ?>
	<div class="media">
		<a class="pull-left" href="<?php echo $file->getUrl(); ?>"><img class="media-object" src="/app/index.php?action=file_thumbnail&file=<?php echo $file->getSystemName(); ?>" /></a>
		<div class="media-body">
			<div>
				<div class="pull-left"><h4 class="media-heading"><a href="<?php echo $file->getUrl(); ?>"><?php echo htmlspecialchars($file->getName() . '.' . $file->getExtension()); ?></a></h4></div>
				<div class="btn-group pull-left">
					<a class="btn btn-mini btn-info" href="/app/index.php?action=file_rename&file=<?php echo urlencode($file->getSystemName()); ?>"><?php echo $this->l('action_rename'); ?></a>
					<a class="btn btn-mini btn-danger" href="/app/index.php?action=file_delete&file=<?php echo urlencode($file->getSystemName()); ?>"><?php echo $this->l('action_delete'); ?></a>
				</div>
				<div class="clearfix"></div>
			</div>
			<p class="muted"><small><?php echo $this->date($file->getTime()); ?></small></p>
			<p><?php echo htmlspecialchars($file->getDescription()); ?></p>
		</div>
	</div>
<?php } ?>

<?php
	require(dirname(__FILE__) . '/includes/footer.php');
?>