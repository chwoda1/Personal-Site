

mergeInto(LibraryManager.library , {

	jsDrawImage: function(x , y , imageNum) {
		context.drawImage(images[imageNum] , x , y);
	},

	jsDrawRectangle: function(x_position , height , color_number) {
		context.fillStyle = colors[color_number]; 
		context.fillRect(x_position , window.innerHeight - height , 60 , height); 
	
	},

});



