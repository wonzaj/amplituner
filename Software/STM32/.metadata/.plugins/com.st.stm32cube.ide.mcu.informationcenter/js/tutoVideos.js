/*******************************************************************************
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved. This program and the accompanying materials
 * is the property of STMicroelectronics and must not be
 * reproduced, disclosed to any third party, or used in any
 * unauthorized manner without written consent.
 *******************************************************************************/
let oldSelectiCate = 0;
let oldSelectyVideos = 0;

function ProcessCategoriesJson(videosTab) {
	for (i = 0; i < videosTab.length; i++) {
		let div = document.createElement("div");
		div.setAttribute('class', 'videoCat');
		document.getElementById('videoDiv').appendChild(div);
		let span = document.createElement("span");
		span.setAttribute('class', 'container__box__link--text');
		span.innerHTML += videosTab[i].category;
		div.appendChild(span);
		let table = document.createElement("table");
		table.setAttribute('id', 'videoTable'+i);
		table.setAttribute('class', 'videoCatTable');
		div.appendChild(table);
		let trT = document.createElement("tr");
		table.appendChild(trT);
		FillJSonPlaylist(videosTab[i].id, i);
	}
}

function ProcessDuration(videoDuration, idElemI, idElemeY){
	if(idElemI == 0 && idElemeY == 0){
		// add on big pictures
		let span = document.getElementById('curVidDuration');
		span.innerText = videoDuration;
	}
	// also on the little one
	let span = document.getElementById('dur' + idElemI + idElemeY);
	span.innerText = videoDuration;
}

function ProcessPlaylistJson(playlistId, dataPlaylist, i){
		let tr = document.createElement("tr");
		document.getElementById('videoTable'+i).appendChild(tr);
		// center area
		// by default we show the 1st videos of the 1st category
		if(i == 0){
			// add Title and description to the center area
			let trarea = document.getElementById('curVideoDesc');
			let parea = document.createElement("p");
			parea.setAttribute('align', 'left');
			parea.setAttribute('id', 'pCenter');
			let descript;
			descript = dataPlaylist[0].snippet.description;
			descript = descript.replace(/\n/g, '<br />');
			// if url in the text to it clickable
			descript = DoUrlClickable(descript);
			parea.innerHTML  = '<p class=descTitle>' + dataPlaylist[0].snippet.title + '</p> <br> <p class=descContent>' + descript + '</p>';
			trarea.appendChild(parea);
			// add a picture to the center area
			let imgarea = document.getElementById('curVideoPic');
			imgarea.setAttribute('src', dataPlaylist[0].snippet.thumbnails.medium.url);
			// add the link to the video to the center area
			let areaarea = document.getElementById('videoLink');
			// keep this line to remember that it is not in full screen but description visible
			// areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/watch?v%3D" + dataPlaylist[0].snippet.resourceId.videoId);
			areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/embed/" + dataPlaylist[0].snippet.resourceId.videoId + "?cc_load_policy%3D1%26autoplay%3D1");
			// get duration 
			GetDuration(playlistId, dataPlaylist[0].snippet.resourceId.videoId, i,0);
		}
		// fill videos row
		for (y = 0; y < dataPlaylist.length; y++) {
			let td = document.createElement("td");
			tr.appendChild(td);
			let div = document.createElement("div");
			div.setAttribute('style', 'width:200px; position:relative;');
			td.appendChild(div);
			let img = document.createElement("img");
			if(i == 0 && y == 0){
				img.setAttribute('border', '0');
				img.setAttribute('style', "box-shadow:0px 0px 22px 8px #3CB4E6; border-radius: 6px;");
			}
			else{
				img.setAttribute('border', '0');
				img.setAttribute('style', "border-radius: 6px;");
			}
			img.setAttribute('class', 'thumbnail');
			img.setAttribute('id', "img" + i + y);
			img.setAttribute('src', dataPlaylist[y].snippet.thumbnails.default.url);
			img.setAttribute('position', 'absolute');
			img.setAttribute('align', 'right');
			img.setAttribute('usemap', "#click-map" + i + y);
			div.appendChild(img);
			let span = document.createElement("span");
			span.setAttribute('class', 'vidDuration');
			span.setAttribute('id', "dur" + i + y);
			span.innerText = "N/A";
			div.appendChild(span)
			let map = document.createElement("map");
			map.setAttribute('name', "click-map" + i + y);
			img.appendChild(map);
			let descriptImg;
			let txt;
			txt = dataPlaylist[y].snippet.description;
			txt = txt.replace(/\n/g, '<br />');
			// if url in the text to it clickable
			txt = DoUrlClickable(txt);
			descriptImg  = '<p class=descTitle>' + dataPlaylist[y].snippet.title + '</p> <br> <p class=descContent>' + txt + '</p>';
			let area = document.createElement("area");
			area.setAttribute('id', "area" + i + y);
			area.setAttribute('data-txtImg', descriptImg);
			area.setAttribute('data-urlImg', dataPlaylist[y].snippet.thumbnails.medium.url);
			area.setAttribute('data-idImg', dataPlaylist[y].snippet.resourceId.videoId);
			area.setAttribute('shape', 'shape');
			area.setAttribute('coords', '0, 0, 120, 90');
			area.setAttribute('onclick', "ChangeCenterArea(" + i + "," + y + ")");
			area.setAttribute('href', "#!");
			// get duration
			GetDuration(playlistId, dataPlaylist[y].snippet.resourceId.videoId, i , y);
			map.appendChild(area);
		}
		// Fill title row
		let trt = document.createElement("tr");
		document.getElementById('videoTable'+i).appendChild(trt);
		for (x = 0; x < dataPlaylist.length; x++) {
			let tdt = document.createElement("td");
			trt.appendChild(tdt);
			let divt = document.createElement("div");
			divt.setAttribute('style', 'width:200px');
			tdt.appendChild(divt);
			let pt = document.createElement("p");
			pt.setAttribute('class', 'container__title--text');
			pt.innerHTML = dataPlaylist[x].snippet.title;
			divt.appendChild(pt);
		}
	}

function ChangeCenterArea(iCate, yVideos){
	// update center area
	// suppress old border on the last selected picture
	let imgOld = document.getElementById('img' + oldSelectiCate + oldSelectyVideos);
	imgOld.setAttribute('border', '0');
	imgOld.setAttribute('style', 'border-radius: 6px;');
	// add border on the selected picture
	oldSelectiCate = iCate;
	oldSelectyVideos = yVideos;
	let img = document.getElementById('img' + iCate + yVideos);
	img.setAttribute('border', '0');
	img.setAttribute('style', "box-shadow:0px 0px 22px 8px #3CB4E6; border-radius: 6px;");
	// add Title and descriptino to the center area
	let area = document.getElementById('area' + iCate + yVideos);
	let text = area.getAttribute('data-txtImg');
	let elt = document.getElementById('pCenter');
	elt.innerHTML = text;
	// add a picture to the center area	
	let imgarea = document.getElementById('curVideoPic');
	imgarea.setAttribute('src', area.getAttribute('data-urlImg'));
	// add the link to the video to the center area
	let areaarea = document.getElementById('videoLink');
	// keep this line to remember that it is not in full screen but description visible
	// areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/watch?v%3D" + area.getAttribute('data-idImg'));
	areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/embed/" + area.getAttribute('data-idImg') + "?cc_load_policy%3D1%26autoplay%3D1");
	let span = document.getElementById('curVidDuration');
	span.innerText = document.getElementById('dur' + iCate + yVideos).innerText;
}

function GetDuration(playlistId, videoId, idElemI, idElemY){
	baseurl = 'https://sw-center.st.com/stm32cubeide/videos/1.0/';
	url = baseurl + '/' + playlistId + '/' + videoId + '.json';
	$.ajax({
		url: url,
		dataType: 'json',
		type: 'GET',
		crossDomain: true,
		success: function(dataremote){
			// Adapt Marcom Json format
			if (dataremote.hasOwnProperty('items')) {
				if (Array.isArray(dataremote.items)) {
					dataremote = dataremote.items;
				}
				else{
					dataremote = [ dataremote.items ];
				}
				duration = dataremote[0].contentDetails.duration;
				//hours = duration.match(/(\d+)H/);
				minutes = duration.match(/(\d+)M/);
				seconds = duration.match(/(\d+)S/);
				if(minutes == null){
					if(parseInt(seconds[1]) <= 9){
							ProcessDuration('0:0' + seconds[1], idElemI, idElemY);
						}else{
							ProcessDuration('0:' + seconds[1], idElemI, idElemY);
						}
				}else{
					if(seconds == null){
						ProcessDuration(minutes[1] + ':00', idElemI, idElemY);
					}else{
						if(parseInt(seconds[1]) <= 9){
							ProcessDuration(minutes[1] + ':0' + seconds[1], idElemI, idElemY);
						}else{
							ProcessDuration(minutes[1] + ':' + seconds[1], idElemI, idElemY);
						}
					}
				}
			} else {
				console.log('[ERROR] unknown video:' + videoId + ' youtube format');
			}
		},
		error: function(XHR, textStatus, errorThrown) {
		console.log('GetDuration');
		console.log(textStatus);
		console.log(errorThrown);
    }
	 });
}

function FillJSonPlaylist(id, i){
	baseurl = 'https://sw-center.st.com/stm32cubeide/videos/1.0/';
	url = baseurl + id + '.json';
	$.ajax({
		url: url,
		dataType: 'json',
		type: 'GET',
		crossDomain: true,
		success: function(dataremote){
			// Adapt Marcom Json format
			if (dataremote.hasOwnProperty('items')) {
				if (Array.isArray(dataremote.items)) {
					dataremote = dataremote.items;
				} 
				else{
					dataremote = [ dataremote.items ];
				}
				ProcessPlaylistJson(id, dataremote, i);
			} else {
				console.log('[ERROR] unknown playlist:' + id + ' youtube format');
			}
		},
		error: function(XHR, textStatus, errorThrown) {
		console.log('FillJSonPlaylist');
		console.log(textStatus);
		console.log(errorThrown);
    }
	 });
}

function DoUrlClickable(text) {
    var urlRegex =/(\b(https?|ftp|file):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig;
    return text.replace(urlRegex, function(url) {
        return '<a href="' + url + '">' + url + '</a>';
    });
}

// json categories
//processCategoriesJson(JSON.parse(items));

// remote pictures
baseurl = 'https://sw-center.st.com/stm32cubeide/videos/1.0/';
url = baseurl + 'videos.json';
$.ajax({
	url: url,
	dataType: 'json',
	type: 'GET',
	crossDomain: true,
	success: function(dataremote){
		// Adapt Videos Json format
		if (dataremote.hasOwnProperty('items')) {
			if (Array.isArray(dataremote.items)) {
				dataremote = dataremote.items;
			}
			else {
				dataremote = [ dataremote.items ];
			}
			ProcessCategoriesJson(dataremote);
		}
		else {
			ProcessCategoriesJson(JSON.parse(items));
			console.log('[ERROR] unknown videos list format');
		}
	},
	error: function(XHR, textStatus, errorThrown) {
		let imgarea = document.getElementById('curVideoPic');
		imgarea.setAttribute('border', 0);
		let trarea = document.getElementById('curVideoDesc');
		let parea = document.createElement("p");
		parea.setAttribute('align', 'left');
		parea.setAttribute('id', 'pCenter');
		parea.innerHTML = '<b>Network Error</b>' + '<br><br>Could not complete the operation due to an error';
		trarea.appendChild(parea);
		console.log(textStatus);
		console.log(errorThrown);
    }
 });