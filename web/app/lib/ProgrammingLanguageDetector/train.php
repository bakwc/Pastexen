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
	
	if($_SERVER['REMOTE_ADDR'] != '127.0.0.1') {
		echo 'This script is accessible only from localhost.';
		exit();
	}
	
	require_once(dirname(__FILE__) . '/ProgrammingLanguageDetector.php');
	
	$detector = new ProgrammingLanguageDetector();
	
	$trainingFiles = glob(dirname(__FILE__) . '/training_data/*/*.*');
	foreach($trainingFiles as $file)
		if(strtolower(basename($file)) != 'license')
			$detector->train(file_get_contents($file), pathinfo($file, PATHINFO_EXTENSION));
	
	file_put_contents(dirname(__FILE__) . '/knowledge_base.dat', $detector->exportKnowledgeBase());
	
	echo 'Done!';