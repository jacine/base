<?php
/**
 * @file
 * Output for Maintenance Page.
 */

?>
<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
  <head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if lt IE 9]>
    <script src="<?php print file_create_url($path . '/js/libraries/html5shiv/dist/html5shiv.js'); ?>"></script>
  <![endif]-->
</head>
<body class="<?php print $classes;?>">
  <?php if ($messages): ?>
    <section class="console">
      <h2 class="visuallyhidden"><?php print t('Console'); ?></h2>
      <?php print $messages; ?>
    </section>
  <?php endif; ?>
  <header role="banner">
    <div class="container">
      <?php $logo = NULL; ?>
      <?php if ($logo): ?>
        <img class="logo" src="<?php print $logo; ?>" alt="<?php print $site_name; ?> Logo">
      <?php elseif ($site_name): ?>
        <h1 class="site-name"><?php print $site_name; ?></h1>
      <?php endif; ?>
    </div>
  </header>
  <div role="main">
    <div class="container">
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <div id="message">
        <?php print $message; ?>
      </div>
    </div>
  </div>
  <footer role="contentinfo">
    <div class="container">
      <?php print $footer; ?>
    </div>
  </footer>
</body>
</html>
