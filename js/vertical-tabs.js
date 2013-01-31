(function ($) {

"use strict";

/**
 * This script transforms a set of fieldset into a stack of vertical
 * tabs. Another tab pane can be selected by clicking on the respective
 * tab.
 *
 * Each tab may have a summary which can be updated by another
 * script. For that to work, each fieldset element has an associated
 * 'verticalTabCallback' (with jQuery.data() attached to the fieldset),
 * which is called every time the user performs an update to a form
 * element inside the tab pane.
 */
Drupal.behaviors.verticalTabs = {
  attach: function (context) {

    // if (!Drupal.checkWidthBreakpoint()) {
    //   return;
    // }

    $(context).find('.vertical-tabs-panes').once('vertical-tabs', function () {
      var $this = $(this);
      var focusID = $this.find(':hidden.vertical-tabs-active-tab').val();
      var tab_focus;

      // Check if there are some fieldset that can be converted to vertical-tabs
      var $fieldset = $this.find('> fieldset');
      if ($fieldset.length === 0) {
        return;
      }

      // Create the tab column.
      var tab_list = $('<ul class="vertical-tabs-list"></ul>');
      $this.wrap('<div class="vertical-tabs"></div>').before(tab_list);

      // Transform each fieldset into a tab.
      $fieldset.each(function () {
        var $this = $(this);
        var vertical_tab = new Drupal.verticalTab({
          title: $this.find('> legend').text(),
          fieldset: $this
        });
        tab_list.append(vertical_tab.item);
        $this
          .attr({
            'data-component': 'vertical-tabs',
            'data-state': 'open'
          })
          .addClass('vertical-tabs-pane')
          .data('verticalTab', vertical_tab);
        if (this.id === focusID) {
          tab_focus = $this;
        }
      });

      $(tab_list).find('> li:first').addClass('first');
      $(tab_list).find('> li:last').addClass('last');

      if (!tab_focus) {
        // If the current URL has a fragment and one of the tabs contains an
        // element that matches the URL fragment, activate that tab.
        var $locationHash = $this.find(window.location.hash);
        if (window.location.hash && $locationHash.length) {
          tab_focus = $locationHash.closest('.vertical-tabs-pane');
        }
        else {
          tab_focus = $this.find('> .vertical-tabs-pane:first');
        }
      }
      if (tab_focus.length) {
        tab_focus.data('verticalTab').focus();
      }
    });
  }
};

/**
 * The vertical tab object represents a single tab within a tab group.
 *
 * @param settings
 *   An object with the following keys:
 *   - title: The name of the tab.
 *   - fieldset: The jQuery object of the fieldset element that is the tab pane.
 */
Drupal.verticalTab = function (settings) {
  var self = this;
  $.extend(this, settings, Drupal.theme('verticalTab', settings));

  this.link.click(function (e) {
    e.preventDefault();
    self.focus();
  });

  // Keyboard events added:
  // Pressing the Enter key will open the tab pane.
  this.link.keydown(function(event) {
    event.preventDefault();
    if (event.keyCode === 13) {
      self.focus();
      // Set focus on the first input field of the visible fieldset/tab pane.
      $('.vertical-tabs-pane :input:visible:enabled:first').focus();
    }
  });

  this.fieldset
    .bind('summaryUpdated', function () {
      self.updateSummary();
    })
    .trigger('summaryUpdated');
};

Drupal.verticalTab.prototype = {
  /**
   * Displays the tab's content pane.
   */
  focus: function () {
    this.fieldset
      .siblings('.vertical-tabs-pane')
        .each(function () {
          var tab = $(this).data('verticalTab');
          tab.fieldset.attr('data-state', 'closed');
          tab.item.removeClass('selected');
        })
        .end()
      .attr('data-state', 'open')
      .siblings(':hidden.vertical-tabs-active-tab')
        .val(this.fieldset.attr('id'));
    this.item.addClass('selected');
    // Mark the active tab for screen readers.
    $('#active-vertical-tab').remove();
    this.link.append('<span id="active-vertical-tab" class="element-invisible">' + Drupal.t('(active tab)') + '</span>');
  },

  /**
   * Updates the tab's summary.
   */
  updateSummary: function () {
    this.summary.html(this.fieldset.drupalGetSummary());
  },

  /**
   * Shows a vertical tab pane.
   */
  tabShow: function () {
    // Display the tab.
    this.item.attr('data-state', 'open');
    // Update .first marker for items. We need recurse from parent to retain the
    // actual DOM element order as jQuery implements sortOrder, but not as public
    // method.
    this.item.parent().children('.vertical-tab-button').removeClass('first')
      .filter(':visible:first').addClass('first');
    // Display the fieldset element.
    this.fieldset.removeClass('vertical-tab-hidden').attr('data-state', 'open');
    // Focus this tab.
    this.focus();
    return this;
  },

  /**
   * Hides a vertical tab pane.
   */
  tabHide: function () {
    // Hide this tab.
    this.item.attr('data-state', 'closed');
    // Update .first marker for items. We need recurse from parent to retain the
    // actual DOM element order as jQuery implements sortOrder, but not as public
    // method.
    this.item.parent().children('.vertical-tab-button').removeClass('first')
      .filter(':visible:first').addClass('first');
    // Hide the fieldset element.
    this.fieldset.addClass('vertical-tab-hidden').attr('data-state', 'closed');
    // Focus the first visible tab (if there is one).
    var $firstTab = this.fieldset.siblings('.vertical-tabs-pane:not(.vertical-tab-hidden):first');
    if ($firstTab.length) {
      $firstTab.data('verticalTab').focus();
    }
    return this;
  }
};

/**
 * Theme function for a vertical tab.
 *
 * @param settings
 *   An object with the following keys:
 *   - title: The name of the tab.
 * @return
 *   This function has to return an object with at least these keys:
 *   - item: The root tab jQuery element
 *   - link: The anchor tag that acts as the clickable area of the tab
 *       (jQuery version)
 *   - summary: The jQuery element that contains the tab summary
 */
Drupal.theme.verticalTab = function (settings) {
  var tab = {};
  tab.item = $('<li class="vertical-tab-button" tabindex="-1"></li>')
    .append(tab.link = $('<a href="#"></a>')
      .append(tab.title = $('<strong></strong>').text(settings.title))
      .append(tab.summary = $('<span class="summary"></span>')
    )
  );
  return tab;
};

})(jQuery);
