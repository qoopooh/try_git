let minutes = 0;
let seconds = 0;
let milliseconds = 0;
let timerRunning = false;
let interval;

const toggleButton = document.getElementById("start");
const resetButton = document.getElementById("reset");
const minutesDisplay = document.getElementById("minutes");
const secondsDisplay = document.getElementById("seconds");
const millisecondsDisplay = document.getElementById("milliseconds");

function toggleTimer() {
  if (interval) {
    clearInterval(interval);
    interval = null;
    toggleButton.innerText = 'Start';
    toggleButton.classList.remove('running');
    resetButton.disabled = false;
  } else {
    interval = setInterval(function() {
      milliseconds += 100;
      if (milliseconds == 1000) {
        seconds++;
        milliseconds = 0;
      }
      if (seconds == 60) {
        minutes++;
        seconds = 0;
      }
      updateDisplay();
    }, 100);
    toggleButton.innerText = 'Stop';
    toggleButton.classList.add('running');
    resetButton.disabled = true;
  }
}

function resetTimer() {
  clearInterval(interval);
  minutes = 0;
  seconds = 0;
  milliseconds = 0;
  updateDisplay();
  resetButton.disabled = true;
  toggleButton.classList.remove('running');
  toggleButton.focus();
}

function updateDisplay() {
  minutesDisplay.innerHTML = padZeroes(minutes);
  secondsDisplay.innerHTML = padZeroes(seconds);
  millisecondsDisplay.innerHTML = padZeroes(milliseconds / 10);
}

function padZeroes(num) {
  if (num < 10) {
    return "0" + num;
  } else {
    return num;
  }
}

toggleButton.addEventListener("click", toggleTimer);
resetButton.addEventListener("click", resetTimer);
