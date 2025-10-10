#include <clocale>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <FL/fl_ask.H>
#include "clsForm1Context.h"
#include "../clsAppContext.h"


size_t string_tokenizer(const std::string & str,const std::string & delims, std::vector<std::string> & strlist){
    size_t beg, pos = 0;
    strlist.clear();
    while ((beg = str.find_first_not_of(delims, pos)) != std::string::npos)
    {
        pos = str.find_first_of(delims, beg + 1);
        strlist.push_back(str.substr(beg, pos - beg));
    }

    return strlist.size();
}

clsForm1Context::clsForm1Context()
{
    //ctor
    parent_win=nullptr;
    backs.push(0);

}

clsForm1Context::~clsForm1Context()
{
    //dtor
}
void clsForm1Context::PrepWidgets(){
 {
  auto ch=GetWg<Fl_Choice>("ch_frm1_colsep");
  const static char csp_chars[]=",; \t-";

  ch->add("<,>",0,nullptr,(void*)csp_chars);
  ch->add("<;>",0,nullptr,(void*)(csp_chars+1));
  ch->add("SPACE",0,nullptr,(void*)(csp_chars+2));
  ch->add("TAB",0,nullptr,(void*)(csp_chars+3));
  ch->add("Width",0,nullptr,(void*)(csp_chars+4));
  ch->value(0);
 }
 {
  auto ch=GetWg<Fl_Choice>("ch_frm1_decsep");
  const static char dsp_chars[]=".,";
  ch->add("<.>",0,nullptr,(void*)dsp_chars);
  ch->add("<,>",0,nullptr,(void*)(dsp_chars+1));
  ch->value(0);
 }
 {
  auto ch=GetWg<Fl_Choice>("ch_Frq_mode");
  ch->add(flabs[0],0,nullptr,(void*)0);
  ch->add(flabs[1],0,nullptr,(void*)1);
  ch->value(1);
 }
 {
  auto ch=GetWg<Fl_Choice>("ch_ReIm_mode");
  ch->add("Z(Re,-Im)",0,nullptr,(void*)0);
  ch->add("Z(Re,Im)",0,nullptr,(void*)2);
  ch->add("|Z|,φ",0,nullptr,(void*)4);
  ch->value(0);
 }

 {
  auto ch=GetWg<Fl_Choice>("ch_fi_mode");
  ch->add("rad",0,nullptr,(void*)0);
  ch->add("grad",0,nullptr,(void*)1);
  ch->value(0);
 }
  {
  static double _2Ohms[]={1.0e6,1.0e3,1.0,1.0e-3,1.0e-6};
  auto ch=GetWg<Fl_Choice>("ch_Z_mode");
  ch->add("MΩ",0,nullptr,(void*)_2Ohms);
  ch->add("kΩ",0,nullptr,(void*)(_2Ohms+1));
  ch->add("Ω",0,nullptr,(void*)(_2Ohms+2));
  ch->add("mΩ",0,nullptr,(void*)(_2Ohms+3));
  ch->add("μΩ",0,nullptr,(void*)(_2Ohms+4));
  ch->value(2);
 }

 PrepPlot();
 };
void clsForm1Context::ParentWin( Fl_Double_Window *w){
   clsGenContext::ParentWin(w);
   Fl::option(Fl::OPTION_ARROW_FOCUS, 1);
   clsAppContext * pcntx= (clsAppContext *) w->user_data();
   rawFile=pcntx->rawFile;
   auto cbrw=GetWg<Fl_Multi_Browser1>("brw_SLFile");
   cbrw->clear();
   for(const auto & rst:rawFile)
    cbrw->add(rst.c_str());
   set_active_tbs(0);
};

void clsForm1Context::PrepTable(){
  auto brw=GetWg<Fl_Multi_Browser1>("brw_SLFile");
  size_t nl=brw->size();
  size_t fstl=GetWg<Fl_Value_Output>("vout_frm1_1stLn")->value();
  size_t lstl=GetWg<Fl_Value_Output>("vout_frm1_LastLn")->value();
  size_t hdrl=GetWg<Fl_Value_Output>("vout_frm1_hdr")->value();
  if(hdrl<1||hdrl>=nl){
   fl_alert("Erratic header line: %d:/%d",hdrl,nl);
   return;
  }
  if((lstl<=fstl)
     ||(fstl>nl)
     ||(lstl>nl)){
    fl_alert("Erratic selection: [%d:%d]/%d",fstl,lstl,nl);
    return;
  }
    size_t idx=0;
  std::vector<std::string> bf;
  std::string delims(" \t");
  auto tbl=GetWg<clsSelectCols>("tbl_frm1_Data");
  tbl->Data.clear();
  for(auto const & rln:rawFile){
   idx++;
   if(idx==hdrl){
     string_tokenizer(rln,delims,bf);
     tbl->SetHdr(bf);
    }
   if(idx<fstl||idx>lstl) continue;
   string_tokenizer(rln,delims,bf);
   tbl->AddRow(bf);
  }

  tbl->update();
 };
 //----  -------------------------------------------------------------------------------------------------------
 void clsForm1Context::PrepColsBrw(){
  auto brwrf=GetWg<Fl_Multi_Browser1>("brw_SLFile");
  size_t nl=brwrf->size();
  size_t fstl=GetWg<Fl_Value_Output>("vout_frm1_1stLn")->value();
  size_t lstl=GetWg<Fl_Value_Output>("vout_frm1_LastLn")->value();
  size_t hdrl=GetWg<Fl_Value_Output>("vout_frm1_hdr")->value();
  if(hdrl<1||hdrl>nl){
   fl_alert("Erratic header line: %d:/%d",hdrl,nl);
   return;
  }
  if((lstl<=fstl)
     ||(fstl>nl)
     ||(lstl>nl)){
    fl_alert("Erratic selection: [%d:%d]/%d",fstl,lstl,nl);
    return;
  }
    size_t idx=0;
  auto brww=GetWg<clsCWBrowser>("brw_shw");
  brww->Init();
  brww->add(brwrf->text(hdrl));
  for(size_t i=fstl;i<=lstl;++i)
   brww->add(brwrf->text(i));
  brww->redraw();
 };

 //----  -------------------------------------------------------------------------------------------------------
void clsForm1Context::set_active_tbs(const size_t i, const bool isBack){
  size_t ib=0;
if(!isBack){
 ib=i;
 for(size_t j=0;j<nTbs;++j){
   if(tbs[j]->active()) {backs.push(j); break;};
 }
} else {
  ib=backs.top();
  backs.pop();

}
 for(size_t j=0;j<nTbs;++j)
     tbs[j]->deactivate();
 tbs[ib]->activate();

 auto tb=GetWg<Fl_Tabs>("tbs_frm1_map");
 tb->value(tbs[ib]);
 tb->redraw();
};
void clsForm1Context::Prepare_FrqImRe(){
 if(GetWg<Fl_Choice>("ch_frm1_decsep")->value()==1){
  setlocale(LC_ALL, ".1250");

 }
 auto ch=GetWg<Fl_Choice>("ch_ReIm_mode");
  double zScale=*(double *)GetWg<Fl_Choice>("ch_Z_mode")->mvalue()->user_data();
 int f_mode=GetWg<Fl_Choice>("ch_ReIm_mode")->value();
 size_t fidx=GetWg<Fl_Value_Output>("vout_FrqIdx")->value();
 size_t z1idx=GetWg<Fl_Value_Output>("vout_ReIdx")->value();
 size_t z2idx=GetWg<Fl_Value_Output>("vout_ImIdx")->value();
 auto tb_freim=GetWg<clsTblOutZ>("tbl_frm1_TblOutZ");
 auto & freim=tb_freim->Data;
 freim.clear();
 auto tb_str=GetWg<clsSelectCols>("tbl_frm1_Data");
 auto & strd=tb_str->Data;
 switch(ch->value()){
  case 0:
         for(const auto &st:strd ){
           double fr=std::stod(st.at(fidx).c_str());
           double z1=zScale*std::stod(st.at(z1idx).c_str());
           double z2=zScale*std::stod(st.at(z2idx).c_str());
           if(f_mode==0) fr*=1./(2.*M_PI);
           std::array<double,3> ta={fr,z1,z2};
           freim.push_back(ta);
         }
       break;
  case 1:for(const auto &st:strd ){
           double fr=std::stod(st.at(fidx).c_str());
           double z1=zScale*std::stod(st.at(z1idx).c_str());
           double z2=zScale*std::stod(st.at(z2idx).c_str());
           if(f_mode==0) fr*=1./(2.*M_PI);
           std::array<double,3> ta={fr,z1,-z2};
           freim.push_back(ta);
         }
       break;
  case 2: {  int fi_mode=GetWg<Fl_Choice>("ch_fi_mode",ERR_PLACE)->value();
          for(const auto &st:strd ){
           double fr=std::stod(st.at(fidx).c_str());
           double z1=zScale*std::stod(st.at(z1idx).c_str());
           double z2=std::stod(st.at(z2idx).c_str());
           if(f_mode==0) fr*=1./(2.*M_PI);
           if(fi_mode==1) z2*=M_PI/180.;
           auto cx=gsl_complex_polar(z1,z2);
           std::array<double,3> ta={fr,GSL_REAL(cx),-GSL_IMAG(cx)};
           freim.push_back(ta);
         }
        }
       break;
 default:;

 }
 setlocale(LC_ALL, "C");
 tb_freim->update();
 Plot(freim);
}
