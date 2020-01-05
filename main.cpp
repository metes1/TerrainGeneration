#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include<iostream>

using namespace std;

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "Terrain.h"
#include "mathLib3D.h"

//global variables

int alg = 1;
int maxx; int width; int height; 

int shade = 0; 
int tX, tZ;
bool quadMesh = true;
bool lights = false;
bool light0 = true;
int wire = 1;
bool flatShade = true;
int quadV[4][2] = {{0,0}, {0,1}, {1,1}, {1,0}};
int triV[6][2] = {{0,0}, {0,1}, {1,0}, {1,0}, {0,1}, {1,1}};
float eye[] = {25, 30, 25};
int text = 0;  

//lighting variables
float light0_pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float light1_pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float amb[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float spec[4] = {1.0f, 1.0f, 1.0f, 1.0f};

//material variables
float m_amb[4] = {0.0f, 0.1f, 0.0f, 1.0f};
float m_diff[4] = {0.0f, 0.50980392f, 0.0f, 1.0f};
float m_spec[4] = {0.50196078f, 0.50196078f, 0.50196078f, 1.0f};
float shiny = 32.0f;

Terrain * terrain = new Terrain(tX, tZ, 1);

//draw terrain
void drawTerrain()
{   
    for (int z = 0; z < tZ-1; z++){
        for (int x = 0; x < tX-1; x++){
            if (quadMesh) {
                //draw terrain using quads
                glBegin(GL_QUADS);
                for (int i = 0; i <= 3; i++) {
                    int xV = x+quadV[i][0];
                    int zV = z+quadV[i][1];
                    float yV = terrain->heightMap[xV][zV];
                    float color = (yV-(terrain->minHeight))/(terrain->maxHeight);
                    float nX = terrain->normals[xV][zV].dx;
                    float nY = terrain->normals[xV][zV].dy;
                    float nZ = terrain->normals[xV][zV].dz;
                    glNormal3f(nX, nY, nZ);
                    
                    if (wire == 1 || wire == 3)
                        glColor3f(color, color, color);
                    else
                        glColor3f(0, 1, 0);
                    
                    if(i == 0 ) {
                        glTexCoord2f(0, 0); }
                    else if(i == 1) {
                        glTexCoord2f(1, 0);
                    }
                    else if(i == 2) {
                        glTexCoord2f(0, 1);
                    }
                    else if (i == 3) {
                        glTexCoord2f(1, 1);
                    } 

                    glVertex3f(xV, yV, zV);
                } 
                glEnd();
            }
            else {
                //draw terrain using triangles
                glBegin(GL_TRIANGLES);
                for (int i = 0; i <= 5; i++) {
                    int xV = x+triV[i][0];
                    int zV = z+triV[i][1];
                    float yV = terrain->heightMap[xV][zV];
                    float color = (yV-(terrain->minHeight))/(terrain->maxHeight);
                    float nX = terrain->normals[xV][zV].dx;
                    float nY = terrain->normals[xV][zV].dy;
                    float nZ = terrain->normals[xV][zV].dz;
                    glNormal3f(nX, nY, nZ);

                    if (wire == 1 || wire == 3)
                        glColor3f(color, color, color);
                    else
                        glColor3f(0, 1, 0);

                    if(i == 0 ) {
                        glTexCoord2f(0, 0); }
                    else if(i == 1) {
                        glTexCoord2f(1, 0);
                    }
                    else if(i == 2) {
                        glTexCoord2f(0, 1);
                    }
                    else if (i == 3) {
                        glTexCoord2f(1, 1);
                    } 

                    glVertex3f(xV, yV, zV);
                } 
                glEnd();
            }

        }
    }
}    

//loads ppm files
GLubyte* LoadPPM(char* file, int* width, int* height, int* maxx)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        exit(0);
    }
    fscanf(fd, "%c",&c);
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    nm = n*m;
    
    img = (GLubyte*)(malloc(3*sizeof(GLuint)*nm));
    
    s=255.0/k;
    
    
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    *width = n;
    *height = m;
    *maxx = k;
    
    return img;
}

//terrain textures
void init(void) {
    if (text == 1) {
        glEnable(GL_TEXTURE_2D);
        GLubyte* img_data = LoadPPM("marble.ppm", &width, &height, &maxx);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
    }
    else if (text == 2) {
        glEnable(GL_TEXTURE_2D);
        GLubyte* img_data = LoadPPM("carpet.ppm", &width, &height, &maxx);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else if (text == 3) {
        glEnable(GL_TEXTURE_2D);
        GLubyte* img_data = LoadPPM("star.ppm", &width, &height, &maxx);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else if (text == 0) {
        glDisable(GL_TEXTURE_2D); 
    }
}

//display function - GLUT display callback function
void display(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 700);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if (shade == 1) {
        glShadeModel(GL_FLAT);
    }
    else if (shade == 2) {
        glShadeModel(GL_SMOOTH);
    }
    glLoadIdentity();

    //sets up camera
    gluLookAt(eye[0],eye[1],eye[2],0,0,0,0,1,0);

    //sets up lights
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

    //sets up material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

    if (wire == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawTerrain();
    }
    else if (wire == 2) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawTerrain();
    }
    else if (wire == 3) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawTerrain();
        wire = 4;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawTerrain();
        wire = 3;
    }

    //switch our buffers for a smooth animation
    glutSwapBuffers();
    glutPostRedisplay();
}

void display2D() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
        for (int z = 0; z < tZ; z++) {
            for (int x = 0; x < tX; x++) {
                glPointSize(1.0); //size of point
                float y = terrain->heightMap[x][z];
                float color = (y-(terrain->minHeight))/(terrain->maxHeight);
                glColor3f(color, color, color); //color of point
                glBegin(GL_POINTS); //draws point on screen
                    glVertex3f(x, z, 0.0f);
                glEnd();
            }
        }
    //switch our buffers for a smooth animation
    glutSwapBuffers();
    glutPostRedisplay();
}

void view2D()
{
    glutInitWindowPosition(700,50);
    glutInitWindowSize(tX, tZ);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("2D Terrain Overview");    //creates the window
    glutDisplayFunc(display2D);

    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0,tX,tZ,0);
}


//actions invoked by keyboard
void keyboard(unsigned char key, int xIn, int yIn)
{
    switch (key)
    {
        //toggles wireframe mode
        case 'w':
        case 'W':
            if (wire == 1)
                wire += 1;
            else if (wire == 2)
                wire += 1;
            else if (wire == 3)
                wire = 1;
            break;

        //toggles lighting
        case 'l':
        case 'L':
        {
            lights = !lights;
            if (lights == true)
                glEnable(GL_LIGHTING);
            else if (lights == false)
                glDisable(GL_LIGHTING);
            break;
        }
            break;

        //toggles polygon mesh
        case 's':
        case 'S':
            quadMesh = !quadMesh;
            break;

        case 'o': 
        case 'O': 
            if (shade == 0) {
                shade += 1;
            }
            else if (shade == 1) {
                shade += 1;
            }
            else if (shade == 2) {
                shade = 0; 
            }
            //flat shading 
            // gourad shading 
            break;

        //generates terrain using a different algorithm
        case 'x':
        case 'X': 
            if (alg == 1) {
                terrain = new Terrain(tX, tZ, 0);
                alg = 0; }
            else {
                terrain = new Terrain(tX, tZ, 1);
                alg = 1; 
            }
            break;

        //changes terrain texture
        case 't':
        case 'T':
            //cout << text;
            if (text == 0)
                text += 1;
            else if (text == 1) {
                text += 1;}
            else if (text == 2)
                text += 1;
            else if (text == 3)
                text = 0;
            init();
            break;

        //displays 2D terrain overview
        case 'p':
        case 'P':
            view2D();
            break;

        //resets terrain
        case 'r':
        case 'R':
            terrain = new Terrain(tX, tZ, 1);
            break;

        //light controls
        case 49:
            light0 = true;
            break;
        case 50:
            light0 = false;
            break;
        case 'c':
        case 'C':
            if (light0) {
                light0_pos[0] -= 5;
                light0_pos[2] -= 5;
            }
            else {
                light1_pos[0] -= 5;
                light1_pos[2] -= 5;
            }
            break;
        case 'b':
        case 'B':
            if (light0) {
                light0_pos[0] += 5;
                light0_pos[2] += 5;
            }
            else {
                light1_pos[0] += 5;
                light1_pos[2] += 5;
            }
            break;
        case 'f':
        case 'F':
            if (light0) {
                light0_pos[1] += 5;
            }
            else {
                light1_pos[1] += 5;
            }
            break;
        case 'v':
        case 'V':
            if (light0) {
                light0_pos[1] -= 5;
            }
            else {
                light1_pos[1] -= 5;
            }
            break;

        //quits program
        case 27: //ESC key
        case 'q':
        case 'Q':
            exit(0);
            break;
    }
}

void specialKey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            if (eye[1] <= 200+tZ)
                eye[1] += 5;
            break;

        case GLUT_KEY_DOWN:
            if (eye[1] >= -5)
                eye[1] -= 5;
            break;

        case GLUT_KEY_RIGHT:
            if (eye[0] <= 200+tX)
                eye[0] += 5;
            break;

        case GLUT_KEY_LEFT:
            if (eye[0] >= -200-tX)
                eye[0] -= 5;
            break;
    }
}

void projection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

//prints instructions to console
void instruct()
{
    cout << "---------------------------------------------------------\n";
    cout << "                 Keyboard Controls         \n";
    cout << "---------------------------------------------------------\n\n";
    cout << " Left-Arrow   => Rotate scene left on y-axis\n";
    cout << " Right-Arrow  => Rotate scene right on y-axis\n";
    cout << " Up-Arrow Key => Rotate scene on x-axis\n";
    cout << " Down-Arrow   => Rotate scene on x-axis\n";
    cout << " W            => Toggle wireframe mode\n";
    cout << " L            => Toggle lighting\n";
    cout << " O            => Toggle between flat and Gouraud shading\n";
    cout << " S            => Toggle polygon mesh (quads/triangles)\n";
    cout << " T            => Change terrain texture\n";
    cout << " P            => Display 2D terrain overview\n";
    cout << " X            => Switch terrain generation algorithm\n";
    cout << " 1            => Switch to Light 1 Control\n";
    cout << " 2            => Switch to Light 2 Control\n";
    cout << " C/B          => Move light left/right\n";
    cout << " F/V          => Move light up/down\n";
    cout << " R            => Reset: generate new terrain\n";
    cout << " q/ESC        => Quit program\n";
    cout << "--------------------------------------------------------\n";
}

//gets dimensions from user()
void getDimensions()
{
    int tempX, tempZ;
    bool correct = false;
    while (correct == false) {
        cout << "Enter a terrain dimensions between 50 and 300 ([number] [number]):";
        cin >> tempX >> tempZ;
        if (tempX < 50 || tempX > 300 || tempZ < 50 || tempZ > 300) {
            cout << "Dimensions out of bounds.\n";
        }
        else {
            tX = tempX;
            tZ = tempZ;
            correct = true;
        }
    }
}

/* main function - program entry point */
int main(int argc, char** argv)
{
    srand(time(0)); //number randomizer

    instruct(); //prints instructions
    getDimensions();//gets dimensions from user
    terrain = new Terrain(tX, tZ, alg); //create terrain with given dimensions
    eye[0] += tX;
    eye[2] += tZ;

	glutInit(&argc, argv);		//starts up GLUT
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 50);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Terrain");	//creates the window

    //enable lights
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    light0_pos[0] = 0.0;
    light0_pos[2] = tZ;
    light1_pos[0] = tX;
    light1_pos[2] = 0.0;

    glEnable(GL_DEPTH_TEST);

    glClearColor(0,0,0,0);
    projection();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	glutDisplayFunc(display);   //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);

	glutMainLoop();				//starts the event glutMainLoop
	return(0);
}
