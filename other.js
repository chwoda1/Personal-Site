const storageKey = "CHRISHighScore";
var initialize = false; 

function loadImages() {
	
	var sprites = new Array(); 	

	for (var i = 0 ; i < 19 ; i++) 
		sprites[i] = new Image(); 

	sprites[0].src = 'static/sprites/still_sprite.png';
	
	sprites[1].src = 'static/sprites/left_sprite_1.png'; 
	sprites[2].src = 'static/sprites/left_sprite_2.png'; 
	sprites[3].src = 'static/sprites/left_sprite_3.png'; 
	sprites[4].src = 'static/sprites/left_sprite_4.png'; 
	sprites[5].src = 'static/sprites/left_sprite_5.png';  
	sprites[6].src = 'static/sprites/left_sprite_6.png'; 
 
	sprites[7].src = 'static/sprites/right_sprite_1.png'; 
	sprites[8].src = 'static/sprites/right_sprite_2.png'; 
	sprites[9].src = 'static/sprites/right_sprite_3.png'; 
	sprites[10].src = 'static/sprites/right_sprite_4.png'; 
	sprites[11].src = 'static/sprites/right_sprite_5.png'; 
	sprites[12].src = 'static/sprites/right_sprite_6.png'; 
	
	sprites[13].src = 'static/sprites/falling_left.png'; 
	sprites[14].src = 'static/sprites/falling_right.png';

	sprites[15].src = 'static/sprites/jumping_left.png'; 
	sprites[16].src = 'static/sprites/jumping_right.png';

	sprites[17].src = 'static/sprites/dying_left.png'; 
	sprites[18].src = 'static/sprites/dying_right.png'; 

	images = sprites;
}

function initHS() {
	var scoreData = sessionStorage.getItem(storageKey); 

	if (initialize === false && scoreData === null) {		
		sessionStorage.setItem(storageKey , "0"); 
		scoreData = sessionStorage.getItem(storageKey); 
	}
	 
	return scoreData; 

}

function resize() {

	_resize(canvas.width , canvas.height , window.innerHeight);
        canvas.autofocus;
	context.canvas.width = window.innerWidth;
        context.canvas.height = window.innerHeight;

}

function clear() {

	var gameLoop = (timestamp) => {
	
	
	}
	

	context.fillText("Hi! I'm Chris and welcome to my site!"); 

}
