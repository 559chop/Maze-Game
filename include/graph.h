#ifndef GRAPH_H
#define GRAPH_H

#include "wall.h"

class Graph
{
    public:
        Graph();
        virtual ~Graph();

        void buildMatrix(wall);
        void BFS();



        //int[][] grid;

    private:
        int grid[20][20];

};

#endif // GRAPH_H
