/**
 * Constructs an HTML string that will be inserted into the content container.
 * 
 * Parses through the RSS entries and creates a "slideshow" representation
 * of the RSS entries. Cycling through stories is done through left/right
 * swipe gestures. The picture (if available), title, and short summary of the
 * article are shown.
 */
/*global variable
 * var maxEntries;
 */ 
var maxEntries = 0;
function getHorizViewHTML(feed){

    var nextImg = '<img src="s40-theme/images/nextArrow.png" width="10" height="23">';
    var prevImg = '<img src="s40-theme/images/prevArrow.png" width="10" height="23">';
    var entryIndex = 0;
    
	var entries = feed.items;
	if(entries.length > 0){
    	if(entries.length > 10)
    		maxEntries = 10;
    	else 
    		maxEntries = entries.length;
    }
	
    var outputstr = "";
	outputstr += '<div class="horContainer">';
	outputstr += '<div class="header_contnr">';
	outputstr += '<div class="ui-title inline "><img src="s40-theme/images/title_news.png" width="113" height="40" alt="News" ;"></div>';
    outputstr += '<div id="td_back" class="refresh-icon inline ">';
    for (var i = 0; i < entries.length && i < maxEntries; i++){
    outputstr += '<div id="back_id_'+i+'" class="hide" onclick="mwl.addClass(\'#entry' + i + '\', \'height_cur2\');mwl.switchClass(\'#views\',\'view2\', \'view1\');mwl.removeClass(\'#entries\', \'entry*\');mwl.removeClass(\'#entries\', \'hortransition\');mwl.switchClass(\'#horview\', \'show\', \'hide\');mwl.removeClass(\'#verviewcontainer\', \'height_cur\');"><img src="s40-theme/images/back_arrow.png"></div>';
    }
    outputstr += '</div>';
	outputstr += '</div>';
    outputstr += '<div id="btns">';
	
    
    for (var i = 0; i < entries.length && i < maxEntries; i++)
    {
        if (i == 0)
            outputstr += '<div class="show" id="btn' + i + '">';
        else
            outputstr += '<div class="hide" id="btn' + i + '">';
            
        
        outputstr += '<div class="navBar">';
        if (i == 0 )
        {
        	outputstr += '<div class="alignC navBar_sep navBar_dis inline">'+prevImg+'</div>';
           	outputstr += '<div class="alignC inline" style="width:49%;" onclick="mwl.setGroupTarget(\'#btns\',\'#btn1\', \'show\', \'hide\');mwl.setGroupTarget(\'#td_back\',\'#back_id_' + (i + 1) + '\', \'show\', \'hide\');mwl.addClass(\'#entry' + i + '\', \'height_cur2\');mwl.removeClass(\'#entry' + (i+1) + '\', \'height_cur2\');mwl.switchClass(\'#entries\', \'entry' + i + '\',  \'entry' + (i + 1) + '\');">' + nextImg + '</div>';
        }
        else if ( (i == entries.length -1) || (i ==maxEntries -1))
        {
           	outputstr += '<div class="alignC navBar_sep inline" style="width:49.5%" onclick="mwl.setGroupTarget(\'#btns\',\'#btn' + (i - 1) + '\', \'show\', \'hide\');mwl.setGroupTarget(\'#td_back\',\'#back_id_' + (i - 1) + '\', \'show\', \'hide\');mwl.addClass(\'#entry' + i + '\', \'height_cur2\');mwl.removeClass(\'#entry' + (i-1) + '\', \'height_cur2\');mwl.switchClass(\'#entries\', \'entry' + i + '\',  \'entry' + (i - 1) + '\');">' + prevImg + '</div>';
        	outputstr += '<div class="alignC navBar_dis inline";>'+nextImg+'</div>';
        }
        else 
        {
           	outputstr += '<div class="alignC navBar_sep inline" style="width:50%" onclick="mwl.setGroupTarget(\'#btns\',\'#btn' + (i - 1) + '\', \'show\', \'hide\');mwl.setGroupTarget(\'#td_back\',\'#back_id_' + (i - 1) + '\', \'show\', \'hide\');mwl.addClass(\'#entry' + i + '\', \'height_cur2\');mwl.removeClass(\'#entry' + (i-1) + '\', \'height_cur2\');mwl.switchClass(\'#entries\', \'entry' + i + '\',  \'entry' + (i - 1) + '\');">' + prevImg + '</div>';
          	outputstr += '<div class="alignC inline" style="width:49%" onclick="mwl.setGroupTarget(\'#btns\',\'#btn' + (i + 1) + '\', \'show\', \'hide\');mwl.setGroupTarget(\'#td_back\',\'#back_id_' + (i + 1) + '\', \'show\', \'hide\');mwl.addClass(\'#entry' + i + '\', \'height_cur2\');mwl.removeClass(\'#entry' + (i+1) + '\', \'height_cur2\');mwl.switchClass(\'#entries\', \'entry' + i + '\',  \'entry' + (i + 1) + '\');">' + nextImg + '</div>';
        }
        outputstr += '</div>';
        outputstr += '</div>';
                        
    }
    outputstr += '</div>'; // end btns
    outputstr += '  <div class="horstrip" id="entries">'; 
    
        
    for (var i = 0; i < entries.length && i < maxEntries; i++)
    {
        
        outputstr += '<div id="entry'+i+'" class="valignTp horEntry height_cur2 inline">';

        
        outputstr += '<div class="headlinePic inline">';
        outputstr += "<img width=\"" + ((entries[i].image && entries[i].image.width && parseInt(entries[i].image.width) <= 75)? entries[i].image.width: "75") + 
		             "\" height=\"" + ((entries[i].image && entries[i].image.height && parseInt(entries[i].image.height) <= 75)? entries[i].image.height: "75") + 
					 "\" src=\"" + ((entries[i].image && entries[i].image.url)? entries[i].image.url: "s40-theme/images/rss3.png") + "\"/>";
        outputstr += '</div>';
        
        outputstr += '<div class="headline inline">';
        outputstr += entries[i].title;
        outputstr += '</div>';
        
        if (entries[i].updated)
        {
            outputstr += '<div class="pubDate">';
            outputstr += entries[i].updated;
            outputstr += '</div>';
        }       
        outputstr += '<div class="headlineDesc" id="headlineDesc'+i+'">';
        
        outputstr += '<div class="headlineDesc" id="headlineDesc'+i+'">';
        if( entries[i].fulltext == '')
        	outputstr += entries[i].description;
        else 
        	outputstr += entries[i].fulltext;
                       
        outputstr += '<p><a style="color:#000000" href="'+entries[i].link+'"><img src="s40-theme/images/readfull.png" /></a></p>';
        outputstr += '</div>'; // end headlineDesc+i
        
        outputstr += '</div>'; // end entry
        outputstr += '  </div>'; // end entries 
    }

    
  
    outputstr += '</div>'; // end container
    return outputstr;
}

