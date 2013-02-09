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
	
	if(!$success)
		require(dirname(__FILE__) . '/user_attach.php');
	
	else {
		$pageTitle = $this->l('account_attach_title');
		require(dirname(__FILE__) . '/includes/header.php');
?>

<h1><?php echo $this->l('account_attach_title'); ?></h1>
<hr />
<div class="alert alert-block alert-success">
	<h4><?php echo $this->l('congratulations'); ?></h4>
	<?php echo $this->l('account_' . ($registered ? 'registered' : 'attached') . '_successfully'); ?>
	<a href="/login.php" class="btn btn-success"><?php echo $this->l('action_continue'); ?> &rarr;</a>
</div>


<?php
		require(dirname(__FILE__) . '/includes/footer.php');
	}
?>