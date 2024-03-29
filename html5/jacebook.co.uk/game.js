//Variables to handle game parameters
var gameloopId;
var speed=3;
var horizontalSpeed = speed;
var verticalSpeed = -speed;	//Start heading up (to give player a change to get the mouse in position)
var screenWidth;
var screenHeight
var gameRunning = false;
var ctx;
var showBounds = false;
var frameCount = 0;
var frameCountId;
var score=0;


//Create images
var mushroomImg = new Image();
var mushroomWobbleImg = new Image();
var backgroundForestImg = new Image();
var bearEyesOpenImg = new Image();
var bearEyesClosedImg = new Image();
var flowerImg = new Image();
var leafImg = new Image();
var acornImg = new Image();
var scoreImg = new Image();
var bonusImg = new Image();
var owl1 = new Image();
var owl2 = new Image();
var multiPoint = 0;
var currentBonusImage = 0;
var delayCount=0;
//var showBonus = false;
var bonusStarted;
var bonusActive = false;


//Baddy
var BADDY_OFF = 0;
var BADDY_FLYING = 1;
var BADDY_HIT = 3;
var baddyX = 480;
var baddyStatus = BADDY_OFF;
var currentBaddyImage = 0;
var baddyImages = new Array();

//Lives Object
var livesImages = new Array();

//bonus images
var bonusImages = new Array();


var soundType = "wav";

//change sounds type based on browser (i.e. chrome)
if(navigator.userAgent.toLowerCase().indexOf('chrome') > -1)
{
  soundType = "mp3";
}

//Create and load sounds 
var boing1 = new Audio("./sounds/boing_1." + soundType);
var boing2 = new Audio("./sounds/boing_2." + soundType);
var boing3 = new Audio("./sounds/boing_3." + soundType);
var boing4 = new Audio("./sounds/boing_4." + soundType);
var boing5 = new Audio("./sounds/boing_5." + soundType);
var awwwww = new Audio("./sounds/crowdgroan." + soundType);
var collisionPrize = new Audio("./sounds/collisionPrize." + soundType);
var collisionStarPrize = new Audio("./sounds/collisionStarPrize." + soundType);
var groan = new Audio("./sounds/groan." + soundType);
var newLevel = new Audio("./sounds/newLevel." + soundType);
var owl = new Audio("./sounds/owl." + soundType);
var squawk = new Audio("./sounds/squawk." + soundType);

//set background sound of birds, loop and reduce volume
var birds = new Audio("./sounds/birds." + soundType);
birds.loop = true;
birds.volume = 0.1;

//base definition of a game object
function GameObject()
{
  this.x = 0;
  this.y = 0;
  this.image = null;
}

//Extend GameObject for Mushroom
function Mushroom() {};
Mushroom.prototype = new GameObject();
Mushroom.prototype.boing = false;
Mushroom.prototype.wobble = false;
Mushroom.prototype.startBoing = null;
Mushroom.prototype.lastBoing = null;
Mushroom.prototype.squashedImg = null;

var mushroom = new Mushroom();

//Declare and extend GameObject for Animal
function Animal() {};
Animal.prototype = new GameObject();
Animal.prototype.angle = 0;
Animal.prototype.eyesOpenImg = null;

var animal = new Animal();

//Declare and extend GameObject for prize
var prizes = new Array();	
function Prize() {};
Prize.prototype = new GameObject();
Prize.prototype.row = 0;
Prize.prototype.col = 0;
Prize.prototype.angle = 0;
Prize.spinning = false;
Prize.hit = false;

//Delcare and extend GameOject for baddy
function Baddy() {};
Baddy.prototype = new GameObject();
Baddy.prototype.angle = 0;
Baddy.prototype.altImg = null;

var baddy = new Baddy();
baddy.y = 125;
baddy.x = 480;

//Wait for DOM to load and init game
$(window).ready(function(){ 
  init(); 
});

function init(){
  
  loadImages();
  initSettings();
  addEventHandlers();
  startGame();
  startFPSCounter();
  
}  

function startGame()
{
  score=0;
  lives=3;
  startLevel(1);
}

function startLevel(level)
{
  initPrizes();
}


function startFPSCounter()
{
  var start = new Date().getTime(),
    time = 0;
  function instance()
  {
    time += 1000;
    fps();
    
    var diff = (new Date().getTime() - start) - time;
    window.setTimeout(instance, (1000 - diff));
  }
  window.setTimeout(instance, 1000);

}

//Using jQuery add the event handlers after the DOM is loaded
function addEventHandlers()
{
  
  //add event handler to surrounding DIV to monitor mouse move and update mushroom's x position
  $("#container").mousemove(function(e){
    mushroom.x = e.pageX - (mushroom.image.width/2);
  });
  
  //Add event handler for start button
  $("#BtnImgStart").click(function (){		
    toggleGameplay();
  });

  //Add event handler for show/hide bounds button	
  $("#showHideBounds").click(function (){		
    showBounds = !showBounds;
    gameLoop();
  });
  
}


//init all game variables that can only be achieved after the DOM is loaded
function initSettings()
{
  //Get a handle to the 2d context of the canvas
  ctx = document.getElementById('canvas').getContext('2d'); 
  
  //Calulate screen height and width
  screenWidth = parseInt($("#canvas").attr("width"));
  screenHeight = parseInt($("#canvas").attr("height"));
  
  //center mushroom on the horizontal axis
  mushroom.x = parseInt(screenWidth/2);
  mushroom.y = screenHeight - 40; 
  
  //Place the animal on top of the mushroom
  animal.x = parseInt(screenWidth/2);
  animal.y = parseInt(screenHeight/2);

}

//load all images for game
function loadImages()
{
  
  mushroomImg.src = "images/mushroom.png";
  mushroomWobbleImg.src = "images/mushroom2.png";
  backgroundForestImg.src = "images/forest1.jpg";
  bearEyesOpenImg.src = "images/bear_eyesopen.png";
  bearEyesClosedImg.src = "images/bear_eyesclosed.png";
  flowerImg.src = "images/flower.png";
  acornImg.src = "images/acorn.png";
  leafImg.src = "images/leaf.png";
  scoreImg.src = "images/score.png";
  owl1.src = "images/owl1.png";
  owl2.src = "images/owl2.png";
  
  //load combo bonus images
  for(var x=0; x<3; x++)
  {
    bonusImages[x] = new Image();	
    bonusImages[x].src = "images/comboBonus" + (x + 1) + ".png";
  }
  
  //load lives images
  for(var x=0; x<6; x++)
  {
    livesImages[x] = new Image();	
    livesImages[x].src = "images/lives" + x + ".png";
  }
  
  baddy.image = owl1;
  baddy.altImg = owl2;
  mushroom.image = mushroomImg;
  mushroom.squashedImg = mushroomWobbleImg;
  animal.image = bearEyesClosedImg;
  animal.eyesOpenImg = bearEyesOpenImg;
  
  //Wait for background image to load and then call gameLoop to draw initial stage
  backgroundForestImg.onload = function(){gameLoop(); };
  
}

function initPrizes()
{
  var count=0;
  for(var x=0; x<3; x++)
  {
    for(var y=0; y<23; y++)
    {
      prize = new Prize();
      if(x==0)
        prize.image = flowerImg;
      if(x==1)
        prize.image = acornImg;
      if(x==2)
        prize.image = leafImg;
      
      
      prize.row = x;
      prize.col = y;
      prize.x = 20 * prize.col + 10;
      prize.y = 30 * prize.row + 40;
      prize.hit = false;
      prizes[count] = prize;
      count++;
    }
  }
}

function drawPrizes()
{

  for(var x=0; x<prizes.length; x++)
  {
    currentPrize = prizes[x];
    if(!currentPrize.hit)
    {
      if(currentPrize.spinning)
      {
        //alert("spinning" + currentPrize.angle);
        currentPrize.angle += 10;
        ctx.save();
        
        //Translate to the center of the prize (i.e. the point about which we are going to perform the rotation
        ctx.translate(currentPrize.x + (currentPrize.image.width/2), currentPrize.y + (currentPrize.image.height/2));
        
        //Perform the rotation based on the current prize angle
        ctx.rotate(currentPrize.angle * Math.PI/180);
        
        ctx.drawImage(currentPrize.image, - (currentPrize.image.width/2), - (currentPrize.image.width/2));
        
        showObjectBounds(currentPrize, - (currentPrize.image.width/2), - (currentPrize.image.width/2));
        
        ctx.restore();
        
        if(currentPrize.angle == 360)
        {
          currentPrize.hit = true;
          multiPoint++;
          
          //Update score based on row
          if(multiPoint > 3)
          {
            //we've hit more that 3 prizes in a row, so add bonus and show bonus sign
            score += (3-currentPrize.row) * (multiPoint);
          
            //showBonus = true;
            showBonus();
            //bonusStarted = new Date().getTime();
          }
          else
          {
            score += (3-currentPrize.row);
          }
          
          //Check if all prizes have been caught!
          if(allPrizesHit())
          {
            newLevel.play();
            gameOver();
            
          }
        }
      }
      else
      {
        ctx.drawImage(currentPrize.image, prizes[x].x, prizes[x].y);
        showObjectBounds(currentPrize);
        
      }
    }
  }
  
}

function allPrizesHit()
{
  for(var c=0; c<prizes.length; c++)
  {
    checkPrize = prizes[c];
    if(checkPrize.hit == false)
      return false;
      
  }
  return true;
}

//Draw the mushroom (depending on its current status)
function drawMushroom()
{
  showObjectBounds(mushroom);
  if(!mushroom.boing)
  {
    ctx.drawImage(mushroom.image, mushroom.x, mushroom.y);
    
  }
  else
  {
    //calculate difference in time since last boing
    var sinceLastBoing = (new Date().getTime() - mushroom.lastBoing);
    var sinceStartBoing = (new Date().getTime() - mushroom.startBoing);
    
    //Wobble approx. every 40msec
    if(sinceLastBoing > 40)
    {
      //toggle the wobble!
      mushroom.wobble = !mushroom.wobble;
      mushroom.lastBoing = new Date().getTime();
    }
    
    //Stop wobbling after 500msec
    if(sinceStartBoing > 500)
    {
      mushroom.boing = false;
      mushroom.wobble = false;
    }
    
    //Mushroom has just been hit so make it wobble!
    if(mushroom.wobble)
      ctx.drawImage(mushroom.squashedImg, mushroom.x, mushroom.y);
    else
      ctx.drawImage(mushroom.image, mushroom.x, mushroom.y);
      
  }
}

function drawAnimal()
{
  ctx.save();
  
  //Translate to the center of the bear (i.e. the point about which we are going to perform the rotation
  ctx.translate(animal.x + (animal.image.width/2), animal.y + (animal.image.height/2));
  
  //Adjust the angle according to the horizontal speed
  animal.angle += horizontalSpeed;
  if(animal.angle < 0) animal.angle=360;
  else if(animal.angle>360) animal.angle=0;
  
  //Perform the rotation based on the current bear angle
  ctx.rotate(animal.angle * Math.PI/180);
      
  //Draw the bear (if he's going down open eyes!)
  //Also note that the X,Y coordinates are based on the moving out from the center of the bear and not the original bearX and bearY
  //This is because of the translate function we performed earlier
  showObjectBounds(animal, - (animal.image.width/2), - (animal.image.width/2));
  if(verticalSpeed>0)
  {
    ctx.drawImage(animal.eyesOpenImg, - (animal.eyesOpenImg.width/2), - (animal.eyesOpenImg.width/2));
  }
  else
  {
    ctx.drawImage(animal.image, - (animal.image.width/2), - (animal.image.width/2));
  }
  
  ctx.restore();
  
}

function drawScore()
{
  ctx.drawImage(scoreImg, screenWidth-(scoreImg.width),0);
  ctx.font = "12pt Arial";
  ctx.fillText("" + score, 425, 25);  
}

function drawLives()
{
  //livesImg.src = "images/lives"+lives+".png";
  ctx.drawImage(livesImages[lives], 0, 0);
}

function hasAnimalHitEdge()
{
  
  //Has the bear reached the far right hand side?
  if(animal.x>screenWidth - animal.image.width)
  {
    //bouncing off the right hand side so play boing and reverse horizontal speed
    boing2.play();
    if(horizontalSpeed > 0)
      horizontalSpeed =-horizontalSpeed;
  }
  
  //Has bear reached the far left hand side?
  if(animal.x<-10)
  {
    //bouncing off the left hand side so play boing and reverse horizontal speed
    boing3.play();
    if(horizontalSpeed < 0)
      horizontalSpeed = -horizontalSpeed;
  }
  
  //Has bear hit the bottom of the screen - Ouch!
  if(animal.y>screenHeight - animal.image.height)
  {
    //Bouncing off bottom, so animal groans and game stops, oh and you lose a life
    verticalSpeed = -speed;
    
    multiPoint = 0;
    
    //Stop game for now
    toggleGameplay();	
    
    //Wait 2 seconds and reset for next life
    setTimeout(function(){
      
      if(lives != 0)
      {
        //Place the animal on top of the mushroom
        animal.x = parseInt(screenWidth/2);
        animal.y = parseInt(screenHeight/2);
        $("#BtnImgStart").show();
        gameLoop();
      }
      
    }, 2000);
    
    //Deduct lives and check if we have any left
    lives -= 1;
    
    if(lives > 0)
    {
      groan.play();
      drawLives();
    }
    else
    {
      awwwww.play();
      gameOver();
    }
  }
  
  //Has bear hit to the top of the screen
  if(animal.y<0)
  {
    //Bouncing off top, so play boing and reverse vertical speed
    boing4.play();
    verticalSpeed = speed;
  }
  
}

function hasAnimalHitMushroom()
{
  //Has bear hit mushroom
  if(checkIntersect(animal, mushroom, 5))
  {
  
    if((animal.x + animal.image.width/2) < (mushroom.x + mushroom.image.width*0.25))
    {
      horizontalSpeed = -speed;
    }
    else if((animal.x + animal.image.width/2) < (mushroom.x + mushroom.image.width*0.5))
    {
      horizontalSpeed = -speed/2;
    }
    else if((animal.x + animal.image.width/2) < (mushroom.x + mushroom.image.width*0.75))
    {
      horizontalSpeed = speed/2;
    }
    else
    {
      horizontalSpeed = speed;
    }
    boing1.play();
    mushroom.startBoing = new Date().getTime();
    mushroom.lastBoing = mushroom.lastBoing;
    mushroom.boing = true;
    verticalSpeed = -speed;
    multiPoint = 0;

  }
}

function hasAnimalHitPrize()
{
  for(var x=0; x<prizes.length; x++)
  {
    var prize = prizes[x];
    
    if(!prize.hit)
    {
      if(checkIntersect(prize, animal, 0))
      {
        //alert("hit prize");
        //prize.hit = true;
        prize.spinning = true;
        verticalSpeed = speed;
        //Play sound based on hight of prize
        if(prize.row == 0)
          collisionStarPrize.play();
        else
          collisionPrize.play();		
      }
    }
  }
  
}

function showBaddy()
{
  
  
  if(baddyStatus == BADDY_OFF)
  {
    setTimeout(changeBaddyImage, 100);
    baddyStatus = BADDY_FLYING;
  }
}

function drawBaddy()
{
  
  //If the baddy (owl) is currently sleeping then gamble on it coming back
  if(baddyStatus == BADDY_OFF)
  {
    //Pick a number between 1 and 100
    shallWeStart = Math.floor(Math.random()*100);
    
    //Is this your card?
    if (shallWeStart == 50)
      showBaddy();
  }
  
  //Check if the animal has hit the baddy
  if(checkIntersect(baddy, animal, 5))
  {
    //If not already done, change the horizontal speed of the animal
    if(baddyStatus != BADDY_HIT)
      horizontalSpeed = -horizontalSpeed;
    
    //Update new status, send animal down and squawk
    baddyStatus = BADDY_HIT;
    verticalSpeed = speed;
    squawk.play();
  }
  
  //If sleeping reset x co-ordinate
  if(baddyStatus == BADDY_OFF)
    baddy.x = 500;
  
  //If baddy (owl) is flying, then move it from right to left
  if(baddyStatus == BADDY_FLYING)
    baddy.x -=2;
  
  //If baddy (owl) is hit then send him crashing down
  if(baddyStatus == BADDY_HIT)
  {
    baddy.y +=2;
    //If he's gone beyond the ground then reset
    if(baddy.y > 320)
    {
      baddyStatus = BADDY_OFF;
      baddy.x = 500;
      baddy.y = 125;
    }
  }
  
  //If baddy(owl) has flown past without being hit, then rest the x position, update the status and play a hoot!
  if(baddy.x < -20)
  {
    baddyStatus = BADDY_OFF;
    owl.play();
    baddy.x = 500;
  }
  
  //If baddy (owl) isn't asleep then draw him
  if(baddyStatus != BADDY_OFF)
  {
    //rotate flap wings i.e. alternate images
    if(currentBaddyImage == 0)
      currentBaddy = baddy.image;
    else
      currentBaddy = baddy.altImg;
    
    //If he's not hit just draw as normal
    if(baddyStatus != BADDY_HIT)
    {
      showObjectBounds(baddy, baddy.x, baddy.y);
      ctx.drawImage(currentBaddy, baddy.x, baddy.y);	
    }
    else
    {
      //He's been hit so rotate him as he flies out of control!
      ctx.save();
    
      //Translate to the center of the bear (i.e. the point about which we are going to perform the rotation
      ctx.translate(baddy.x + (baddy.image.width/2), baddy.y + (baddy.image.height/2));
      
      //Adjust the angle according to the horizontal speed
      baddy.angle += 7;
      
      if(baddy.angle < 0) baddy.angle=360;
      else if(baddy.angle>360) baddy.angle=0;
      
      //Perform the rotation based on the current bear angle
      ctx.rotate(baddy.angle * Math.PI/180);
          
      showObjectBounds(baddy, - (baddy.image.width/2), - (baddy.image.width/2));
      
      ctx.drawImage(currentBaddy, - (baddy.image.width/2), - (baddy.image.width/2));
      
      ctx.restore();
    }
    
    
  }
}

//if not alseep calls itself to alternate current baddy image
function changeBaddyImage()
{
  currentBaddyImage++;
  
  if(currentBaddyImage == 2)
    currentBaddyImage = 0;
  
  if(baddyStatus != BADDY_OFF)
    setTimeout(changeBaddyImage, 100);
  
}


function showBonus()
{
  if(!bonusActive)
  {
    bonusStarted = new Date().getTime();
    setTimeout(changeBonusImage, 50);
    bonusActive = true;
  }
}


function drawBonus()
{
  if(bonusActive)
    ctx.drawImage(bonusImages[currentBonusImage], 200, 150);	
}

function changeBonusImage()
{
  currentBonusImage++;
  
  if(currentBonusImage == 3)
    currentBonusImage = 0;
      
  now = new Date().getTime();
  if((now-bonusStarted) < 1000)
  {
    setTimeout(changeBonusImage, 50);
  }
  else
  {
    bonusActive = false;
  }
}

//Main game loop, it all happens here!
function gameLoop(){  
  
  //Clear the screen (i.e. a draw a clear rectangle the size of the screen)
  ctx.clearRect(0, 0, screenWidth, screenHeight);
  
  //Move the bear in the current direction
  animal.x += horizontalSpeed;
  animal.y += verticalSpeed;
  
  //Draw the background forest
  ctx.drawImage(backgroundForestImg, 0, 0);
  
  //draw score
  drawScore();
  
  //draw lives
  drawLives();
  
  //Draw prizes
  drawPrizes();
  
  drawBaddy();
  
  drawBonus();
  
  //Draw the mushroom (depending on its current status)
  drawMushroom();
  
  //draw animal
  drawAnimal();
  
  //Check collisions for animal
  hasAnimalHitEdge();
  
  hasAnimalHitMushroom();
  
  hasAnimalHitPrize();
  
  //increment frame count
  frameCount++;
  

}  

function checkIntersect(object1, object2, overlap)
{
  //    x-Axis                      x-Axis
  //  A1------>B1 C1              A2------>B2 C2
  //  +--------+   ^              +--------+   ^ 
  //  | object1|   | y-Axis       | object2|   | y-Axis
  //  |        |   |              |        |   |
  //  +--------+  D1              +--------+  D2
  //
  
  A1 = object1.x + overlap;
  B1 = object1.x + object1.image.width - overlap;
  C1 = object1.y + overlap; 
  D1 = object1.y + object1.image.height - overlap;
  
  A2 = object2.x + overlap;
  B2 = object2.x + object2.image.width - overlap;
  C2 = object2.y + overlap;
  D2 = object2.y + object2.image.width - overlap;
  
  //Do they overlap on the x-Axis
  if(A1 > A2 && A1 < B2
     || B1 > A2 && B1 < B2)
  {
    //x axis intersects so check y axis
    if(C1 > C2 && C1 < D2
     || D1 > C2 && D1 < D2)
    {
      //overlap
      return true;
    }
    
  }
  //as you were
  return false;
}


function showObjectBounds(gameObject, transitionX, transitionY)
{
    
  if(showBounds)
  {
    if(typeof(transitionX) != 'undefined')
      rectX = transitionX;
    else
      rectX = gameObject.x;
    
    if(typeof(transitionY) != 'undefined')
      rectY = transitionY;
    else
      rectY = gameObject.y;
      
    ctx.save();
    
    ctx.strokeStyle = '#f00'; // red
    ctx.lineWidth   = 2;
    ctx.strokeRect(rectX, rectY, gameObject.image.width, gameObject.image.height);
    
    ctx.restore();
  }
}

//Update the display to show frames per second and reset ready for next count
function fps()
{
  $("#fps").html(frameCount + " fps");
  frameCount=0;
}

//Start game timer, i.e. setTimeout that calls itself taking into account the
//actual real difference in time. This is better than 
function startGameTimer()
{
  var start = new Date().getTime(),
    time = 0;
  function timer()
  {
    time += 15;
    var diff = (new Date().getTime() - start) - time;
    if(gameRunning)
    {
      gameLoop();
      window.setTimeout(timer, (15 - diff));
    }
  }
  if(gameRunning)
    window.setTimeout(timer, 15);
}

//Start/stop the game loop (and more importantly that annoying boinging!)
function toggleGameplay()
{
  gameRunning = !gameRunning;
  
  if(gameRunning)
  {
    $("#BtnImgStart").hide();
    birds.play();
    startGameTimer();
  }
  else
  {
    birds.pause();
  }
}

function gameOver()
{
  gameRunning = false;
  birds.pause();
  
  alert("Game Over: thanks for playing!");
  
}
	
