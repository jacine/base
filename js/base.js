/**
 * MBP - Mobile boilerplate helper functions
 */
(function(a){window.MBP=window.MBP||{},MBP.viewportmeta=a.querySelector&&a.querySelector('meta[name="viewport"]'),MBP.ua=navigator.userAgent,MBP.scaleFix=function(){MBP.viewportmeta&&/iPhone|iPad|iPod/.test(MBP.ua)&&!/Opera Mini/.test(MBP.ua)&&(MBP.viewportmeta.content="width=device-width, minimum-scale=1.0, maximum-scale=1.0",a.addEventListener("gesturestart",MBP.gestureStart,!1))},MBP.gestureStart=function(){MBP.viewportmeta.content="width=device-width, minimum-scale=0.25, maximum-scale=1.6"},MBP.BODY_SCROLL_TOP=!1,MBP.getScrollTop=function(){var b=window,c=a;return b.pageYOffset||"CSS1Compat"===c.compatMode&&c.documentElement.scrollTop||c.body.scrollTop||0},MBP.hideUrlBar=function(){var a=window;location.hash||MBP.BODY_SCROLL_TOP===!1||a.scrollTo(0,1===MBP.BODY_SCROLL_TOP?0:1)},MBP.hideUrlBarOnLoad=function(){var c,a=window,b=a.document;!location.hash&&a.addEventListener&&(window.scrollTo(0,1),MBP.BODY_SCROLL_TOP=1,c=setInterval(function(){b.body&&(clearInterval(c),MBP.BODY_SCROLL_TOP=MBP.getScrollTop(),MBP.hideUrlBar())},15),a.addEventListener("load",function(){setTimeout(function(){20>MBP.getScrollTop()&&MBP.hideUrlBar()},0)}))},MBP.autogrow=function(a,b){function c(){var b=this.scrollHeight,c=this.clientHeight;b>c&&(this.style.height=b+3*e+"px")}var d=b?b:12,e=a.currentStyle?a.currentStyle.lineHeight:getComputedStyle(a,null).lineHeight;e=-1==e.indexOf("px")?d:parseInt(e,10),a.style.overflow="hidden",a.addEventListener?a.addEventListener("input",c,!1):a.attachEvent("onpropertychange",c)},MBP.enableActive=function(){a.addEventListener("touchstart",function(){},!1)},MBP.preventScrolling=function(){a.addEventListener("touchmove",function(a){"range"!==a.target.type&&a.preventDefault()},!1)},MBP.preventZoom=function(){for(var b=a.querySelectorAll("input, select, textarea"),c="width=device-width,initial-scale=1,maximum-scale=",d=0,e=b.length,f=function(){MBP.viewportmeta.content=c+"1"},g=function(){MBP.viewportmeta.content=c+"10"};e>d;d++)b[d].onfocus=f,b[d].onblur=g}})(document);


;(function ($, window, document, undefined) {

  $.fn.listToggler = function (options) {
    // Merge options.
    options = $.extend({}, $.fn.listToggler.options, options);

    var button = $('<button />').addClass(options.buttonClass).text(Drupal.t(options.buttonTextClosed));

    // Find any open containers and trigger a click so they go away.
    closeLists = function() {
        $('.'+ options.containerOpenClass +'[data-toggle="true"]').find('.' + options.buttonClass).click();
    };

    // Loop through elements, find trigger and apply toggle.
    return this.each(function() {
      var element = $(this);

      // Add the button right before the toggle content wrapper.
      $(button).insertBefore('.' + options.contentClass, $(element));

      $(element)
        .find('.' + options.buttonClass)
        .toggle(function(e) {
          $(this).attr('aria-pressed', 'true').html(Drupal.t(options.buttonTextOpen)).parent('[data-toggle="true"]').addClass(options.containerOpenClass);
        }, function() {
          $(this).attr('aria-pressed', 'false').html(Drupal.t(options.buttonTextClosed)).parent('[data-toggle="true"]').removeClass(options.containerOpenClass);
      });

      // Close lists when the user clicks outside the list.
      $(document).click(function(event) {
        closeLists();
      });

    });

  };


  // Plugin defaults.
  $.fn.listToggler.options = {
    buttonClass: 'toggle',
    buttonTextOpen: 'Collapse',
    buttonTextClosed: 'Expand',
    contentClass: 'toggle-content',
    containerOpenClass: 'open'
  };

  Drupal.behaviors.baseToggle = {
    attach: function(context, settings) {
      $('[data-toggle="true"]', context).listToggler();
    }
  };

})(jQuery, window, document);
