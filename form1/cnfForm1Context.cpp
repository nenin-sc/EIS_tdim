 #include <fmt/core.h>
 #include <FL/fl_ask.H>
#include  <FL/fl_utf8.h>
#include <config/libconfig.h++>
#include "clsForm1Context.h"
using namespace libconfig;
const char *FIDtxsc="Txt imp. schema v0";
const char *EXTtxsc=".ftsch";
void clsForm1Context::cbSavePrf(){
   Fl_Native_File_Chooser native;

  native.title("Select file to save import schema");
  native.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  auto buf=fl_check_access_nfc(native, EXTtxsc, "EIS text import schema");
  if(buf==nullptr) return;
  Config cfg;
  cfg.setFloatPrecision(12);
  cfg.setOption(Config::OptionAllowScientificNotation, true);
  Setting &root = cfg.getRoot();
  root.add("FID", Setting::TypeString)= FIDtxsc;
  root.add("MetaStartLine", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_frm1_Meta1stLn",ERR_PLACE)->value();
  root.add("MetaEndLine", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_frm1_MetaLastLn",ERR_PLACE)->value();
  root.add("DataHeader", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_frm1_hdr",ERR_PLACE)->value();
  root.add("DataFirstLine", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_frm1_1stLn",ERR_PLACE)->value();
  root.add("DataLastLine", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_frm1_LastLn",ERR_PLACE)->value();

  root.add("SP_Columns", Setting::TypeInt)= GetWg<Fl_Choice>("ch_frm1_colsep",ERR_PLACE)->value();
  root.add("SP_Decimal", Setting::TypeInt)= GetWg<Fl_Choice>("ch_frm1_decsep",ERR_PLACE)->value();
  root.add("SP_ShowTabs", Setting::TypeInt)= GetWg<Fl_Check_Button>("chbtn_ShowTabs",ERR_PLACE)->value();
  root.add("DataFCol", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_FrqIdx",ERR_PLACE)->value();
  root.add("DataZ1Col", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_ReIdx",ERR_PLACE)->value();
  root.add("DataZ2Col", Setting::TypeInt)= (int)GetWg<Fl_Value_Output>("vout_ImIdx",ERR_PLACE)->value();

  root.add("DataFType", Setting::TypeInt)= GetWg<Fl_Choice>("ch_Frq_mode",ERR_PLACE)->value();
  root.add("DataZType", Setting::TypeInt)= GetWg<Fl_Choice>("ch_ReIm_mode",ERR_PLACE)->value();
  root.add("DataZScale", Setting::TypeInt)= GetWg<Fl_Choice>("ch_Z_mode",ERR_PLACE)->value();
  root.add("DataFiMode", Setting::TypeInt)= GetWg<Fl_Choice>("ch_fi_mode",ERR_PLACE)->value();

    std::string rts="OK";
  FILE * wf=fl_fopen(buf,"w");
   try
  {
   cfg.write(wf);
   fclose(wf);

  }
  catch(const FileIOException &fioex)
  {
    fclose(wf);
    rts="I/O error while writing file: %s";

  }
  if(rts!="OK"){
   fl_message(rts.c_str(),buf);
  }

};
void clsForm1Context::cbOpenPrf(){
  Fl_Native_File_Chooser native;

  native.title("Select a file");

  native.type(Fl_Native_File_Chooser::BROWSE_FILE);
  native.filter(fmt::format("EIS text import schema\t*{}\n",EXTtxsc).c_str());
   switch ( native.show() ) {
    case -1:  fl_message("EIS text import schema open error\n, %s.", native.errmsg());  return;	// ERROR
    case  1:  fl_message("EIS text import schema load canceled\n ."); return;		// CANCEL
    default: 								// PICKED FILE
      if ( !native.filename() ) {
         fl_message("No EIS text import schema selected.");
          return;
      };
   };
  Config cfg;
  std::string rts="OK";
   FILE * wf=nullptr;
   try
  {
   wf= fl_fopen(native.filename(),"r");
   cfg.read(wf);
  }
  catch(const FileIOException &fioex)
  {
    fclose(wf);
    rts= fmt::format("I/O error while reading file: {}", native.filename());
    }
    catch(const ParseException &pex)
  {
     fclose(wf);
    rts=fmt::format("Parse error at {} : {} - {}", pex.getFile(), pex.getLine(), pex.getError());

    //return rts ;
  }
    fclose(wf);
  if(rts!="OK"){
   fl_message("EIS data file read error:\n%s .", rts.c_str());
   return;
  }
  const Setting& root = cfg.getRoot();
  std::string fid=root.lookup("FID");

  if(fid!=FIDtxsc){

    fl_message("EIS text import schema parse error:\n%s .", rts.c_str());
    return;
    };
  GetWg<Fl_Value_Output>("vout_frm1_Meta1stLn",ERR_PLACE)->value((int)root.lookup("MetaStartLine"));
  GetWg<Fl_Value_Output>("vout_frm1_MetaLastLn",ERR_PLACE)->value((int)root.lookup("MetaEndLine"));

  GetWg<Fl_Value_Output>("vout_frm1_hdr",ERR_PLACE)->value((int)root.lookup("DataHeader"));
  GetWg<Fl_Value_Output>("vout_frm1_1stLn",ERR_PLACE)->value((int)root.lookup("DataFirstLine"));
  GetWg<Fl_Value_Output>("vout_frm1_LastLn",ERR_PLACE)->value((int)root.lookup("DataLastLine"));
  GetWg<Fl_Choice>("ch_frm1_colsep",ERR_PLACE)->value((int)root.lookup("SP_Columns"));
  GetWg<Fl_Choice>("ch_frm1_decsep",ERR_PLACE)->value((int)root.lookup("SP_Decimal"));
  GetWg<Fl_Check_Button>("chbtn_ShowTabs",ERR_PLACE)->value((int)root.lookup("SP_ShowTabs"));
  GetWg<Fl_Value_Output>("vout_FrqIdx",ERR_PLACE)->value((int)root.lookup("DataFCol"));
  GetWg<Fl_Value_Output>("vout_ReIdx",ERR_PLACE)->value((int)root.lookup("DataZ1Col"));
  GetWg<Fl_Value_Output>("vout_ImIdx",ERR_PLACE)->value((int)root.lookup("DataZ2Col"));

  GetWg<Fl_Choice>("ch_Frq_mode",ERR_PLACE)->value((int)root.lookup("DataFType"));
  GetWg<Fl_Choice>("ch_ReIm_mode",ERR_PLACE)->value((int)root.lookup("DataZType"));
  GetWg<Fl_Choice>("ch_Z_mode",ERR_PLACE)->value((int)root.lookup("DataZScale"));
  GetWg<Fl_Choice>("ch_fi_mode",ERR_PLACE)->value((int)root.lookup("DataFiMode"));



};
