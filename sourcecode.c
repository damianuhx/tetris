#define kDown 60
#define kRight 30

#define IsKeyDown(keyMap, theKey) (((unsigned char *)(keyMap))[(theKey) / 8] & 1 << ((theKey) % 8))

#define				keyArrowLeft			0x7b
#define				keyArrowRight			0x7c
#define				keyArrowUp				0x7e
#define				keyArrowDown			0x7d

#define				keyCommand				0x37
#define				keyOption				0x3A
#define				keyShift				0x38
#define				keyControl				0x3B
#define				keySpace				0x31
#define				keyEscape				0x35
#define				keyTab					0x30
#define				keyBackSpace			0x33
#define				keyDelete				0x75
 #define				keyCapsLock				0x39
 
#define				keyF1					0x7A
#define				keyF2					0x78
#define				keyF3					0x63
#define				keyF4					0x76
#define				keyF5					0x60
#define				keyF6					0x61
#define				keyF7					0x62
#define				keyF8					0x64
#define				keyF9					0x65
#define				keyF10					0x6D
#define				keyF11					0x67
#define				keyF12					0x6F
#define				keyF13					0x69
#define				keyF14					0x6B
#define				keyF15					0x71

#define				keyReturn				0x24
#define				keyEnter				0x4C
#define				keyEnterPB				0x34

#define				keyPad0					0x52
#define				keyPad1					0x53
#define				keyPad2					0x54
#define				keyPad3					0x55
#define				keyPad4					0x56
#define				keyPad5					0x57
#define				keyPad6					0x58
#define				keyPad7					0x59
#define				keyPad8					0x5B
#define				keyPad9					0x5C

#define				kButtonStateOn			0
#define				kButtonStateOff			128

#include <MacWindows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <PictUtils.h>
#include <Devices.h>
#include <LowMem.h>
#include <TextUtils.h>
RGBColor gBlackColour = { 0x0000, 0x0000, 0x0000 };
RGBColor gWhiteColour = { 0xFFFF, 0xFFFF, 0xFFFF };

KeyMap myKeyMap;

int error;
float h1, h2;
float au, au2;
int bu, bu2, bu3, bu4, bu5;
int P,X,Y;
float SzX, SzY;
short l, r, u, d, f;

short hgkzd[220], hgkzm[220]; /*Wo hat es kl�tze, wo m�ssen kl�tze gezeichnet werden*/
short hgkzot, hgkzop, hgkzox[4], hgkzoy[4]; /*Angaben f�r den herabfallenden Klotz: Typ, Rotation, X, Y)*/
short hgkzh1, hgkzh2, hgkzh3, hgkzh4; /*Hilfsvariablen*/
short hgkzdif; /*Geschwindigkeit der Kl�tze (wieviele Frames warten, bis wieder bewegen*/
short hgkzrot, hgkzrot2; /*Hilfsvariablen beim rotieren*/
short hgkzscore, hgkzrun; /*Punktezahl, Hilfsvariable um Loop zu stoppen*/
int hgkzplay;

int zeit, ko, kot, Q, Q2; /*Variablen, um die Zeit zu z�hlen (zeit, ko, kot), Q und Q2, werden im Moment noch nicht gebraucht*/
short C, C4, C16, C64, C256, C0, rd, rd2, rd3, rd4, rd5; /*Zufallszahlen (rd), sowie div. Z�hler*/
short FPS=2; /*Warten zwischen den einzelnen Bildern: 2 => 60 ticks/2 = 30 Frames per second*/

PicHandle Pic[1000];
Rect Bounds[1000];
Rect OBounds[1000];

Boolean gSoundAchtung = false;

OSErr AS_Initialise (Boolean *attnFlag,short numChannels);
OSErr AS_PlayID (short resID, short refNum);

OSErr damianIgnoriertFehler;


void AS_CloseChannel(void);
void AS_CloseDown(void);

void hgkzdraw();
void hgkzobj (int hgkzobj1, int hgkzobj2);
void hgkzmove ();
void hgkzadd ();
void hgkzcontrol();
void load();
void draw(int P, int X, int Y,float SzX, float SzY);
void input ();
void count ();
void play (short snd);
void DaLoadFile	(short str);
void stopsound ();


static void GetMBarRgn(RgnHandle mBarRgn);
static void SH_ForceUpdate(RgnHandle rgn);


int main(void)
{

	
	
	int i;
	
	RgnHandle		GrayRgn = LMGetGrayRgn();
	RgnHandle			mBarRgn;
	unsigned short		mBarHeight;
	
	QDErr help;
	WindowPtr dasWindow;
	OSStatus err = 0;
	
	
	GWorldPtr myGworld = (GWorldPtr)NewPtr(sizeof(GWorldPtr));
	GWorldPtr oldWorld = (GWorldPtr)NewPtr(sizeof(GWorldPtr));
	
	damianIgnoriertFehler = AS_Initialise (&gSoundAchtung, 256);
	DaLoadFile (101);
	
	load();
	
	DaLoadFile (111);
	DaLoadFile (112);
	DaLoadFile (113);
	
	
	
	SetRect (&Bounds[0], 0, 0, 640, 480);
	Bounds[0]=OBounds[0];
	
		
	/*Menubar ausblenden*/
	mBarHeight = GetMBarHeight();
	mBarRgn = NewRgn();
	GetMBarRgn(mBarRgn);
	LMSetMBarHeight(0);
	UnionRgn(GrayRgn,mBarRgn,GrayRgn);
	SH_ForceUpdate(mBarRgn);
	
	/*Zeichnungsebene Festlegen*/
	
	InitGraf(&qd.thePort);
	InitWindows();
	dasWindow = GetNewCWindow(128, nil, (WindowPtr)-1);
	SetPort(dasWindow);
	err = TransitionWindow(dasWindow, kWindowZoomTransitionEffect, kWindowShowTransitionAction, nil);
	help = NewGWorld(&myGworld,0,&(dasWindow->portRect),NULL,NULL,0);
	GetGWorld(&oldWorld,nil);
	SetGWorld(myGworld,nil);
	LockPixels(GetGWorldPixMap(myGworld));
	
	HideCursor ();

	/*SetGWorld(oldWorld,nil);*/
	/*RGBBackColor(&gWhiteColour);
	RGBForeColor(&gBlackColour);*/
	/*CopyBits(&((GrafPtr) myGworld)->portBits,&((GrafPtr) dasWindow)->portBits,&Bounds[128],&Bounds[128],srcCopy,NULL);
	Bounds[128] = OBounds[128];*/
	
	
	kot=(int)TickCount();
	
	hgkzobj (1, 0);
	
	draw (2, 455,240, 1,1);
	hgkzplay = (int)TickCount();
	play (10);
	
	for (bu=0; bu<10; bu++)
	{
	hgkzm[bu]=1;
	/*hgkzd[bu]=1;*/
	}

	
	

/*MAIN PRG LOOP*/
	while (hgkzrun<1)
	{
	if ((int)TickCount()-zeit>FPS)
	{
	
	draw (3, 135,240, 1,1);
	zeit = (int)TickCount();
	count();
	
	if (hgkzplay+(84*60)<(int)TickCount()) 
	{play (10);
	hgkzplay = (int)TickCount();}
	
	GetGWorld(&oldWorld,nil);
	SetGWorld(myGworld,nil);
		hgkzh4++;
		hgkzdraw();
		input();
		hgkzcontrol ();
		hgkzdif=12-sqrt(hgkzscore);

		if (i++>hgkzdif)
			{
			i=0;
			hgkzmove ();
			}
		
		
		CopyBits(&((GrafPtr) myGworld)->portBits,&((GrafPtr) dasWindow)->portBits,&OBounds[0],&Bounds[0],srcCopy,NULL);
		
	}
	}
/*MAIN PRG LOOP END*/

/*for (bu=0; bu<200; bu++)
{
printf("%d", hgkzm[bu]);
printf("%d", hgkzd[bu]);
}
printf("%d", hgkzox[0]);
printf("%d", hgkzoy[0]);
printf("%d", hgkzox[1]);
printf("%d", hgkzoy[1]);
printf("%d", hgkzox[2]);
printf("%d", hgkzoy[2]);
printf("%d", hgkzox[3]);
printf("%d", hgkzoy[3]);
*/
	
	printf("Score: %d", hgkzscore);
	SetGWorld(oldWorld,nil);
	AS_CloseDown();
	while(!Button())
	
	return 0;
} 
  
void load()
{
for (bu=0; bu<1000; bu++)
{
Pic[bu] = GetPicture (bu);
Bounds[bu] = ((**Pic[bu]).picFrame);
OBounds[bu] = Bounds[bu];
}
}

void draw(int P, int X, int Y,float SzX, float SzY)
{
h1=(((OBounds[P].right)-(OBounds[P].left)));
h2=(((OBounds[P].bottom)-(OBounds[P].top)));
SetRect (&Bounds[P], 0, 0, ((SzX)*(h1)), ((SzY)*(h2)));


OffsetRect(&Bounds[P], (X+((((Bounds[P]).left)-((Bounds[P]).right)/2))), (int)(Y+(((Bounds[P]).top)-((Bounds[P]).bottom)/2)));

DrawPicture(Pic[P], &Bounds[P]);	

Bounds[P]=OBounds[P];
}


			
			



void input ()
{
GetKeys(myKeyMap);
l = IsKeyDown(myKeyMap, keyArrowLeft);	
if (l>0) {l=1;}
r = IsKeyDown(myKeyMap, keyArrowRight);	
if (r>0) {r=1;}
u = IsKeyDown(myKeyMap, keyArrowUp);	
if (u>0) {u=1;}
d = IsKeyDown(myKeyMap, keyArrowDown);	
if (d>0) {d=1;}
f = IsKeyDown(myKeyMap, keySpace);
hgkzrun= IsKeyDown(myKeyMap, keyEscape);
}
 
void count ()
	{
	 if (++C0==2)
  	{
 	C0=0;
	 }
	
 	ko=(int)TickCount()-kot;
 	
	 
 	/*Zufallszahl (Jede Runde Neu)*/
	
	qd.randSeed=TickCount();
	rd=(Random()&0X7FFF)%100;
	  
	qd.randSeed=TickCount()/2;
	  rd2=(Random()&0X7FFF)%100;
	
	qd.randSeed=TickCount()/3;
 	rd3=(Random ()&0X7FFF)%100;  
	 
	qd.randSeed=TickCount()/4;
	rd4=(Random()&0X7FFF)%100;
	
	qd.randSeed=TickCount();  
	rd5=(Random()&0X7FFF)%7;
	  }
	 
	
	 
 	 void hgkzdraw()
	{
	 
	
	
 	for (hgkzh1=0; hgkzh1<=220; hgkzh1++)
	{
	
	if (hgkzd[hgkzh1])
	{
	hgkzh3=hgkzh1/10;
	hgkzh2=hgkzh1-hgkzh3*10;
	
	draw (1, 30+25*hgkzh2, 480-25*hgkzh3, 1, 1);
	}
	}
	
	}
	
	void hgkzmove  ()
	{
	if (hgkzm[10*(hgkzoy[0]-1)+hgkzox[0]])
	{ hgkzadd();}
	if (hgkzm[10*(hgkzoy[1]-1)+hgkzox[1]])
	{ hgkzadd();}
	if (hgkzm[10*(hgkzoy[2]-1)+hgkzox[2]])
	{ hgkzadd();}
	if (hgkzm[10*(hgkzoy[3]-1)+hgkzox[3]])
	{ hgkzadd();}
	
	if (hgkzoy[0]==1)
	{ hgkzadd();}
	if (hgkzoy[1]==1)
	{ hgkzadd();}
	if (hgkzoy[2]==1)
	{ hgkzadd();}
	if (hgkzoy[3]==1)
	{ hgkzadd();}
	
	
	hgkzd[hgkzox[0]+hgkzoy[0]*10]=0;
	hgkzd[hgkzox[1]+hgkzoy[1]*10]=0;
	hgkzd[hgkzox[2]+hgkzoy[2]*10]=0;
	hgkzd[hgkzox[3]+hgkzoy[3]*10]=0;
	
	hgkzoy[0]--;
	hgkzoy[1]--;
	hgkzoy[2]--;
	hgkzoy[3]--;
	
	hgkzd[hgkzox[0]+hgkzoy[0]*10]=1;
	hgkzd[hgkzox[1]+hgkzoy[1]*10]=1;
	hgkzd[hgkzox[2]+hgkzoy[2]*10]=1;
	hgkzd[hgkzox[3]+hgkzoy[3]*10]=1;
	}
	
	
	void hgkzcontrol()
	{
	
	
	
	
	if (f)
	{
	if (hgkzh4>3)
	{
	bu2=0;
	for (bu=1; bu<4; bu++)
	{
	hgkzrot=hgkzox[0]+(hgkzoy[0]-hgkzoy[bu]);
	hgkzrot2=hgkzoy[0]-(hgkzox[0]-hgkzox[bu]);
	if (hgkzm[10*hgkzrot2+hgkzrot]) {bu2=3;}
	if (hgkzrot<0) {bu2=3;}
	if (hgkzrot>9) {bu2=3;}
	}
	if (bu2!=3)
	{
	
	for (bu=1; bu<4; bu++)
	{
	hgkzd[hgkzoy[bu]*10+hgkzox[bu]]=0;
	hgkzrot=hgkzox[0]+(hgkzoy[0]-hgkzoy[bu]);
	hgkzoy[bu]=hgkzoy[0]-(hgkzox[0]-hgkzox[bu]);
	hgkzox[bu]=hgkzrot;
	}
	
	for (bu=1; bu<4; bu++)
	{
	hgkzd[hgkzoy[bu]*10+hgkzox[bu]]=1;
  	 }
  	 
  	hgkzdraw();
  	hgkzh4=0;
	}
	}
 	}
 	
	
	
	if (d)
	{
	hgkzmove();
	}
	
	if (l)
	{
	if (hgkzox[0]>0)
	{
	if (hgkzox[1]>0)
	{
	if (hgkzox[2]>0)
	{
	if (hgkzox[3]>0)
	{
	if (hgkzm[10*(hgkzoy[0])+hgkzox[0]-1]==0)
	{ 
	if (hgkzm[(10*hgkzoy[1])+hgkzox[1]-1]==0)
	{ 
	if (hgkzm[10*(hgkzoy[2])+hgkzox[2]-1]==0)
	{ 
	if (hgkzm[10*(hgkzoy[3])+hgkzox[3]-1]==0)
	{ 
	hgkzd [hgkzox[0]+hgkzoy[0]*10]=0;
	hgkzd [hgkzox[1]+hgkzoy[1]*10]=0;
	hgkzd [hgkzox[2]+hgkzoy[2]*10]=0;
	hgkzd [hgkzox[3]+hgkzoy[3]*10]=0;
	
	hgkzox[0]--;
	hgkzox[1]--;
	hgkzox[2]--;
	hgkzox[3]--;
	
	hgkzd [hgkzox[0]+hgkzoy[0]*10]=1;
	hgkzd [hgkzox[1]+hgkzoy[1]*10]=1;
	hgkzd [hgkzox[2]+hgkzoy[2]*10]=1;
	hgkzd [hgkzox[3]+hgkzoy[3]*10]=1;
	
	}}}}}}}}}
	
	if (r)
	{
	if (hgkzox[0]<9)
	{
	if (hgkzox[1]<9)
	{
	if (hgkzox[2]<9)
	{
	if (hgkzox[3]<9)
	{
	if (hgkzm[10*(hgkzoy[0])+hgkzox[0]+1]==0)
	{ 
	if (hgkzm[(10*hgkzoy[1])+hgkzox[1]+1]==0)
	{ 
	if (hgkzm[10*(hgkzoy[2])+hgkzox[2]+1]==0)
	{ 
	if (hgkzm[10*(hgkzoy[3])+hgkzox[3]+1]==0)
	{ 
	hgkzd [hgkzox[0]+hgkzoy[0]*10]=0;
	hgkzd [hgkzox[1]+hgkzoy[1]*10]=0;
	hgkzd [hgkzox[2]+hgkzoy[2]*10]=0;
	hgkzd [hgkzox[3]+hgkzoy[3]*10]=0;
	
	hgkzox[0]++;
	hgkzox[1]++;
	hgkzox[2]++;
	hgkzox[3]++;
	
	hgkzd [hgkzox[0]+hgkzoy[0]*10]=1;
	hgkzd [hgkzox[1]+hgkzoy[1]*10]=1;
	hgkzd [hgkzox[2]+hgkzoy[2]*10]=1;
	hgkzd [hgkzox[3]+hgkzoy[3]*10]=1;
	
	}}}}}}}}}
	
	}
	
	
	
	void hgkzobj (int hgkzobj1, int hgkzobj2)
	{
	hgkzot=hgkzobj1;
	hgkzop=hgkzobj2;
	hgkzox[0]=5;
	hgkzoy[0]=21;
	
	
	
	if (hgkzot==1)
	{
	hgkzox[1]=hgkzox[0]-1;
	hgkzox[2]=hgkzox[0]-2;
	hgkzox[3]=hgkzox[0]+1;
	
	hgkzoy[1]=hgkzoy[0];
	hgkzoy[2]=hgkzoy[0];
	hgkzoy[3]=hgkzoy[0];
	}
	
	if (hgkzot==0)
	{
	hgkzox[2]=hgkzox[0]-1;
	hgkzox[1]=hgkzox[0]-1;
	hgkzox[3]=hgkzox[0]+1;
	
	hgkzoy[2]=hgkzoy[0];
	hgkzoy[1]=hgkzoy[0]+1;
	hgkzoy[3]=hgkzoy[0];
	}
	
	if (hgkzot==2)
	{
	hgkzox[1]=hgkzox[0]-1;
	hgkzox[2]=hgkzox[0]-1;
	hgkzox[3]=hgkzox[0]+1;
	
	hgkzoy[1]=hgkzoy[0];
	hgkzoy[2]=hgkzoy[0]-1;
	hgkzoy[3]=hgkzoy[0];
	}
	
	if (hgkzot==3)
	{
	hgkzox[1]=hgkzox[0]-1;
	hgkzox[2]=hgkzox[0];
	hgkzox[3]=hgkzox[0]+1;
	
	hgkzoy[1]=hgkzoy[0];
	hgkzoy[2]=hgkzoy[0]-1;
	hgkzoy[3]=hgkzoy[0];
	}
	
	if (hgkzot==4)
	{
	hgkzox[1]=hgkzox[0];
	hgkzox[2]=hgkzox[0]-1;
	hgkzox[3]=hgkzox[0]-1;
	
	hgkzoy[1]=hgkzoy[0]+1;
	hgkzoy[2]=hgkzoy[0];
	hgkzoy[3]=hgkzoy[0]-1;
	}
	
	if (hgkzot==5)
	{
	hgkzox[1]=hgkzox[0];
	hgkzox[2]=hgkzox[0]+1;
	hgkzox[3]=hgkzox[0]+1;
	
	hgkzoy[1]=hgkzoy[0]+1;
	hgkzoy[2]=hgkzoy[0];
	hgkzoy[3]=hgkzoy[0]+1;
	}
	
	if (hgkzot==6)
	{
	hgkzox[1]=hgkzox[0];
	hgkzox[2]=hgkzox[0]+1;
	hgkzox[3]=hgkzox[0]+1;
	
	hgkzoy[1]=hgkzoy[0]+1;
	hgkzoy[2]=hgkzoy[0];
	hgkzoy[3]=hgkzoy[0]-1;
	}
	
	}
	
	
	void hgkzadd()
	{
	hgkzm[hgkzox[0]+hgkzoy[0]*10]=1;
	hgkzm[hgkzox[1]+hgkzoy[1]*10]=1;
	hgkzm[hgkzox[2]+hgkzoy[2]*10]=1;
	hgkzm[hgkzox[3]+hgkzoy[3]*10]=1;
	
	if (hgkzm[200]+hgkzm[200]+hgkzm[201]+hgkzm[202]+hgkzm[203]+hgkzm[205]+hgkzm[206]+hgkzm[207]+hgkzm[208]+hgkzm[209]+hgkzm[204])
	{hgkzrun=10;}
	
	for (bu=1; bu<22; bu++)
	{
	bu3=0;

	for (bu2=0; bu2<10; bu2++)
	{
	if (hgkzm[10*bu+bu2])
	{ bu3++;}
	}
	
	if (bu3==10)
	{
	hgkzscore++;
	bu4=bu*10;
	for (bu5=0; bu5<10; bu5++)
	{
	hgkzm[bu*10+bu5]=0;
	hgkzd[bu*10+bu5]=0;
	}
	
	for (bu4; bu4<220; bu4++)
	{
	if (hgkzm[bu4])
	{
	hgkzm[bu4-10]=1;
	hgkzm[bu4]=0;
	hgkzd[bu4-10]=1;
	hgkzd[bu4]=0;
	}
	}
	bu--;
	}
	}
	
	
	 hgkzobj (rd5,0);
	 }
	void play (short snd)
	{
	damianIgnoriertFehler = AS_PlayID (snd, nil);
	}
	
	
	
	
	void DaLoadFile	(short str)
{
	int daFile;
	OSErr ignore;
	FSSpec daSpec, newSpec;
	FSRef daRef;
	
	ignore = FSGetForkCBInfo(CurResFile(), 0, nil, nil, nil, &daRef, nil);
	ignore = FSGetCatalogInfo(&daRef, kFSCatInfoNone, nil, nil, &daSpec, nil);
	ignore = FSMakeFSSpec(daSpec.vRefNum, daSpec.parID, *(GetString(str)), &newSpec);  
	daFile = FSpOpenResFile(&newSpec, fsRdWrPerm);
	
	UseResFile(daFile);
}


	
	
void stopsound ()
{
AS_CloseDown();
damianIgnoriertFehler = AS_Initialise (&gSoundAchtung, 256);
}


static void GetMBarRgn(RgnHandle mBarRgn)
{
	Rect			mBarRect;

	mBarRect = qd.screenBits.bounds;
	mBarRect.bottom = mBarRect.top + GetMBarHeight();
	RectRgn(mBarRgn, &mBarRect);
}

static void SH_ForceUpdate(RgnHandle rgn)
{
	WindowRef		wpFirst = LMGetWindowList();
	
	PaintBehind(wpFirst, rgn);
	CalcVisBehind(wpFirst, rgn);
}
