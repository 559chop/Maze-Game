/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef WALL_H
#define WALL_H

#include<CommonThings.h>
#include<string.h>
#include<gl/gl.h>
#include<GL/glut.h>
#include<SOIL.h>
#include<iostream>
#include <Timer.h>
#include <math.h>       /* ceil */


class wall
{
    public:
        wall();                            // constructor
        virtual ~wall();                   // De constructor
        wall(int);                         // overload constructor

            void drawWall();               // Draw the Maze wall
            void wallInit(int, char *);    // initialize the wall
            void placeWall(int, int);      // place the wall
            void wallTrue();

            float unitWidth;               // unit width of the grid cell
            int gridSize;                  // grid size
            GridLoc getWallLocation();
            GridLoc getWallLoc;          // Grid Location of the wall
            loc converter(int x, int y);



            bool liveWall;                 // wall status (broken or not)
    protected:

    private:
              loc wallBrk;                 // viewport location of the wall
              GLuint wallTex;              // wall texture handler

};

#endif // WALL_H
