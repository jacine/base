<?php
/**
 * @file
 * Output for main HTML page content.
 */

$header = render($page['header']);
$navigation = render($page['navigation']);
$highlighted = render($page['highlighted']);
$help = render($page['help']);
$content = render($page['content']);
$sidebar_first = render($page['sidebar_first']);
$sidebar_second = render($page['sidebar_second']);
$footer = render($page['footer']);
$tabs = render($tabs);
$actions = count($action_links);
$action_links = render($action_links);

?>
<?php if ($messages): ?>
  <section class="console">
    <h2 class="invisible"><?php print t('Console'); ?></h2>
    <?php print $messages; ?>
  </section>
<?php endif; ?>
<header role="banner" id="header">
  <div class="container">
    <hgroup class="branding">
      <?php if ($site_name): ?>
        <h1 class="site-name">
          <?php if ($logo): ?>
            <a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>" class="logo"><img src="<?php print $logo ?>" alt="<?php print $site_name ?> Logo"></a>
          <?php else: ?>
          <a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>"><?php print $site_name; ?></a>
          <?php endif; ?>
        </h1>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <h2 class="site-slogan"><?php print $site_slogan; ?></h2>
      <?php endif; ?>
    </hgroup>
    <?php print $header; ?>
  </div>
</header>
<?php if ($navigation): ?>
  <nav role="navigation" class="main-menu">
    <div class="container">
      <h2 class="invisible"><?php print t('Main menu'); ?></h2>
      <?php print $navigation; ?>
    </div>
  </nav>
<?php endif; ?>
<?php if ($highlighted): ?>
  <div class="highlighted">
    <div class="container">
      <?php print $highlighted; ?>
    </div>
  </div>
<?php endif; ?>
<div class="main" id="main">
  <div class="container">
    <div role="main" class="column-main">
      <div class="wrap">
        <?php print $breadcrumb; ?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="title" id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs || $action_links): ?>
          <div class="tasks">
          <?php print $tabs; ?>
          <?php if ($action_links): ?>
            <div class="actions actions-<?php print ($actions == 1) ? 'single' : 'multiple'; ?>">
              <div class="wrap">
                <h2><?php print t('Page actions'); ?></h2>
                <ul class="action-links">
                  <?php print $action_links; ?>
                </ul>
              </div>
            </div>
          <?php endif; ?>
          </div>
        <?php endif; ?>
        <?php print $help; ?>
        <div id="content">
          <?php print $content; ?>
        </div>
        <?php print $feed_icons; ?>
      </div>
    </div>
    <?php if ($sidebar_first): ?>
      <div class="column-first sidebar">
        <div class="wrap">
          <?php print $sidebar_first; ?>
        </div>
      </div>
    <?php endif; ?>
    <?php if ($sidebar_second): ?>
      <div class="column-second sidebar">
        <div class="wrap">
          <?php print $sidebar_second; ?>
        </div>
      </div>
    <?php endif; ?>
  </div>
</div>
<footer role="contentinfo" id="footer">
  <div class="container">
    <div class="wrap">
      <?php print $footer; ?>
    </div>
  </div>
</footer>