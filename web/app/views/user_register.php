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
	
	$pageTitle = $this->l('account_register_title');
	require(dirname(__FILE__) . '/includes/header.php');
?>

<h1><?php echo $this->l('account_register_title'); ?></h1>
<hr />
<div class="alert alert-info"><?php echo $this->l('account_register_description'); ?></div>
<?php
	if(isset($success) && !$success) {
		echo '<div class="alert alert-block alert-error">';
		echo '<h4>' . $this->l('account_register_errors_occured') . '</h4>';
		if($loginBad)
			echo '<br />' . $this->l('error_login_bad');
		if($passwordWrong)
			echo '<br />' . $this->l('error_password_wrong');
		if($uuidTaken)
			echo '<br />' . $this->l('error_uuid_taken');
		echo '</div>';
	}
?>
<form action="/app/index.php?action=user_register_handler" method="post" class="form-horizontal">
	<input type="hidden" name="uuid" value="<?php echo htmlspecialchars($uuid); ?>" />

	<div class="control-group">
		<label class="control-label" for="login"><?php echo $this->l('field_login'); ?></label>
		<div class="controls">
			<input type="text" class="span6" id="login" name="login" maxlength="25" autocomplete="off" value="<?php echo htmlspecialchars($login); ?>" required />
			<p class="muted"><small><?php echo $this->l('field_login_description'); ?></small></p>
		</div>
	</div>
	
	<div class="control-group">
		<label class="control-label" for="password"><?php echo $this->l('field_password'); ?></label>
		<div class="controls">
			<input type="text" class="span6" id="password" name="password" autocomplete="off" required />
		</div>
	</div>
	
	<div class="form-actions">
		<input type="submit" class="btn btn-primary" value="<?php echo $this->l('action_register'); ?>" />
	</div>
</form>

<?php
	require(dirname(__FILE__) . '/includes/footer.php');
?>