(function ($) {

/**
 * Toggle the collapse data attribute states so they can be styled with CSS and
 * update the screen reader text.
 */
Drupal.toggleContainer = function(container) {
  var $container = $(container);

  if ($container.data('state') === 'closed') {
    $container
      .data('state', 'open').attr('data-state', 'open')
      .find('> .title .element-invisible').text(Drupal.t('Hide')
    );
  }
  else {
    $container
      .data('state', 'closed').attr('data-state', 'closed')
      .find('> .title .element-invisible').text(Drupal.t('Show')
    );
  }
};


Drupal.behaviors.collapse = {
  attach: function (context, settings) {
    $('[data-component="collapse"]', context).each(function() {
      var $container = $(this);

      // Expand container if there are errors inside, or if it contains an
      // element that is targeted by the URI fragment identifier.
      var anchor = location.hash && location.hash != '#' ? ', ' + location.hash : '';
      if ($('.error' + anchor, $container).length) {
        $container
          .data('state', 'open')
          .attr('data-state', 'open');
      }

      var summary = $('<span class="summary"/>');
      $container.
        bind('summaryUpdated', function () {
          var text = $.trim($container.drupalGetSummary());
          summary.text(text ? ' (' + text + ')' : '');
        })
        .trigger('summaryUpdated');

      // Turn the legend into a clickable link
      var $legend = $('> .title', this);

      $('<span class="element-invisible"/>')
        .append($container.data('state') === 'closed' ? Drupal.t('Show') + ' ' : Drupal.t('Hide ') + ' ')
        .prependTo($legend);

      // .wrapInner() does not retain bound events.
      var $link = $('<a href="#"/>')
        .prepend($legend.contents())
        .appendTo($legend)
        .click(function() {
          var container = $container.get(0);
          Drupal.toggleContainer(container);
          return false;
        });

      $legend.append(summary);
    });
  }
};

})(jQuery);
