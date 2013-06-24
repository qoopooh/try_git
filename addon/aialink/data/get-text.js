var textArea = document.getElementById("edit-box");
textArea.addEventListener("keyup", function onkeyup(event) {
  if (event.keyCode === 0x0d) {
    text = textArea.value.replace(/(\r\n|\n|\r)/gm, "");
    self.port.emit("text-entered", text);
    textArea.value = '';
  }
}, false);

self.port.on("show", function onShow() {
  textArea.focus();
});

