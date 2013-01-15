<?php
$count = file_get_contents('tmp/log.txt');
if((int)$count != 0){
	file_put_contents('tmp/log.txt', $count+1);
}else{
	file_put_contents('tmp/log.txt', 1);
}
?>
<html>
<head>
	<meta charset="utf-8">
	<title>Yo!)</title>
	<style type="text/css">
		pre{
			background: #1D1F21;
			color: #C5C8C6;
			padding: 30px 0;
		}
		.purp{
			color: #B294BB;
		}
		.red{
			color:#C66;
		}
		.green{
			color: #B5BD68;
		}
		.gray{
			color: #969896;
		}

	</style>
</head>
<body>
<div style="width:100%">
<div style="margin:40px auto; width:580px">
	<pre>
	<span class="purp">
	#######################################################
	##_____________________Pastexen.com__________________##
	#######################################################</span>
	<span class="gray">=======================================================</span>
	OpenSource project PASTEXEN 
	@ <a class="red" href="https://github.com/bakwc/Pastexen">https://github.com/bakwc/Pastexen</a>

	> The application allows you to publish your source code 
	and screenshots to the server, and receives a reference 
	to clipboard.

	<span class="gray">=======================================================</span>
	<span class="green">fippo <b><<a class="red" href="https://plus.google.com/117752260157117351376">Filipp Ozinov</a>></b> - teamlead, coding, client</span>
	<span class="green">Анастасия <b>&#x3C;popovanasta&#x40;gmail.com&#x3E;</b> - graphics, design</span>
	<span class="green">Николай <b>&#x3C;dakuenjery&#x40;gmail.com&#x3E;</b> - coding (server daemon)</span>
	<span class="green">denis <b>&#x3C;denisavvakumov&#x40;gmail.com&#x3E;</b> - coding, web</span>
	<span class="green">roman <b>dutchakdev.com</b> - web</span>
	<span class="green">ont <b>&#x3C;ont&#x2E;rif&#x40;gmail&#x2E;com&#x3E;</b> - crash test</span>
	</pre>
</div>
</div>
</body>
</html>
