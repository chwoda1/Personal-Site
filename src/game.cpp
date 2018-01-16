#include <emscripten/emscripten.h>
#include <stdlib.h> 
#include <math.h> 
#include "game.h"
 
extern "C" {
	extern void jsDrawImage(int x , int y , int image_number);
	extern void jsDrawRectangle(int x_position , int random_num , int color_number); 
}

double time_sum = 0.0;

int score = 0; 
int image_counter = 0; 
int number_rects = 0; 

int dir_right = 0; 
int dir_left = 0;  
int dir_jumping = 0; 
int falling;
int flag = 0;

int x = 0; 
int y = 0; 

int spawn_rate = 2;  
int base_speed = 75; 

int canvas_x; 
int canvas_y; 
int original_y;
int ceiling; 
int window_height; 

Player player;

Rectangle rectangles[10];

extern "C" {

	int compare_func(const void *a , const void *b) {
	
		Rectangle *rect1 = (Rectangle*) a;
		Rectangle *rect2 = (Rectangle*) b; 

		if (rect1 -> x_position == rect2 -> x_position) {
			return 0; 
		}

		else if (rect1 -> x_position > rect2 -> x_position) {
			return 1; 
		}
		else {
			return -1; 
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

		if (time_sum > 2) {
		
			time_sum = 0.0; 

			int color_rand = rand() % 5;
			int velocity = rand() % 200 + 100; 
			int height = rand() % 100 + 20; 
			int x_position = 0; 
			int direction = 0; 

			if (rand() % 2 == 1) {
				x_position = canvas_x; 
				direction = 1; 
			} 

			Rectangle rect = { x_position , height , color_rand , direction , velocity }; 
			
			rectangles[number_rects] = rect; 
			number_rects++;  	
			 
			for (int i = 0 ; i < number_rects ; i++) {
			
				if (rectangles[i].x_position > canvas_x || rectangles[i].x_position < 0) {
					rectangles[i].x_position = MAX; 
					score++; 
					number_rects--; 
				}
			}

			qsort(rectangles , 10 , sizeof(Rectangle) , compare_func);
	 
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

				if (dir_right == 1) {
					player.sprite_position = 16;
				}

				else {
					player.sprite_position = 15;
				}	
			}

			else {
				player.y_position = ceiling; 
				falling = 1; 
				dir_jumping = 0; 
			}

		}

		else if (falling == 1) {
			
			if (player.y_position + movement < original_y) {
			
				player.y_position += fall_speed; 		
			
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

		move_player(time_delta);	 

		make_rectangles(time_delta); 

		move_rectangles(time_delta); 

		jsDrawImage(player.x_position , player.y_position , player.sprite_position); 

		int colliding = check_collision(number_rects/2); 	

		if (colliding == 0) {
			for (int i = 0 ; i < 10 ; i++)
				jsDrawRectangle(rectangles[i].x_position,rectangles[i].height,rectangles[i].rect_color); 
		}
		else {

			


		}
	}

	int EMSCRIPTEN_KEEPALIVE get_score() { return score; }

	int check_collision(int pivot) { 

		int answer; 

		if (rectangles[pivot].x_position >= player.x_position) {
			
			for (int i = 0 ; i <= pivot ; i++) {
				
				answer = intersects(rectangles[i]); 

				if (answer == 1)
					break;  

			}
		}

		else {

			for (int i = pivot ; i < number_rects ; i++) {
				
				answer = intersects(rectangles[i]); 

				if (answer == 1)
					break; 

			} 	
		}		 

		if (answer == 1){ 
			return -1; 
		}
	
		return 0;
	}

	void reset() { 
		 
		for(int i = 0 ; i < number_rects ; i++)
			rectangles[i].x_position = MAX; 

		number_rects = 0; 
		score = 0; 
		player = (Player) {canvas_x / 2 , original_y , 0}; 
	
	}

	int intersects(Rectangle rect) {  

		if (rect.x_position <= (player.x_position + 20) && rect.x_position + RECT_HEIGHT >= player.x_position &&
		    (window_height - rect.height) <= (player.y_position + 46) && window_height >= player.y_position) 
		   {
		
			return 1; 
		}

		return 0;
	}

	int main() { printf("WebAssembly ready and loaded\n"); }

}
