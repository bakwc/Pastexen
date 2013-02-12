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
	
	$pageTitle = $this->l('file_delete');
	require(dirname(__FILE__) . '/includes/header.php');
?>

<h1><?php echo $this->l('file_delete'); ?></h1>

<hr />

<h2><?php echo $this->l('file_delete_confirmation'); ?></h2>

<div class="well">
	<div class="media">
		<a class="pull-left"><img class="media-object" src="/app/index.php?action=file_thumbnail&file=<?php echo $file->getId(); ?>" /></a>
		<div class="media-body">
			<h4 class="media-heading"><a href="<?php echo $file->getUrl(); ?>"><?php echo htmlspecialchars($file->getName() . '.' . $file->getExtension()); ?></a></h4>
			<p><?php echo htmlspecialchars($file->getDescription()); ?></p>
		</div>
	</div>
</div>

<div class="align-center">
	<div class="btn-group">
		<a class="btn btn-danger btn-large" href="/app/index.php?action=file_delete_handler&file=<?php echo $file->getId(); ?>"><?php echo $this->l('action_delete'); ?></a>
		<a class="btn btn-large" href="/account.php"><?php echo $this->l('action_cancel'); ?></a>
	</div>
</div>

<?php
	require(dirname(__FILE__) . '/includes/footer.php');
?>