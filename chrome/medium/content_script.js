(function() {

  // remove medium bars
  var mediumbars = document.getElementsByClassName("u-fixed");
  for (i in mediumbars) {
    if (mediumbars[i].style) {
      mediumbars[i].style.display = "none";
    }
  }

  // remove header tag
  var headers = document.getElementsByTagName('header');
  for (i in headers) {
    if (headers[i].style) {
      headers[i].style.display = "none";
    }
  }

  // remove footer tag
  var footers = document.getElementsByTagName('footer');
  for (i in footers) {
    if (footers[i].style) {
      footers[i].style.display = "none";
    }
  }

  // remove header id
  var header = document.getElementById('header');
  if (header && header.style) {
    header.style.display = "none";
  }

  // remove footer id
  var footer = document.getElementById('footer');
  if (footer && footer.style) {
    footer.style.display = "none";
  }

  console.log('medium is hidden');
})();
