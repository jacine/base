<!DOCTYPE html>
<html<?php print $html_attributes; ?>>
  <head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if lt IE 9]><script src="/<?php print $path; ?>/js/html5.js"></script><![endif]-->
</head>
<body<?php print $body_attributes;?>>
  <a href="#content" class="invisible focusable"><?php print t('Skip to main content'); ?></a>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>