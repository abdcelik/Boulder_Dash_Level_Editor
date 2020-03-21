#include "LE_basis.h"

bool SDL_Initialize()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Error! SDL couldn't initialized...\n");
		return false;
	}

	win = SDL_CreateWindow("Boulder Dash Level Editor",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
	if(!win)
	{
		printf("Error! Window couldn't created...\n");
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
	if(!renderer)
	{
		printf("Error! Render couldn't created...\n");
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	}

	SDL_Surface *sur = IMG_Load(IMG_FILE_PATH"/Textures.png");
	if(!sur)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	} 
	
	tex = SDL_CreateTextureFromSurface(renderer,sur);
	SDL_FreeSurface(sur);
	if(!tex)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	}

	SDL_Surface *introsur = IMG_Load(IMG_FILE_PATH"/intro.png");
	if(!introsur)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	} 
	
	introTex = SDL_CreateTextureFromSurface(renderer,introsur);
	SDL_FreeSurface(introsur);
	if(!introTex)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	}
	
	return true;
}

void Menu(Editor *edit)
{
	SDL_Rect buttonPos[3],buttonCrop;
	int button_state=0,i=0,j=0;

	buttonPos[0] = Create_Rect(WINDOW_WIDTH*1000/2327,WINDOW_HEIGHT*100/195,3*CELL_SIZE/4,3*CELL_SIZE/4);
	buttonPos[1] = Create_Rect(WINDOW_WIDTH*1000/2348,WINDOW_HEIGHT*1000/1735,3*CELL_SIZE/4,3*CELL_SIZE/4);
	buttonPos[2] = Create_Rect(WINDOW_WIDTH*100/301,WINDOW_HEIGHT*1000/1564,3*CELL_SIZE/4,3*CELL_SIZE/4);
	buttonCrop = Create_Rect(0,64*6,16,16);

	for(i=0 ; i<10 ; i++)
		Set_Object(edit,&edit->palette[i]);

	for(i=0 ; i<10 ; i++)
		if(edit->palette[i].id != MINER)
		{
			edit->palette[i].posRect = Create_Rect(j+WINDOW_WIDTH/20,59*WINDOW_HEIGHT/80,48,48);
			j += WINDOW_WIDTH*1000/9208;
		}

	while(edit->state == CONTINUE)
	{
		edit->prevTime = edit->curTime;
		edit->curTime = SDL_GetTicks();
		edit->deltaTime = edit->curTime - edit->prevTime;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				edit->state = FINISH;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				edit->state = FINISH;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_UP && button_state > 0)
				button_state--;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN && button_state < 2)
				button_state++;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				if(button_state == 0)		edit->state = NEW;
				else if(button_state == 1)	edit->state = LOAD;
				else
				{
					edit->theme++;
					if(edit->theme > 5)
						edit->theme = 0;
					for(i=0 ; i<10 ; i++)
						Set_Object(edit,&edit->palette[i]);
				}
			}
		}
		
		Update_Palette(edit);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,introTex,NULL,NULL);
		SDL_RenderCopy(renderer,tex,&buttonCrop,&buttonPos[button_state]);
		for(i=0 ; i<10 ; i++)
			if(edit->palette[i].id != MINER)
				SDL_RenderCopy(renderer,tex,&edit->palette[i].cropRect,&edit->palette[i].posRect);
		SDL_RenderPresent(renderer);
	}
}

Editor Create_Editor()
{
	Editor editor;

	editor.obj = NULL;
	editor.size = 0;
	editor.cam.cameraRect.x = editor.cam.cameraRect.y = 0;
	editor.cam.cameraRect.w = WINDOW_WIDTH , editor.cam.cameraRect.h = WINDOW_HEIGHT;
	editor.theme = 0;
	editor.state = CONTINUE;
	editor.prevTime = editor.curTime = editor.deltaTime = 0;

	editor.palette[0].id = EMPTY;
	editor.palette[1].id = MINER;
	editor.palette[2].id = EARTH;
	editor.palette[3].id = BORDER;
	editor.palette[4].id = ROCK;
	editor.palette[5].id = DIAMOND;
	editor.palette[6].id = SPIDER;
	editor.palette[7].id = MONSTER;
	editor.palette[8].id = WATER;
	editor.palette[9].id = DOOR;
	editor.palette[10].id = CLEAR;
	editor.palette[11].id = SAVE;

	return editor;
}

Object Create_Cursor(Editor *edit)
{
	Object cur;

	cur.id = EMPTY;
	Set_Object(edit,&cur);
	cur.posRect = Create_Rect(0,0,CELL_SIZE,CELL_SIZE);
	cur.row = cur.column = 0;
	
	return cur;
}

SDL_Rect Create_Rect(int x, int y, int w, int h)
{
	SDL_Rect rect = {x,y,w,h};
	return rect;
}

void Set_Palet(Editor *edit)
{
	int i;

	for(i=0 ; i < 12 ; i++)
	{
		Set_Object(edit,&edit->palette[i]);
		edit->palette[i].posRect = Create_Rect(15*WINDOW_WIDTH/16 + 16 , i*WINDOW_HEIGHT/12 + 8 , 48 , 48);
	}
}

void Set_Object(Editor *edit, Object *obj)
{
	obj->cropRect.w = obj->cropRect.h = 16;
	obj->cropRect.y = edit->theme * 64;
	obj->frameWidth = obj->frameHeight = 4;
	obj->frameCounter = 0;
	
	switch(obj->id)
	{
		case EMPTY:
			obj->cropRect.x = 2*16;
			if(edit->theme == 0 || edit->theme == 1)
				obj->frameWidth = obj->frameHeight = 1;
			break;
		case MINER:
			obj->cropRect.x = 13;
			obj->cropRect.y = 6*64;
			obj->cropRect.w = obj->cropRect.h = 32;
			obj->frameWidth = obj->frameHeight = 1;
			break;
		case EARTH:
			obj->cropRect.x = 1*16;
			if(edit->theme == 0 || edit->theme == 1 || edit->theme ==3)
				obj->frameWidth = obj->frameHeight = 1;
			break;
		case BORDER:
			obj->cropRect.x = 0*16;
			obj->frameWidth = obj->frameHeight = 1;
			break;
		case ROCK:
			obj->cropRect.x = 3*16;
			if(edit->theme == 4)
				obj->frameWidth = obj->frameHeight = 1;
			break;
		case DIAMOND:
			obj->cropRect.x = 4*16;
			break;
		case SPIDER:
			obj->cropRect.x = 8*16;
			break;
		case MONSTER:
			obj->cropRect.x = 7*16;
			break;
		case WATER:
			obj->cropRect.x = 6*16;
			break;
		case DOOR:
			obj->cropRect.x = 5*16;
			break;
		case CLEAR:
			obj->cropRect.x = 45;
			obj->cropRect.y = 6*64+16;
			obj->cropRect.w = 80;
			obj->frameWidth = obj->frameHeight = 1;
			break;
		case SAVE:
			obj->cropRect.x = 45;
			obj->cropRect.y = 6*64;
			obj->cropRect.w = 80;
			obj->frameWidth = obj->frameHeight = 1;
			break;
	}
}

int Check_Object(Editor edit, Object cur)
{
	int i;

	if(edit.size > 0)
		for(i=0 ; i < edit.size ; i++)
			if(edit.obj[i].row == cur.row && edit.obj[i].column == cur.column)
			{
				if(edit.obj[i].id == cur.id)
					return -i-1;
				else
					return i+1;
			}
	return 0;
}

void Update_Palette(Editor *edit)
{
	int i;

	for(i=0 ; i<10 ; i++)
	{
		edit->palette[i].frameCounter += edit->deltaTime;
		if(edit->palette[i].frameCounter > 2*FPS && edit->palette[i].id != MINER)
		{
			edit->palette[i].frameCounter = 0;
			edit->palette[i].cropRect.y += edit->palette[i].cropRect.h;
			if(edit->palette[i].cropRect.y >= edit->theme*64 + edit->palette[i].cropRect.h * edit->palette[i].frameHeight)
				edit->palette[i].cropRect.y = edit->theme*64;
		}
	}
}

void Update_Object(Editor *edit)
{
	int i;

	for(i=0 ; i < edit->size ; i++)
	{
		edit->obj[i].frameCounter += edit->deltaTime;
		if(edit->obj[i].frameCounter > 2*FPS && edit->obj[i].id != MINER)
		{
			edit->obj[i].frameCounter = 0;
			edit->obj[i].cropRect.y += edit->obj[i].cropRect.h;
			if(edit->obj[i].cropRect.y >= edit->theme*64 + edit->obj[i].cropRect.h * edit->obj[i].frameHeight)
				edit->obj[i].cropRect.y = edit->theme*64;
		}
	}
}

void Cursor_Pressed_Left(Editor *edit, Object *cur)
{
	int situation;

	if(event.button.x >= 15*WINDOW_WIDTH/16)
	{
		if(event.button.y < WINDOW_HEIGHT/12)			cur->id = EMPTY;
		else if(event.button.y < 2*WINDOW_HEIGHT/12)	cur->id = MINER;
		else if(event.button.y < 3*WINDOW_HEIGHT/12)	cur->id = EARTH;
		else if(event.button.y < 4*WINDOW_HEIGHT/12)	cur->id = BORDER;
		else if(event.button.y < 5*WINDOW_HEIGHT/12)	cur->id = ROCK;
		else if(event.button.y < 6*WINDOW_HEIGHT/12)	cur->id = DIAMOND;
		else if(event.button.y < 7*WINDOW_HEIGHT/12)	cur->id = SPIDER;
		else if(event.button.y < 8*WINDOW_HEIGHT/12)	cur->id = MONSTER;
		else if(event.button.y < 9*WINDOW_HEIGHT/12)	cur->id = WATER;
		else if(event.button.y < 10*WINDOW_HEIGHT/12)	cur->id = DOOR;
		else if(event.button.y < 11*WINDOW_HEIGHT/12)	cur->id = CLEAR;
		else if(event.button.y < WINDOW_HEIGHT)			cur->id = SAVE;

		if(cur->id != CLEAR && cur->id != SAVE)
			Set_Object(edit,cur);
	}
	else if(event.button.x > 0 && event.button.x < WINDOW_WIDTH-1 && event.button.y > 0 && event.button.y < WINDOW_HEIGHT-1)
	{
		cur->column = (event.button.x + edit->cam.cameraRect.x)/CELL_SIZE;
		cur->row = (event.button.y + edit->cam.cameraRect.y)/CELL_SIZE;

		if(event.button.x + edit->cam.cameraRect.x < 0)	cur->column--;
		if(event.button.y + edit->cam.cameraRect.y < 0)	cur->row--;

		cur->posRect.x = cur->column*CELL_SIZE;
		cur->posRect.y = cur->row*CELL_SIZE;

		situation = Check_Object(*edit,*cur);

		if(situation == 0)
		{
			edit->size++;

			if(edit->obj == NULL)
				edit->obj = (Object*)malloc(sizeof(Object));
			else
				edit->obj = (Object*)realloc(edit->obj,sizeof(Object)*edit->size);

			edit->obj[edit->size-1] = *cur;
		}
		else if(situation > 0)
			edit->obj[situation-1] = *cur;
	}
}

void Cursor_Pressed_Right(Editor *edit, Object *cur)
{
	int situation;

	cur->column = (event.button.x + edit->cam.cameraRect.x)/CELL_SIZE;
	cur->row = (event.button.y + edit->cam.cameraRect.y)/CELL_SIZE;

	if(event.button.x + edit->cam.cameraRect.x < 0)	cur->column--;
	if(event.button.y + edit->cam.cameraRect.y < 0)	cur->row--;

	cur->posRect.x = cur->column*CELL_SIZE;
	cur->posRect.y = cur->row*CELL_SIZE;

	situation = Check_Object(*edit,*cur);

	if(situation != 0)
	{
		if(situation < 0)	situation = -situation-1;
		else				situation = situation-1;

		edit->obj[situation] = edit->obj[edit->size-1];
		edit->size--;
		edit->obj = (Object*)realloc(edit->obj,sizeof(Object)*edit->size);
	}
}

void Clear_Editor(Editor *edit, Object *cur)
{
	edit->size = 0;
	edit->obj = (Object*)realloc(edit->obj,sizeof(Object)*edit->size);
	cur->id = EMPTY;
	Set_Object(edit,cur);
}

void Save_Editor(Editor *edit, Object *cur)
{
	if(!Export_Level(edit))
		printf("Error! Level couldn't exported...\n");

	cur->id = EMPTY;
	Set_Object(edit,cur);
}

void Load_Editor(Editor *edit)
{
	FILE *fp = NULL;
	char buffer[32] = {0},buf;
	int x=0,y=0,exit=0;

	fp = fopen("input.txt","r");

	if(!fp)
	{
		printf("input.txt couldn't founded...\n");
		edit->state = FINISH;
	}

	if(edit->state != FINISH)
	{
		edit->state = CONTINUE;

		while(str_cmp(buffer,"#map#") != 0)
		fscanf(fp,"%s",buffer);

		while(!exit)
		{
			buf = fgetc(fp);
			if(buf == '#' || buf == EOF)
				exit = 1;
			else if(buf == '\n')
				x=0,y++;
			else
			{
				edit->size++;

				if(edit->obj == NULL)
					edit->obj = (Object*)malloc(sizeof(Object));
				else
					edit->obj = (Object*)realloc(edit->obj,sizeof(Object)*edit->size);

				edit->obj[edit->size-1].id = buf;
				Set_Object(edit,&edit->obj[edit->size-1]);
				edit->obj[edit->size-1].row = y , edit->obj[edit->size-1].column = x;
				edit->obj[edit->size-1].posRect = Create_Rect(x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE,CELL_SIZE);
				x++;
			}
		}
		fclose(fp);
	}
}

bool Export_Level(Editor *edit)
{
	FILE *fp;
	int min_x=0,min_y=0,max_x=0,max_y=0,dia=0;
	int i,j,k,flag=1;

	if(edit->size < 1)
		return false;
	else
	{
		max_x = min_x = edit->obj[0].column;
		max_y = min_y = edit->obj[0].row;

		for(i=0; i < edit->size ; i++)
		{
			if(edit->obj[i].column < min_x)
				min_x = edit->obj[i].column;
			if(edit->obj[i].row < min_y)
				min_y = edit->obj[i].row;
			if(edit->obj[i].column > max_x)
				max_x = edit->obj[i].column;
			if(edit->obj[i].row > max_y)
				max_y = edit->obj[i].row;
			if(edit->obj[i].id == DIAMOND)
				dia++;
		}

		fp = fopen("level.txt","w");

		if(!fp)
			return false;

		fprintf(fp,"#level#\n");
		fprintf(fp,"ID=\n");
		fprintf(fp,"Row= %d\n",max_y-min_y+1);
		fprintf(fp,"Column= %d\n",max_x-min_x+1);
		fprintf(fp,"Diamond= %d\n",dia);
		fprintf(fp,"Time=\n");
		fprintf(fp,"WaterDischarge=\n\n");
		fprintf(fp,"#map#\n");

		for(i=min_y ; i <= max_y ; i++)
		{
			for(j=min_x ; j <= max_x ; j++)
			{
				for(k=0 ; k < edit->size && flag ; k++)
					if(edit->obj[k].row == i && edit->obj[k].column == j)
						flag = 0;

				if(!flag)	fprintf(fp,"%c",edit->obj[k-1].id);
				else		fprintf(fp," ");
				flag = 1;
			}
			fprintf(fp,"\n");
		}

		fprintf(fp,"#\n");
		fclose(fp);
	}
	return true;
}

void Do_Render(Editor *edit)
{
	int i;

	SDL_RenderClear(renderer);

	for(i=0 ; i < edit->size ; i++)
	{
		edit->obj[i].drawingRect.x = edit->obj[i].posRect.x - edit->cam.cameraRect.x;
		edit->obj[i].drawingRect.y = edit->obj[i].posRect.y - edit->cam.cameraRect.y;
		edit->obj[i].drawingRect.w = edit->obj[i].posRect.w;
		edit->obj[i].drawingRect.h = edit->obj[i].posRect.h;

		SDL_RenderCopy(renderer,tex,&edit->obj[i].cropRect,&edit->obj[i].drawingRect);
	}

	for(i=0 ; i < 12 ; i++)
		SDL_RenderCopy(renderer,tex,&edit->palette[i].cropRect,&edit->palette[i].posRect);

	SDL_RenderPresent(renderer);
}

void Destroy_All()
{
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int str_cmp(char *s1, char *s2)
{
	while(*s1 && *s2 && *s1 == *s2)
		s1++,s2++;
	return *s1-*s2;
}
