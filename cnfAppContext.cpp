// #include <FL/Fl_Native_File_Chooser.H>
 #include <FL/fl_ask.H>
#include  <FL/fl_utf8.h>
#include <config/libconfig.h++>
#include "clsAppContext.h"
using namespace libconfig;
void clsAppContext::cbSaveCnf(){
  double area=GetWg<Fl_Value_Input>("inpv_Area")->value();
 if(area<=0.0){
  fl_alert("Area must be greater than zero!");
  return;

 }
if(fReIm.size()<3){
  fl_alert("Empty or too small spectrum!");
  return;

}

  Fl_Native_File_Chooser native;

  native.title("Select file to save EIS");
  native.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
/*
  native.filter("EIS data file\t*.eisdf\n");
 // native.directory(cntxt->GetDataDir().c_str());
  //native.preset_file("asr.conf");
  char buf[2048];
  // Show native chooser
  switch ( native.show() ) {
    case -1:  fl_message("EIS data file select error\n, %s.", native.errmsg());  return;	// ERROR
    case  1:  fl_message("EIS data file select save canceled\n ."); return;		// CANCEL
    default: 								// PICKED FILE
      if ( native.filename() ) {

        strncpy(buf,native.filename(),sizeof(buf));
        fl_filename_setext(buf, sizeof(buf), ".eisdf");
        break;
         }
        fl_message("empty file name\n ."); return;		// CANCEL
        return;

  }
*/
auto buf=fl_check_access_nfc(native, ".eisdf", "EIS data file");
if(buf==nullptr) return;
  Config cfg;
 /*  cfg.setOptions( Config::OptionSemicolonSeparators
                 | Config::OptionColonAssignmentForGroups
                 | Config::OptionOpenBraceOnSeparateLine);*/
  cfg.setFloatPrecision(12);
  cfg.setOption(Config::OptionAllowScientificNotation, true);
  Setting &root = cfg.getRoot();
  root.add("FID", Setting::TypeString)= FIDsls;

  root.add("Area", Setting::TypeFloat)= area;
  root.add("Descr", Setting::TypeString)= "F,Z.Re,-Z.Im";
  Setting &mlist =root.add("MetaData", Setting::TypeList);
//  auto tbl=GetWg<clsMetaDataTable>("tbl_Metadata");
//  auto & tdata=tbl->SLData;
  for(const auto & md:MetaData){
   Setting &rc1=mlist.add(Setting::TypeArray);
   rc1.add(Setting::TypeString) = md.first;
   rc1.add(Setting::TypeString) = md.second;
  }
  mlist.setLineCapacity(1);
 Setting &dlist =root.add("Data", Setting::TypeList);
 //Frq,reZ,imZ,Phi,modZ,wFrq;
 for(const auto & dt:fReIm){
  Setting &rc1=dlist.add(Setting::TypeArray);
  rc1.add(Setting::TypeFloat) = dt[0];
  rc1.add(Setting::TypeFloat) = dt[1];
  rc1.add(Setting::TypeFloat) = dt[2];

 }
  dlist.setLineCapacity(1);
  std::string rts="OK";
  FILE * wf=fl_fopen(buf,"w");
   try
  {
   cfg.write(wf);
   fclose(wf);
   // cfg.writeFile(fname);

  }
  catch(const FileIOException &fioex)
  {
    fclose(wf);
    rts="I/O error while writing file: %s";

  }
  if(rts!="OK"){
   fl_message(rts.c_str(),buf);
  }

  return;

};
