#include <algorithm>

#include <FL/Fl.H>
#include <FL/Fl_ask.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.h>
#include "clsCWBrowser.h"



///****************************************************
void clsCWBrowser::dbInit(){
   static   int widths[] = { 50, 50, 50, 70, 70, 40, 40, 70, 70, 50, 0 };
   columns_widths(widths);
   showcolsep(1);
    colsepcolor(FL_RED);
                                                          // tabs as column delimiters


    //// SIMPLE UN-COLORED HEADING
    add("Number   Voltage,V   Current,A   Power W   Wgt");
    add("  1        0.0         0.0          0.0     100% ");
    add("  2        1.0         1.0          1.0     100% ");
    add("  3        2.0         2.0          4.1     100% ");
    add("0123456789012345678901234567890     4.1     100% ");

};

void clsCWBrowser::Init(){
   static   int widths[] = { 50, 50, 50, 0 };
   columns_widths(widths);
   showcolsep(1);
   colsepcolor(FL_RED);
   clear();
};

void clsCWBrowser::MapCols(std::vector<size_t> & cmap){
  cmap.clear();
  auto itm=item_at(1);
  int wgp=item_width(itm);
  const char *tx1=item_text(itm);
  int wgch=strlen(tx1);
  int ltw=wgp/wgch;
  const size_t * cwgt=columns_widths();
  for(;*cwgt!=0;cwgt++){
    int cwt=*cwgt/ltw;
    if(*cwgt%ltw>ltw/2) cwt++;
    cmap.push_back(cwt);
    printf("%d\t",cwt);
  }
  printf("\n");
  };

//----  --------------------------------------------------------------------------------------------------------
void clsCWBrowser::AddCol(const size_t wg){
  size_t idx=widthsN();
  if(idx<maxNWidth){
    _widths[idx]=wg;
    return;
  };
    fl_message("No space for another column\n .");
 }

//----  --------------------------------------------------------------------------------------------------------
void clsCWBrowser::DelCol(const size_t idxd){
  size_t idx=widthsN();
  if(idx<4){
    fl_message("At least 3 columns must be set. \n .");
    return;
  }
  idx--;
  if(idxd>maxNWidth){
    _widths[idx]=0; // del last tab
  } else {
  for(size_t i=idx+1;i<maxNWidth;++i)
   _widths[i-1]=_widths[i]; // del selectet tab ---???
  };
};
//----  --------------------------------------------------------------------------------------------------------
static void cbmnu_fxw_add_col(Fl_Widget * wg, void* userdata){
 clsCWBrowser *brw = (clsCWBrowser *) wg;
 brw->AddCol(10);
 brw->redraw();
}

static void cbmnu_fxw_del_col(Fl_Widget * wg, void* userdata){
 clsCWBrowser *brw = (clsCWBrowser *) wg;
 brw->DelCol(brw->szNWidth);
 brw->redraw();
}

static Fl_Menu_Item row_sel_fxw[] = {
  {"Add col. marker.",0,cbmnu_fxw_add_col},
  {"Delete col. marker.",0,cbmnu_fxw_del_col},
   {0}
};

//----  --------------------------------------------------------------------------------------------------------
void clsCWBrowser::draw() {
        // DRAW BROWSER
        Fl_Browser::draw();
        if ( _showcolsep ) {
            // DRAW COLUMN SEPARATORS
            int colx = this->x() - hposition();
            int X,Y,W,H;
            Fl_Browser::bbox(X,Y,W,H);
            fl_color(_colsepcolor);
            for ( int t=0; _widths[t]; t++ ) {
                colx += _widths[t];
                if ( colx > X && colx < (X+W) ) {
                    fl_line(colx, Y, colx, Y+H-1);
                }
            }
        }
    }
//----  --------------------------------------------------------------------------------------------------------
// RETURN THE COLUMN MOUSE IS 'NEAR'
    //     Returns -1 if none.
    //
 int clsCWBrowser::which_col_near_mouse() {
        int X,Y,W,H;
        Fl_Browser::bbox(X,Y,W,H);            // area inside browser's box()
        // EVENT NOT INSIDE BROWSER AREA? (eg. on a scrollbar)
        if ( ! Fl::event_inside(X,Y,W,H) ) {
            return(-1);
        }
        int mousex = Fl::event_x() + hposition();
        int colx = this->x();
        for ( int t=0; _widths[t]; t++ ) {
            colx += _widths[t];
            int diff = mousex - colx;
            // MOUSE 'NEAR' A COLUMN?
            //     Return column #
            //
            if ( diff >= -4 && diff <= 4 ) {
                return(t);
            }
        }
        return(-1);
    }
//----  --------------------------------------------------------------------------------------------------------
 int clsCWBrowser::handle(int e) {
        // Not showing column separators? Use default Fl_Browser::handle() logic
        if ( !showcolsep() ) return(Fl_Browser::handle(e));
        // Handle column resizing
        bool ln_sel_click=false;
        int ret = 0;
        switch ( e ) {
            case FL_ENTER: {
                ret = 1;
                break;
            }
            case FL_MOVE: {
                change_cursor( (which_col_near_mouse() >= 0) ? FL_CURSOR_WE
                                                             : FL_CURSOR_DEFAULT);
                ret = 1;
                break;
            }
            case FL_PUSH: {
                int whichcol = which_col_near_mouse();
                if ( whichcol >= 0 ) {
                    // CLICKED ON RESIZER? START DRAGGING
                    _drag_col = whichcol;
                    change_cursor(FL_CURSOR_DEFAULT);
                    return 1;   // eclipse event from Fl_Browser's handle()
                }
                         // (prevents FL_PUSH from selecting item)

                   if (Fl::event_button () == FL_RIGHT_MOUSE)
                        {
                          const Fl_Menu_Item *m = row_sel_fxw->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
                          if ( m ) m->do_callback(this, user_data());
                          printf("R click on browser \n");
                           return 1;
                            }
                        else
                           {
                            if (Fl::event_button () == FL_LEFT_MOUSE){
                            ln_sel_click=true;

                            }

                        }


                break;
            }
            case FL_DRAG: {
                if ( _drag_col != -1 ) {
                    // Sum up column widths to determine position
                    int mousex = Fl::event_x() + hposition();
                    int newwidth = mousex - x();
                    for ( int t=0; _widths[t] && t<_drag_col; t++ ) {
                        newwidth -= _widths[t];
                    }
                    if ( newwidth > 0 ) {
                        // Apply new width, redraw interface
                        _widths[_drag_col] = newwidth;
                        if ( _widths[_drag_col] < 2 ) {
                            _widths[_drag_col] = 2;
                        }
                        recalc_hscroll();
                        redraw();
                    }
                    return 1;   // eclipse event from Fl_Browser's handle()
                }
                break;
            }
            case FL_LEAVE:
            case FL_RELEASE: {
                _drag_col = -1;                         // disable drag mode
                change_cursor(FL_CURSOR_DEFAULT);       // ensure normal cursor
                if ( e == FL_RELEASE ) return 1;        // eclipse event
                ret = 1;
                break;
            }
        }
        ret=Fl_Browser::handle(e) ? 1 : ret;

        return ret;
    }
