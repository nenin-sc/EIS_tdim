#include <FL/fl_draw.H>
#include "clsForm1Context.h"

   clsTblOutZ::~clsTblOutZ(){};
//----  --------------------------------------------------------------------------------------------------------

clsTblOutZ::clsTblOutZ(int x, int y, int w, int h, const char *l):Fl_Table(x,y,w,h,l){


    ColsHDR={"Frequency,Hz","Z.Re, Ohm","-Z.Im, Ohm"};
    Data={{0.0,0.0,0.0},{0.0,0.0,0.0}};
    col_header(1);
    col_resize(1);
    col_header_height(25);
    row_header(1);
    row_resize(0);
	row_header_width(75);
	rows(Data.size());
	cols(ColsHDR.size());

	end();

 };
//----  --------------------------------------------------------------------------------------------------------
 void clsTblOutZ::update(){
 	rows(Data.size());
//	cols(ColsHDR.size());
    redraw();
  };
//----  --------------------------------------------------------------------------------------------------------
void clsTblOutZ::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H){

   static char s[40];
    switch ( context ) {
      case CONTEXT_STARTPAGE:                   // before page is drawn..
        fl_font(FL_HELVETICA, 16);              // set the font for our drawing operations
        return;
    	case CONTEXT_RC_RESIZE:
	{
	    int X, Y, W, H;
	    int index = 0;
	    for ( int r = 0; r<rows(); r++ )
	    {
		for ( int c = 0; c<cols(); c++ )
		{
		    if ( index >= children() ) break;
		    find_cell(CONTEXT_TABLE, r, c, X, Y, W, H);
		    child(index++)->resize(X,Y,W,H);
		}
	    }
	    init_sizes();			// tell group children resized
	    return;
	}

      case CONTEXT_COL_HEADER:                  // Draw column headers

        DrawHeader(ColsHDR[COL].c_str(),X,Y,W,H);
        return;
      case CONTEXT_ROW_HEADER:                  // Draw row headers
        sprintf(s,"%02d",ROW);                 // "001:", "002:", etc
        DrawHeader(s,X,Y,W,H);
        return;
      case CONTEXT_CELL:  // Draw data in cells
               DrawData(std::to_string(Data[ROW][COL]),X,Y,W,H); // normal cell
               return;

      default:
        return;
    }
};
//----  --------------------------------------------------------------------------------------------------------
void clsTblOutZ::DrawHeader(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
      fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
      fl_color(FL_BLACK);
      fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
    fl_pop_clip();
  }
//----  --------------------------------------------------------------------------------------------------------
void clsTblOutZ::DrawData(const std::string &s, int X, int Y, int W, int H){
    fl_push_clip(X,Y,W,H);
      // Draw cell bg
       fl_color(FL_WHITE);  fl_rectf(X,Y,W,H);

       if(s.size()>0){
       fl_color(FL_BLACK);
      fl_draw(s.c_str(), X,Y,W,H, FL_ALIGN_CENTER);
       };
      fl_color(color()); fl_rect(X,Y,W,H);
    fl_pop_clip();
  };
