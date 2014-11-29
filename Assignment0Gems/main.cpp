//
//  main.cpp
//  Assignment0Gems
//
//  Created by Emma Meersman on 9/16/14.
//  Copyright (c) 2014 Emma Meersman. All rights reserved.
//
//  Creates a grid containing gems, when groups of 4 or more adjacent gems are selected, they
//  disappear. Pressing the 'r' key resets the board with a new layout of gems.

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <list>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

typedef struct {
    int x, y;
    int shapeType;
} Cell;

int rows = 10;
int cols = 10;
Cell grid[10][10];
Cell gridCopy[10][10];

void fillGrid() {
    int shape = 0;
    for (int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            shape = rand() % 4;
            Cell curr;
            curr.x = i;
            curr.y = j;
            curr.shapeType = shape;
            grid[i][j] = curr;
        }
    }
}

// The following 4 functions draw each of the different shapes available in the grid

void drawTriangle(double x, double y) {
    glBegin(GL_TRIANGLE_STRIP);
    glColor3d(1.0, 0.0, 1.0);
    glVertex2d(x-0.06, y-0.06);
    glColor3d(0.1,0,0.1);
    glVertex2d(x+0.06, y-0.06);
    glColor3d(0.7,0,1);
    glVertex2d(x, y+0.06);
    glEnd();
}

void drawSquare(double x, double y) {
    glBegin(GL_POLYGON);
    glColor3d(0, 0.8, 1);
    glVertex2d(x-0.06, y-0.06);
    glColor3d(0, 0.2, 0.2);
    glVertex2d(x+0.06, y-0.06);
    glColor3d(0, 0.5, 1);
    glVertex2d(x+0.06, y+0.06);
    glColor3d(0, 0.9, 1);
    glVertex2d(x-0.06, y+0.06);
    glEnd();
}

void drawPentagon(double x, double y) {
    glBegin(GL_POLYGON);
    glColor3d(0, 0.6, 0);
    glVertex2d(x-0.03, y-0.06);
    glColor3d(0, 0.2, 0);
    glVertex2d(x+0.03, y-0.06);
    glColor3d(0, 0.6, 0);
    glVertex2d(x+0.06, y);
    glColor3d(0, 1, 0);
    glVertex2d(x, y+0.06);
    glColor3d(0, 1, 0);
    glVertex2d(x-0.06, y);
    glEnd();
}

void drawHexagon(double x, double y) {
    
    glBegin(GL_POLYGON);
    glColor3d(0.2, 0.2, 1);
    glVertex2d(x-0.06, y);
    glColor3d(0,0,0.5);
    glVertex2d(x-0.02, y-0.06);
    glColor3d(0, 0, 0.2);
    glVertex2d(x+0.02, y-0.06);
    glColor3d(0, 0, 0.5);
    glVertex2d(x+0.06, y);
    glColor3d(0, 0.3, 1);
    glVertex2d(x+0.02, y+0.06);
    glColor3d(0.2, 0.2, 1);
    glVertex2d(x-0.02, y+0.06);
    glEnd();
}

// Assigns shapes to the cells and displays them on the grid
void drawGrid() {
    double x = -1.0;
    double y;
    for(int i = 0; i < rows; ++i) {
        y = -1.0;
        for(int j = 0; j < cols; ++j) {
            if(grid[i][j].shapeType == 0) {
                drawTriangle(x + 0.1, y + 0.1);
            } else if(grid[i][j].shapeType == 1) {
                drawSquare(x + 0.1, y + 0.1);
            } else if(grid[i][j].shapeType == 2) {
                drawPentagon(x + 0.1, y + 0.1);
            } else if(grid[i][j].shapeType == 3) {
                drawHexagon(x + 0.1, y + 0.1);
            } else {
                //drawBlackSquare(x+0.1, y+0.1);
            }
            y += 0.2;
        }
        x += 0.2;
    }
}

// Shifts cells down by 1 grid unit if the cell under them was deleted
void shiftCells() {
    for(int it=0; it<10; it++) {
        for(int i = 0; i < rows; ++i) {
            for(int j = cols-2; j >= 0; --j) {
                if(grid[i][j].shapeType == 4) {
                    grid[i][j].shapeType = grid[i][j+1].shapeType;
                    grid[i][j+1].shapeType = 4;
                }
            }
        }
    }
    drawGrid();
    glutPostRedisplay();
}

std::list<Cell> visited;

// Perform BFS to find all adjacent shapes to clicked shape in the grid
void findCells(int ix, int iy) {
    // Copy initial array in case the clicked cell is not adjacent to enough of the same shapes
    for (int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            gridCopy[i][j] = grid[i][j];
        }
    }
    Cell clicked = grid[ix][iy];
    if(clicked.shapeType != 4) {
        visited.push_back(clicked);
        // Identify the shape that was clicked
        int currShapeType = clicked.shapeType;
        clicked.shapeType = 4;
        grid[ix][iy].shapeType = 4;
        int adjacent = 1;
        Cell curr;
        while (!visited.empty()) {
            curr = visited.front();
            visited.pop_front();
            int x = curr.x;
            int y = curr.y;
            // Searches all adjacent shapes and adds them to visited list if they are the same as the
            // clicked shape
            if(x-1 >= 0 && grid[x-1][y].shapeType == currShapeType) {
                visited.push_back(grid[x-1][y]);
                grid[x-1][y].shapeType = 4;
                ++adjacent;
            }
            if(x+1 < cols && grid[x+1][y].shapeType == currShapeType) {
                visited.push_back(grid[x+1][y]);
                grid[x+1][y].shapeType = 4;
                ++adjacent;
            }
            if(y-1 >= 0 && grid[x][y-1].shapeType == currShapeType) {
                visited.push_back(grid[x][y-1]);
                grid[x][y-1].shapeType = 4;
                ++adjacent;
            }
            if(y+1 < rows && grid[x][y+1].shapeType == currShapeType) {
                visited.push_back(grid[x][y+1]);
                grid[x][y+1].shapeType = 4;
                ++adjacent;
            }
        }
        // Checks for 4 or more identical adjacent shapes
        if(adjacent > 3) {
            shiftCells();
        } else {
            for (int i = 0; i < rows; ++i) {
                for(int j = 0; j < cols; ++j) {
                    grid[i][j] = gridCopy[i][j];
                }
            }

        }
    }
    glutPostRedisplay();
}

//--------------------------------------------------------
// Display callback that is responsible for updating the screen
//--------------------------------------------------------
void onDisplay( ) {
    //fillGrid();
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);        	// clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen
    
    // Draw Grid
    glColor3d(1,1,1);
    
    glBegin(GL_LINES);
    
    int gridSize = 10;
    double lines = 2.0/gridSize;
    for(double i = -1.0; i < 1.0; i+=lines) {
        glVertex2d(-1.0, i);
        glVertex2d(1.0, i);
        glVertex2d(i, -1.0);
        glVertex2d(i, 1.0);
    }
    
    glColor3d(0,0,0);
    glVertex2d(-1.0, -1.0);
    glVertex2d(1.0, -1.0);

    glEnd();
    
    drawGrid();
    
	glutSwapBuffers();                     		// Swap buffers for double buffering
	
}

void onReshape(int winWidth0, int winHeight0) {
    glViewport(0, 0, winWidth0, winHeight0);
}

// Press 'r' to restart with different configuration.
void onKeyboard(unsigned char key, int x, int y) {
    if(key == 'r') {
        fillGrid();
    }
    glutPostRedisplay();
}

void onClick(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Send the x and y to be handled
        int xi = 10*x/glutGet(GLUT_WINDOW_WIDTH);
        int yi = 9-10*y/glutGet(GLUT_WINDOW_HEIGHT);
        findCells(xi, yi);
    }
}

//--------------------------------------------------------
// The entry point of the application
//--------------------------------------------------------
int main(int argc, char *argv[]) {
    srand(time(0));
    glutInit(&argc, argv);                 		// GLUT initialization
    glutInitWindowSize(640, 480);				// Initial resolution of the MsWindows Window is 600x600 pixels
    glutInitWindowPosition(100, 100);            // Initial location of the MsWindows window
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);    // Image = 8 bit R,G,B + double buffer + depth buffer
    
    glutCreateWindow("Gems");        	// Window is born
    fillGrid();
    glutDisplayFunc(onDisplay);                	// Register event handlers
    glutReshapeFunc(onReshape);                 // Handle window resizing
    glutMouseFunc(onClick);                     // Detect clicks on grid
    glutKeyboardFunc(onKeyboard);
    
    glutMainLoop();                    			// Event loop
    return 0;
}

