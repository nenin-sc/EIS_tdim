#include <FL/filename.H>

#include "clsAppContext.h"
#include  "form1/clsForm1Context.h"
extern Fl_Double_Window* winit_form1();


clsAppContext::clsAppContext()
{
    //ctor

}

clsAppContext::~clsAppContext()
{
    //dtor

}

void clsAppContext::init_windows(){
  RegisterWin("Main",win);
  auto wf1=winit_form1();
  RegisterWin("Form1",wf1);
  reinterpret_cast<clsForm1Context*>(wf1->user_data())->MetaData=&MetaData;
  GetWg<clsMetaDataTable>("tbl_Metadata")->Data=&MetaData;

  {
 auto ch=GetWg<Fl_Choice>("ch_Area");
 ch->add("Custom");
 areas.push_back(0.0);
 ch->add("Std 4x4");
 areas.push_back(16.0);
 ch->add("Elco 10x10");
 areas.push_back(92.16);
 ch->value(0);


 }
 cbSelectArea(0);

};
//----  --------------------------------------------------------------------------------------------------------
void clsAppContext::Win_activate(const std::vector<std::array<double,3> > &data){
 fReIm=data;
 auto tbl=GetWg<clsMetaDataTable>("tbl_Metadata");
 tbl->update();

 Win()->activate();
};




