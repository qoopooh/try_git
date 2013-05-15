/**
 * Use xmlhttpreq to get the raw rss xml
 */
function getRSS(vid, hid, url){
	feedUrl = url;
	jQuery.getFeed({
	  url: url,
	  success: function processRSS(feed,data){
        jQuery('#' + vid).html(getVerticalViewHTML(feed));
		jQuery('#' + hid).html(getHorizViewHTML(feed));
				
		var scriptNode = document.createElement('SCRIPT');
		var text = "";
		for(i = 1; i < maxEntries; i++){
			text = text + "mwl.addSwipeLeftListener('#entry" + (i - 1) + "', \"mwl.scrollTo('#views');mwl.setGroupTarget('#td_back','#back_id_" + i + "', 'show', 'hide');mwl.setGroupTarget('#btns','#btn" + i + "', 'show', 'hide');mwl.addClass('#entry" + (i - 1) + "', 'height_cur2');mwl.removeClass('#entry" + i + "', 'height_cur2');mwl.switchClass('#entries', 'entry"+(i-1)+"','entry"+i+"')\");";
		}
		for(i = (maxEntries - 1); i >= 1; i--){
			text = text + "mwl.addSwipeRightListener('#entry"+i+"', \"mwl.scrollTo('#views');mwl.setGroupTarget('#td_back','#back_id_" + (i - 1) + "', 'show', 'hide');mwl.setGroupTarget('#btns','#btn"+(i-1)+"', 'show', 'hide');mwl.addClass('#entry" + i + "', 'height_cur2');mwl.removeClass('#entry" + (i - 1) + "', 'height_cur2');mwl.switchClass('#entries', 'entry"+i+"','entry"+(i-1)+"')\");";
		}
						
		var scriptText = document.createTextNode(text);
		scriptNode.appendChild(scriptText);
		
		var bodyNode = document.getElementsByTagName('body')[0];
	    bodyNode.appendChild(scriptNode);
	  } 
	});  
}