#include <emscripten/emscripten.h>
#include <stdlib.h> 
#include <assert.h> 
#include "game.h"

extern "C" {
	extern void jsDrawImage(int x , int y , int image_number);
	extern void jsDrawRectangle(int x_position , int random_num , int color_number); 
}

double time_sum = 0.0;
double spawn_rate = 3.0;
double animation_accumulator = 0; 

int score = 0; 
int image_counter = 0; 
int number_rects = 0; 
int about_flag = 0; 

int dir_right = 0; 
int dir_left = 0;  
int dir_jumping = 0; 
int falling;
int flag = 0;

int resetting; 
int colliding; 
int collision_index; 

long long x = 0; 
long long y = 0; 
  
int base_speed = 275; 
int max_speed = 325;

int canvas_x; 
int canvas_y; 
int lose_counter; 
int original_y;
int ceiling; 
int window_height; 

int locked = 0; 
Player player;

Rectangle rectangles[10];

extern "C" {

	void sort() {

		for (int i = 0 ; i < 9 ; i++) {
			for(int j = 0 ; j < 10 - i - 1 ; j++) {
				if (rectangles[j].x_position > rectangles[j + 1].x_position) {
					Rectangle temp = rectangles[j]; 
					rectangles[j] = rectangles[j+1];
					rectangles[j+1] = temp; 
				}
			}
		}

		// why is this here? 
		for (int i = 0 ; i < 10 ; i++) {
			if (rectangles[i].colliding == 1) {
				collision_index = i; 
				break; 
			}
		}
	}

	void EMSCRIPTEN_KEEPALIVE right(int key_event) {
	
		if(locked || about_flag)
			return;

		dir_right = key_event; 		
		
		if (key_event == 0 && locked == 0) {
			player.sprite_position = 7; 
			y = 0; 
		}
	
	}

	void EMSCRIPTEN_KEEPALIVE left(int key_event) {
	
		if (locked || about_flag)
			return;

		dir_left = key_event;
	
		if (key_event == 0 && locked == 0) {	
			player.sprite_position = 1; 
			x = 0; 
		}
	}
	
	void EMSCRIPTEN_KEEPALIVE jump() {
	
		if(locked || about_flag)
			return; 	

		dir_jumping = (falling != 1) ? 1 : 0; 
		
	}
	
	/** 
	 * Sets the about me flag to tell the board to clear without restarting 
	 **/ 
	void EMSCRIPTEN_KEEPALIVE set_flag() {
		
		if (about_flag == 0) {
			about_flag = 1;
		}
	 	else {
			about_flag = 0;
			reset();
		}
		
	}

	void EMSCRIPTEN_KEEPALIVE resize(int x_param , int y_param , int window) {

		canvas_x = x_param; 
		canvas_y = y_param; 
		lose_counter = canvas_y; 
		window_height = window; 

		ceiling = canvas_y / 2;

		if (!flag) {
			original_y = canvas_y - 50;
			player = (Player) {canvas_x / 2 , original_y , 0}; 
			flag = 1; // why do i name variables like this... Figure out what it means 
			
			for (int i = 0 ; i < 10 ; i++)
				rectangles[i].x_position = MAX; 
	
		}
	}

	void make_rectangles(double time_delta) {	
 
		time_sum += time_delta; 

		if (time_sum > spawn_rate) {
		
			time_sum = 0.0;

			if (score % 5 == 0 && base_speed < 500 && max_speed < 500) {
				base_speed += 25;
				max_speed += 25; 
				if (spawn_rate > 1)
					spawn_rate -= .25; 
			}
			

			int velocity = rand() % max_speed + base_speed; 
			int height = rand() % ((canvas_y/2) - 200) + 90; 
			int x_position = 0; 
			int direction = 0; 

			if (rand() % 2 == 1) {
				x_position = canvas_x; 
				direction = 1; // left
			} 

			Rectangle rect = { x_position , height , rand() % 5 , 0 , direction , velocity }; 
			
			rectangles[number_rects] = rect; 
			number_rects++;  	
			
		        int data = number_rects; 	

			for (int i = 0 ; i < data ; i++) {
	
				if (rectangles[i].x_position > canvas_x && rectangles[i].direction == 0) {
					rectangles[i].x_position = MAX; 
					number_rects--;
				        score++;	
				}
				else if (rectangles[i].x_position < 0) {
					rectangles[i].x_position = MAX;
					number_rects--; 
					score++;
				}

			}

			sort(); 
		}
	}

	void move_player(double time_delta) {
	
		double movement = time_delta * SPEED; 
		double fall_speed = time_delta * FALLING_SPEED;
		
		animation_accumulator += time_delta; 

		if (dir_right == 1) {

			if (player.x_position + movement > canvas_x - 2) 
				goto jumping;
			else {
				player.x_position += movement; 

				if (animation_accumulator > 0.125) {
					player.sprite_position = (x++ % 6) + 7;				
					animation_accumulator = 0; 
				}

			}
			
 	
		}

		else if (dir_left == 1) {

			if(player.x_position - movement < 0)
				goto jumping; // corner case handled  
			else {
				player.x_position -= movement;
				
				if (animation_accumulator > 0.125) {
					player.sprite_position = (y++ % 6) + 1;
					animation_accumulator = 0; 	
				}
			
			
			}
			 
		}
		
jumping: 
		if (dir_jumping == 1) {
		
			if (player.y_position - movement > ceiling) {
			
				player.y_position -= fall_speed + (GRAV * time_delta);
	
				player.sprite_position = (dir_right == 1) ? 16 : 15;
					
			}

			else {
				player.y_position -= fall_speed; 
				player.y_position -= GRAV * time_delta;
				falling = 1; 
				dir_jumping = 0; 
			}

		}

		else if (falling == 1) {
			
			if (player.y_position - movement < original_y) {
			
				player.y_position += fall_speed + (GRAV * time_delta); 			
				player.sprite_position = (dir_right == 1) ? 14 : 13;

			} 
			else {
				player.y_position = original_y; 
				falling = 0; 
				player.sprite_position = 0; 
			}	 
		}
	}

	void move_rectangles(double time_delta) { 
	
		for (int i = 0 ; i < number_rects ; i++) {
	
			double movement = time_delta * rectangles[i].velocity; 
	 
			switch (rectangles[i].direction) {

				case 0: 
					rectangles[i].x_position += movement; 
					break; 

				case 1: 
					rectangles[i].x_position -= movement;  
					break; 
			}
		}
	} 
	
	// use double sum and only move player sprite if > a certain value 
	void EMSCRIPTEN_KEEPALIVE update(double time_delta , double sum) {  

		if (resetting == 0) {
			
			printf("hello\n");
			move_player(time_delta);	 

			make_rectangles(time_delta);

			move_rectangles(time_delta);

			colliding = check_collision();

			rectangles[colliding].colliding = 1;

			if (colliding == -1) { 
				
				jsDrawImage(player.x_position , player.y_position , player.sprite_position);

				for (int i = 0 ; i < number_rects ; i++)
					jsDrawRectangle(rectangles[i].x_position , rectangles[i].height , rectangles[i].rect_color); 	 
			}
		}

		if (colliding >= 0 || resetting == 1 || about_flag == 1) {
			resetting = 1; 
			reset_board(time_delta);   
			
		}

	}

	/**
	 *
	 * Created this function AFTER desiging literally the entire game. Whenever the player loses pass in an index
	 * which then ID's the rectangle that collided with the player. This will tell the direction that the player was
	 * hit from.
	 *
	 * However, you can also pass in a -1 value. This will have a special use which will clear the board in a pretty way
	 * and keep it cleared so you can draw other stuff.
	 *
	 *
	 * @TODO remove that about_flag at some point. There has to be a better way 
	 *
	 **/ 
	void EMSCRIPTEN_KEEPALIVE reset_board(double time_delta) {
		
		double movement = time_delta * RESET_RECT; 
		int data = number_rects;

		for (int i = 0 ; i < data ; i++) {
			
			// switch this to a ternary operator, actually i like this better
			switch (rectangles[i].direction) {
				
				case 0: 
					rectangles[i].x_position -= movement; 
					break; 
				case 1: 
					rectangles[i].x_position += movement; 
					break; 
			}

			if (rectangles[i].x_position > canvas_x && rectangles[i].direction == 1) {
                                        rectangles[i].x_position = MAX;
                                        number_rects--; 
                        }   
			else if (rectangles[i].x_position < 0 - RECT_WIDTH && rectangles[i].direction == 0) {
				rectangles[i].x_position = MAX;
				number_rects--;
			}   
 
                        sort();

			jsDrawRectangle(rectangles[i].x_position , rectangles[i].height , rectangles[i].rect_color); 

			// this is where the -1 gets passed in

			if (about_flag == 0) {
				kill_player(time_delta , collision_index);

				if(number_rects == 0)
					reset();
			}

		}
	}

	int EMSCRIPTEN_KEEPALIVE get_score() { return score; }
	
	int check_collision() { 

		for (int i = 0 ; i < number_rects ; i++) {
			if (intersects(rectangles[i]) == 1)
				return i; 
		}			
	
		return -1;
	}

	void reset() { 
		base_speed = 275; 
		max_speed = 325;
		locked = 0;
		number_rects = 0; 
		score = 0; 
		lose_counter = canvas_y;
		resetting = 0;
		player = (Player) {canvas_x / 2 , original_y , 0}; 
		dir_jumping = 0; 
		dir_right = 0; 
	        dir_left = 0; 
		colliding = -1; 
		rectangles[collision_index].colliding = 0; 

	}

	void kill_player(double time_delta , int rect_lookup) {

		locked = 1;
		int deadly_direction = rectangles[rect_lookup].direction;
 		
		// going left 
		if (deadly_direction == 1) {
			player.sprite_position = 18;
			jsDrawImage(player.x_position - 10 , player.y_position - 10, player.sprite_position);
		}
		// going right 
		else {
			player.sprite_position = 17;
			jsDrawImage(player.x_position + 10 , player.y_position + 10 , player.sprite_position); 
		}

	}

	int intersects(Rectangle rect) {  

		if (rect.x_position < (player.x_position + 20) && rect.x_position + RECT_WIDTH > player.x_position &&
		    (window_height - rect.height) < (player.y_position + 45) && window_height > player.y_position) 
		   {
		
			return 1; 
		}

		return 0;
	}

	int main() { printf("WebAssembly ready and loaded\n"); }

}
