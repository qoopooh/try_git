(function() {

  // remove medium bars
  var f = document.getElementsByClassName("u-fixed");
  for (i in f) {
    if (f[i].style) {
      f[i].style.display = "none";
    }
  }

  // remove header tag
  var h = document.getElementsByTagName('header');
  for (i in h) {
    if (h[i].style) {
      h[i].style.display = "none";
    }
  }

  console.log('hidden');
})();
