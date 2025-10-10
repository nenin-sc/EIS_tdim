#include <FL/fl_draw.H>
#include "../clsAppContext.h"

clsTblSLStat::clsTblSLStat(int x, int y, int w, int h, const char *l):Fl_Table(x,y,w,h,l){
    txtin_sl= new Fl_Input(w/2,h/2,0,0);
    txtin_sl->hide();
    txtin_sl->when(FL_WHEN_ENTER_KEY_ALWAYS);
    txtin_sl->color(FL_YELLOW);
    txtin_sl->callback(input_cb, (void*)this);
    end();

    pulldown=nullptr;
    ColsHDR={"Key","Value"};
    SLData={{"Key1","Empty"},{"Key2","Empty"}};
    col_header(1);
    col_resize(1);
    col_header_height(25);
    row_header(1);
    row_resize(0);
	row_header_width(75);
	rows(SLData.size());
	cols(ColsHDR.size());

	end();

 };

//----  --------------------------------------------------------------------------------------------------------
clsTblSLStat::~clsTblSLStat(){};
//----  --------------------------------------------------------------------------------------------------------
 void clsTblSLStat::update(){
 	rows(SLData.size());
	cols(ColsHDR.size());
    redraw();
  };
//----  --------------------------------------------------------------------------------------------------------
void clsTblSLStat::cbAddRow(){
  SLData.push_back({"Key?","Empty"});
  update();
  };

//----  --------------------------------------------------------------------------------------------------------
void clsTblSLStat::DrawHeader(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
      fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
      fl_color(FL_BLACK);
      fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
    fl_pop_clip();
  }
//----  --------------------------------------------------------------------------------------------------------
void clsTblSLStat::DrawData(const std::string &s,const bool isSeleted, int X, int Y, int W, int H){
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
void clsTblSLStat::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
    static char s[40];
    switch ( context ) {
      case CONTEXT_STARTPAGE:                   // before page is drawn..
        fl_font(FL_HELVETICA, 14);              // set the font for our drawing operations
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
          {
          if ( context_edit == context && ROW == row_edit && COL == col_edit && txtin_sl->visible())
           return;
        switch(COL){
         case 0: if(ROW<SLData.size()){
                 DrawData(SLData.at(ROW).first,is_selected(ROW,COL),X,Y,W,H); // normal cell
                  };
              break;
         case 1: if(ROW<SLData.size()){

                  DrawData(SLData.at(ROW).second,is_selected(ROW,COL),X,Y,W,H); // normal cell
                  };
              break;

          default:;
             }
        }


      default:
        return;

    }
  }

//----  --------------------------------------------------------------------------------------------------------
void clsTblSLStat::tbl_callback() {
  int R = callback_row();
  int C = callback_col();
  TableContext context = callback_context();

  switch ( context ) {
    case CONTEXT_ROW_HEADER:done_editing();   break;
    case CONTEXT_COL_HEADER:done_editing();   break;
    case CONTEXT_CELL: {                                // A table event occurred on a cell
      switch (Fl::event()) {                            // see what FLTK event caused it
        case FL_PUSH:                                   // mouse click?
          done_editing();
           set_selection(R, C, R, C);                          // finish editing previous
         // if (C!=2)  return;
          start_editing(context, R, C);
          return;

        case FL_KEYBOARD:                           // key press in table?
            done_editing();                               // finish any previous editing
            set_selection(R, C, R, C);
	      if (*Fl::event_text() != '\r') {
	       txtin_sl->handle(Fl::event());			// pass keypress to input widget
	     start_editing(context,R,C);
	  }
          return;
      }
      return;
    }

    case CONTEXT_TABLE:                                 // A table event occurred on dead zone in table
      done_editing();                                   // done editing, hide
      return;

    default:
      return;
  }
}
//----  --------------------------------------------------------------------------------------------------------
  void clsTblSLStat::set_value_hide() {
    if(row_edit<SLData.size()){
                if(col_edit==1) {
                   SLData.at(row_edit).second=txtin_sl->value(); }
                   else
                   SLData.at(row_edit).first=txtin_sl->value();
                 }
    txtin_sl->hide();
    window()->cursor(FL_CURSOR_DEFAULT);                    // if we don't do this, cursor can disappear!
    take_focus();
    redraw();
  }

// --------------Tell the input widget it's done editing, and to 'hide'
void clsTblSLStat::done_editing() {


    if (txtin_sl->visible()) {                             // input widget visible, ie. edit in progress?
      set_value_hide();                                 // Transfer its current contents to cell and hide
    }
  }


//----  --------------------------------------------------------------------------------------------------------


  void  clsTblSLStat::start_editing(TableContext context, int R, int C) {
    // Keep track of cell being edited
    context_edit = context;
    row_edit = R;
    col_edit = C;
    // Clear any previous multicell selection
    // Find X/Y/W/H of cell
    int X,Y,W,H;
    find_cell(context, R,C, X,Y,W,H);
    txtin_sl->resize(X,Y,W,H);                             // Move Fl_Input widget there
   if(R<SLData.size()){
                table->add(*txtin_sl);                     // *** parent to scrollable table
                 if(col_edit==1) {
                 txtin_sl->value(SLData.at(row_edit).second.c_str());
                 } else txtin_sl->value(SLData.at(row_edit).first.c_str());
    }
    txtin_sl->show();                                      // Show the input widget, now that we've positioned it
    txtin_sl->take_focus();
    redraw();
  }
//----  --------------------------------------------------------------------------------------------------------
 int clsTblSLStat::handle(int e) {
        int ret=Fl_Table::handle(e);
    if(e== FL_FOCUS || e==FL_UNFOCUS){
      return 1;
    }

	if ( e == FL_KEYBOARD){
	   int r1,c1,r2,c2;
       get_selection(r1,c1,r2,c2);
       if(r1>=0 && c1>=0 && r1<rows() && c1<cols() && Fl::event_key()==FL_Enter ){
        do_callback(CONTEXT_CELL,r1,c1);
       }
       //
       return 1;
	 }
   return ret;
};
