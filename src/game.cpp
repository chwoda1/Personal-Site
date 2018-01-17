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

int score = 0; 
int image_counter = 0; 
int number_rects = 0; 

int dir_right = 0; 
int dir_left = 0;  
int dir_jumping = 0; 
int falling;
int flag = 0;
int resetting; 

int x = 0; 
int y = 0; 
  
int base_speed = 275; 

int canvas_x; 
int canvas_y; 
int original_y;
int ceiling; 
int window_height; 

Player player;

Rectangle rectangles[10];

extern "C" {

	void sort() {
	
		for (int i = 1 ; i < 10 ; i++) {
		
			Rectangle to_insert = rectangles[i]; 

			int hole = i - 1; 

			while (hole >= 0 && rectangles[hole].x_position > to_insert.x_position ) {
				rectangles[hole+1] = rectangles[hole]; 
				hole--; 
			}
			
			rectangles[hole+1] = to_insert; 

		}
		for (int i = 1 ; i < 9 ; i++) {
			assert(rectangles[i].x_position <= rectangles[i+1].x_position && rectangles[i].x_position >= rectangles[i-1].x_position);
		}

	}

	void EMSCRIPTEN_KEEPALIVE right(int key_event) {
	
		dir_right = key_event; 		
		
		if (key_event == 0) {
			player.sprite_position = 7; 
			y = 0; 
		}
	
	}

	void EMSCRIPTEN_KEEPALIVE left(int key_event) {
		
		dir_left = key_event;
	
		if (key_event == 0) {
		
			player.sprite_position = 1; 
			x = 0; 
		}
	}
	
	void EMSCRIPTEN_KEEPALIVE jump(int key_event) {
		
		if (key_event == 1 && falling != 1) {
			dir_jumping = 1; 
		}
		else {
			dir_jumping = 0;
		}
	}

	void EMSCRIPTEN_KEEPALIVE resize(int x_param , int y_param , int window) {

		canvas_x = x_param; 
		canvas_y = y_param; 
		window_height = window; 

		ceiling = canvas_y / 2;

		if (!flag) {
			original_y = canvas_y - 50;
			player = (Player) {canvas_x / 2 , original_y , 0}; 
			flag = 1; 
			
			for (int i = 0 ; i < 10 ; i++)
				rectangles[i].x_position = MAX; 
	
		}
	}

	void make_rectangles(double time_delta) {	
 
		time_sum += time_delta; 

		if (time_sum > spawn_rate) {
		
			time_sum = 0.0; 

			int color_rand = rand() % 5;

			if (score % 5 == 0 && base_speed < 500) {
				base_speed += 25;

				if (spawn_rate > 1)
					spawn_rate -= .25; 
			}
			

			int velocity = rand() % 400 + base_speed; 
			int height = rand() % ((canvas_y/2) - 200) + 90; 
			int x_position = 0; 
			int direction = 0; 

			if (rand() % 2 == 1) {
				x_position = canvas_x; 
				direction = 1; 
			} 

			Rectangle rect = { x_position , height , color_rand , direction , velocity }; 
			
			rectangles[number_rects] = rect; 
			number_rects++;  	
			
		        int data = number_rects; 	

			for (int i = 0 ; i < data ; i++) {
	
				if (rectangles[i].x_position > canvas_x && rectangles[i].direction == 0) {
					rectangles[i].x_position = MAX; 
					number_rects--;
				        score++;	
				}
				else if (rectangles[i].x_position < 0 && rectangles[i].direction == 1) {
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

		if (dir_right == 1) {
		
			player.x_position += movement;  
			player.sprite_position = x++ % (12 - 7 + 1) + 7;
 	
		}

		else if (dir_left == 1) {
			player.x_position -= movement;
			player.sprite_position = (y++ % 7 + 1); 
		}
		
		if (dir_jumping == 1) {
		
			if (player.y_position - movement > ceiling) {
			
				player.y_position -= fall_speed; 
				player.y_position -= GRAV * time_delta; 
				if (dir_right == 1) {
					player.sprite_position = 16;
				}

				else {
					player.sprite_position = 15;
				}	
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
			
				player.y_position += fall_speed; 		
				player.y_position += GRAV * time_delta;

				if (dir_right == 1) {
					player.sprite_position = 14;
				}

				else {
					player.sprite_position = 13;
				}

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
	
	void EMSCRIPTEN_KEEPALIVE update(double time_delta) {

		int colliding; 

		if (resetting == 0) {

			move_player(time_delta);	 

			make_rectangles(time_delta);

			move_rectangles(time_delta);

			colliding = check_collision();
	
			if (colliding == 0) { 

				jsDrawImage(player.x_position , player.y_position , player.sprite_position);

				for (int i = 0 ; i < number_rects ; i++)
					jsDrawRectangle(rectangles[i].x_position,rectangles[i].height,rectangles[i].rect_color); 
		 
			}
		}

		if (colliding == 1 || resetting == 1) {
			resetting = 1; 

			int data = number_rects; 

			double movement = time_delta * RESET_RECT; 

			for (int i = 0 ; i < data ; i++) {
			
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

				jsDrawRectangle(rectangles[i].x_position,rectangles[i].height,rectangles[i].rect_color);

			} 
		
			if (number_rects == 0)
				reset(); 
			
		}
	}

	int EMSCRIPTEN_KEEPALIVE get_score() { return score; }

	int check_collision() { 

		for (int i = 0 ; i < number_rects ; i++) {
			if (intersects(rectangles[i]) == 1)
				return 1; 
		}			
	
		return 0;
	}

	void reset() { 
		number_rects = 0; 
		score = 0; 
		resetting = 0;
		player = (Player) {canvas_x / 2 , original_y , 0}; 
		dir_jumping = 0; 
		dir_right = 0; 
	}

	void kill_player(double time_delta) {
	
		if (dir_right == 1) {
		
		}

		else if (dir_left == 1) {
		
		}

		else if (dir_jumping == 1) {
		
		
		}

		else if (falling == 1) {
		
		}

		else {
		
		}
	
	}

	int intersects(Rectangle rect) {  

		if (rect.x_position < (player.x_position + 20) && rect.x_position + RECT_WIDTH > player.x_position &&
		    (window_height - rect.height) < (player.y_position + 46) && window_height > player.y_position) 
		   {
		
			return 1; 
		}

		return 0;
	}

	int main() { printf("WebAssembly ready and loaded\n"); }

}
