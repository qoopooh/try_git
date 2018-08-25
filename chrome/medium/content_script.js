(function() {
  /*console.log('medium is starting..');*/

  /**
   * Hide element (div)
   */
  function hideId(e) {
    if (e && e.style) {
      e.style.display = "none";
    }
  }

  /**
   * Hide elements in array
   */
  function hideArray(a) {
    for (i in a) {
      hideId(a[i]);
    }
  }

  function hideIdByName(name) {
    var e = document.getElementById(name);
    hideId(e);
  }

  function hideTags(name) {
    var tags = document.getElementsByTagName(name);
    hideArray(tags);
  }

  function hideClass(name) {
    var tags = document.getElementsByClassName(name);
    hideArray(tags);
  }

  // remove medium bars
  hideClass('u-fixed');

  // remove header tag
  hideTags('headers');

  // remove footer tag
  hideTags('footer');

  // remove header id
  hideIdByName('header');

  // remove footer id
  hideIdByName('footer');

  // remove facebook login header
  hideIdByName('pagelet_bluebar');

  // remove facebook login footer
  hideIdByName('u_0_c');

  // remove addthis
  hideClass('addthis-smartlayers');

  // remove header sidebar
  hideIdByName('sidebar');
  hideIdByName('logo');
  hideClass('nav-header');
  hideClass('entry-media');
  hideClass('ads-leaderboard');
  /*hideArray(document.getElementsByClassName('videoAdUi')); */
  /*hideId(document.getElementById('gni-interstitial-bg')); */

  console.log('medium is hidden');
})();
