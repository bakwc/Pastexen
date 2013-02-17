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
	
	$pageTitle = $this->l('file_edit');
	require(dirname(__FILE__) . '/includes/header.php');
?>

<h1><?php echo $this->l('file_edit'); ?></h1>

<hr />

<?php
	if(isset($success) && !$success) {
		echo '<div class="alert alert-block alert-error">';
		echo '<h4>' . $this->l('file_edit_errors_occured') . '</h4>';
		if($filenameBad)
			echo '<br />' . $this->l('error_filename_bad');
		if($extensionBad)
			echo '<br />' . $this->l('error_extension_bad');
		if($descriptionBad)
			echo '<br />' . $this->l('error_description_bad');
		echo '</div>';
	}
?>

<form action="/app/index.php?action=file_edit_handler" method="post" class="form-horizontal">
	<input type="hidden" name="file" value="<?php echo $file->getId(); ?>" />
	<div class="well">
		<div class="media">
			<a class="pull-left"><img class="media-object" src="<?php echo $file->getThumbnailUrl(); ?>" /></a>
			<div class="media-body">
				<div class="input-prepend pull-left">
					<span class="add-on"><?php echo $this->l('field_filename'); ?></span>
					<input type="text" class="span4" name="name" value="<?php
						if(isset($filename) && !empty($filename))
							echo htmlspecialchars($filename);
						else
							echo htmlspecialchars($file->getName());
					?>" required />
				</div>
				<div class="pull-left">
					<?php if($file->getType() == ApplicationModel_File::TYPE_IMAGE)
						echo '.' . $file->getExtension();
						else { ?>
						<select data-placeholder="<?php echo $this->l('field_extension'); ?>" name="extension" class="span2" tabindex="2">
							<?php
								foreach($allowedExtensions as $allowedExtension => $allowedLanguage) {
									$optionName = $allowedExtension . ' (' . (strtoupper($allowedLanguage{0}) . substr($allowedLanguage, 1)) . ')';
									if($allowedExtension == $file->getExtension())
										echo '<option value="' . $allowedExtension . '" selected>' . $optionName . '</option>';
									else
										echo '<option value="' . $allowedExtension . '">' . $optionName . '</option>';
								}
							?>
						</select>
					<?php	} ?>
				</div>
				<div class="clearfix"></div>
				
				<br />
				
				<p><?php echo $this->l('field_description'); ?>:</p>
				<textarea class="input-xxlarge" name="description" rows="6" required><?php
					if(isset($description) && !empty($description))
						echo htmlspecialchars($description);
					else
						echo htmlspecialchars($file->getDescription());
				?></textarea>
			</div>
		</div>
	</div>

	<div class="align-center">
		<div class="btn-group">
			<input class="btn btn-primary btn-large" type="submit" value="<?php echo $this->l('action_save'); ?>" />
			<a class="btn btn-large" href="/account.php"><?php echo $this->l('action_cancel'); ?></a>
		</div>
	</div>
</div>

<?php
	require(dirname(__FILE__) . '/includes/footer.php');
?>