// breakout.c
// Computer Science 50
// Problem Set 4

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400
#define XMID 200
#define YMID 300

/* bricks */

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

/* ball */

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

/* paddle */

// paddle
#define PADWIDTH 65
#define PADHEIGHT 10
#define PADLOC 500

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));
    
    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // initial velocity of x and y
    
    double xvelocity = (drand48()*4-2);
    
    double yvelocity = drand48();
        
        if (yvelocity > .5)
        {
            yvelocity = -2.0;
        } 
        else if (yvelocity <= .5)
        {
            yvelocity = 2.0;
        }
    
    // keep playing until game over
    waitForClick();
    
    while (lives > 0 && bricks > 0)
    {
        // TODO **DONE**
        
        // listen for mouse events

        GEvent event = getNextEvent(MOUSE_EVENT);

        if (event != NULL)
        {
            // listen for mouse movement
            if (getEventType(event) == MOUSE_MOVED)
            {        
                // stop paddle from moving beyond left side of window
                if((getX(event)) - (PADWIDTH/2) <= 0)
                {
                    double xl = 0;
                    double yl = PADLOC;
                    setLocation(paddle, xl, yl);
                } 
                // stop paddle from moving beyond right side of window
                else if((getX(event)) + (PADWIDTH/2) >= WIDTH) 
                {
                    double xr = WIDTH - PADWIDTH;
                    double yr = PADLOC;
                    setLocation(paddle, xr, yr);
                }
                // follow mouse movement set paddle to x and y or mouse
                else
                {
                    double x = getX(event) - (getWidth(paddle) / 2);
                    double y = PADLOC;
                    setLocation(paddle, x, y);
                }
            }     
        }       
            
        // move circle (ball) along x-axis
        move(ball, xvelocity, yvelocity);
       
        // bounce circle (ball) off of the right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            xvelocity = -xvelocity;
        }

        // bounce circle (ball) off of the left edge of window
        else if (getX(ball) <= 0)
        {
            xvelocity = -xvelocity;
        }
        
        // bounce circle (ball) off of the top edge of window
        else if (getY(ball) <= 0)
        {
            yvelocity = -yvelocity;
        }
        
        // if circle (ball) touches bottom edge of window, end game, lose life, reset on click
        else if (getY(ball) >= HEIGHT)
        {
            yvelocity = -yvelocity;
            
            lives--;
            removeGWindow(window, ball);
            waitForClick();
            ball = initBall(window);
        
        }

        //Manipulate speed before moving again
        pause(8);
        
        //Detect collisions 
        
        GObject object = detectCollision(window, ball);
            
            // bounce off paddle
            
            if (object == paddle && object != label) 
            {
                yvelocity = -yvelocity;
            }
            
            // bounce off brick
            
            if (object != NULL && object != paddle && object != label ) 
            {
                yvelocity = -yvelocity;
                removeGWindow(window, object);
                points = points + 1;
                bricks = bricks - 1;
                updateScoreboard(window,label,points);
            }         
   
   } //End of the while loop

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO **DONE**
     for (int nr = 0; nr < ROWS; nr++)
     {
        
        int c;
        
        for (int c = 0; c < COLS; c++)   
        {
                int gap = 2;
                int brickWidth = (WIDTH/COLS)-gap;
                int brickHeight = 20;
                
                int gcounter = 1; //gap counter
                int bcounter = 0; //brick counter
                
                int brickx = ((brickWidth+gap)*c);
                int bricky = gap +((brickHeight+gap)*nr);
                
                GRect brick = newGRect(brickx,bricky,brickWidth,brickHeight); //newGrect(x,y,w,h)
                
                if (nr % 2 == 0)
                {
                setColor(brick, "#66CCFF");
                }
                else if (nr % 2 > 0)
                {
                setColor(brick, "#66FF99");
                }
                
                
                setFilled(brick, true);
                
                add(window,brick);         
                
                gcounter++;
                bcounter++;
         }   
        
        c = 0;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO **DONE**
    
    GOval circle = newGOval(0,0,15,15);
    setColor(circle, "#000000");
    setFilled(circle, true);
    setLocation(circle, WIDTH/2,HEIGHT/2);
    
    add(window,circle);
    
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO **DONE**
    
    int setXCenter = XMID - (PADWIDTH/2);
    
    GRect paddle = newGRect(setXCenter,PADLOC,PADWIDTH,PADHEIGHT);
    setColor(paddle, "#000000");
    setFilled(paddle, true);
    
    add(window,paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO **DONE**
    
    GLabel label = newGLabel("0");
    
    setFont(label, "SansSerif-25");
    setColor(label, "#000000");
    
    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    
    add(window, label);
    
    return label;
    
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
