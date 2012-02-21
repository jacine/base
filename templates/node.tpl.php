<?php

// We hide the comments and links now so that we can render them later.
hide($content['comments']);
hide($content['links']);

?>
<article<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if ($page): ?>
    <h1<?php print $title_attributes; ?>><?php print $title; ?></h1>
  <?php else: ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" rel="bookmark"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($display_submitted || !empty($content['field_tags'])): ?>
    <footer>
      <time datetime="<?php print $timestamp; ?>">
        <span class="month-year"><?php print format_date($node->created, 'custom', 'M Y'); ?></span>
        <span class="day"><?php print format_date($node->created, 'custom', 'j'); ?></span>
        <span class="time"><?php print format_date($node->created, 'custom', 'g:m a'); ?></span>
      </time>
      <?php print $user_picture; ?>
      <?php print render($content['field_tags']); ?>
    </footer>
  <?php endif; ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>
  <?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>
</article>