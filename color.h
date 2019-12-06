/*
    Author: Matthew Sun, Nilei Pan, Tianyi Zhang, Runze Li, Maiqi Hou
    Assignment Title: Mario Brazazu
    Assignment Description: creating a playable Mario like game
    Due Date: 12/3/2018
    Date Created: 11/20/2018
    Date Last Modified: 12/3/2018

*/
/*
    Data Abstraction:
                Some variables and functions are set to be used
    Input:
                The user makes the input
    Process:
                The program responds to the input by the user and
                moves the Mario
    Output:
                The Mario and the game board are drawn to the screen
    Assumptions:
                Assumes that user only controls one Mario object
                Assumes that there are only two turtles needed
*/
#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

struct color{
    int R,G,B;

    color(){
        R = G = B = 0;
    }

    color(int r, int g, int b){
        R = r;
        G = g;
        B = b;
    }


    void setColor(color c){
        R = c.R;
        G = c.G;
        B = c.B;
    }

};

const color BLACK(0,0,0);
const color WHIT(255,255,255);
const color RED(255,0,0);
const color GREEN(0,255,0);
const color BLUE(0,0,255);
const color DARK_GREEN(0,108,0);
const color FLOWER_BLUE(100,149,237);
const color FIREBRICK(178,34,34);
const color TUBEGREEN(154,205,50);
const color DARKORANGE(255,140,0);








#endif // COLOR_H_INCLUDED
