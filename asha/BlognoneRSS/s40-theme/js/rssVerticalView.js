/**
 * Construct HTML string that will be inserted into the content container.
 * 
 * This goes through the RSS entries and creates a vertical list of RSS
 * entries. Each entry is represented by a picture and title. Clicking on the
 * title unrolls the summary attached to the RSS entry.
 */
var maxEntries;
var feedUrl;

function refreshPageContent() {
	// Add code for refreshing the page here...	
	getRSS('verview', 'horview', feedUrl);
}	

function getVerticalViewHTML(feed){

	var entryIndex = 0;
    
	var entries = feed.items;

	var outputstr = "";
	outputstr += '<div class="header_contnr">';
	outputstr += '<div class="inline ui-title "><img src="s40-theme/images/title_news.png" width="113" height="40" alt="News"/></div>';   
	outputstr += '<div class="inline refresh-icon"><a onclick="refreshPageContent();"><img alt="icon" src="s40-theme/images/reload.png"/></a></div>';
	outputstr += '</div>';
	outputstr += '<div class="pubDate_contnr"><div id="catPubDate" class="pubDate_txt">';
	outputstr += feed.updated;
	outputstr += '</div></div>';
	
	if(entries.length > 0){
    	if(entries.length > 10)
    		maxEntries = 10;
    	else 
    		maxEntries = entries.length;
    }
	
	outputstr +='<div class="content">';
	for (var i = 0; i < entries.length && entryIndex < maxEntries; i++)
	{
		var entryToShow = "entry"+entryIndex;
		outputstr += '<div id="entryn' + i + '" class="entry " onclick="';
		outputstr += 'mwl.setGroupTarget(\'#btns\',\'#btn' + (entryIndex) + '\', \'show\', \'hide\');';
		outputstr += 'mwl.setGroupTarget(\'#td_back\', \'#back_id_' + (entryIndex) + '\', \'show\', \'hide\');'; 
		outputstr += 'mwl.addClass(\'#verviewcontainer\', \'height_cur\');'; 
		outputstr += 'mwl.scrollTo(\'#views\');'; 
		outputstr += 'mwl.switchClass(\'#horview\', \'hide\', \'show\');'; 
		outputstr += 'mwl.removeClass(\'#entry' + entryIndex + '\', \'height_cur2\');';
		outputstr += 'mwl.addClass(\'#entries\', \''+entryToShow+'\');';
		outputstr += 'mwl.switchClass(\'#views\', \'view1\', \'view2\');';
		outputstr += 'mwl.addClass(\'#entries\', \'hortransition\');';
		
		outputstr += '">';
		
		outputstr += '<div style="display:block;">';	
		
		outputstr += '<div class="headlinePic inline">';
		outputstr += "<img width=\"" + ((entries[i].image && entries[i].image.width && parseInt(entries[i].image.width) <= 77)? entries[i].image.width: "75") + 
		             "\" height=\"" + ((entries[i].image && entries[i].image.height && parseInt(entries[i].image.height) <= 75)? entries[i].image.height: "50") + 
					 "\" src=\"" + ((entries[i].image && entries[i].image.url)? entries[i].image.url: "s40-theme/images/rss3.png") + "\"/>";
		outputstr += '</div>';
		
		outputstr += '<div class="headline inline">';
		if(window.innerWidth>240){
			if((entries[i].title).length>=50){
	       	 outputstr += (entries[i].title).substring(0,50)+"...";
	       }
	       else {
	       	outputstr += entries[i].title;
	       }
		}
		else{
			if((entries[i].title).length>=50){
		       	 outputstr += (entries[i].title).substring(0,50)+"...";
	       }
	       else {
	       	outputstr += entries[i].title;
	       }
		}
		outputstr += '</div>';
		if (entries[i].updated)
		{
			outputstr += '<div class="pubDate">';
			outputstr += entries[i].updated;
			outputstr += '</div>';
		}
		

		outputstr += '</div>'; // end entry
		
		entryIndex++;
		outputstr +='</div>';//end Content
		outputstr += '<div class="greySep"></div>';	
		
	}
	
	return outputstr;
}