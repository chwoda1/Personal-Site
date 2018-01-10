const int SPEED = 400; 
const int FALLING_SPEED = 600; 

typedef struct Player {
	double x_position; 
	double y_position; 

	/**
	 * Current sprite image
	 **/

	int sprite_position; 

} Player; 

typedef struct Rectangle {
	double x_position; 
	int height; 
	int rect_color; 

	int direction; 

	/** 
	 * Determines how fast the rectangle
	 * will move across the screen. Increase 
	 * as game continues
	 **/ 

	int velocity; 

} Rectangle; 

extern "C" void make_rectangles(double time_delta); 
extern "C" void move_rectangles(double time_delta); 
extern "C" void move_player(double movement); 
