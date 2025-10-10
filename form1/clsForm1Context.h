#ifndef CLSFORM1CONTEXT_H
#define CLSFORM1CONTEXT_H
#include <vector>
#include <array>
#include <map>
#include <stack>
#include <FL/Fl_Input.h>
#include <FL/Fl_Multi_Browser.h>
#include <FL/Fl_Table.h>
#include <FL/user/fl_small_stuff.h>
#include <Fl/user/Plot3.13/Fl_PlotXY3.h>
#include <FL/user/clsGenContext.h>
#include "clsCWBrowser/clsCWBrowser.h"
#include "form1.h"


class Fl_Multi_Browser1 : public Fl_Multi_Browser {
    public:
    Fl_Multi_Browser1(int X,int Y,int W,int H,const char*L=0):Fl_Multi_Browser(X,Y,W,H,L) {

        type(FL_MULTI_BROWSER);
    }
    int handle(int e);
    int GetFirst();
    int GetLast();
    int GetHDR();
};

class clsSelectCols:public Fl_Table
{

 public:
    std::vector<std::vector<std::string> > Data;
    std::vector<std::string> ColsHDR;
   clsSelectCols(int x, int y, int w, int h, const char *l=0);
   virtual ~clsSelectCols();
   void update();

   size_t GetSelectedIdx();
   const std::string & GetSelectedCell();
   size_t GetSelectedColumn(const bool fst=true); // first or last column cell
   void AddRow(const std::vector<std::string> &rw);
   void SetHdr(const std::vector<std::string> &rw);
   int  handle(int e);

 protected:
   Fl_Menu_Item *pulldown;

   void draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H);
   void DrawData(const std::string &s,const bool isSeleted,int X, int Y, int W, int H);
   void DrawHeader(const char *s, int X, int Y, int W, int H);
};
//----  --------------------------------------------------------------------------------------------------------
class clsTblOutZ:public Fl_Table
{

 public:
   std::vector<std::array<double,3> > Data;
   std::vector<std::string> ColsHDR;
    clsTblOutZ(int x, int y, int w, int h, const char *l=0);
   virtual ~clsTblOutZ();
   void update();

 protected:
   void draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H);
   void DrawData(const std::string &s,int X, int Y, int W, int H);
   void DrawHeader(const char *s, int X, int Y, int W, int H);
};


class clsForm1Context:public clsGenContext
{
    public:
        std::vector<std::pair<std::string, std::string>> *MetaData;
        enum {nTbs=4,};

        Fl_Group * tbs[nTbs];
        const char *zlabs[6]={"Z(Re)","-Z(Im)","Z(Re)","Z(Im)","|Z|","φ"};
        const char *flabs[2]={"ω","f"};
        using clsGenContext::ParentWin;
        clsForm1Context();

        virtual ~clsForm1Context();
        virtual void ParentWin( Fl_Double_Window *w);
        void PrepTable();
        void PrepColsBrw();
       void PrepWidgets();
    // cb
        void cbtn_frm12Tabl();
        void cbtn_frm12Stat();
        void cbCancel();
        void cbFinish();
        void cbtn_frm1Back();
        void cbWgt2Tab();
        void cbShowTabs(const bool show);
        void cbSavePrf();
        void cbOpenPrf();
    protected:
       void PrepPlot();
       void Plot(const std::vector<std::array<double,3> > & fzdata);
       void Prepare_FrqImRe();
       void set_active_tbs(const size_t i,  const bool isBack=false);
       std::stack<size_t> backs;
       std::vector<std::string>  rawFile;

    private:
};

#endif // CLSFORM1CONTEXT_H
