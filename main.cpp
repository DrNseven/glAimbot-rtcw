//#######################################################################
//
// GL hook 2003
//
// credits: Pansemuckl, Rainerstoff, Sinner, game-deception 
//
// Menu Key is INSERT and DELETE reloads font buffer
// seta r_primitives 2 is required for glDrawElements to work!
// Primitives have an influence on how ET renders its graphics. 0 = auto select, 1 = single gldrawelements, 2 = multiple gldrawelements.
//
//#######################################################################

#include "main.h" //tools here

// =============================================================================================== //

void WINAPI newglBindTexture(GLenum target, GLuint texture)
{
	//get shader
	//Shader = *((char **)0x016BB418); //mov     eax, dword_16BB418

	//get shader
	__asm mov Shader, esi
	if (Shader > 0x1000 && texture != NULL && Shader != NULL)
		shaderfound = true;
	else shaderfound = false;


	//model rec
	if (shaderfound)
	{
		//all axis models
		if (
			(strstr((char*)Shader, "models/players/multi_axis"))
			)
			allaxis = true;
		else allaxis = false;

		//all allies models
		if (
			(strstr((char*)Shader, "models/players/multi"))
			)
			allallies = true;
		else allallies = false;

		//axis heads
		if (strstr((char*)Shader, "models/players/multi_axis/head_german"))
			axis_head = true;
		else axis_head = false;

		//allies heads
		if (
			(strstr((char*)Shader, "models/players/multi/head_american")) ||
			(strstr((char*)Shader, "models/players/multi/head_american_3"))
			)
			allies_head = true;
		else allies_head = false;
	}

	//log shader names
	if (GetAsyncKeyState(VK_F10) < 0)
	if (texture != NULL && Shader != NULL)
	if (strstr((char*)Shader, "models"))
	Log("Shader == %s\r", Shader);

	(*origglBindTexture)(target, texture);
}

// =============================================================================================== //

//seta r_primitives 2 is required for glDrawElements to work!!
//Primitives have an influence on how ET renders its graphics. 0 = auto select, 1 = single gldrawelements, 2 = multiple gldrawelements.
void WINAPI newglDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	// test, draw text on everything (low fps)
	//DrawTextTo(0, 0, 0, 1.0f, 1.0f, 1.0f, "%s", (char*)Shader);

	//aimbot part 1
	if (
		(aimbot == 1 && IsDead() == false && axis_head) ||		//aim at axis head
		(aimbot == 2 && IsDead() == false && allies_head)		//aim at allies head
		)
	{
		//tweak
		//glTranslatef(0,0,1);

		glGetFloatv(GL_MODELVIEW_MATRIX, MViewMX);
		glGetFloatv(GL_PROJECTION_MATRIX, ProjMX);

		//if not behind walls
		GetLargestVertexFromArray(2, count, 16, indices, vpointer, Window);
		if ((IsVertexVisible(Window)) || (IsPointVisible(Window[0] + 0, Window[1] + 0, Window[2] + 5) || IsPointVisible(Window[0] + 10, Window[1] + 10, Window[2] + 5) || IsPointVisible(Window[0] + 10, Window[1] - 10, Window[2] + 5) || IsPointVisible(Window[0] - 10, Window[1] - 10, Window[2] - 30) || IsPointVisible(Window[0] + 10, Window[1] - 10, Window[2] - 30) || IsPointVisible(Window[0] + 10, Window[1] + 10, Window[2] - 55) || IsPointVisible(Window[0] - 10, Window[1] + 10, Window[2] - 55)))
		{
			ObjectToWindowAim(MViewMX, ProjMX, Viewport, Window); //w2s

			//test
			//if (esp == 1 && all_heads)
			//DrawESP(1, MViewMX, ProjMX, Window);

			float ScrCenter[3];
			ScrCenter[0] = float(Viewport[2] / 2);
			ScrCenter[1] = float(Viewport[3] / 2);
			ScrCenter[2] = 0.0f;

			float radiusx = aimfov * (ScrCenter[0] / 100);
			float radiusy = aimfov * (ScrCenter[1] / 100);

			//aimfov
			if (Window[0] >= (ScrCenter[0] - radiusx) && Window[0] <= (ScrCenter[0] + radiusx) && Window[1] >= (ScrCenter[1] - radiusy) && Window[1] <= (ScrCenter[1] + radiusy))
			{
				tPointf p = { Window[0], Window[1], MViewMX[14] };
				AimPoint.push_back(p);
			}

		}
	}

	//wallhack
	if ((wallhack == 1) && (allaxis || allallies))
	{
		glDisable(GL_DEPTH_TEST);
		origglDrawElements(mode, count, type, indices);
		glEnable(GL_DEPTH_TEST);
	}

	//chams, normal
	if ((chams == 1) && (allaxis || allallies))
	{
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
		glDisableClientState(GL_COLOR_ARRAY);
		if (allaxis)
			glColor4ub(255, 80, 0, 0); //behind walls
		else if (allallies)
			glColor4ub(0, 255, 125, 0); //behind walls
		origglDrawElements(mode, count, type, indices);
		glEnable(GL_COLOR_MATERIAL);
		glDisableClientState(GL_COLOR_ARRAY);
		if (allaxis)
			glColor4ub(255, 0, 0, 0); //infront of walls
		else if (allallies)
			glColor4ub(0, 255, 0, 0); //infront of  walls
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
	
	(*origglDrawElements)(mode, count, type, indices);
}

// =============================================================================================== //

void APIENTRY newglVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	//used for walls check, not used anymore for dead body check
	if (pointer != NULL)
	{
		vpointer = (GLfloat *)pointer;

		GLfloat *x = (GLfloat *)vpointer + 0;
		GLfloat *y = (GLfloat *)vpointer + 1;
		GLfloat *z = (GLfloat *)vpointer + 2;
		Xe = *x;
		Ye = *y;
		Ze = *z;
	}

	origglVertexPointer(size, type, stride, pointer);
}

// =============================================================================================== //

void __stdcall newwglSwapBuffers(HDC hDC) // GDI32
{
	//get viewport
	glGetIntegerv(GL_VIEWPORT, Viewport);

	//build font once
	if (!bTextLoaded)
	{
		BuildFont(); //load font
		LoadSettings(); //load menu values
		bTextLoaded = true;
	}

	if ((GetAsyncKeyState(VK_DELETE) < 0))//buffer reload, if switch fullscreen/window deletes font buffer
	{
		BuildFont(); //load font
	}

	//menu key
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		showmenu = !showmenu;

		SaveSettings(); //save menu settings
	}

	//draw menu
	if (showmenu)
	{
		rendermenu();
	}

	
	//Shift|RMouse|LMouse|Ctrl|Alt|Space|X|C
	if (aimkey == 1) Daimkey = VK_SHIFT;
	if (aimkey == 2) Daimkey = VK_RBUTTON;
	if (aimkey == 3) Daimkey = VK_LBUTTON;
	if (aimkey == 4) Daimkey = VK_CONTROL;
	if (aimkey == 5) Daimkey = VK_MENU;
	if (aimkey == 6) Daimkey = VK_SPACE;
	if (aimkey == 7) Daimkey = 0x58; //X
	if (aimkey == 8) Daimkey = 0x43; //C
	
	//aimbot part 2
	if (aimbot == 1|| aimbot == 2) //&& GetAsyncKeyState(aimkey)) //& 0x8000)
	{
		unsigned int pointNum = -1;
		unsigned int i = 0;
		float Closest = -0xFFFFFF;
		float Closest2 = 0xFFFFFF;
		float CenterX = (float)Viewport[2] / 2;
		float CenterY = (float)Viewport[3] / 2;

		for (i = 0; i<AimPoint.size(); i++)
		{
			//test
			//drawMessage(AimPoint[i].x, AimPoint[i].y, 1.0, "Model");
			//drawMessage(AimPoint[pointNum].x, AimPoint[pointNum].y, 0.8, "Iterations this frame: %d (1st %.2f %.2f %.2f)", AimPoint.size(), AimPoint[0].x, AimPoint[0].y, AimPoint[0].z);

			float xdif = CenterX - AimPoint[i].x;
			float ydif = CenterY - AimPoint[i].y;

			float hyp = sqrt((xdif*xdif) + (ydif*ydif));

			if (hyp < Closest2)
			{
				Closest2 = hyp;
				pointNum = i;
			}
		}

		if (pointNum != -1)
		{
			//gl aim
			if (aimmode == 1)
			AimPoint[pointNum].y = Viewport[3] - AimPoint[pointNum].y; //inverted mouse
			else
			AimPoint[pointNum].y = AimPoint[pointNum].y; //normal mouse

			double DistX = (double)AimPoint[pointNum].x - CenterX;
			double DistY = (double)AimPoint[pointNum].y - CenterY;

			//drawMessage(AimPoint[pointNum].x, AimPoint[pointNum].y, 1.0, "aiming at");

			DistX /= aimsmooth;
			DistY /= aimsmooth;

			if (GetAsyncKeyState(Daimkey) & 0x8000)
			{
				if(aimsmooth == 0)
				SetCursorPos((int)AimPoint[pointNum].x, (int)AimPoint[pointNum].y); //best accuracy for fullscreen
				else
				mouse_event(MOUSEEVENTF_MOVE, (DWORD)DistX, (DWORD)DistY, NULL, NULL);
			}

			//autoshoot on
			if (!GetAsyncKeyState(VK_LBUTTON)) //manual override, if player attacks manually don't interfere
			//return;
			if (autoshoot == 1 && GetAsyncKeyState(Daimkey) & 0x8000)
			{
				if (!IsPressed)
				{
					IsPressed = true;
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				}
			}

		}
	}

	AimPoint.clear();

	if (autoshoot == 1 && IsPressed)
	{
		if (timeGetTime() - gametick > asdelay) //
		{
			//Log("timeGetTime() - gametick == %d", timeGetTime() - gametick);
			IsPressed = false;
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			gametick = timeGetTime();
		}
	}

	
	origwglSwapBuffers(hDC);
}

// =============================================================================================== //

bool bGLSet = false; //hook and init once
void HookFunctions()
{
	HMODULE oMod = GetModuleHandle("opengl32.dll");
	HMODULE gMod = GetModuleHandle("gdi32.dll");

	if (!bGLSet)
	{
		if(oMod)
		{
			origglBindTexture = (glBindTexture_t)(DWORD)GetProcAddress(oMod, "glBindTexture");
			origglDrawElements = (glDrawElements_t)(DWORD)GetProcAddress(oMod, "glDrawElements");
			origglVertexPointer = (glVertexPointer_t)(DWORD)GetProcAddress(oMod, "glVertexPointer");

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID &)origglBindTexture, newglBindTexture);
			DetourAttach(&(PVOID &)origglDrawElements, newglDrawElements);
			DetourAttach(&(PVOID &)origglVertexPointer, newglVertexPointer);
			DetourTransactionCommit();
		}

		if (gMod)
		{
			origwglSwapBuffers = (wglSwapBuffers_t)(DWORD)GetProcAddress(gMod, "SwapBuffers");

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID &)origwglSwapBuffers, newwglSwapBuffers);
			DetourTransactionCommit();
		}

	}
	bGLSet = true;
}

// =============================================================================================== //

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			GetModuleFileName(hModule, dlldir, 512);
			for (int i = strlen(dlldir); i > 0; i--) { if (dlldir[i] == '\\') { dlldir[i + 1] = 0; break; } }
			HookFunctions();
			break;
		}	

	case DLL_PROCESS_DETACH:
		{
			KillFont(); // Destroy The Font
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID &)origglBindTexture, newglBindTexture);
			DetourDetach(&(PVOID &)origglDrawElements, newglDrawElements);
			DetourDetach(&(PVOID &)origglVertexPointer, newglVertexPointer);
			DetourDetach(&(PVOID &)origwglSwapBuffers, newwglSwapBuffers);
			DetourTransactionCommit();
			::FreeLibrary(hModule);
			hModule = 0;
			break;
		}
	}
	return TRUE;
}