#ifndef CLSMETADATATABLE_H
#define CLSMETADATATABLE_H
#include <vector>
#include <string>
#include <FL/Fl_Input.h>
#include <FL/Fl_Table_Row.h>

class Fl_TableScroll_Input:public Fl_Input
{

 public:

   Fl_TableScroll_Input(int x, int y, int w, int h, const char *l=0):Fl_Input(x, y, w, h, l){};

   virtual ~Fl_TableScroll_Input(){};
   int handle(int e) {

   if(e==FL_UNFOCUS){
       do_callback();

    }
    return(Fl_Input::handle(e));
   };

};

class clsMetaDataTable:public Fl_Table_Row
{
  int row_edit, col_edit;                               // row/col being modified
  TableContext context_edit;


  void set_value_hide();
  void start_editing(TableContext context, int R, int C);

    static void input_cb(Fl_Widget* o,void* v) {            // input widget's callback
    ((clsMetaDataTable*)v)->set_value_hide();
    }
 public:
   Fl_TableScroll_Input *txtin_sl;
   std::vector<std::pair<std::string, std::string>> *Data;
   void BindData(std::vector<std::pair<std::string, std::string>> &data) {Data=&data;};
   clsMetaDataTable(int x, int y, int w, int h, const char *l=0);
   virtual ~clsMetaDataTable();
   void update();
   size_t GetSelectedIdx();
   const std::string & GetSelectedCell();
   size_t GetSelectedColumn(const bool fst=true); // first or last column cell
   void tbl_callback();
  int  handle(int e);
 //---- cb -----------------------------------------------------------------------------------------------------

 void cbAddRow();
 void cbDelRow();
 void cbClearAll();
 void done_editing();
 protected:
   Fl_Menu_Item *pulldown;
   std::vector<std::string> ColsHDR;
   void draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H);
   void DrawData(const std::string &s,const bool isSeleted,int X, int Y, int W, int H);
   void DrawHeader(const char *s, int X, int Y, int W, int H);

};



#endif // CLSMETADATATABLE_H
