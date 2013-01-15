<?php

require_once('sources.php');

if(isset($_GET['file'])){
  $source = new Sources(Sources::encode($_GET['file']));
  if(isset($_GET['download'])){
    $source->download();
  }
  if(isset($_GET['raw'])){
    $isRaw = true;
  }else{
    $isRaw = false;
  }
}else{
  header("location: /");
}
if($source){
  $rendered = $source->renderFile();
}
?>
<?  if($isRaw):?>
<pre><?=$source->raw();?>
</pre>
<? else: ?>
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Pastexen</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta name="description" content="">
  <meta name="author" content="">

  <link href="../css/style.min.css" rel="stylesheet">
  <? if(isset($_GET['light'])):?>
      <link rel="stylesheet" type="text/css" href="../css/xcode.css">
    <? else: ?>
      <link rel="stylesheet" type="text/css" href="../css/night.css">
  <? endif;?>
  <link rel="shortcut icon" href="../favicon.png">
  <!--[if gte IE 9]>
  <style type="text/css">
    .gradient {
       filter: none;
    }
  </style>
  <![endif]-->
  <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
  <!--[if lt IE 9]>
  <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
  <![endif]-->
  <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
  <script type="text/javascript" src="//vk.com/js/api/openapi.js?60"></script>
  <script type="text/javascript">VK.init({apiId: 3196338, onlyWidgets: true});</script>
  <link type="text/css" href="css/jquery.jscrollpane.css" rel="stylesheet" media="all" />
  </head>

<body id="codepage" data-spy="scroll" data-target=".bs-docs-sidebar">
<div class="navbg">
  <div class="container">
    <div class="code-page-head">
      <div class="pull-left">
          <a href="/"><img src="../img/logo_medium_beta.png"></a>
          <ul class="code-page-nav">
            <li><a href="/">Main</a></li>
            <li class="hide"><a href="/">Contact</a></li>
          </ul>
      </div>
      <div class="pull-right">
            <ul class="social">
              <li><a href="http://fb.com/pastexen" class="fb">FB</a></li>
              <li><a href="http://vk.com/pastexen" class="vk">BK</a></li>
            </ul>
      </div>
    </div>
  </div>
</div>
  <!-- breadcrumb -->
  <div class="container bread hide">
    <ul class="breadcrumb">
      <li><a href="#">Home</a> <span class="divider">/</span></li>
      <li><a href="#">Source</a> <span class="divider">/</span></li>
      <li class="active">Filename</li>
    </ul>
  </div><br>
   <!-- content -->
   <div class="container">
      <div class="span12 content">
        <!-- <h1>Document name</h1> -->
       
        <? if(!$source->file_not_found){?>
        <div class="pull-left">
          <a href="http://<?=$_SERVER['SERVER_NAME'].Sources::encode($_SERVER['REQUEST_URI'])?>&download" class="btn btn-primary">Download</a> <a href="http://<?=$_SERVER['SERVER_NAME'].Sources::encode($_SERVER['REQUEST_URI'])?>&raw" class="btn">Raw</a>
        </div>
        <div class="pull-right share">
        
          <div id="vk_like" class="vk_like"></div>
          <script type="text/javascript">
          VK.Widgets.Like("vk_like", {type: "mini"});
          </script>
		
          <div class="fb-like" data-href="http://pastexen.com/source/s" data-send="false" data-layout="button_count" data-width="450" data-show-faces="false" data-font="trebuchet ms"></div>
		<g:plusone></g:plusone>
        </div>
        <? } ?>
        <div class="" style="clear:both">

         <? if($source):?> 
        <div class="syn">
          <pre class="blockcode"><code class="<?=$source->syntax;?> lineselect"><?=$rendered?></code></pre>
        </div>
        <!-- <textarea class="raw" rows="40" cols="60"><pre><?=$source->renderFile();?></pre></textarea> -->
        <? endif;?>
        </div>
      </div>
    
    </div>

   </div>
  <script type="text/javascript" src="../js/app.js"></script>
  <div id="fb-root"></div>
  <script>
  (function() {
    var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
    po.src = 'https://apis.google.com/js/plusone.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
  })();
  (function(d, s, id) {
    var js, fjs = d.getElementsByTagName(s)[0];
    if (d.getElementById(id)) return;
    js = d.createElement(s); js.id = id;
    js.src = "//connect.facebook.net/en_US/all.js#xfbml=1&appId=124238757728309";
    fjs.parentNode.insertBefore(js, fjs);
  }(document, 'script', 'facebook-jssdk'));</script>

  <script type="text/javascript" src="../js/jquery.jscrollpane.min.js"></script>
  <script type="text/javascript" src="../js/jquery.mousewheel.js"></script>
  <script type="text/javascript" src="../js/highlight.js"></script>
  <script type="text/javascropt" src="../js/mwheelIntent.js"></script>
  <script type="text/javascript">
    hljs.tabReplace = '    ';
    hljs.showPages = true;
    hljs.initHighlightingOnLoad();
</script>
    <script>
        var pres = document.getElementsByClassName('blockcode'), pre, code, lines, pl, cl;
        for (pl = pres.length, p = 0; p < pl; p++) {
            pre  = pres[p];
            code = pre.children[0];
            code.innerHTML = code.innerHTML.split("\n").map(function(line) {
                return '<span class="line">' + line + '</span>';
            }).join("\n");
            lines = code.getElementsByClassName('line');
            for (cl = lines.length, l = 0; l < cl; l++) {
                lines[l].style.width = pre.scrollWidth + 'px';
            }
        }
    </script>

</body>
</html>
<? endif;?>
