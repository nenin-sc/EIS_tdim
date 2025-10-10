#ifndef CLSCWBROWSER_H
#define CLSCWBROWSER_H
#include <vector>
#include <FL/Fl_Browser.h>
#include <FL/user/clsGenContext.h>

class clsCWBrowser : public Fl_Browser {
 public:
    enum {maxNWidth=150,szNWidth=maxNWidth+1,};
 protected:
    Fl_Color  _colsepcolor;     // color of column separator lines
    int       _showcolsep;      // flag to enable drawing column separators
    Fl_Cursor _last_cursor;     // saved cursor state info
    int       _drag_col;        // col# user is dragging (-1 = not dragging)
    size_t  _widths[szNWidth]={0};
   // int       _nowidths[1];     // default width array (non-const)
    // CHANGE CURSOR
    //     Does nothing if cursor already set to value specified.
    //
    void change_cursor(Fl_Cursor newcursor) {
        if ( newcursor == _last_cursor ) return;
     //   ((clsGenContext*) user_data())->Win()->window()->cursor(newcursor);
        ((clsGenContext*) user_data())->Win()->cursor(newcursor);
        _last_cursor = newcursor;
    }
    // RETURN THE COLUMN MOUSE IS 'NEAR'
    //     Returns -1 if none.
    //
    int which_col_near_mouse();
    // FORCE SCROLLBAR RECALC
    //    Prevents scrollbar from getting out of sync during column drags
    void recalc_hscroll() {
      int size = textsize();
      int hp=hposition();
      textsize(size+1);     // XXX: changing textsize() briefly triggers
      textsize(size);       // XXX: recalc Fl_Browser's scrollbars
      hposition(hp);
      redraw();
    }

    // MANAGE EVENTS TO HANDLE COLUMN RESIZING
     int handle(int e);
    void draw() ;
public:
    // CTOR
    clsCWBrowser(int X,int Y,int W,int H,const char*L=0) : Fl_Browser(X,Y,W,H,L) {

        _colsepcolor = Fl_Color(FL_BLUE);
        _last_cursor = FL_CURSOR_DEFAULT;
   //      type(FL_MULTI_BROWSER);

         column_char('\t');
        _showcolsep  = 1;
        _drag_col    = -1;
      //  _nowidths[0] = 0;

    }
    void Init();
    // GET/SET COLUMN SEPARATOR LINE COLOR
    Fl_Color colsepcolor() const {
        return(_colsepcolor);
    }
    void colsepcolor(Fl_Color val) {
        _colsepcolor = val;
    }
    // GET/SET DISPLAY OF COLUMN SEPARATOR LINES
    //     1: show lines, 0: don't show lines
    //
    int showcolsep() const {
        return(_showcolsep);
    }
    void showcolsep(int val) {
        _showcolsep = val;
    }
    // GET/SET COLUMN WIDTHS ARRAY
    //    Just like fltk method, but array is non-const.
    //
   size_t *columns_widths()  {
        return(_widths);
    }
   void columns_widths(int *val) {
      for(size_t i=0;i<maxNWidth;i++){
          _widths[i]=val[i];
          if(!val[i]) break;
        }
   };
   size_t widthsN() const{
       for(size_t i=0;i<maxNWidth;i++){
        if(_widths[i]==0) return i;

       }
       return maxNWidth;
    };
   void AddCol(const size_t wg);
   void DelCol(const size_t idxd);

   void dbInit();
   void MapCols(std::vector<size_t> & cmap);

 int SelectedIdx() const {
   for ( int t=1; t<=size(); ++t )
    if(selected(t)){
    return t;
    };
   return 0;
  };

};



#endif // CLSCWBROWSER_H
