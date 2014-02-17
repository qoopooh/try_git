var omitformtags=["input", "textarea", "select"];
//var message="You may not right mouse click this page.";

/* Disable Right Click */
if (navigator.appName == 'Microsoft Internet Explorer'){
  function NOclickIE(e) {
    if (event.button == 2 || event.button == 3) {
      alert(message);
      return false;
    }
      return true;
  }
  document.onmousedown=NOclickIE;
  document.onmouseup=NOclickIE;
  window.onmousedown=NOclickIE;
  window.onmouseup=NOclickIE;
} else {
  function NOclickNN(e){
    if (document.layers||document.getElementById&&!document.all){
      if (e.which==2||e.which==3){
        //alert(message);
        return false;
      }
    }
  }
  if (document.layers){
    document.captureEvents(Event.MOUSEDOWN);
    document.onmousedown=NOclickNN; 
  }
  document.oncontextmenu=new Function("return false")
}

/* ห้ามคลุมดำ */
function disableselect(e){
  for (i = 0; i  < omitformtags.length; i++)
    if (omitformtags[i]==(e.target.tagName.toLowerCase()))
      return;
  return false
}

function reEnable(){
  return true;
}

function noSelect(){
  if (typeof document.onselectstart!="undefined"){
    document.onselectstart=new Function ("return false")
      if (document.getElementsByTagName){

      tags=document.getElementsByTagName('*')
        for (j = 0; j < tags.length; j++){
        for (i = 0; i < omitformtags.length; i++)
          if (tags[j].tagName.toLowerCase()==omitformtags[i]){
              tags[j].onselectstart=function(){
              document.onselectstart=new Function ('return true')
            }

            tags[j].onmouseup=function(){
              document.onselectstart=new Function ('return false')
            }
          }
        }
      }
  }
  else{
  document.onmousedown=disableselect

  document.onmouseup=reEnable
  }
}

window.onload=noSelect;
