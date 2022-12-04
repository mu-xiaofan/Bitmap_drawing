#include "config.h"
#include <FL/Fl_Cairo_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <fstream>
#include <FL/Fl_Color_Chooser.H>
const int WIDTH = 400;
const int HEIGHT = 400;
int scale = 2;
const int COLUMNS = 32;  //depend on actual size of image
const int ROWS = 32;
int arrow[ROWS][COLUMNS];//integer

//fuchsia rgb(100%,0%,100%)
double r=1;double g=0; double b=1;

//@   \newpage @
//flip the image vertically
void flipv(int a[ROWS][COLUMNS])
{
	for(int row = 0; row < ROWS/2; row++)
	{
		for(int column = 0; column < COLUMNS; column++)
		{
			std::swap(a[row][column],
			a[ROWS - 1 - row][column]);
			//change the pixel in the coordination from:
			//(x,y)->(x,-y)

		}
	}
}

//@   \newpage @
//flip the color of the image
void flipc(int a[ROWS][COLUMNS])
{
	for(int row = 0; row < ROWS; row++)
	{
		for(int column = 0; column < COLUMNS; column++)
		{
			a[row][column] = abs(a[row][column] - 1);
			//chang the data stored in the array from:
			//0->1
			//1->0
		}
	}
}

//@   \newpage @
//flip the image horizontally
void fliph(int a[ROWS][COLUMNS])
{
	for(int column = 0; column < COLUMNS/2; column++)
	{
		for(int row = 0; row < ROWS; row++)
		{
			std::swap(a[row][column],
			a[row][COLUMNS - 1 - column]);
			//change the pixel in the coordination from:
			//(x,y)->(-x,y)
			//condition ensures the array won't
			//be swap twice
			
		}
	}
}

//@   \newpage @
//flip the image diagonally(in this way-> / )
void flipd1(int a[ROWS][COLUMNS])
{
	for(int row = 0; row < ROWS; row++)
	{
		for(int column = 0; column < (COLUMNS-row); column++)
		{
			std::swap(a[row][column],
			a[COLUMNS-1-column][ROWS-1-row]);
			//change the pixel in the coordination from:
			//(x,y)->(-x,-y)
			//condition ensures loop stop swapping when 
			//reach the diagonal
		}
	}
}

//@   \newpage @
//flip the image diagonally(in this way-> \ )
void flipd2(int a[ROWS][COLUMNS])
{
	for(int row = 0; row < ROWS; row++)
	{
		for(int column = 0; column < row; column++)
		{
			std::swap(a[row][column],a[column][row]);
			//(x,y)->(y,x)
			//condition ensures loop stop swapping when 
			//reach the diagonal
		}
	}
}

//@   \newpage @
//Choose color, make the window, add text...
void drawCB(Fl_Cairo_Window* cw,cairo_t* cr)
{
	cairo_set_source_rgb(cr,1,1,1); //white
	cairo_paint(cr);
	
	cairo_set_source_rgb(cr,1,0,0); //red
	cairo_rectangle(cr,HEIGHT-scale,
	WIDTH-scale,scale,scale);  //x,y,w,h
 	cairo_stroke(cr); // to outline the button


	cairo_set_source_rgb (cr, 1, 0, 0);//  red  rgb
	cairo_select_font_face (cr, "Georgia",
		CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 12);
	cairo_move_to (cr, cw->x() , cw->h()/6 );
	cairo_show_text (cr, "Click Anywhere");
	// put label on window
	
	cairo_translate(cr,cw->w()/2-COLUMNS,cw->h()/2-ROWS);
	//decide the position of the text
	
	for(int row = 0; row < ROWS; row++)
		for(int column = 0; column < COLUMNS; column++)
		{
			if(arrow[row][column] == 1)
			{
				cairo_set_source_rgb(cr,r,g,b); 
				//fuchsia rgb(100%,0%,100%)   
			}
			else
			{
				cairo_set_source_rgb(cr,0,0,0); 
				//black
			}
							//x,y,w,h
			cairo_rectangle(cr,column*scale,
			row*scale,1*scale,1*scale);  
			cairo_fill(cr);
		}	
}

//@   \newpage @
//create buttons to let user choose options 
//in order to modify the image
void goCB(void*,void*)
{
	switch ( fl_choice("Choose option:",
	"menu","by 2",
	"resize by 3"))
	{
	case 0: 
		  switch(fl_choice("Choose option:",
		  "color","flip",nullptr))
		  //both nullptr and 0 work
		  //no enough space in pdf
				{
				case 0: 
					switch(fl_choice("color?","inverse",
					"choose",nullptr))
					//both nullptr and 0 work
					{
						case 0:
							flipc(arrow);
							//flip the color
							break;
						case 1:
							fl_color_chooser(
							"choose a color",
							r,g,b);
							//choose color
							break;
						}
					break;
				case 1: 
					switch(fl_choice("How?",
					"horizontal","vertical",
					"diagonal"))
					//no enough space in pdf
						{
							case 0:
								fliph(arrow);
						//flip horizontally
								break;
							case 1:
								flipv(arrow);
						//flip vertically
								break;
							case 2:
								switch
								(fl_choice
								("Which?",
								"/","\\",
								nullptr))
				/*
				no space in the pdf...
				I have no choice...
				In order to let them visible,
				I deleted some spaces in my code
				*/
				{
					case 0:
							flipd1(arrow);
								/*flip the 
								image 
								diagonally(/)
								*/
							break;
					case 1:
							flipd2(arrow);
								/*flip the 
								image 
								diagonally(\)
								*/
							break;
									}
								break;
							}
					break;
				}
				break;
		case 1: scale = 4; break;//resize by 2
		case 2: scale = 6; break;//resize by 3
	}
}

//@   \newpage @
//translate the image(stored in a string) into 
//an array(which can be showed as a bit map)
void load_image(std::string f)
{
	std::ifstream ifs(f);//get the string
	if(not ifs) std::cerr << "File not found" <<std::endl;
	// read and ignore the P1 and width and height
	//                    P    1    32  32
	char c; int i; ifs >> c >> i >> i >> i;
	std::cout << c << " " << i << std::endl;
	for(int row = 0; row < ROWS; row++)
		for(int column = 0; column < COLUMNS; column++)
		{
			ifs >> arrow[row][column];
			//now we get the array
		}
}

//@   \newpage @
//create buttons to let user choose a image,calls 
//function to load it, create the whole window, 
//calls functions we have created to achieve our goal
int main()
{
	/*
	another way to access the goal:
	if(fl_ask("Do you want the first image?"))
	{load_image("102arrow.pbm");}
	else
	{load_image("arrow2.pbm");}
	*/
	switch ( fl_choice("Choose an image:","2","1",nullptr))
	{
		case 0: 
				load_image("102arrow.pbm");
				//load the first one
				break;
		case 1:
				load_image("arrow2.pbm");
				//load the second one
				break;
	}
	Fl_Cairo_Window cw(WIDTH,HEIGHT);
	cw.color(FL_WHITE);
	cw.label("Cairo Graphics");
	cw.set_draw_cb(drawCB);
	Fl_Button b(cw.x(),cw.y(),cw.w(),cw.h());
	b.callback((Fl_Callback*) goCB);
	cw.show();
	return Fl::run();
}
