const int SPEED = 400; 
const int FALLING_SPEED = 600; 

const int RECT_HEIGHT = 60;

typedef struct Sprite {

	int width; 
	int height; 

}Sprite; 

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
extern "C" void check_collision(int pivot);
extern "C" void reset(); 
extern "C" void make_rectangles(double time_delta); 
extern "C" void move_rectangles(double time_delta); 
extern "C" void move_player(double movement); 
