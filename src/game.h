const int SPEED = 800; 
const int FALLING_SPEED = 800; 
const int MAX = 0xFFFFF; 
const int RECT_WIDTH = 20;
const int RESET_RECT = 600;
const float GRAV = 0.8f;

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

extern "C" int intersects(Rectangle rect); 
extern "C" int check_collision();
extern "C" void reset(); 
extern "C" void make_rectangles(double time_delta); 
extern "C" void move_rectangles(double time_delta); 
extern "C" void move_player(double movement); 
