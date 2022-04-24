// [main.c]
// this template is provided for the 2D shooter game.

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 4 audios can be played at a time.
const int RESERVE_SAMPLES = 4;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;

enum {
	SCENE_MENU = 1,
	SCENE_START = 2
	// [HACKATHON 3-7] done
	// TODO: Declare a new scene id.
    //只是uncomment,可能是把第三個叫做settings
     , SCENE_SETTINGS = 3
     ,SCENE_WIN = 4
     ,SCENE_LOSE = 5
};

/* Input states */
int win=0;
int fail=0;
int touch=0;
int enemies_bomb[5]={0};
int level = 0;
// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...
int enemy_x,enemy_y;
/* Variables for allegro basic routines. */

ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1] done
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
//只有uncomment, 就像之前的task一樣，建立變數儲存圖片指標
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;
ALLEGRO_SAMPLE* win_bgm;
ALLEGRO_SAMPLE_ID win_bgm_id;
ALLEGRO_SAMPLE* lose_bgm;
ALLEGRO_SAMPLE_ID lose_bgm_id;



/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_doctor;
ALLEGRO_BITMAP* start_img_enemy_1;
ALLEGRO_BITMAP* start_img_enemy_2;
ALLEGRO_BITMAP* start_img_enemy_3;
ALLEGRO_BITMAP* img_choose1; 
ALLEGRO_BITMAP* img_choose2;
ALLEGRO_BITMAP* img_choose3;
ALLEGRO_BITMAP* img_choose;

ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;

// [HACKATHON 2-1] done
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
//因為img_bullet is image,so use ALLEGRO_BITMAP
ALLEGRO_BITMAP* img_teethbrush;
ALLEGRO_BITMAP* img_score;
ALLEGRO_BITMAP* img_easy;
ALLEGRO_BITMAP* img_hard;
ALLEGRO_BITMAP* img_horrible;
ALLEGRO_BITMAP* img_return;
ALLEGRO_BITMAP* img_again;
ALLEGRO_BITMAP* img_win;
ALLEGRO_BITMAP* img_lose;
ALLEGRO_BITMAP* img_blood;
ALLEGRO_BITMAP* img_fail; 
 
typedef struct {
	// The center coordinate of the image.
	float x, y;
	// The width and height of the object.
	float w, h;
	// The velocity in x, y axes.
	float vx, vy;
	// Should we draw this object on the screen.
	bool hidden;
	// The pointer to the object’s image.
	ALLEGRO_BITMAP* img;
} MovableObject;
void draw_movable_object(MovableObject obj);
//#define MAX_ENEMY 5
int MAX_ENEMY;
// [HACKATHON 2-2] done
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
//反正就先用4個子彈看看囉
#define MAX_BULLET 5
MovableObject plane;
MovableObject enemies[5];
MovableObject tem_plane;
MovableObject tem_enemies[5] = {-80};
// [HACKATHON 2-3]done
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
//從上面那兩行發現子彈可能也會動所以是movableobject,也可以從後面的code看到有draw movable objects的function，推測bullets array應該是movable object
MovableObject bullets[MAX_BULLET];
MovableObject tem_bullets[MAX_BULLET] = {-80};


// [HACKATHON 2-4]done
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
//就試試看0.2f
const float MAX_COOLDOWN = 0.2f;
double last_shoot_timestamp;
double tem=0;

/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]done
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
//只有uncomment吧，宣告function
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv) {
	// Set random seed for better random outcome.
	srand(time(NULL));
	allegro5_init();
	game_log("Allegro5 initialized");
	game_log("Game begin");
	// Initialize game variables.
	game_init();
	game_log("Game initialized");
	// Draw the first frame.
	game_draw();
	game_log("Game start event loop");
	// This call blocks until the game is finished.
	game_start_event_loop();
	game_log("Game end");
	game_destroy();
	return 0;
}

void allegro5_init(void) {
	if (!al_init())
		game_abort("failed to initialize allegro");

	// Initialize add-ons.
	if (!al_init_primitives_addon())
		game_abort("failed to initialize primitives add-on");
	if (!al_init_font_addon())
		game_abort("failed to initialize font add-on");
	if (!al_init_ttf_addon())
		game_abort("failed to initialize ttf add-on");
	if (!al_init_image_addon())
		game_abort("failed to initialize image add-on");
	if (!al_install_audio())
		game_abort("failed to initialize audio add-on");
	if (!al_init_acodec_addon())
		game_abort("failed to initialize audio codec add-on");
	if (!al_reserve_samples(RESERVE_SAMPLES))
		game_abort("failed to reserve samples");
	if (!al_install_keyboard())
		game_abort("failed to install keyboard");
	if (!al_install_mouse())
		game_abort("failed to install mouse");
	// TODO: Initialize other addons such as video, ...

	// Setup game display.
	game_display = al_create_display(SCREEN_W, SCREEN_H);
	if (!game_display)
		game_abort("failed to create display");
	al_set_window_title(game_display, "I2P(I)_2019 Final Project <student_id>");

	// Setup update timer.
	game_update_timer = al_create_timer(1.0f / FPS);
	if (!game_update_timer)
		game_abort("failed to create timer");

	// Setup event queue.
	game_event_queue = al_create_event_queue();
	if (!game_event_queue)
		game_abort("failed to create event queue");

	// Malloc mouse buttons state according to button counts.
	const unsigned m_buttons = al_get_mouse_num_buttons();
	game_log("There are total %u supported mouse buttons", m_buttons);
	// mouse_state[0] will not be used.
	mouse_state = malloc((m_buttons + 1) * sizeof(bool));
	memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

	// Register display, timer, keyboard, mouse events to the event queue.
	al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
	al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
	al_register_event_source(game_event_queue, al_get_keyboard_event_source());
	al_register_event_source(game_event_queue, al_get_mouse_event_source());
	// TODO: Register other event sources such as timer, video, ...

	// Start the timer to update and draw the game.
	al_start_timer(game_update_timer);
}

void game_init(void) {
	/* Shared resources*/
	font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
	if (!font_pirulen_32)
		game_abort("failed to load font: pirulen.ttf with size 32");

	font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
	if (!font_pirulen_24)
		game_abort("failed to load font: pirulen.ttf with size 24");

	/* Menu Scene resources*/
	main_img_background = load_bitmap_resized("cover.png", SCREEN_W, SCREEN_H);

	main_bgm = al_load_sample("start.ogg");
	if (!main_bgm)
		game_abort("failed to load audio: S31-Night Prowler.ogg");
	win_bgm = al_load_sample("win~.ogg");
	if (!win_bgm)
		game_abort("failed to load audio: win~.ogg");
	lose_bgm = al_load_sample("lose~.ogg");
	if (!lose_bgm)
		game_abort("failed to load audio: lose~.ogg");
	// [HACKATHON 3-4]done
	// TODO: Load settings images.
	// Don't forget to check their return values.
	// Uncomment and fill in the code below.
    //在task裡面應該處理過類似的東西了，記得把圖片放進同一個資料夾
	img_settings = al_load_bitmap("settings.png");
	if (!img_settings)
		game_abort("failed to load image: settings.png");
    img_settings2 = al_load_bitmap("settings2.png");
    if (!img_settings2)
        game_abort("failed to load image: settings2.png");

	/* Start Scene resources*/
	start_img_background = load_bitmap_resized("room.png", SCREEN_W, SCREEN_H);

	start_img_doctor = al_load_bitmap("doctor.png");
	if (!start_img_doctor)
		game_abort("failed to load image: doctor.png");

	start_img_enemy_1 = al_load_bitmap("enemy 1.png");
	img_choose =  start_img_enemy_1;
	if (!start_img_enemy_1)
		game_abort("failed to load image: enemy 1.png");
	start_img_enemy_2 = al_load_bitmap("enemy 2.png");
	if (!start_img_enemy_2)
		game_abort("failed to load image: enemy 2.png");
	start_img_enemy_3 = al_load_bitmap("enemy 3.png");
	if (!start_img_enemy_3)
		game_abort("failed to load image: enemy 3.png");

	start_bgm = al_load_sample("brush teeth song.ogg");
	if (!start_bgm)
		game_abort("failed to load audio: start.ogg");

	// [HACKATHON 2-5]done
	// TODO: Initialize bullets.
	// 1) Search for a bullet image online and put it in your project.
	//    You can use the image we provided.
	// 2) Load it in by 'al_load_bitmap' or 'load_bitmap_resized'.
	// 3) If you use 'al_load_bitmap', don't forget to check its return value.
	// Uncomment and fill in the code below.
    //跟task一樣
    img_teethbrush = al_load_bitmap("brush teeth.png");
    if (!img_teethbrush)
        game_abort("failed to load image: brush teeth.png");
    img_score = al_load_bitmap("score.png");
    if (!img_score)
        game_abort("failed to load image: score.png");
    img_easy = al_load_bitmap("easy.png");
    if (!img_easy)
        game_abort("failed to load image: easy.png");
    img_hard = al_load_bitmap("hard.png");
    if (!img_hard)
        game_abort("failed to load image: hard.png");
    img_horrible = al_load_bitmap("horrible.png");
    if (!img_horrible)
        game_abort("failed to load image: horrible.png");
    img_return = al_load_bitmap("return.png");
    if (!img_return)
        game_abort("failed to load image: return.png");
    img_again = al_load_bitmap("again.png");
    if (!img_again)
        game_abort("failed to load image: again.png");
    img_win = al_load_bitmap("win.png");
    if(!img_win)
        game_abort("failed to load image: win.png");
    img_lose = al_load_bitmap("lose.png");
    if(!img_lose)
        game_abort("failed to load image: lose.png");
	// Change to first scene.
	img_blood = al_load_bitmap("blood.png");
    if(!img_blood)
        game_abort("failed to load image: blood.png");
    img_fail = al_load_bitmap("fail.png");
    if(!img_fail)
        game_abort("failed to load image: fail.png");
    img_choose1 = al_load_bitmap("choose1.png");
    if(!img_choose1)
        game_abort("failed to load image: choose1.png");
    img_choose2 = al_load_bitmap("choose2.png");
    if(!img_choose2)
        game_abort("failed to load image: choose2.png");
    img_choose3 = al_load_bitmap("choose3.png");
    if(!img_choose3)
        game_abort("failed to load image: choose3.png");
	game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
	bool done = false;
	ALLEGRO_EVENT event;
	int redraws = 0;
	while (!done) {
		al_wait_for_event(game_event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Event for clicking the window close button.
			game_log("Window close button clicked");
			done = true;
		} else if (event.type == ALLEGRO_EVENT_TIMER) {
			// Event for redrawing the display.
			if (event.timer.source == game_update_timer)
				// The redraw timer has ticked.
				redraws++;
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			// Event for keyboard key down.
			game_log("Key with keycode %d down", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = true;
		} else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			// Event for keyboard key up.
			game_log("Key with keycode %d up", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			// Event for mouse key down.
			game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = true;
			on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// Event for mouse key up.
			game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (event.mouse.dx != 0 || event.mouse.dy != 0) {
				// Event for mouse move.
				game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
			} else if (event.mouse.dz != 0) {
				// Event for mouse scroll.
				game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
			}
		}
		// TODO: Process more events and call callbacks by adding more
		// entries inside Scene.

		// Redraw
		if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
			// if (redraws > 1)
			// 	game_log("%d frame(s) dropped", redraws - 1);
			// Update and draw the next frame.
			game_update();
			game_draw();
			redraws = 0;
		}
	}
}

void game_update(void) {
	if (active_scene == SCENE_START) {
		plane.vx = plane.vy = 0;
		if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
			plane.vy -= 1;
		if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
			plane.vy += 1;
		if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
			plane.vx -= 1;
		if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
			plane.vx += 1;
		// 0.71 is (1/sqrt(2)).
		plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
		plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
		// [HACKATHON 1-1]kind of done
		// TODO: Limit the plane's collision box inside the frame.
		//       (x, y axes can be separated.)
		// Uncomment and fill in the code below.
        //x,y是中心點，w,h是長寬，如果中心點的x加上一半的w會跑出左界或右界，要把他調回來，把中心點設定在離邊界一半w的地方，同理可知道y
        if (plane.x-plane.w/2 < 0)
            plane.x = plane.w/2;
        else if (plane.x+plane.w/2 > SCREEN_W)
            plane.x = SCREEN_W-plane.w/2;
        if (plane.y-plane.h/2 < 0)
            plane.y = plane.h/2;
        else if (plane.y+plane.h/2 > SCREEN_H)
            plane.y = SCREEN_H-plane.h/2;
        int i,j;
        // [HACKATHON 2-7] done not sure about #401
		// TODO: Update bullet coordinates.
		// 1) For each bullets, if it's not hidden, update x, y
        // according to vx, vy. #not sure
		// 2) If the bullet is out of the screen, hide it.
		// Uncomment and fill in the code below.
        //大概有一些keyboard event會輸入到board.vx 跟board.vy，所以用迴圈讀取bullet array，判斷如果子彈現在沒有在螢幕上（被回收了），就跳出迴圈，然後根據那個i(那個子彈)，把他加上他動的距離。
        for (i=0;i<MAX_BULLET;i++) {
            if (bullets[i].hidden)
                continue;
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;//???
            if (bullets[i].x < 0 || bullets[i].y<0)
                bullets[i].hidden = true;
        }

		// [HACKATHON 2-8]done
		// TODO: Shoot if key is down and cool-down is over.
		// 1) Get the time now using 'al_get_time'.
		// 2) If Space key is down in 'key_state' and the time
		//    between now and last shoot is not less that cool
		//    down time.
		// 3) Loop through the bullet array and find one that is hidden.
		//    (This part can be optimized.)
		// 4) The bullet will be found if your array is large enough.
		// 5) Set the last shoot time to now.
		// 6) Set hidden to false (recycle the bullet) and set its x, y to the
		//    front part of your plane.
		// Uncomment and fill in the code below.
        double now = al_get_time();//用get time get time
        double now2 = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
            for (i = 0; i<MAX_BULLET;i++) {
                if (bullets[i].hidden)
                    break;
            }
            if (i < MAX_BULLET) {
                last_shoot_timestamp = now;
                bullets[i].hidden = false;
                bullets[i].x = plane.x;//從那個x射出去
                bullets[i].y = plane.y-(plane.h)/2;//要從中心點減一半高度
            }
        }
        //enemy update
        for (j=0;j<MAX_ENEMY;j++) {
            if (enemies[j].hidden){
            	enemies[j].hidden = false;
        		enemies[j].x = enemies[j].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[j].w);
        		enemies[j].y = 0;
			}
            else {
			if(level == 0){
				enemies[j].x += enemies[j].vx;
				enemies[j].y += enemies[j].vy;
			}
			else{
				if(now2 - tem >= 1){
					tem = now2 ;
					for(i=0;i<MAX_ENEMY;i++)
					enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
				}
				enemies[j].y += enemies[j].vy;
			} 
            if (enemies[j].y>SCREEN_H){
                fail++;
            	enemies[j].hidden = true;
            	enemies[j].vy++;
            	if(fail == 2*MAX_ENEMY) game_change_scene(SCENE_LOSE);
			}
			//子彈和敵人碰到 
			for(i=0;i<MAX_BULLET;i++){
        			if(bullets[i].x+bullets[i].w/2+20>enemies[j].x&&bullets[i].x-bullets[i].w/2-20<enemies[j].x&&bullets[i].y-bullets[i].h/2-20<enemies[j].y&&bullets[i].y+bullets[i].h/2+20>enemies[j].y){
        						bullets[i].hidden = true;
        						bullets[i].x = -80;
        						bullets[i].y = -80;
        						enemies_bomb[j]++;
        						game_log("bomb");
        						if(enemies_bomb[j]==2){
        							enemies[j].hidden = true;
        							enemies[j].vy++;
        							enemies_bomb[j]=0;
        							win++;
        							if(win == 4*MAX_ENEMY)
        							game_change_scene(SCENE_WIN);
								}
					}	
		}
			if((plane.x-plane.w/2<enemies[j].x+enemies[j].w/2&&plane.x-plane.w/2>enemies[j].x-enemies[j].w/2)||(plane.x+plane.w/2<=enemies[j].x+enemies[j].w/2&&plane.x+plane.w/2>enemies[j].x-enemies[j].w/2)){
				if((plane.y-plane.h/2<enemies[j].y+enemies[j].h/2&&plane.y-plane.h/2>enemies[j].y-enemies[j].h/2)||(plane.y+plane.h/2<=enemies[j].y+enemies[j].h/2&&plane.y+plane.h/2>enemies[j].y-enemies[j].h/2)){
				    	touch++;
				    	plane.x = 400;
				    	plane.y = 500;
				    	if(touch == 4){
				    		game_change_scene(SCENE_LOSE);
						}
					}
			}					    
        }}		
}}


void game_draw(void) {
	if (active_scene == SCENE_MENU) {
		al_draw_bitmap(main_img_background, 0, 0, 0);
		al_draw_bitmap(img_easy,SCREEN_W/7,SCREEN_H/3,0);
		al_draw_bitmap(img_hard,3*SCREEN_W/7,SCREEN_H/3,0);
		al_draw_bitmap(img_horrible,5*SCREEN_W/7,SCREEN_H/3,0);
		// [HACKATHON 3-5]done
		// TODO: Draw settings images.
		// The settings icon should be located at (x, y, w, h) =
		// (SCREEN_W - 48, 10, 38, 38).
		// Change its image according to your mouse position.
		// Uncomment and fill in the code below.
        //照著填進來
        //如果那個點在長方形裡面（使用者滑鼠有點到settings按鈕），就把icon改成img_settings2，不然就照原本的
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W-48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W-48, 10, 0);
	} else if (active_scene == SCENE_START) { 
		int i;
		al_draw_bitmap(start_img_background, 0, 0, 0);
		// [HACKATHON 2-9]done
		// TODO: Draw all bullets in your bullet array.
		// Uncomment and fill in the code below.
        //參照下面ENEMY的作法，同理類推
        for (i=0;i<MAX_BULLET;i++)
            draw_movable_object(bullets[i]);
		draw_movable_object(plane);
		for (i = 0; i < MAX_ENEMY; i++)
			draw_movable_object(enemies[i]);
		al_draw_text(font_pirulen_32, al_map_rgb(100, 100, 0), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "blood 3");
		if(MAX_ENEMY == 2){
			al_draw_text(font_pirulen_32, al_map_rgb(100, 255, 255), SCREEN_W / 2, 60, ALLEGRO_ALIGN_CENTER, "EASY");
		}
		else if(MAX_ENEMY == 5){
			if(level == 0)
			al_draw_text(font_pirulen_32, al_map_rgb(100, 255, 255), SCREEN_W / 2, 60, ALLEGRO_ALIGN_CENTER, "HARD");
			else
			al_draw_text(font_pirulen_32, al_map_rgb(100, 255, 255), SCREEN_W / 2, 60, ALLEGRO_ALIGN_CENTER, "HORRIBLE");

		}
		for (i=0;i<3-touch;i++)
            al_draw_bitmap(img_blood, i*48, 10, 0);
        for (i=0;i<win;i++)
            al_draw_bitmap(img_score, SCREEN_W-(i+1)*48, 10, 0);
        for (i=0;i<fail;i++)
            al_draw_bitmap(img_fail, i*48, 50, 0);
		
	}
	// [HACKATHON 3-9]done
	// TODO: If active_scene is SCENE_SETTINGS.
	// Draw anything you want, or simply clear the display.
    //佈置切換到settings頁面的樣子，照著打就是變全黑
    else if (active_scene == SCENE_SETTINGS) {
    	al_clear_to_color(al_map_rgb(0,0,0));
    	al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "change your rival");
        al_draw_bitmap(img_choose1, SCREEN_W/5, SCREEN_H/3, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 100, 0), SCREEN_W/4, 3*SCREEN_H/4, ALLEGRO_ALIGN_CENTER, "1. I am hurt!");
        al_draw_bitmap(img_choose2, 2*SCREEN_W/5, SCREEN_H/3, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), SCREEN_W/2, SCREEN_H/4, ALLEGRO_ALIGN_CENTER, "2. nothing ><");
        al_draw_bitmap(img_choose3, 3*SCREEN_W/5, SCREEN_H/3, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 3*SCREEN_W/4, 3*SCREEN_H/4, ALLEGRO_ALIGN_CENTER, "3. enjoying~~");
     }
	al_flip_display();
}

void game_destroy(void) {
	// Destroy everything you have created.
	// Free the memories allocated by malloc or allegro functions.
	// Destroy shared resources.
	al_destroy_font(font_pirulen_32);
	al_destroy_font(font_pirulen_24);

	/* Menu Scene resources*/
	al_destroy_bitmap(main_img_background);
	al_destroy_sample(main_bgm);
	// [HACKATHON 3-6]done
	// TODO: Destroy the 2 settings images.
	// Uncomment and fill in the code below.
    //如同task,destroy資源
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);

	/* Start Scene resources*/
	al_destroy_bitmap(start_img_background);
	al_destroy_bitmap(start_img_doctor);
	al_destroy_bitmap(start_img_enemy_1);
	al_destroy_bitmap(start_img_enemy_2);
	al_destroy_bitmap(start_img_enemy_3);
	al_destroy_sample(start_bgm);
	// [HACKATHON 2-10]done
	// TODO: Destroy your bullet image.
	// Uncomment and fill in the code below.
    //跟上面一樣
    al_destroy_bitmap(img_teethbrush);

	al_destroy_timer(game_update_timer);
	al_destroy_event_queue(game_event_queue);
	al_destroy_display(game_display);
	free(mouse_state);
}

void game_change_scene(int next_scene) {
	game_log("Change scene from %d to %d", active_scene, next_scene);
	// TODO: Destroy resources initialized when creating scene.
	if (active_scene == SCENE_MENU) {
		al_stop_sample(&main_bgm_id);
		game_log("stop audio (bgm)");
	} else if (active_scene == SCENE_START) {
		al_stop_sample(&start_bgm_id);
		game_log("stop audio (bgm)");
	}
	active_scene = next_scene;
	// TODO: Allocate resources before entering scene.
	if (active_scene == SCENE_MENU) {
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&lose_bgm_id);
	if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
		game_abort("failed to play audio (bgm)");
	} else if (active_scene == SCENE_START) {
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&lose_bgm_id);
		int i;
		plane.img = start_img_doctor;
		plane.x = 400;
		plane.y = 500;
		plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);
		for (i = 0; i < MAX_ENEMY; i++) {
			enemies[i].img = img_choose;
			enemies[i].w = al_get_bitmap_width(img_choose);
			enemies[i].h = al_get_bitmap_height(img_choose);
			enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
			enemies[i].y = 0;
			enemies[i].vy = 1 ;
            }
			//enemies[i].hidden = true;
		// [HACKATHON 2-6]done
		// TODO: Initialize bullets.
		// For each bullets in array, set their w and h to the size of
		// the image, and set their img to bullet image, hidden to true,
		// (vx, vy) to (0, -3).
		// Uncomment and fill in the code below.
        //照著指示做，自行理解
        for (i=0;i<MAX_BULLET;i++) {
            bullets[i].w = al_get_bitmap_width(img_teethbrush);
            bullets[i].h = al_get_bitmap_height(img_teethbrush);
            bullets[i].img = img_teethbrush;
            bullets[i].vx = 0;
            bullets[i].vy = -3;
            bullets[i].hidden = true;
        }
		if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if(active_scene == SCENE_WIN){
		al_draw_bitmap(img_win, 0, 0, 0);
		if (!al_play_sample(win_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &win_bgm_id))
			game_abort("failed to play audio (bgm)");
		al_draw_bitmap(img_return, 5*SCREEN_W/7, 7*SCREEN_H/10, 0);
		al_draw_bitmap(img_again, 5*SCREEN_W/7, 10*SCREEN_H/12, 0);
	}
	else if(active_scene == SCENE_LOSE){
		al_draw_bitmap(img_lose, 0, 0, 0);
		if (!al_play_sample(lose_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &lose_bgm_id))
			game_abort("failed to play audio (bgm)");
		al_draw_bitmap(img_return, 5*SCREEN_W/7, 7*SCREEN_H/10, 0);
		al_draw_bitmap(img_again, 5*SCREEN_W/7, 10*SCREEN_H/12, 0);
	}
}

void on_mouse_down(int btn, int x, int y) {
	// [HACKATHON 3-8]done
	// TODO: When settings clicked, switch to settings scene.
	// Uncomment and fill in the code below.
    // 從上面的code(event loop那裡) 可以看到會傳mouse event給btn，如果event.mouse.button==1代表左鍵點下去，可參考task5，所以點下去的時候就是要切換到settings
    if (active_scene == SCENE_MENU) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W-48, 10, 38, 38))
                game_change_scene(SCENE_SETTINGS);
            if (pnt_in_rect(x,y,115,200,150,90)){
            	level = 0;
				MAX_ENEMY = 2; 
            	game_change_scene(SCENE_START);
			}
			if(pnt_in_rect(x,y,345,200,150,90)){
				level = 0;
				MAX_ENEMY = 5;
				game_change_scene(SCENE_START);
			}
			if(pnt_in_rect(x,y,570,200,150,90)){
				MAX_ENEMY = 5;
				level=1;
				game_change_scene(SCENE_START);
			}
        }
    }
    if (active_scene == SCENE_SETTINGS) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, 160, 200, 150, 200)){
            	img_choose = start_img_enemy_1;
            	game_change_scene(SCENE_MENU);
			}
            if (pnt_in_rect(x,y,320,200,150,200)){
            	img_choose = start_img_enemy_2;
            	game_change_scene(SCENE_MENU);
			}
			if(pnt_in_rect(x,y,480,200,150,200)){
				img_choose = start_img_enemy_3;
            	game_change_scene(SCENE_MENU);
			}
        }
    }
    if(active_scene == SCENE_WIN||active_scene == SCENE_LOSE){
win=0;
fail=0;
touch=0;
int i;
for(i=0;i<MAX_ENEMY;i++)
enemies_bomb[i]=0;
		if (pnt_in_rect(x, y, 575,452 , 150, 80)){
				game_change_scene(SCENE_MENU);
			}
		if (pnt_in_rect(x, y, 575,500 , 150, 80)){
            	img_choose = start_img_enemy_1;
            	game_change_scene(SCENE_START);
			}
	}
}

void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
		game_abort("failed to load image: %s", filename);
	ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		al_get_bitmap_width(loaded_bmp),
		al_get_bitmap_height(loaded_bmp),
		0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);
	game_log("resized image: %s", filename);
	return resized_bmp;
}

// [HACKATHON 3-3]done
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
// 怎麼判斷一個點是不是在一個長方形裡面？
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    return (px>=x &&px<=x+w &&py>=y &&py<=py+h)? true:false;
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}
