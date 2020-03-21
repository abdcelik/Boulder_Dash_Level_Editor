#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LE_basis
#define LE_basis

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 760
#define CELL_SIZE 48
#define FPS 60
#define IMG_FILE_PATH "Images"

typedef enum{false,true}bool;
typedef enum{EMPTY=' ',MINER='p',EARTH='.',BORDER='|',ROCK='o',DIAMOND='*',SPIDER='c',MONSTER='e',WATER='x',DOOR='g',CLEAR,SAVE}ID;
typedef enum{CONTINUE,FINISH,NEW,LOAD}State;

typedef struct
{
	ID id;
	SDL_Rect posRect,cropRect,drawingRect;
	int frameWidth,frameHeight;
	Uint32 frameCounter;
	int row,column;
}Object;

typedef struct
{
	SDL_Rect cameraRect;	
}Camera;

typedef struct
{
	Object *obj;
	int size;
	Object palette[12];
	Camera cam;
	int theme;
	Uint32 prevTime,curTime,deltaTime;
	State state;
}Editor;

SDL_Window *win;
SDL_Renderer *renderer;
SDL_Event event;
SDL_Texture *tex;
SDL_Texture *introTex;

bool SDL_Initialize();
void Menu(Editor *edit);
Editor Create_Editor();
Object Create_Cursor(Editor *edit);
SDL_Rect Create_Rect(int x, int y, int w, int h);
void Set_Palet(Editor *edit);
void Set_Object(Editor *edit, Object *obj);
int Check_Object(Editor edit, Object cur);
void Update_Palette(Editor *edit);
void Update_Object(Editor *edit);
void Cursor_Pressed_Left(Editor *edit, Object *cur);
void Cursor_Pressed_Right(Editor *edit, Object *cur);
void Clear_Editor(Editor *edit, Object *cur);
void Save_Editor(Editor *edit, Object *cur);
void Load_Editor(Editor *edit);
bool Export_Level(Editor *edit);
void Do_Render(Editor *edit);
void Destroy_All();
int str_cmp(char *s1, char *s2);

#endif
