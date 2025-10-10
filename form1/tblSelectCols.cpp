#include <FL/Fl_Menu_Item.h>
#include <FL/fl_draw.H>
#include "clsForm1Context.h"


void cbmnu_frm1_crd(Fl_Widget *p, void* userdata){
 clsSelectCols *tbl = (clsSelectCols *) p;
 clsForm1Context *cntxt= (clsForm1Context*) tbl->user_data();
 Fl_Menu_Item * mn = (Fl_Menu_Item *) userdata;
 int idx= (size_t) mn->user_data();
  int cl=tbl->GetSelectedColumn();
 switch(idx){
  case 0:cntxt->GetWg<Fl_Value_Output>("vout_FrqIdx")->value(cl);
         break;
  case 1:cntxt->GetWg<Fl_Value_Output>("vout_ReIdx")->value(cl);
         break;
  case 2:cntxt->GetWg<Fl_Value_Output>("vout_ImIdx")->value(cl);
         break;
  default:break;
 }
}

//----  --------------------------------------------------------------------------------------------------------
clsSelectCols::clsSelectCols(int x, int y, int w, int h, const char *l):Fl_Table(x,y,w,h,l){
    pulldown=nullptr;
    ColsHDR={"A","B"};
    Data={{"Empty","Empty"},{"Empty","Empty"}};
    col_header(1);
    col_resize(1);
    col_header_height(25);
    row_header(1);
    row_resize(0);
	row_header_width(75);
	rows(Data.size());
	cols(Data.front().size());

	end();
    static Fl_Menu_Item mnu_cpy[] = {
    {"Frq",	0,cbmnu_frm1_crd, (void*)0},
    {"Z1",	0,cbmnu_frm1_crd, (void*)1},
    {"Z2",	0,cbmnu_frm1_crd, (void*)2},
    {0}};
    pulldown=mnu_cpy;
 };

clsSelectCols::~clsSelectCols(){};
//----  --------------------------------------------------------------------------------------------------------
 void clsSelectCols::update(){
 	rows(Data.size());
	cols(ColsHDR.size());
    redraw();
  };

void clsSelectCols::SetHdr(const std::vector<std::string> &rw){
 ColsHDR= rw;
};

void clsSelectCols::AddRow(const std::vector<std::string> &rw){
 Data.push_back(rw);
};
//----  --------------------------------------------------------------------------------------------------------
const std::string & clsSelectCols::GetSelectedCell(){
 static std::string err="";
 int r0,r1,c0,c1;
 get_selection(r0,c0,r1,c1);
 if(r0<0||c0<0) return err;
 return Data[r0][c0];
 };
size_t clsSelectCols::GetSelectedColumn(const bool fst){
 int r0,r1,c0,c1;
 get_selection(r0,c0,r1,c1);
 if(r0<0||c0<0) return -1;
 return fst?c0:c1;
 };

//----  --------------------------------------------------------------------------------------------------------
void clsSelectCols::DrawHeader(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
      fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
      fl_color(FL_BLACK);
      fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
    fl_pop_clip();
  }
//----  --------------------------------------------------------------------------------------------------------
void clsSelectCols::DrawData(const std::string &s,const bool isSeleted, int X, int Y, int W, int H){
    fl_push_clip(X,Y,W,H);
      // Draw cell bg
       fl_color(isSeleted ? FL_YELLOW : FL_WHITE);  fl_rectf(X,Y,W,H);

       if(s.size()>0){
       fl_color(FL_BLACK);
      fl_draw(s.c_str(), X,Y,W,H, FL_ALIGN_CENTER);
       };
      fl_color(color()); fl_rect(X,Y,W,H);
    fl_pop_clip();
  };
//----  --------------------------------------------------------------------------------------------------------
void clsSelectCols::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
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
        sprintf(s,"%d",ROW);                 // "001:", "002:", etc
        DrawHeader(s,X,Y,W,H);
        return;
      case CONTEXT_CELL:  // Draw data in cells
              if(COL<ColsHDR.size()&&COL<Data[ROW].size()){
               DrawData(Data[ROW][COL],is_selected(ROW,COL),X,Y,W,H); // normal cell
            ///   return;
              };

             return;

      default:
        return;
    }
  }


//----  --------------------------------------------------------------------------------------------------------
 int clsSelectCols::handle(int e){
  switch (e) {
            case FL_PUSH:
                // RIGHT MOUSE PUSHED? Popup menu on right click
                if ( Fl::event_button() == FL_RIGHT_MOUSE ) {
                    const Fl_Menu_Item *m = pulldown->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
                    if ( m ) m->do_callback(this, (void *)m);
                    return(1);          // (tells caller we handled this event)
                }
                break;
            case FL_RELEASE:
                // RIGHT MOUSE RELEASED? Mask it from Fl_Input
                if ( Fl::event_button() == FL_RIGHT_MOUSE ) {
                  return(1);          // (tells caller we handled this event)
                }
                break;
        }
        return(Fl_Table::handle(e));    // let Fl_Input handle all other events
  };

