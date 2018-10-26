/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <wall.h>
#include <iostream>

using namespace std;


wall::wall()
{
    //ctor
    liveWall = false;
}

wall::~wall()
{
    //dtor
}

void wall::wallInit(int grid,char* FileName)
{
    gridSize = grid;
    unitWidth = (float)2/grid;
    wallTex = TextureLoader(FileName);
}

void wall::drawWall()
{
    if(liveWall)
    {

    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,wallTex);

    glPushMatrix();
    glTranslatef(wallBrk.x,wallBrk.y,0.0);

    glScalef(1.0/(float)gridSize,1.0/(float)gridSize,1);


     glBegin(GL_POLYGON);
            glTexCoord2f(0,0);
            glVertex3f(1,1,0.0f);

            glTexCoord2f(1,0);
            glVertex3f(-1,1,0.0f);

            glTexCoord2f(1,1);
            glVertex3f(-1,-1,0.0f);

            glTexCoord2f(0,1);
            glVertex3f(1,-1,0.0f);
    glEnd();
    glPopMatrix();
    }
}

void wall::placeWall(int x, int y)
{
    getWallLoc.x= x;
    getWallLoc.y= y;

    x+=1;
    y+=1;

    //wallBrk.x =  converter(x,y).x;
    //wallBrk.y =  converter(x,y).y;

    wallBrk.x =  (unitWidth)*x-1-unitWidth/2;
    wallBrk.y =  (unitWidth)*y-1-unitWidth/2;
}

void wall::wallTrue()
{
    liveWall = true;
}

GridLoc wall::getWallLocation()
{
    GridLoc val;

    val.x = (int)(ceil((wallBrk.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((wallBrk.y +(1-unitWidth))/unitWidth));

    return val;

}

loc wall::converter(int x, int y)
{
    loc val;
    x+=1;
    y+=1;

    val.x = -1-unitWidth/2+(unitWidth)*x;
    val.y = -1-unitWidth/2+(unitWidth)*y;
    return val;
}
