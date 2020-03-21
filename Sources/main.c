#include "LE_basis.h"

int main()
{
	Editor editor = Create_Editor();
	Object cursor;
	const Uint8 *keyState = NULL;
	
	if(!SDL_Initialize())
		return false;

	Menu(&editor);
	Set_Palet(&editor);	

	if(editor.state == NEW)
		editor.state = CONTINUE;

	if(editor.state == LOAD)
		Load_Editor(&editor);

	cursor = Create_Cursor(&editor);
	editor.curTime = SDL_GetTicks();

	while(editor.state == CONTINUE)
	{
		editor.prevTime = editor.curTime;
		editor.curTime = SDL_GetTicks();
		editor.deltaTime = editor.curTime - editor.prevTime;

		while(SDL_PollEvent(&event))
			if(event.type == SDL_QUIT)
				editor.state = FINISH;

		keyState = SDL_GetKeyboardState(NULL);

		if(keyState[SDL_SCANCODE_ESCAPE])	editor.state = FINISH;
		if(keyState[SDL_SCANCODE_UP])		editor.cam.cameraRect.y--;
		if(keyState[SDL_SCANCODE_DOWN])		editor.cam.cameraRect.y++;
		if(keyState[SDL_SCANCODE_RIGHT])	editor.cam.cameraRect.x++;
		if(keyState[SDL_SCANCODE_LEFT])		editor.cam.cameraRect.x--;

		if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))	Cursor_Pressed_Left(&editor,&cursor);
		if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))	Cursor_Pressed_Right(&editor,&cursor);

		if(cursor.id == CLEAR)	Clear_Editor(&editor,&cursor);
		if(cursor.id == SAVE)	Save_Editor(&editor,&cursor);

		Update_Palette(&editor);
		Update_Object(&editor);
		Do_Render(&editor);
	}

	Destroy_All();
	return 0;
}