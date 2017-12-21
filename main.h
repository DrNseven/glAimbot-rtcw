#pragma once

#pragma warning(disable:4244) //disable warning C4244
#pragma warning(disable:4996)
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "gl/Glu32.lib") //gluproject should be replaced with glhProjectf in future
#pragma comment(lib,"detours.lib")
#pragma comment(lib, "winmm.lib")  //timeGetTime, playsound

#include <Windows.h>
#include <gl/GL.h>
#include <gl/glu.h> //gluProject
#include <vector> //AimPoint
#include <string> //std::string
#include <fstream> //ofstream
#include <time.h> //initparticles time
#include "detours.h"
using namespace std; //logfile

// =============================================================================================== //

typedef void (WINAPI *glDrawElements_t) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (WINAPI *glBindTexture_t) (GLenum target, GLuint texture);
typedef void (WINAPI *glVertexPointer_t) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (__stdcall *wglSwapBuffers_t) (HDC hDC);

glDrawElements_t origglDrawElements;
glBindTexture_t origglBindTexture;
glVertexPointer_t origglVertexPointer;
wglSwapBuffers_t origwglSwapBuffers;

// =============================================================================================== //

int countnum = -1;

// getdir & log
char dlldir[320];
char* GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}

void Log(const char *fmt, ...)
{
	if (!fmt)	return;

	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);

	ofstream logfile(GetDirectoryFile("log.txt"), ios::app);
	if (logfile.is_open() && text)	logfile << text << endl;
	logfile.close();
}

// =============================================================================================== //

//config wars / features
int				wallhack = 2;
int				chams = 1;
int				aimbot = 1;
//aim settings
DWORD Daimkey = VK_SHIFT;
int aimkey = 1;
float aimfov = 60.0f;
float aimsmooth = 1.0f;
float aimheight = 1.0f;
int				aimmode = 0; //normal or inverted
//autoshoot settings
int				autoshoot = 1;
bool IsPressed = false;
DWORD gametick = timeGetTime();
float asdelay = 320.0f;
//esp settngs
int				esp = 1;
float espheight = 30.0f;
float espsize = 10.0f;
int				laser = 1;
int				ballhack = 0;
//misc
int				removals = 1;
int				ranks = 1;
int				hitsounds = 1;
bool SoundInit1 = false;
bool SoundInit2 = false;
bool SoundInit3 = false;
bool SoundInit4 = false;
//zoom
int zoom = 0;
DWORD Dzoomkey = VK_CONTROL;
int zoomkey = 0;
int zoomlevel = 0;

//viewport
int Viewport[4]; //use this viewport

//aimangle
float FOV[2];
#define M_PI 3.14159265358979323846f

DWORD Shader; // shader
bool shaderfound = false;


//models
// globals
bool allaxis;
bool allallies;
bool axis_head;
bool allies_head;

//w2s matrix
float MViewMX[16];
float ProjMX[16];
float Window[3];

//vertexpointer
GLfloat *vpointer;
GLfloat Xe, Ye, Ze;

//ispointvisible matrix
GLdouble mm[16], pm[16];

//aim stuff
typedef struct
{
	float x, y, z;
} tPointf;

std::vector<tPointf> AimPoint;

//save settings to ini
void SaveSettings()
{
	ofstream fout;

	//DeleteFile(GetDirectoryFile("n7rtcw.ini"));

	fout.open(GetDirectoryFile("n7rtcw.ini"));

	fout << "Wallhack: " << wallhack << endl;
	fout << "Chams: " << chams << endl;
	fout << "Aimbot: " << aimbot << endl;
	fout << "Aimkey: " << aimkey << endl;
	fout << "Aimfov: " << aimfov << endl;
	fout << "Aimsmooth: " << aimsmooth << endl;
	fout << "Aimheight: " << aimheight << endl;
	fout << "Aimmode: " << aimmode << endl;
	fout << "Autoshoot: " << autoshoot << endl;
	fout << "Esp: " << esp << endl;
	fout << "Laser: " << laser << endl;
	fout << "Ballhack: " << ballhack << endl;
	fout << "Removals: " << removals << endl;
	fout << "Ranks: " << ranks << endl;
	fout << "Hitsounds: " << hitsounds << endl;
	fout << "Zoomlevel: " << zoomlevel << endl;
	fout << "Zoomkey: " << zoomkey << endl;
	fout.close();
}

//load settings from ini
void LoadSettings()
{
	ifstream fin;

	string Word = "";

	fin.open(GetDirectoryFile("n7rtcw.ini"));

	fin >> Word >> wallhack;
	fin >> Word >> chams;
	fin >> Word >> aimbot;
	fin >> Word >> aimkey;
	fin >> Word >> aimfov;
	fin >> Word >> aimsmooth;
	fin >> Word >> aimheight;
	fin >> Word >> aimmode;
	fin >> Word >> autoshoot;
	fin >> Word >> esp;
	fin >> Word >> laser;
	fin >> Word >> ballhack;
	fin >> Word >> removals;
	fin >> Word >> ranks;
	fin >> Word >> hitsounds;
	fin >> Word >> zoomlevel;
	fin >> Word >> zoomkey;
	fin.close();
}

// =============================================================================================== //
// menu stuff
// =============================================================================================== //

//MENU
HDC			hDC = NULL;
GLuint		base;

typedef struct {
	char *mainmenuentry;
	char *info;
	float width;
	int count;
}tmainentry;

typedef struct {
	char *submenuentry;
	char *info;
	float value;
	float maxvalue;
	float minvalue;
	char *valuenames[10];
	bool usevnames;
}tsubentry;

tmainentry		mainmenu[17]; //max
tsubentry		submenu[17][17]; //max

float			colorpulse = 0.0f;

//MENUSETTINGS
float			normheight = 0.04f;
float			startx = -0.5f;
float			starty = 0.4f;
float			mainspan = 0.02f;
int				maxmenus = 2;//
int				selectedmenu = 0;
int				selectedsubmenu = -1;
bool			keypush = false;
bool			initpt = false;
bool			initpic = false;
bool			initmenu = false;
/*************************/

//GLOBALS
bool			menukeypush = false;
bool			zua = true,
showmenu = false,
menudrawnthisframe = false,
init = false;
bool			bTextLoaded = false;

///////////////VAR STUFF //////////////////
void resetvars()
{
	wallhack = submenu[0][0].value;
	chams = submenu[0][1].value;
	aimbot = submenu[0][2].value;
	aimkey = submenu[0][3].value;
	aimfov = submenu[0][4].value;
	aimsmooth = submenu[0][5].value;
	aimheight = submenu[0][6].value;
	aimmode = submenu[0][7].value;
	autoshoot = submenu[0][8].value;
	//esp = submenu[0][9].value;
	//laser = submenu[0][10].value;
	//ballhack = submenu[0][11].value;

	//removals = submenu[1][0].value;
	//ranks = submenu[1][1].value;
	//hitsounds = submenu[1][2].value;
	//zoomlevel = submenu[1][3].value;
	//zoomkey = submenu[1][4].value;
}

void initvars()
{
	//////////////////////////
	//main menu entries:
	mainmenu[0].mainmenuentry = "Hacks";
	mainmenu[0].width = 0.1f;
	mainmenu[0].count = 8; //max entries
	submenu[0][0].submenuentry = "Wallhack";
	submenu[0][0].value = wallhack;
	submenu[0][0].maxvalue = 1;
	submenu[0][0].minvalue = 0;
	submenu[0][0].usevnames = true;
	submenu[0][0].valuenames[0] = "Off";
	submenu[0][0].valuenames[1] = "Models";
	submenu[0][0].valuenames[2] = "All";

	submenu[0][1].submenuentry = "Chams";
	submenu[0][1].value = chams;
	submenu[0][1].maxvalue = 1;
	submenu[0][1].minvalue = 0;
	submenu[0][1].usevnames = true;
	submenu[0][1].valuenames[0] = "Off";
	submenu[0][1].valuenames[1] = "Normal";
	submenu[0][1].valuenames[2] = "Outline";
	submenu[0][1].valuenames[3] = "SoftGlow";
	submenu[0][1].valuenames[4] = "HardGlow";
	//submenu[0][1].valuenames[5] = "AnimGlow";

	submenu[0][2].submenuentry = "Aimbot"; //
	submenu[0][2].value = aimbot;
	submenu[0][2].maxvalue = 2;
	submenu[0][2].minvalue = 0;
	submenu[0][2].usevnames = true;
	submenu[0][2].valuenames[0] = "Off";
	submenu[0][2].valuenames[1] = "Aim at Axis";
	submenu[0][2].valuenames[2] = "Aim at Allies";

	//Shift | RMouse | LMouse | Ctrl | Alt | Space | X | C
	submenu[0][3].submenuentry = "Aimkey"; 
	submenu[0][3].value = aimkey;
	submenu[0][3].maxvalue = 8;
	submenu[0][3].minvalue = 0;
	submenu[0][3].usevnames = true;
	submenu[0][3].valuenames[0] = "Off";
	submenu[0][3].valuenames[1] = "Shift";
	submenu[0][3].valuenames[2] = "RMouse";
	submenu[0][3].valuenames[3] = "LMouse";
	submenu[0][3].valuenames[4] = "Ctrl";
	submenu[0][3].valuenames[5] = "Alt";
	submenu[0][3].valuenames[6] = "Space";
	submenu[0][3].valuenames[7] = "X";
	submenu[0][3].valuenames[8] = "C";

	submenu[0][4].submenuentry = "Aimfov";
	submenu[0][4].minvalue = 0;
	submenu[0][4].maxvalue = 80; //84 = spinaims at self in hud if aim is set to own team
	submenu[0][4].value = aimfov;

	submenu[0][5].submenuentry = "Aimsmooth"; //depends on mouse sensitivity, low setting = better accuracy, higher setting = more smooth
	submenu[0][5].minvalue = 0;
	submenu[0][5].maxvalue = 20;
	submenu[0][5].value = aimsmooth;

	submenu[0][6].submenuentry = "Aimheight";
	submenu[0][6].minvalue = -20;
	submenu[0][6].maxvalue = 20;
	submenu[0][6].value = aimheight;

	submenu[0][7].submenuentry = "Aimmode"; 
	submenu[0][7].minvalue = 0;
	submenu[0][7].maxvalue = 1;
	submenu[0][7].value = aimmode;
	submenu[0][7].usevnames = true;
	submenu[0][7].valuenames[0] = "Normal";
	submenu[0][7].valuenames[1] = "Inverted";

	submenu[0][8].submenuentry = "Autoshoot"; //designed for aimbot
	submenu[0][8].value = autoshoot;
	submenu[0][8].maxvalue = 1;
	submenu[0][8].minvalue = 0;
	submenu[0][8].usevnames = true;
	submenu[0][8].valuenames[0] = "Off";
	submenu[0][8].valuenames[1] = "On";
	/*
	submenu[0][9].submenuentry = "Esp";
	submenu[0][9].minvalue = 0;
	submenu[0][9].maxvalue = 4;
	submenu[0][9].value = esp;
	submenu[0][9].usevnames = true;
	submenu[0][9].valuenames[0] = "Off";
	submenu[0][9].valuenames[1] = "Icon";
	submenu[0][9].valuenames[2] = "Team";
	submenu[0][9].valuenames[3] = "Class";
	submenu[0][9].valuenames[4] = "H.Weapon";

	submenu[0][10].submenuentry = "Laser";
	submenu[0][10].minvalue = 0;
	submenu[0][10].maxvalue = 2;
	submenu[0][10].value = laser;
	submenu[0][10].usevnames = true;
	submenu[0][10].valuenames[0] = "Off";
	submenu[0][10].valuenames[1] = "Axis";
	submenu[0][10].valuenames[2] = "Allies";

	submenu[0][11].submenuentry = "Ballhack";
	submenu[0][11].minvalue = 0;
	submenu[0][11].maxvalue = 4;
	submenu[0][11].value = ballhack;
	submenu[0][11].usevnames = true;
	submenu[0][11].valuenames[0] = "Off";
	submenu[0][11].valuenames[1] = "Axis ()";
	submenu[0][11].valuenames[2] = "Allies ()";
	submenu[0][11].valuenames[3] = "Axis []";
	submenu[0][11].valuenames[4] = "Allies []";

	
	mainmenu[1].mainmenuentry = "Misc";
	mainmenu[1].width = 0.1f;
	mainmenu[1].count = 4; //max entries
	submenu[1][0].submenuentry = "Removals";
	submenu[1][0].value = removals;
	submenu[1][0].maxvalue = 3;
	submenu[1][0].minvalue = 0;
	submenu[1][0].usevnames = true;
	submenu[1][0].valuenames[0] = "Off";
	submenu[1][0].valuenames[1] = "Grass";
	submenu[1][0].valuenames[2] = "Foliage"; //no scope included
	submenu[1][0].valuenames[3] = "All";

	submenu[1][1].submenuentry = "Ranks";
	submenu[1][1].value = ranks;
	submenu[1][1].maxvalue = 1;
	submenu[1][1].minvalue = 0;
	submenu[1][1].usevnames = true;
	submenu[1][1].valuenames[0] = "Off";
	submenu[1][1].valuenames[1] = "On";

	submenu[1][2].submenuentry = "Hitsounds"; //self damage hitsounds
	submenu[1][2].value = hitsounds;
	submenu[1][2].maxvalue = 3;
	submenu[1][2].minvalue = 0;
	submenu[1][2].usevnames = true;
	submenu[1][2].valuenames[0] = "Off";
	submenu[1][2].valuenames[1] = "Self";
	submenu[1][2].valuenames[2] = "Airstrike";
	submenu[1][2].valuenames[3] = "Both";

	submenu[1][3].submenuentry = "Zoomlevel";
	submenu[1][3].minvalue = 0;
	submenu[1][3].maxvalue = 17;
	submenu[1][3].value = zoomlevel;

	submenu[1][4].submenuentry = "Zoomkey";
	submenu[1][4].value = zoomkey;
	submenu[1][4].maxvalue = 8;
	submenu[1][4].minvalue = 0;
	submenu[1][4].usevnames = true;
	submenu[1][4].valuenames[0] = "Off";
	submenu[1][4].valuenames[1] = "Shift";
	submenu[1][4].valuenames[2] = "RMouse";
	submenu[1][4].valuenames[3] = "LMouse";
	submenu[1][4].valuenames[4] = "Ctrl";
	submenu[1][4].valuenames[5] = "Alt";
	submenu[1][4].valuenames[6] = "Space";
	submenu[1][4].valuenames[7] = "X";
	submenu[1][4].valuenames[8] = "C";
	*/
}

//////////////////////////

GLvoid BuildFont(GLvoid)
{
	hDC = wglGetCurrentDC();
	HFONT	font;
	HFONT	oldfont;

	base = (glGenLists)(96);

	font = CreateFont(-12,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		"Italic Bold");

	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}

GLvoid glPrint(const char *fmt, ...)
{
	char		text[256];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	(glPushAttrib)(GL_LIST_BIT);
	(glListBase)(base - 32); //-255
	(glCallLists)(strlen(text), GL_UNSIGNED_BYTE, text);
	(glPopAttrib)();
}

static void drawMessage(float x, float y, float a, char *string, ...)
{
	char	    buffer[256];
	va_list	    args;
	GLfloat     color[4], position[4];
	GLboolean   texturing;

	va_start(args, string);
	vsprintf(buffer, string, args);
	va_end(args);

	// save OpenGL state
	(glPushAttrib)(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	(glGetFloatv)(GL_CURRENT_COLOR, color);
	(glGetBooleanv)(GL_TEXTURE_2D, &texturing);
	(glGetFloatv)(GL_CURRENT_RASTER_POSITION, position);

	// set up the state we need 
	(glDisable)(GL_TEXTURE_2D);
	(glColor4f)(1.0f, 1.0f, 1.0f, a);
	(glRasterPos2f)(x, y);

	glPrint(buffer);
	//fontBitmapString(buffer);

	// restore OpenGL state 
	(glPopAttrib)();
	glEnable(GL_DEPTH_TEST);
	(glColor4fv)(color);
	if (texturing == GL_TRUE)
	{
		(glEnable)(GL_TEXTURE_2D);
	}
	(glRasterPos4fv)(position);
}

//magic
void DrawTextTo(float x, float y, float z, float r, float g, float b, const char *fmt, ...)
{
	char		buffer[256];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	va_end(ap);

	GLfloat  curcolor[4], position[4];
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glGetFloatv(GL_CURRENT_COLOR, curcolor);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, position);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glRasterPos3f(x + 1, y + 1, z + 1);

	//glPrint(text);
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);//-255
	glCallLists(strlen(buffer), GL_UNSIGNED_BYTE, buffer);
	glPopAttrib();
	glEnable(GL_TEXTURE_2D);

	//restore ogl shit
	glPopAttrib();
	glEnable(GL_DEPTH_TEST);
	glColor4fv(curcolor);
	glRasterPos2f(position[0], position[1]);
}

/********************/

GLvoid KillFont(GLvoid)
{
	(glDeleteLists)(base, 96);
}

void drawnicertext(int shadow, float x, float y, float a, char *text, float fvalue) {

	if (shadow>0) {
		(glColor4f)(0.0f, 0.0f, 0.0f, a);
		(glRasterPos2f)(x + 0.002, y - 0.002);
		if (fvalue != -999)
			glPrint("%.2f", fvalue);	// Print GL Text To The Screen (float value)
		else
			glPrint(text);	// Shadow
	}
	(glColor4f)(1.0f, 1.0f, 1.0f, a);
	(glRasterPos2f)(x, y);
	if (fvalue != -999)
		glPrint("%.2f", fvalue);	// Print GL Text To The Screen (float value)
	else
		glPrint(text);			// Print GL Text To The Screen
}

void drawmenu()
{
	float tempx = startx;
	float width = 0;
	int z;
	(glColor3f)(1.0f, 1.0f, 1.0f);
	(glLineWidth)(1.0f);
	(glDisable)(GL_LINE_STIPPLE);

	//INIT MENU
	initvars();

	//NAVIGATION
	if ((GetAsyncKeyState(VK_LEFT) & 1)) {
		//if (!keypush) {

		if (selectedsubmenu == -1)
			selectedmenu--;
		else
			if (selectedsubmenu>-1)
			{
				if (submenu[selectedmenu][selectedsubmenu].value>submenu[selectedmenu][selectedsubmenu].minvalue)
				{
					if (selectedmenu == 0 && selectedsubmenu == 12) //max here
						submenu[selectedmenu][selectedsubmenu].value -= 10;
					else
						submenu[selectedmenu][selectedsubmenu].value--;
				}

				resetvars();
			}
		if (selectedmenu<0)
			selectedmenu = 0;

		//keypush = true;
		//}
	}
	else {
		if ((GetAsyncKeyState(VK_RIGHT) & 1)) {
			//if (!keypush) {
			if (selectedsubmenu == -1)
				selectedmenu++;
			else
				if (selectedsubmenu>-1)
				{
					if (submenu[selectedmenu][selectedsubmenu].value<submenu[selectedmenu][selectedsubmenu].maxvalue)
					{
						if (selectedmenu == 0 && selectedsubmenu == 12) //max here
							submenu[selectedmenu][selectedsubmenu].value += 10;
						else
							submenu[selectedmenu][selectedsubmenu].value++;
					}
					resetvars();
				}
			if (selectedmenu>maxmenus - 1)
				selectedmenu = maxmenus - 1;
			//keypush = true;
			//}
		}
		else {
			if ((GetAsyncKeyState(VK_DOWN)&1)) {
				//if (!keypush) {
					selectedsubmenu++;
					if (selectedsubmenu>mainmenu[selectedmenu].count)
						selectedsubmenu = mainmenu[selectedmenu].count;

					//keypush = true;
				//}
			}
			else {
				if ((GetAsyncKeyState(VK_UP)&1)) {
					//if (!keypush) {
						selectedsubmenu--;
						if (selectedsubmenu<-1)
							selectedsubmenu = -1;

						//keypush = true;
					//}
				}
				//else {
					//if ((GetAsyncKeyState(VK_RETURN))) {
						//if (!keypush) {
							//if (selectedmenu == 5 && selectedsubmenu == -1)
								//showmenu = false;
							//keypush = true;
						//}
					//}
					//else keypush = false;
				//}
			}
		}
	}

	if (showmenu)
	{
		//Highlight background (before we draw the font else wed hide it too much)
		//(glColor4f)(1.0, 0.0, 1, sin(colorpulse) / 1.5);
		glColor4f(1.0f, 0.0f, 0.0f, 0.3f);

		float mepos = 0;
		float addpos = 0;
		for (z = 0; z<selectedmenu; z++)
			mepos += mainmenu[z].width;

		if (selectedsubmenu>-1)
			addpos = mainmenu[selectedmenu].width + 0.18;

		(glBegin)(GL_QUADS);
		(glVertex2f)(startx - mainspan + mepos, starty + 0.03 - (selectedsubmenu + 1)*0.05);
		(glVertex2f)(startx + mainmenu[selectedmenu].width + mepos + addpos, starty + 0.03 - (selectedsubmenu + 1)*0.05);
		(glVertex2f)(startx + mainmenu[selectedmenu].width + mepos + addpos, starty - 0.01 - (selectedsubmenu + 1)*0.05);
		(glVertex2f)(startx - mainspan + mepos, starty - 0.01 - (selectedsubmenu + 1)*0.05);
		(glEnd)();

		colorpulse += 0.5f;
		if (colorpulse >= 180)
			colorpulse = 0;

		float subheight = 0.05f;

		////////////////////////
		for (int u = 0; u <= maxmenus; u++) {
			drawnicertext(0, tempx, starty, 1.0f, mainmenu[u].mainmenuentry, -999);
			if (selectedmenu == u)
			{
				for (int y = 0; y <= mainmenu[u].count; y++) {

					if (submenu[u][y].usevnames == false)
						drawnicertext(0, tempx + 0.22, starty - 0.05f - y*0.05f, 1.0f, "", submenu[u][y].value);
					else
						drawnicertext(0, tempx + 0.22, starty - 0.05f - y*0.05f, 1.0f, submenu[u][y].valuenames[(int)submenu[u][y].value], -999);

					drawnicertext(0, tempx, starty - 0.05f - y*0.05f, 1.0f, submenu[u][y].submenuentry, -999);
					subheight += 0.05f;
				}
			}
			tempx += mainmenu[u].width;
			width += mainmenu[u].width;
		}

		mepos = 0;

		// LEFT FROM SELECTED
		if (selectedmenu != 0)
		{
			for (z = 0; z<selectedmenu; z++)
				mepos += mainmenu[z].width;
			(glColor3f)(0.0f, 1.0f, 0.5f);
			(glBegin)(GL_LINE_LOOP);
			(glVertex2f)(startx - mainspan, starty - 0.01);
			(glVertex2f)(startx + mepos - mainspan, starty - 0.01);
			(glVertex2f)(startx + mepos - mainspan, starty + 0.04);
			(glVertex2f)(startx - mainspan, starty + 0.04);
			(glVertex2f)(startx - mainspan, starty - 0.01);
			(glEnd)();
			(glColor4f)(0, 1, 1, 0.4f);
			(glBegin)(GL_QUADS);
			(glVertex2f)(startx - mainspan, starty + 0.04);
			(glVertex2f)(startx + mepos - mainspan, starty + 0.04);
			(glVertex2f)(startx + mepos - mainspan, starty - 0.01);
			(glVertex2f)(startx - mainspan, starty - 0.01);
			(glEnd)();
		}
		mepos -= mainspan;

		// SELECTED
		float leftpos = mepos;
		mepos += mainmenu[selectedmenu].width;
		(glColor3f)(0.0f, 1.0f, 0.5f);
		float addi = 0;

		addi = mainspan;
		if (selectedmenu == maxmenus)
			mepos += mainspan;

		(glBegin)(GL_LINES);
		(glVertex2f)(startx + leftpos, starty + 0.05);
		(glVertex2f)(startx + mepos + addi, starty + 0.05);
		//-
		(glVertex2f)(startx + mepos + addi, starty - 0.01);
		(glVertex2f)(startx + mepos + addi, starty + 0.05);
		//-
		(glVertex2f)(startx + leftpos, starty - 0.01 - subheight);
		(glVertex2f)(startx + leftpos, starty + 0.05);
		(glEnd)();
		(glColor4f)(0, 1, 1, 0.2f);
		(glBegin)(GL_QUADS);
		(glVertex2f)(startx + leftpos, starty + 0.05);
		(glVertex2f)(startx + mepos + addi, starty + 0.05);
		(glVertex2f)(startx + mepos + addi, starty - 0.01);
		(glVertex2f)(startx + leftpos, starty - 0.01);
		(glEnd)();


		if (mainmenu[selectedmenu].count >= 0) {
			// MENUFIELD
			(glColor3f)(0.0f, 1.0f, 0.5f);
			(glBegin)(GL_LINES);
			(glVertex2f)(startx + leftpos, starty - 0.01 - subheight);
			(glVertex2f)(startx + leftpos + 0.4, starty - 0.01 - subheight);

			(glVertex2f)(startx + leftpos + 0.4, starty - 0.01 - subheight);
			(glVertex2f)(startx + leftpos + 0.4, starty - 0.01);

			(glVertex2f)(startx + leftpos + 0.4, starty - 0.01);
			(glVertex2f)(startx + mepos + addi, starty - 0.01);

			(glEnd)();

			(glColor4f)(0, 1, 1, 0.2f);
			(glBegin)(GL_QUADS);
			(glVertex2f)(startx + leftpos + 0.4, starty - 0.01);
			(glVertex2f)(startx + leftpos + 0.4, starty - 0.01 - subheight);
			(glVertex2f)(startx + leftpos, starty - 0.01 - subheight);
			(glVertex2f)(startx + leftpos, starty - 0.01);

			(glEnd)();
		}

		// RIGHT FROM SELECTED
		// MAIN
		float rightstartpos = startx + mepos + addi;

		mepos = 0;

		for (z = selectedmenu + 1; z <= maxmenus; z++)
			mepos += mainmenu[z].width;

		mepos += mainspan;
		(glColor3f)(0.0f, 1.0f, 0.5f);
		(glBegin)(GL_LINE_LOOP);
		(glVertex2f)(rightstartpos, starty - 0.01);
		(glVertex2f)(rightstartpos + mepos, starty - 0.01);
		(glVertex2f)(rightstartpos + mepos, starty + 0.04);
		(glVertex2f)(rightstartpos, starty + 0.04);
		(glVertex2f)(rightstartpos, starty - 0.01);
		(glEnd)();
		(glColor4f)(0, 1, 1, 0.4f);
		(glBegin)(GL_QUADS);
		(glVertex2f)(rightstartpos, starty + 0.04);
		(glVertex2f)(rightstartpos + mepos, starty + 0.04);
		(glVertex2f)(rightstartpos + mepos, starty - 0.01);
		(glVertex2f)(rightstartpos, starty - 0.01);
		(glEnd)();

		/////Draw particles
		//drawparticles(rightstartpos + mepos);
		////////////////////////////
	}
}

bool rendermenu()
{
	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);

	// otho view where 1.0 unit == 1 pixel:
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//glOrtho(0.0, viewport[2], 0.0, viewport[3], -1.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	// Draw in upper left.  This could become user-controlled if desired.
	//CVect2<float> pos(0.0f, (float)(viewport[3] - 10));
	drawmenu();

	glEnable(GL_TEXTURE_2D);
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();

	return true;
}

//isdead
bool IsDead()
{
	bool Dead = true;
	float m[16], p;

	glTranslatef(0.0f, 0.0f, 5.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	p = m[13];
	glTranslatef(0.0f, 0.0f, -5.0f);
	glTranslatef(0.0f, 0.0f, 250.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	if (p <= m[13])
		Dead = false;

	glTranslatef(0.0f, 0.0f, -250.0f); // translate the head back!

	return Dead;
}

//is point visible
int IsPointVisible(GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble	winX, winY, winZ;
	GLfloat pix;
	glGetDoublev(GL_MODELVIEW_MATRIX, mm);
	glGetDoublev(GL_PROJECTION_MATRIX, pm);

	if (gluProject(x, y, z, mm, pm, Viewport, &winX, &winY, &winZ) == GL_TRUE) {
		(glReadPixels)((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pix);

		if (pix>winZ)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
}

bool IsVertexVisible(GLfloat Vertex[3])
{
	GLdouble wX, wY, wZ, m[16], p[16];
	GLfloat px;
	GLint v[4];

	glGetIntegerv(GL_VIEWPORT, v);
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	glGetDoublev(GL_PROJECTION_MATRIX, p);

	if (gluProject(Vertex[0], Vertex[1], Vertex[2], m, p, v, &wX, &wY, &wZ) == GL_TRUE)
	{
		glReadPixels((int)wX, (int)wY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &px);

		if (px < 0) px = -px;

		if (px > wZ) return true;
	}

	return false;
}

void GetLargestVertexFromArray(int coordnum, int count, int stride, const GLvoid *indices, GLvoid *vertexarray, GLfloat vertex[3])
{
	GLfloat curCoord;
	UINT ci = 0;
	UINT *uIndices = (UINT*)indices;
	UCHAR *uVertexArray = (UCHAR*)vertexarray;
	GLfloat *curVertex;

	// Init vertex to zero
	memset(vertex, 0, sizeof(GLfloat[3]));

	// For all vertices referenced by indices
	for (int i = 0; i<count; i++)
	{
		// Get next z coord
		ci = uIndices[i];
		UINT s = stride / 8;
		UINT ix = ci * (sizeof(GLfloat) * 3 + s);

		curVertex = (GLfloat*)(uVertexArray + ix);
		curCoord = curVertex[coordnum];

		// Is current z coord closer?
		if (curCoord > *(vertex + coordnum))
		{
			// If so save the vertex
			vertex[0] = curVertex[2];
			vertex[1] = curVertex[1];
			vertex[2] = curVertex[0];
		}
	}
}


//w2s
void ObjectToWindowAim(float MViewMX[16], float ProjMX[16], int Viewport[4], float WindowOut[3])
{
	float x2 = ProjMX[0] * MViewMX[12] + ProjMX[4] * MViewMX[13] + ProjMX[8] * MViewMX[14] + ProjMX[12];
	float y2 = ProjMX[1] * MViewMX[12] + ProjMX[5] * MViewMX[13] + ProjMX[9] * MViewMX[14] + ProjMX[13] + aimheight;
	float z2 = ProjMX[2] * MViewMX[12] + ProjMX[6] * MViewMX[13] + ProjMX[10] * MViewMX[14] + ProjMX[14];
	float w2 = ProjMX[3] * MViewMX[12] + ProjMX[7] * MViewMX[13] + ProjMX[11] * MViewMX[14] + ProjMX[15];

	WindowOut[0] = (float)(Viewport[0] + Viewport[2]) * 0.5f * (x2 / w2 + 1.0f);
	WindowOut[1] = (float)(Viewport[1] + Viewport[3]) * 0.5f * (y2 / w2 + 1.0f);
	WindowOut[2] = (float)                               0.5f * (z2 / w2 + 1.0f);

	WindowOut[1] = (float)(Viewport[3] - WindowOut[1]);
}

void ObjectToWindowEsp(float MViewMX[16], float ProjMX[16], int Viewport[4], float WindowOut[3])
{
	float x2 = ProjMX[0] * MViewMX[12] + ProjMX[4] * MViewMX[13] + ProjMX[8] * MViewMX[14] + ProjMX[12];
	float y2 = ProjMX[1] * MViewMX[12] + ProjMX[5] * MViewMX[13] + ProjMX[9] * MViewMX[14] + ProjMX[13] + espheight; //40.0f
	float z2 = ProjMX[2] * MViewMX[12] + ProjMX[6] * MViewMX[13] + ProjMX[10] * MViewMX[14] + ProjMX[14];
	float w2 = ProjMX[3] * MViewMX[12] + ProjMX[7] * MViewMX[13] + ProjMX[11] * MViewMX[14] + ProjMX[15];

	WindowOut[0] = (float)(Viewport[0] + Viewport[2]) * 0.5f * (x2 / w2 + 1.0f);
	WindowOut[1] = (float)(Viewport[1] + Viewport[3]) * 0.5f * (y2 / w2 + 1.0f);
	WindowOut[2] = (float)                               0.5f * (z2 / w2 + 1.0f);

	WindowOut[1] = (float)(Viewport[3] - WindowOut[1]);
}
