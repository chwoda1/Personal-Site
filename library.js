

mergeInto(LibraryManager.library , {

	jsDrawImage: function(x , y , imageNum) {
		context.drawImage(images[imageNum] , x , y , images[imageNum].width , images[imageNum].height);
		 
	},

	jsDrawRectangle: function(x_position , height , color_number) {
		context.globalAlpha = .8; 
		context.fillStyle = colors[color_number]; 
		
		context.fillRect(x_position , window.innerHeight - height , 30 , height); 
	 
		context.strokeStyle = 'black'; 
		context.lineWidth = 5; 
		context.strokeRect(x_position , window.innerHeight - height , 30 , height); 

	}
	
});



