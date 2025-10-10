#include <algorithm>
#include <Fl/Fl_ask.h>
#include <fmt/format.h>
#include <pystring.h>
#include "../clsAppContext.h"
#include "clsForm1Context.h"




void clsForm1Context::cbFinish(){
  int i0=GetWg<Fl_Value_Output>("vout_frm1_Meta1stLn")->value();
  int i1=GetWg<Fl_Value_Output>("vout_frm1_MetaLastLn")->value();
  if(i0>i1) std::swap(i0,i1);
  if(i0>0){
   auto brww=GetWg<clsCWBrowser>("brw_SLFile");
   size_t nl=brww->size();
   if(i1>nl) i1=nl;
   for(size_t idx=i0;idx<=i1;++idx){
       std::string s=brww->text(idx);
      std::replace(s.begin(), s.end(), '\t', ' ');
      std::replace(s.begin(), s.end(), static_cast<char>(164), ' ');
      auto s1=pystring::strip(s);

      if(s1.size()>0)
        MetaData->push_back({fmt::format("Line{}",idx-i0),s1});
    }}
  Fl_Double_Window * parent_win=ParentWin();
  clsAppContext * parent_cntxt= (clsAppContext *) parent_win->user_data();
  auto tb_freim=GetWg<clsTblOutZ>("tbl_frm1_TblOutZ");
  parent_cntxt->Win_activate(tb_freim->Data);
  Win()->hide();
};

//----   ------------------------------------------------------------------------------
void clsForm1Context::cbCancel(){

  ParentWin()->activate();
  Win()->hide();
};
//----   -----------------------------------------------------------------------------------------------------
void clsForm1Context::cbWgt2Tab(){
    auto brww=GetWg<clsCWBrowser>("brw_shw");
    size_t nl=brww->size();

    auto tbl=GetWg<clsSelectCols>("tbl_frm1_Data");
    tbl->Data.clear();
    std::vector<size_t> cmap;
    brww->MapCols(cmap);
    for(size_t idx=1;idx<=nl;++idx){
      const char *s=brww->text(idx);
      if(s==nullptr) continue;
      std::string bf0=s;
      std::vector<std::string> lnbt;
      size_t base=0;
      for(auto const &cl:cmap){
        if(bf0.size()>base){
          std::string subs=bf0.substr(base,cl);
          base+=cl;
          auto subs1=pystring::strip(s);
          lnbt.push_back(subs1);
          continue;
         }
       break;
      }
     if(idx==1) {
        tbl->ColsHDR=lnbt;
        continue;
      }
       tbl->Data.push_back(lnbt);
    }
   tbl->update();
   set_active_tbs(1);

};


//----  -----------------------------------------------------------------------------------------------

void clsForm1Context::cbtn_frm12Tabl(){
  auto ch=GetWg<Fl_Choice>("ch_frm1_colsep")->mvalue();
  char swc=*(char*) ch->user_data();
  if(swc=='-'){
    PrepColsBrw();
    set_active_tbs(3);
  return;
  };
  PrepTable();
  set_active_tbs(1);

};

//--------------------
void clsForm1Context::cbtn_frm12Stat(){
  Prepare_FrqImRe();
  set_active_tbs(2);
};
//----------------------------------------------
 void clsForm1Context::cbtn_frm1Back(){

  set_active_tbs(0,true);
 };

//--------------------
 void clsForm1Context::cbShowTabs(const bool show){
   auto cbrw=GetWg<Fl_Multi_Browser1>("brw_SLFile");
   cbrw->clear();
   if(show){
    for(auto rst:rawFile){

      std::replace(rst.begin(),rst.end(),'\t', static_cast<char>(164)); /// -- ¤?
      cbrw->add(rst.c_str());
     };
    return;
   }
   for(const auto & rst:rawFile)
    cbrw->add(rst.c_str());
   };



