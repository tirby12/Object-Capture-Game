#include <glut.h>			//must be included for OpenGL
#include <gl\gl.h>				//must be included for OpenGL
#include <math.h>
#include <time.h>
#include <string>			//must be included for time functions
#include <iostream>			//must be included for console input/output
using namespace std;

#define WINDOW_WID 900
#define WINDOW_HEI 600

int randomPx, randomPy;
int move1, move2, move3, msg;
int xc, xa, ya, radius;
int lxa, lxb;
const int NBALLS=20;
int powballx[3], powbally[3];
int ballx[NBALLS], bally[NBALLS];
unsigned int picid1, picid2;

int pic;
//cart varibles
	int vradius;
	int vc1x, vc1y;
	int vc2x, vc2y;
	int rectanglex[4];
	int rectangley[4];


/////////////////////////////////////////

#include <windows.h> // Header File For Windows
#include <olectl.h> // Header File For The OLE Controls Library
#include <math.h> // Header File For The Math Library

int BuildTexture(char *szPathName, GLuint &texid)      // Load Image And Convert To A Texture
{
 #include <windows.h> // Header File For Windows
 #include <olectl.h> // Header File For The OLE Controls Library
 #include <math.h> // Header File For The Math Library
 HDC   hdcTemp;            // The DC To Hold Our Bitmap
 HBITMAP  hbmpTemp;            // Holds The Bitmap Temporarily
 IPicture *pPicture;            // IPicture Interface
 OLECHAR  wszPath[MAX_PATH+1];         // Full Path To Picture (WCHAR)
 char  szPath[MAX_PATH+1];          // Full Path To Picture
 long  lWidth;             // Width In Logical Units
 long  lHeight;            // Height In Logical Units
 long  lWidthPixels;           // Width In Pixels
 long  lHeightPixels;           // Height In Pixels
 GLint  glMaxTexDim ;           // Holds Maximum Texture Size

 if (strstr(szPathName, "http://"))         // If PathName Contains http:// Then...
 {
  strcpy(szPath, szPathName);          // Append The PathName To szPath
 }
 else                // Otherwise... We Are Loading From A File
 {
  GetCurrentDirectory(MAX_PATH, szPath);       // Get Our Working Directory
  strcat(szPath, "\\");           // Append "\" After The Working Directory
  strcat(szPath, szPathName);          // Append The PathName
 }

 MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);  // Convert From ASCII To Unicode
 HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

 if(FAILED(hr))              // If Loading Failed
  return FALSE;             // Return False

 hdcTemp = CreateCompatibleDC(GetDC(0));        // Create The Windows Compatible Device Context
 if(!hdcTemp)              // Did Creation Fail?
 {
  pPicture->Release();           // Decrements IPicture Reference Count
  return FALSE;             // Return False (Failure)
 }

 glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);     // Get Maximum Texture Size Supported
 
 pPicture->get_Width(&lWidth);          // Get IPicture Width (Convert To Pixels)
 lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
 pPicture->get_Height(&lHeight);          // Get IPicture Height (Convert To Pixels)
 lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

 // Resize Image To Closest Power Of Two
 if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
  lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
 else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
  lWidthPixels = glMaxTexDim;
 
 if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
  lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
 else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
  lHeightPixels = glMaxTexDim;
 
 // Create A Temporary Bitmap
 BITMAPINFO bi = {0};            // The Type Of Bitmap We Request
 DWORD  *pBits = 0;            // Pointer To The Bitmap Bits

 bi.bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);    // Set Structure Size
 bi.bmiHeader.biBitCount  = 32;         // 32 Bit
 bi.bmiHeader.biWidth  = lWidthPixels;       // Power Of Two Width
 bi.bmiHeader.biHeight  = lHeightPixels;      // Make Image Top Up (Positive Y-Axis)
 bi.bmiHeader.biCompression = BI_RGB;        // RGB Encoding
 bi.bmiHeader.biPlanes  = 1;         // 1 Bitplane

 // Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
 hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
 
 if(!hbmpTemp)              // Did Creation Fail?
 {
  DeleteDC(hdcTemp);            // Delete The Device Context
  pPicture->Release();           // Decrements IPicture Reference Count
  return FALSE;             // Return False (Failure)
 }

 SelectObject(hdcTemp, hbmpTemp);         // Select Handle To Our Temp DC And Our Temp Bitmap Object

 // Render The IPicture On To The Bitmap
 pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

 // Convert From BGR To RGB Format And Add An Alpha Value Of 255
 for(long i = 0; i < lWidthPixels * lHeightPixels; i++)    // Loop Through All Of The Pixels
 {
  BYTE* pPixel = (BYTE*)(&pBits[i]);       // Grab The Current Pixel
  BYTE  temp  = pPixel[0];         // Store 1st Color In Temp Variable (Blue)
  pPixel[0]  = pPixel[2];         // Move Red Value To Correct Position (1st)
  pPixel[2]  = temp;           // Move Temp Value To Correct Blue Position (3rd)

  // This Will Make Any Black Pixels, Completely Transparent  (You Can Hardcode The Value If You Wish)
  //if ((pPixel[0]==0) && (pPixel[1]==0) && (pPixel[2]==0))   // Is Pixel Completely Black
  // pPixel[3] =   0;           // Set The Alpha Value To 0
  //else               // Otherwise
   pPixel[3] = 255;           // Set The Alpha Value To 255
 }

 glGenTextures(1, &texid);           // Create The Texture

#if 1
 // Typical Texture Generation Using Data From The Bitmap
 glBindTexture(GL_TEXTURE_2D, texid);        // Bind To The Texture ID
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  // (Modify This For The Type Of Filtering You Want)
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);     // (Modify This For The Type Of Filtering You Want)
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits); // (Modify This If You Want Mipmaps)
#else
 //Typical Bitmap Texture Generation Using Data From The Bitmap
 glBindTexture(GL_TEXTURE_2D,texid);
 //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
 glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    //Here we are setting the parameter to repeat the texture instead of clamping the texture to the edge of our shape.
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //Generate the texture with mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits );
#endif

 DeleteObject(hbmpTemp);            // Delete The Object
 DeleteDC(hdcTemp);             // Delete The Device Context

 pPicture->Release();

 return TRUE;              // Return True (All Good)
}

void myInit(void)
{
	pic=0;
	msg=0;
	for (int i=0; i<NBALLS;i++)
	{
                ballx[i]=rand() % WINDOW_WID;
                bally[i]=WINDOW_HEI+ rand() % WINDOW_HEI;
                for (int k=0; k<i; k++)
                {
                        if(sqrt(1.0*(ballx[i]-ballx[k])*(ballx[i]-ballx[k])+(bally[i]-bally[k])*(bally[i]-bally[k])) < 30)
                        {
                                ballx[i]=rand() % WINDOW_WID;
		                bally[i]=WINDOW_HEI+ rand() % WINDOW_HEI;
                                k=-1;
                        }
                }
	}
      	for (int i=0; i<3;i++)
	{
                powballx[i]=rand() % WINDOW_WID;
                powbally[i]=WINDOW_HEI+ rand() % WINDOW_HEI;
                for (int k=0; k<i; k++)
                {
                        if(sqrt(1.0*(powballx[i]-powballx[k])*(powballx[i]-powballx[k])+(powbally[i]-powbally[k])*(powbally[i]-powbally[k])) < 30)
                        {
                                powballx[i]=rand() % WINDOW_WID;
		                powbally[i]=WINDOW_HEI+ rand() % WINDOW_HEI;
                                k=-1;
                        }
                }
	}
	radius=5;
	ya=0;
	xc=10;
	xa=20;
	move3=0;
	randomPx = 400;
	randomPy = 300;
	glClearColor (0.0, 0.0, 1.0, 0.0);   // set background color to black
	glShadeModel (GL_FLAT);

	if(BuildTexture("me.jpeg", picid1)==false)
		cout<<"Load Texture Failed\n";

	if(BuildTexture("samantha.jpg", picid2)==false)
		cout<<"Load Texture Failed\n";


}

void DisplayString(string str, int x, int y)
{
        int len, i;
		
        glRasterPos2f(x, y);
        len = str.length();
        for (i = 0; i < len; i++)
        {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
        }
}

void DrawCircle( int a, int b, int r, int n)
{
	double theta = 2*3.14159/n;
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
		for(int i=0;i<n;i++)
			glVertex2d(a+r*cos(i*theta),b+r*sin(i*theta));
	glEnd();
}

void DrawWheel(int a, int b, int c)
{
	glPushMatrix();
	glTranslated(a,b,0);
	glRotated(move3,0,0,1);
	glTranslated(-a,-b,0);
	DrawCircle(a,b,c,100);
	glColor3f(1,1,0);
	glBegin(GL_LINES);
		glVertex2d(a, b-c);
        glVertex2d(a, b+c);
		//Set the position of the vertex
		glVertex2d(a-c, b);
		glVertex2d(a+c, b);
		//Set the position of the vertex
	glEnd();
	glPopMatrix();
}

class CART
{
public:
	CART()
	{
		vradius=3;

		rectanglex[0]=0;
		rectanglex[1]=30;
		rectanglex[2]=rectanglex[1];
		rectanglex[3]=rectanglex[0];

		rectangley[0]=2*vradius;
		rectangley[1]=2*vradius;
		rectangley[2]=2*vradius+20;
		rectangley[3]=2*vradius+20;

		vc1x=vradius+5;
		vc1y=vradius;

		vc2x=rectanglex[1]-vradius-5;
		vc2y=vradius;
	}
	void DisplayCart()
	{
		glBegin(GL_POLYGON);
			for(int i=0;i<4;i++)
			{
				glVertex2d(rectanglex[i], rectangley[i]);
			}
		glEnd();
		DrawCircle(vc1x, vc1y, vradius, 100);
		DrawCircle(vc2x, vc2y, vradius, 100);
	}
	void MoveRight(int p=10)
	{
		if(rectanglex[1]<WINDOW_WID)
		{
			vc1x+=p;
			vc2x+=p;
			rectanglex[0]+=p;
			rectanglex[1]+=p;
			rectanglex[2]+=p;
			rectanglex[3]+=p;
                        msg=1;
		}
		else
                        msg=5;
	}
	void MoveLeft(int p=10)
	{
		if(rectanglex[0]>0)
		{
			vc1x-=p;
			vc2x-=p;
			rectanglex[0]-=p;
			rectanglex[1]-=p;
			rectanglex[2]-=p;
			rectanglex[3]-=p;
                        msg=1;
		}
                else
                        msg=4;

	}

        bool Colliding()
        {
                for(int i=0; i<NBALLS; i++)
                {
                        if(rectanglex[0]<=ballx[i]&&ballx[i]<=rectanglex[1] && (bally[i]-rectangley[2])<=20)
                        {
                                return 1;
                        }
                        if((ballx[i]-rectanglex[0])*(ballx[i]-rectanglex[0])+(bally[i]-rectangley[2])*(bally[i]-rectangley[2])<= 20*20)
                        {
                                return 1;
                        }
                        if((ballx[i]-rectanglex[1])*(ballx[i]-rectanglex[1])+(bally[i]-rectangley[1])*(bally[i]-rectangley[1])<= 20*20)
                        {
                                return 1;
                        }
                }
                return 0;
        }
};
CART cart;

void DrawCart(int a, int b, int c)
{
	DrawWheel(a,c,radius);
	DrawWheel(b,c,radius);
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
		glVertex2d(a-5,c+radius);
		glVertex2d(a-5,c+10);

		glVertex2d(b+5,c+10);
		glVertex2d(b+5,c+radius);
	glEnd();
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);        // clear the screen
	glColor3f(1,1,1);
	glLineWidth(5);
	cart.DisplayCart();//calls cart's Display function to display the cart
	for(int i=0; i<NBALLS; i++)
	{
		DrawCircle(ballx[i], bally[i], 15, 100);
	}


        if (msg==0)
        {
				
            	glPushMatrix();
				glTranslated(10,200,0);
				glRotated(45,0,0,1);
				glScaled(.25,.5,0);
				DisplayString("Left Click to Play", (WINDOW_WID/2), (WINDOW_HEI/2));
				glPopMatrix();    
        }
        else if(msg==1)
        {
                DisplayString("Daddy is playing", 0, WINDOW_HEI-15);
        }
        else if(msg==2)
        {
                DisplayString("Game is Paused", (WINDOW_WID/2)-20, (WINDOW_HEI/2)-15);
                DisplayString("Left Click to Continue", (WINDOW_WID/2)-20, (WINDOW_HEI/2)-30);
        }
        else if (msg==3)
        {
                DisplayString("GAME OVER!!!", (WINDOW_WID/2)-20, (WINDOW_HEI/2)-15);
        }
        else if (msg==4)
        {
                DisplayString("BOUNDRY REACHED!!", (WINDOW_WID/2)-20, (WINDOW_HEI/2)-15);

        }
         else if (msg==5)
        {
                DisplayString("BOUNDRY REACHED!!", (WINDOW_WID/2)-20, (WINDOW_HEI/2)-15);

        }

		// if(pic=1)
		//	glBindTexture(GL_TEXTURE_@D, picid1);
		 //else if(pic=2)
			glBindTexture(GL_TEXTURE_2D, picid2);
		 glEnable(GL_TEXTURE_2D);
		 glBegin(GL_POLYGON);
			glTexCoord2d(0,0);
			glVertex2d(100, 100);
			glTexCoord2d(1,0);
			glVertex2d(400, 100);
			glTexCoord2d(1,1);
			glVertex2d(400, 400);
			glTexCoord2d(0,1);
			glVertex2d(100, 400);
		glEnd();
		
			
		glutSwapBuffers ();					//put everything on your screen
}


void myReshape ( int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);			// set "camera type"
	glLoadIdentity ();						// clear the matrix

	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);		// viewing transformation
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void Animation()
{
	srand(time(NULL)+rand());				//set the seed for your rand function
	randomPx = rand()%WINDOW_WID;
	randomPy = rand()%WINDOW_HEI;
	Sleep(200);//put the program to sleep for 200 ms


	for (int i=0; i<NBALLS; i++)
	{
		bally[i]-=30;
                if (cart.Colliding()==true)
                {
                        glutIdleFunc(NULL);
                        msg=3;
                }


	}

	for (int i=0; i<NBALLS; i++)
	{
		if (bally[i]<0-15)
		{
			bally[i]=WINDOW_HEI+20;
			ballx[i]=rand() % WINDOW_WID;
                        for (int k=0; k<i; k++)
                        {
                                if(sqrt(1.0*(ballx[i]-ballx[k])*(ballx[i]-ballx[k])+(bally[i]-bally[k])*(bally[i]-bally[k])) < 30)
                                {
                                        ballx[i]=rand() % WINDOW_WID;
		                        bally[i]=WINDOW_HEI+ rand() % WINDOW_HEI;
                                        k=-1;
                                }
                        }
		}
	}

      /*	for (int i=0; i<NBALLS;i++)
	{
		if(rectanglex[0]-15 < ballx[i] && ballx[i] < rectanglex[1]+15 && bally[i] < rectangley[2])
		{
				for(int j=0;j<NBALLS;j++)
				{
					bally[j]=-15;
				}
                                msg=3;
                                cout<<msg<<endl;
				Sleep(500);
				exit(0);
		}

	}*/


	myDisplay();
}

void myMouse(int button, int state, int x, int y)
{
	y = WINDOW_HEI - y;
	switch (button)
	{
		case GLUT_LEFT_BUTTON:				//when left mouse button is clicked
			if (state == GLUT_DOWN)
			{
                                
				glutIdleFunc(NULL);
			}
			else if(state == GLUT_UP)
			{
                                msg=1;
								pic=1;
				glutIdleFunc(Animation);//do Animation when idle
			}
			break;
		case GLUT_RIGHT_BUTTON:				//when right mouse button is clicked
			if (state == GLUT_DOWN)
			{
                              
				glutIdleFunc(NULL);			//do nothing when idle
			}         
			else if (state == GLUT_UP)
			{
                                msg=2;
								pic=2;
				glutIdleFunc(NULL);
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
			{
				exit (-1);					//exit your program
			}
			break;
	}
	myDisplay();
} 
     
void myMotion(int x, int y)
{	
	y = WINDOW_HEI - y;


	myDisplay();
}

void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 'l')			
	{
		move3 += 5;
		xa-=(200*((2*3.14159)/360));
		xc-=(200*((2*3.14159)/360));
	}

	if (key == 'r')			
	{
		move3 -= 5;
		xa+=(200*((2*3.14159)/360));
		xc+=(200*((2*3.14159)/360));
	}
	if (key=='<')
	{
		cart.MoveLeft();
	}
	if (key=='>')
	{
		cart.MoveRight();
	}

	//TODO 

	myDisplay();
}

/*
 * Request double buffer display mode.
 * Register mouse input callback functions
 */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);                            // initialize the toolkit
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);     // set display mode
	//glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);     // set display mode
	glutInitWindowSize (WINDOW_WID, WINDOW_HEI);                    // set screen window size
	glutInitWindowPosition (100, 100);       // set window position on screen
	glutCreateWindow (argv[0]);              // open the screen window
	myInit ();
	glutDisplayFunc(myDisplay);              // register redraw function
	glutReshapeFunc(myReshape);              // register reshape function
	glutMouseFunc(myMouse);	//GLUT provides a way for you to register the function that will be responsable for processing events generated by mouse clicks.
	glutMotionFunc(myMotion); //There are two types of motion that GLUT handles: active and passive motion. Active motion occurs when the mouse is moved and a button is pressed. 
	glutKeyboardFunc(myKeyboard);
	//glutPassiveMotionFunc(myPassiveMotion); //Passive motion is when the mouse is moving but no buttons are pressed. If an application is tracking motion, an event will be generated per frame during the period that the mouse is moving.
	//glutEntryFunc(processMouseEntry); //GLUT is also able to detect when the mouse leaves or enters the window region. A callback function can be registered to handle these two events.

	glutMainLoop();                          // go into a perpetual loop
	return 0;
}


