int SPEED = 130; 
int FALLING_SPEED = 100; 
int MAX = 0xFFFFF; 
int RECT_WIDTH = 30;
int RESET_RECT = 600;
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
	int colliding; 
	int direction; 

	/** 
	 * Determines how fast the rectangle
	 * will move across the screen. Increase 
	 * as game continues
	 **/ 

	int velocity; 

} Rectangle; 

extern "C" void kill_player(double time_delta , int rect); 
extern "C" int intersects(Rectangle rect); 
extern "C" int check_collision();
extern "C" void reset(); 
extern "C" void make_rectangles(double time_delta); 
extern "C" void move_rectangles(double time_delta); 
extern "C" void move_player(double movement); 
extern "C" void reset_board(double time_delta); 
extern "C" int get_ratio(int size); 
