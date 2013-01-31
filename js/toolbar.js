/*
 * jQuery pageSlide
 * Version 2.0
 * http://srobbin.com/jquery-pageslide/
 *
 * jQuery Javascript plugin which slides a webpage over to reveal an additional interaction pane.
 *
 * Copyright (c) 2011 Scott Robbin (srobbin.com)
 * Dual licensed under the MIT and GPL licenses.
*/
(function(a){function f(b,d){if(0===b.indexOf("#"))a(b).clone(!0).appendTo(c.empty()).show();else{if(d){var e=a("<iframe />").attr({src:b,frameborder:0,hspace:0}).css({width:"100%",height:"100%"});c.html(e)}else c.load(b);c.data("localEl",!1)}}function g(e,f){var g=c.outerWidth(!0),h={},i={};if(!c.is(":visible")&&!d){switch(d=!0,e){case"left":c.css({left:"auto",right:"-"+g+"px"}),h["margin-left"]="-="+g,i.right="+="+g;break;default:c.css({left:"-"+g+"px",right:"auto"}),h["margin-left"]="+="+g,i.left="+="+g}b.animate(h,f),c.show().animate(i,f,function(){d=!1,a("body").addClass("pageslide-open")})}}var e,b=a("body"),c=a("#pageslide"),d=!1;0==c.length&&(c=a("<div />").attr("id","pageslide").css("display","none").appendTo(a("body"))),a.fn.pageslide=function(b){var d=this;d.click(function(d){var f=a(this),g=a.extend({href:f.attr("href")},b);d.preventDefault(),d.stopPropagation(),c.is(":visible")&&f[0]==e?a.pageslide.close():(a.pageslide(g),e=f[0])})},a.fn.pageslide.defaults={speed:200,direction:"right",modal:!1,iframe:!0,href:null},a.pageslide=function(b){var d=a.extend({},a.fn.pageslide.defaults,b);c.is(":visible")&&c.data("direction")!=d.direction?a.pageslide.close(function(){f(d.href,d.iframe),g(d.direction,d.speed)}):(f(d.href,d.iframe),c.is(":hidden")&&g(d.direction,d.speed)),c.data(d)},a.pageslide.close=function(c){var e=a("#pageslide"),f=e.outerWidth(!0),g=e.data("speed"),h={},i={};if(!e.is(":hidden")&&!d){switch(d=!0,e.data("direction")){case"left":h["margin-left"]="+="+f,i.right="-="+f;break;default:h["margin-left"]="-="+f,i.left="-="+f}e.animate(i,g),b.animate(h,g,function(){e.hide(),d=!1,a("body").removeClass("pageslide-open"),c!==void 0&&c()})}},c.click(function(a){a.stopPropagation()}),a(document).bind("click keyup",function(b){("keyup"!=b.type||27==b.keyCode)&&c.is(":visible")&&!c.data("modal")&&a.pageslide.close()})})(jQuery);

(function ($) {
  $('#toolbarToggle').pageslide({ href: "#toolbar", direction: "right" });
})(jQuery);
