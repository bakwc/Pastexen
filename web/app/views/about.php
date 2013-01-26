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
		<title>Pastexen.com</title>
		<style type="text/css">
			.terminal {
				background: #1D1F21;
				color: #C5C8C6;
				font-family: monospace;
				font-size: 14px;
				margin: 20px auto;
				padding: 30px 60px;
				width: 450px;
			}
			.terminal a {
				color: #C66;
			}
			.terminal .purple {
				color: #B294BB;
			}
			.terminal .red {
				color: #C66;
			}
			.terminal .green {
				color: #B5BD68;
			}
			.terminal .gray {
				color: #969896;
			}
			.email-at:after {
				content: '@';
			}
		</style>
		<!--[if lt IE 9]>
			<style type="text/css">
				.terminal {
					font-size: 12px;
				}
			</style>
		<![endif]-->
	</head>
	<body>
		<div class="terminal">
			<span class="purple">
				#######################################################<br />
				##_____________________Pastexen.com__________________##<br />
				#######################################################<br />
			</span>
			<span class="gray">
				=======================================================<br />
			</span>
			OpenSource project PASTEXEN<br />
			@ <a href="https://github.com/bakwc/Pastexen">https://github.com/bakwc/Pastexen</a><br />
			
			<br />
			
			&gt; The application allows you to publish your source code and screenshots to the server, and receives a reference to clipboard.<br />

			<br />
			
			<span class="gray">
				=======================================================<br />
			</span>
			<span class="green">
				<!-- NOTE: use span with class email-at instead of a @ symbol to give _minimal_ protection from spam! -->
				fippo <strong>&lt;<a href="https://plus.google.com/117752260157117351376">Filipp Ozinov</a>&gt;</strong> - teamlead, coding, client<br />
				Анастасия <strong>&lt;popovanasta<span class="email-at"></span>gmail.com&gt;</strong> - graphics, design<br />
				Николай <strong>&lt;dakuenjery<span class="email-at"></span>gmail.com&gt;</strong> - coding (server daemon)<br />
				denis <strong>&lt;denisavvakumov<span class="email-at"></span>gmail.com&gt;</strong> - coding, web<br />
				roman <strong>&lt;dutchakdev.com&gt;</strong> - web<br />
				ont <strong>&lt;ont.rif@gmail.com&gt;</strong> - crash test<br />
				powder96 <strong>&lt;raincaptcha.driversworld.us&gt;</strong> - web<br />
			</span>
		</div>
	</body>
</html>