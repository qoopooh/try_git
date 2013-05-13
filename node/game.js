var myCanvasWidth = 400;
var myCanvasHeight = 300;
var myCanvasContext = [];

var GameObject = function(width, height) {
  this.x = Math.floor((Math.random() * myCanvasWidth) + 1);
  this.y = Math.floor((Math.random() * myCanvasHeight) + 1);
  this.width = width;
  this.height = height;
  return this;
};

GameObject.prototype = {
  x: 0,
  y: 0,
  width: 5,
  height: 5,
  draw: function() {
   myCanvasContext.fillRect(this.x, this.y, this.width, this.height);
  }
};

var x = 100;
arrayOfGameObject = [];
do {
  arrayOfGameObject.push(new GameObject(10, 10));
} while(x--);

var GameLoop = function() {
  for (gameObject in arrayOfGameObject) {
    gameObject.draw();
  }
};


