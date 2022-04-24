#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdlib.h>

#define GAME_TERMINATE -1

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *image2 = NULL;
ALLEGRO_BITMAP *image3 = NULL;
ALLEGRO_BITMAP *image4 = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *start_background= NULL;
ALLEGRO_KEYBOARD_STATE keyState ;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *timer2 = NULL;
ALLEGRO_TIMER *timer3 = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_FONT *font = NULL;


//Custom Definition
const char *title = "Final Project 100";
const float FPS = 60;
const int WIDTH = 1000;
const int HEIGHT = 600;
typedef struct character
{
    int x;
    int y;
    bool hidden;
    ALLEGRO_BITMAP *image_path;

} Character;

Character character1;
Character character2;
Character character3;
Character character4;
Character character_attack;
//Character characterW;

int imageWidth = 0;
int imageHeight = 0;
int draw = 0;

int done = 0;
int window = 1;
int flag=0;
int temp=0;
bool judge_next_window = false;
bool ture = true; //true: appear, false: disappear
bool next = false; //true: trigger
bool dir = true; //true: left, false: right

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
void game_destroy();

int main(int argc, char *argv[])
{
    int msg = 0;

    game_init();
    printf("Game initialized\n");
    game_begin();
    printf("Game begin\n");
    while (msg != GAME_TERMINATE)
    {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
    }
    printf("Game end");
    game_destroy();
    return 0;
}

void show_err_msg(int msg)
{
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init()
{
    if (!al_init())
    {
        show_err_msg(-1);
    }
    if(!al_install_audio())
    {
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-2);
    }
    if(!al_init_acodec_addon())
    {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-3);
    }
    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-4);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL)
    {
        show_err_msg(-5);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display)); //讓event queue 知道有display這種型態的event 包括關掉視窗
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void game_begin()
{
    // Load sound
    song = al_load_sample( "hello.wav" );
    if (!song)
    {
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-6);
    }
    // Loop the song until the display closes
    al_play_sample(song, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_clear_to_color(al_map_rgb(100,100,100));
    start_background = al_load_bitmap("jacket.jpg");
    al_draw_bitmap(start_background,0,0,0);
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",12,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220, ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_rectangle(WIDTH/2-150, 100, WIDTH/2+150, 100, al_map_rgb(255, 255, 255), 0);
    //background = al_load_bitmap("back.jpg");
    //al_draw_bitmap(background, 100, 0, 0);
    al_flip_display();
}

int game_loop(void)
{
    if(flag==1)
    {
        if(character2.x-character1.x<=50||character2.x-character1.x>=-50&&character2.y-character1.y<=50||character2.y-character1.y>=-50)
        {
            return 1;
        }
        else
        {
            return 0;
        }

    }
    else if(flag==2)
    {
        if(character1.x==character3.x&&character1.y==character3.y)
        {
            return  1;
        }
        else
        {
            return 0;
        }
    }
    else if(flag==3)
    {

    }

}

int process_event()
{
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    // Our setting for controlling animation
    if(event.timer.source == timer)
    {
        if(character2.x < WIDTH/2)
            dir = false;
        else if(character2.x > WIDTH+50)
            dir = true;

        if(dir)
            character2.x -= 10;
        else
            character2.x += 10;
    }

    if(event.timer.source == timer2)
    {
        ture = false;
        next = true;
    }
    if(event.timer.source == timer3)
    {
        if(next)
            next = false;
        else
            ture = true;
    }
    //  int temp=0;
    // Keyboard
    if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(event.keyboard.keycode)
        {
        // Control
        case ALLEGRO_KEY_LEFT:
            character1.x -= 30;
            break;
        case ALLEGRO_KEY_RIGHT:
            character1.x += 30;
            break;
            printf("%d",temp);
        case ALLEGRO_KEY_SPACE:
            temp=1;
            break;
        // For Start Menu
        case ALLEGRO_KEY_ENTER:
            judge_next_window = true;
            flag++;
            break;
        case ALLEGRO_KEY_ESCAPE:
            return GAME_TERMINATE;    
        }
    }
    if(temp==1)
    {
        character1.image_path = al_load_bitmap("T after.png");
        al_draw_bitmap(character1.image_path,character1.x,character1.y,0);
        //hidden的東西
        if(flag==1){
                //printf("success");
        if(character2.x-character1.x<=50||character2.x-character1.x>=-50&&character2.y-character1.y<=50||character2.y-character1.y>=-50){
         //printf("success");
          character2.hidden=true;
         //printf("success");
        }
        
        }
    temp=0;
}

    // Shutdown our program
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;

    return 0;
}

int game_run()
{
    int error = 0;
    // First window(Menu)
    if(window == 1)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
            if(judge_next_window)
            {
                window = 2;
                // Setting Character
                character1.x = 0;
                character1.y = HEIGHT / 2 + 18;
                character2.x = WIDTH + 100;
                character2.y = HEIGHT / 2 +25;
                character1.image_path = al_load_bitmap("Trump.png");
                character2.image_path = al_load_bitmap("G.png");
                character3.image_path = al_load_bitmap("teemo_right.png");
                background = al_load_bitmap("back.jpg");
                //Initialize Timer
                timer  = al_create_timer(1.0/15.0);
                timer2  = al_create_timer(1.0);
                timer3  =  al_create_timer(1.0/10.0);
                al_register_event_source(event_queue, al_get_timer_event_source(timer)) ;
                al_register_event_source(event_queue, al_get_timer_event_source(timer2)) ;
                al_register_event_source(event_queue, al_get_timer_event_source(timer3)) ;
                al_start_timer(timer);
                al_start_timer(timer2);
                al_start_timer(timer3);
            }
        }
    }
    // Second window(Main Game)
    else if(window == 2)
    {
        // Change Image for animation
        al_draw_bitmap(background, 0,0,0);
        if(ture)
            al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);

        if(dir)
            al_draw_bitmap(character2.image_path, character2.x, character2.y, 0);
        else
            al_draw_bitmap(character2.image_path, character2.x, character2.y, 0);

        al_flip_display();

        al_clear_to_color(al_map_rgb(0,0,0));
        // Listening for new event
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
            
        }




    }
    return error;
}


void game_destroy()
{
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(timer2);
    al_destroy_bitmap(image);
    al_destroy_sample(song);
}
