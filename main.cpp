#include "string.h"
#include "CommonThings.h"
#include "maze.h"
#include <iostream>
#include "Timer.h"
#include "player.h"
#include <fstream>
#include <iomanip>
#include <string.h>
#include "Astar.h"
#include <unistd.h>
#include <vector>
#include  <list>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include "Enemies.h"

#include "wall.h"
#include "math.h"

using namespace std;

/*
 * Node structure for keeping track of the Breadth First Search Algorithm.
 */
struct node
{
    int data;

    node* left;
    node* right;
    node* up;
    node* down;
    node* prev;


}node;

struct node* newNode(int newData, struct node* curr)
{
    struct node* n = new struct node;
    n->data = newData;
    n->prev = curr;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------//

//global variables

const int sizeOfGrid = 20;                      //Global size of grid
Maze *M = new Maze(sizeOfGrid);                 // Set Maze grid size
Player *P = new Player();                       // create player

const int wallX = sizeOfGrid;                   // Constant size for walls. Same as sizeOfGrid
const int wallY = sizeOfGrid;

wall W[wallX][wallY];                           // wall with number of bricks

int grid[wallX][wallY];                         // Grid that is built by the walls.

int enemyCount = 0;                               // number of enemies
Enemies E[10];                                   // Creates enemies



Timer *T0 = new Timer();                        // animation timer


bool gameover = false;
bool game = false;
bool win = false;

bool matrix[wallX*wallX][wallY*wallY] = {false};//Matrix of V^2

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping


void display(void);                             // Main Display : this runs in a loop

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}



int gridToMatrix(int x, int y)                 //Returns the Matrix number of a specific vertices
{
    int t = (sizeOfGrid-1) - y;
    return x+(t*sizeOfGrid);
}

GridLoc matrixToGrid(int num)                 //Returns the Grid coordinates from a Matrice number
{
    GridLoc val;
    int x,y;

    x = num%sizeOfGrid;
    y = (sizeOfGrid-1)-(num/sizeOfGrid);

    val.x=x;
    val.y=y;

    return val;
}

void buildMatrix()                            //Builds a Matrix from the wall class and the grid array
{
    int i,j;

    for(int y=wallY-1; y>=0; y--)
    {
        for(int x=0; x<wallX; x++)
        {
            if(grid[x][y]==0)
            {

                if(x-1 >= 0)                //Checks left side of current position.
                {

                    if(grid[x-1][y] == 0)
                    {
                        i = gridToMatrix(x,y);
                        j = gridToMatrix(x-1,y);


                        matrix[i][j] = true;
                        matrix[j][i] = true;
                    }
                }

                if(x+1 < sizeOfGrid)        //Checks right side of current position.
                {

                    if(grid[x+1][y] == 0)
                    {

                        i = gridToMatrix(x,y);
                        j = gridToMatrix(x+1,y);

                        matrix[i][j] = true;
                        matrix[j][i] = true;

                    }
                }

                if(y-1 >= 0)                //Checks bottom side of current position.
                {

                    if(grid[x][y-1] == 0)
                    {
                        i = gridToMatrix(x,y);
                        j = gridToMatrix(x,y-1);

                        matrix[i][j] = true;
                        matrix[j][i] = true;
                    }
                }

                if(y+1 < sizeOfGrid)         //Checks top side of current position.
                {
                    if(grid[x][y+1] == 0)
                    {
                        i = gridToMatrix(x,y);
                        j = gridToMatrix(x,y+1);

                        matrix[i][j] = true;
                        matrix[j][i] = true;
                    }
                }
            }
        }
    }
}

void buildGrid()                                   //Builds a grid using the wall class.
{
    for(int i=wallY-1; i>=0; i--)
    {
        for(int k=0; k<wallX; k++)
        {
            if(W[k][i].liveWall)
            {
                grid[k][i] = 1;
            }
        }
    }
}

/*
 * The shortest path algorithm used is the Breadth First Search method.
 * This function returns the location of the next best location to move to.
 * There are two queues used.
 * One is for the traversal of the Matrixes and the other is to store those
 * values into a node to trace the movement.
 * At the end, the found destination node is traced back to the next node of the source node.
 *
 */
GridLoc shortestPath(int source, int dest)
{
    if(source==dest)
    {
        GridLoc a;

        return a;
    }

    struct node* pos = new struct node;
    pos->data = source;

    struct node* curr = pos;
    struct node* nNode;

    int v = 0;
    int sTemp = source;

    int matrixSize = pow(sizeOfGrid,2);

    bool visited[matrixSize];


    for(int y=wallY-1; y>=0; y--)
    {

        for(int x=0; x<wallX; x++)
        {
           visited[v]=false;
           v++;
        }
    }


    list<int> queue;
    list<struct node*> track;

    visited[sTemp] = true;

    queue.push_back(sTemp);
    track.push_back(curr);



    while(!queue.empty())
    {
        if(sTemp==dest)
        {
             queue.pop_front();
             track.pop_front();
        }
        else
        {
            sTemp = queue.front();
            curr = track.front();


            queue.pop_front();
            track.pop_front();

            if(matrix[sTemp][sTemp-1]==true && sTemp > 0)                      //Checks left
            {
                if(!visited[sTemp-1])
                {
                    curr->left =  newNode(sTemp-1, curr);

                    visited[sTemp-1] = true;
                    queue.push_back(sTemp-1);
                    track.push_back(curr->left);
                }
            }
            if(matrix[sTemp][sTemp+1]==true && sTemp<matrixSize-1)             //Checks right
            {
                if(!visited[sTemp+1])
                {


                    visited[sTemp+1] = true;
                    queue.push_back(sTemp+1);

                    curr->right =  newNode(sTemp+1, curr);
                    track.push_back(curr->right);
                }
            }
            if(matrix[sTemp][sTemp-sizeOfGrid]==true && sTemp-sizeOfGrid >= 0) //checks up
            {
                if(!visited[sTemp-sizeOfGrid])
                {
                    visited[sTemp-sizeOfGrid] = true;
                    queue.push_back(sTemp-sizeOfGrid);

                    curr->up =  newNode(sTemp-sizeOfGrid, curr);
                    track.push_back(curr->up);
                }
            }
            if(matrix[sTemp][sTemp+sizeOfGrid]==true && sTemp+sizeOfGrid < matrixSize) //Checks down
            {
                if(!visited[sTemp+sizeOfGrid])
                {

                    visited[sTemp+sizeOfGrid] = true;
                    queue.push_back(sTemp+sizeOfGrid);

                    curr->down =  newNode(sTemp+sizeOfGrid, curr);
                    track.push_back(curr->down);
                }
            }

        }

    }

    while(curr != NULL && curr->prev->data != source)      //makes curr equal to the next best location to move.
    {
        curr=curr->prev;
    }

  return matrixToGrid(curr->data);

}


/*
 * Opens and reads a txt file to create the Maze.
 *
 */
void populate(){

    ifstream inFile;
    inFile.open("level1.txt");

    string loc;
    int x, y;
    //int enemyCount = 0;

    while (inFile >> loc)
    {
        if(loc == "wall:")
        {
            inFile >> loc;
            x = stoi(loc);
            inFile >> loc;
            y = stoi(loc);


            W[x][y].wallInit(M->getGridSize(), "images/wall2.png"); // Load walls
            W[x][y].placeWall(x,y);
            W[x][y].wallTrue();
        }
        else if(loc == "chest:")
        {
            inFile >> loc;
            x = stoi(loc);
            inFile >> loc;
            y = stoi(loc);

            M->loadChestImage("images/chest3.png");              // load chest image
            M->placeChest(x,y);                                 // place chest in a grid
        }
        else if(loc == "arrows:")
        {
            inFile >> loc;
            x = stoi(loc);
            inFile >> loc;
            y = stoi(loc);

            M->loadSetOfArrowsImage("images/arrwset1.png");      // load set of arrows image
            M->placeStArrws(x,y);                               // place set of arrows
        }
        else if(loc == "player:")
        {
            inFile >> loc;
            x = stoi(loc);
            inFile >> loc;
            y = stoi(loc);

            P->initPlayer(M->getGridSize(),8,"images/p1.png");   // initialize player pass grid size,image and number of frames
            P->loadArrowImage("images/arr.png");                // Load arrow image
            P->placePlayer(x,y);                                // Place player
        }
        else if(loc == "Enemy:")
        {
            inFile >> loc;
            x = stoi(loc);
            inFile >> loc;
            y = stoi(loc);

            E[enemyCount].initEnm(M->getGridSize(),4,"images/e.png"); //Load enemy image
            E[enemyCount].placeEnemy(x,y);
            enemyCount++;
        }
    }

    inFile.close();

    buildGrid();
    buildMatrix();
}



void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    populate();

    M->loadBackgroundImage("images/bak4.jpg");           // Load maze background image
    M->loadMenuImage("images/menu2.png");
    M->loadGameOver("images/over.png");
}

void display(void)
{

    cout<<gameover<<" "<<game<<endl;

    if(gameover && !win)
    {
        glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
        glPushMatrix();
        M->drawGameOver();
        glPopMatrix();

    }
    else
    {
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
    glPushMatrix();
    M->drawMenu();
    glPopMatrix();
        if (game)
            {
            glPushMatrix();
            M->drawBackground();
            glPopMatrix();

            for(int i=0; i<M->getGridSize();i++)
            {
                for(int k=0;     k<M->getGridSize(); k++)
                {
                    W[i][k].drawWall();
                }

            }

            glPushMatrix();
                M->drawGrid();
            glPopMatrix();

            glPushMatrix();
                P->drawplayer();
            glPopMatrix();

            for(int i=0; i< enemyCount; i++)
            {
                E[i].drawEnemy();
            }


            glPushMatrix();
                P->drawArrow();
            glPopMatrix();

             glPushMatrix();
               M->drawChest();
            glPopMatrix();

            glPushMatrix();
                M->drawArrows();
            glPopMatrix();

            }
    }

    glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
             P->shootArrow();
        break;

        case 'n':
            gameover =false;
            game= true;
            win = false;
            break;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}


void idle(void)
{

    for(int i=0; i<enemyCount;i++) //arrow collision
    {
        if (gameover) break;
        if((E[i].getEnemyLoc().x == P->getArrowLoc().x)&&(E[i].getEnemyLoc().y == P->getArrowLoc().y))
        {
            int x = float(rand()%(M->getGridSize()));
            int y = float(rand()%(M->getGridSize()));

            /*
            while(grid[x][y]!=0)
            {
                int x = float(rand()%(M->getGridSize()));
                int y = float(rand()%(M->getGridSize()));
            }
            */

            E[i].placeEnemy(2,18);
        }
    }


    for(int i=0; i<enemyCount; i++)//reset enemy positions when game over
    {
        if (gameover || win)
        {
            /*
            for(int i=0; i<enemyCount;i++)
            {
                E[i].placeEnemy(float(rand()%(M->getGridSize())),float(rand()%(M->getGridSize())));
            }
            P->placePlayer(float(rand()%(M->getGridSize())),float(rand()%(M->getGridSize())));
            */
        }
        if(E[i].getEnemyLoc().x == P->getPlayerLoc().x && E[i].getEnemyLoc().y == P->getPlayerLoc().y) //enemy to player collision
        {
            gameover = true;
            game = false;
            win = false;
            break;
        }

    }


    if(T0->GetTicks()> 600) //enemy movements
    {
        GridLoc enemyLoc;
        GridLoc playerLoc;

        int enemyMatrix, playerMatrix;

        playerLoc = P->getPlayerLoc();
        playerMatrix = gridToMatrix(playerLoc.x, playerLoc.y);

        for(int i=0; i<enemyCount; i++)
        {

            if (gameover) break;

            enemyLoc = E[i].getEnemyLoc();
            enemyMatrix = gridToMatrix(enemyLoc.x, enemyLoc.y);



            E[i].moveEnemy(shortestPath(enemyMatrix, playerMatrix));

        }
        T0->Reset();
    }

    /*
    if((P->getPlayerLoc().x == M->GetChestLoc().x) && (P->getPlayerLoc().y == M->GetChestLoc().y))//chest collision
    {
        win = true;
        game = false;
        P->placePlayer(float(rand()%(M->getGridSize())),float(rand()%(M->getGridSize())));
    }
     if((P->getPlayerLoc().x == M->GetStArrwsLoc().x) && (P->getPlayerLoc().y == M->GetStArrwsLoc().y))
     {
         M->placeStArrws(float(rand()%(M->getGridSize())),float(rand()%(M->getGridSize())));
     }
     */

    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};

bool checkCollision(int x,int y){

    for(int i=0; i<wallX;i++)
    {
        for(int k=0; k<wallY;k++)
        {
            if((W[i][k].getWallLocation().x == x) && (W[i][k].getWallLocation().y == y))
            {
                P->placePlayer(P->getPlayerLoc().x,P->getPlayerLoc().y);
                return true;
            }
        }
    }
    return false;

}

void Specialkeys(int key, int x, int y)
{

    int nextStepX = P->getPlayerLoc().x;
    int nextStepY = P->getPlayerLoc().y;

    switch(key)
    {
    case GLUT_KEY_UP:
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            nextStepY +=1;
            if (checkCollision(nextStepX,nextStepY)==true)break;
            else
            {
                P->movePlayer("up",P->frames);
            }
    break;

    case GLUT_KEY_DOWN:
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            nextStepY -=1;
            if (checkCollision(nextStepX,nextStepY)==true)break;
            else
            {
                P->movePlayer("down",P->frames);
            }
    break;

    case GLUT_KEY_LEFT:
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            nextStepX -=1;
            if (checkCollision(nextStepX,nextStepY)==true)break;
            else
            {
                P->movePlayer("left",P->frames);
            }

    break;

    case GLUT_KEY_RIGHT:
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            nextStepX +=1;
            if (checkCollision(nextStepX,nextStepY)==true)break;
            else
            {
                P->movePlayer("right",P->frames);
            }
    break;

   }
  glutPostRedisplay();
}



/* Program entry point */

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (800, 800);                //window screen
    glutInitWindowPosition (100, 100);            //window position
    glutCreateWindow ("Maze");                    //program title

    init();
    glutDisplayFunc(display);                     //callback function for display
    glutReshapeFunc(resize);                      //callback for reshape
    glutKeyboardFunc(key);                        //callback function for keyboard
    glutSpecialFunc(Specialkeys);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);


    glutMainLoop();
    return EXIT_SUCCESS;


}
