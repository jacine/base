<?php
/**
 * Customizations:
 *  - When there are more than one values for a field, output the items
 *    in an unordered list.
 *  - $heading: When view mode is 'full' it's <h2>, otherwise, it's <h3>.
 *  - $classes: was modified to remove some classes in template.php.
 *
 * @see template_preprocess_field()
 * @see base_preprocess_field()
 *
 */
$count = count($items);
// dpm($theme_hook_suggestions);
?>
<?php if (!empty($items)): ?>
  <div<?php print $attributes; ?>>
    <?php if (!$label_hidden): ?>
      <<?php print $heading . $title_attributes; ?>><?php print $label; ?></<?php print $heading; ?>>
    <?php endif; ?>
    <?php if ($count > 1): ?>
      <?php // Use a list for multiple values. ?>
      <ul<?php print $content_attributes; ?>>
        <?php foreach ($items as $delta => $item): ?>
        <li><?php print render($item); ?></li>
        <?php endforeach; ?>
      </ul>
    <?php else: ?>
      <?php // Omit wrapper unless attributes exist for single values. ?>
      <?php if (!empty($content_attributes)): ?><div<?php print $content_attributes; ?>><?php endif; ?>
        <?php print render($items); ?>
      <?php if (!empty($content_attributes)): ?></div><?php endif; ?>
    <?php endif; ?>
  </div>
<?php endif; ?>
