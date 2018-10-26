/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef MAZE_H
#define MAZE_H

#include "CommonThings.h"
#include "string.h"

#ifdef __APPLE__
#include<OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include<GL/gl.h>
#endif

#include"SOIL.h"
#include<iostream>
#include "Timer.h"
#include "math.h"       /* ceil */

using namespace std;



class Maze
{
    public:
        Maze();                           // Constructor
        Maze(int);                        // Overload Constructor
        virtual ~Maze();                  // Deconstructor

      void loadBackgroundImage(char *);   // set an image for the background
      void loadMenuImage(char *);         // set an image for the menu
      void loadGameOver(char* FileName);

      void loadSetOfArrowsImage(char *);  // set an image for the Arrow Bundle
      void loadChestImage(char *);        // set an image for the Arrow
      void loadVictory(char*);

      void drawBackground();              // Draw background image
      void drawMenu();
      void drawGrid();                    // Draw Grid
      void drawArrows();                  // Draw set of Arrows
      void drawChest();                   // Draw Chest
      void drawGameOver();
      void drawVictory();

      int getGridSize();                  // return grid size

      void placeChest(int,int);           // Place Chest
      GridLoc GetChestLoc();              // Return grid location of the chest

      void placeStArrws(int,int);         // Place set of arrows
      GridLoc GetStArrwsLoc();            // Return grid location of the arrows

      float spin;                         // for spin chest
      bool liveChest;                     // status of the chest (collected or not)
      bool liveSetOfArrws;                // status of the arrows(collected or not)


    protected:

      int gridSize;                       // grid size
      float unitWidth;                    // Unit width of the grid

    private:

      GLuint victoryTex;
      GLuint gameOverTex;
      GLuint bakTex;                       // Handler for background texture
      GLuint menuTex;
      GLuint ArrBnchTex;                   // Handler for set of arrows texture
      GLuint chestTex;                     // Handler for Chest texture
      loc chestLoc;                        // chest Location
      loc setOfArrsLoc;                    // arrows Location
      loc converter(int, int);             // convert grid size to viewport
};

#endif // MAZE_H
