 #include <FL/Fl_Native_File_Chooser.H>
 #include <FL/fl_ask.H>
#include "clsAppContext.h"


void clsAppContext::cbClose(){

 Win()->hide();
};
//----------------------------------------
void clsAppContext::cbSelectArea(const int swc){
 auto inp=GetWg<Fl_Value_Input>("inpv_Area");
 GetWg<Fl_Choice>("ch_Area")->value(swc);
 inp->value(areas[swc]);
 if(swc){
   inp->deactivate();

 } else {inp->activate();

 }

};


//----  --------------------------------------------------------------------------------------------------------
void clsAppContext::cbImport(){


  auto & fnount=GetWg<Fl_ButtonOutput>("btout_FOpen")->output();
{
  Fl_Native_File_Chooser native;
  native.title("Open text file");
  native.type(Fl_Native_File_Chooser::BROWSE_FILE);
  native.filter("Any text file\t*.*\nBioLogic spectra\t*.mpt\n");
  switch ( native.show() ) {
    case -1:  fl_alert("Text file open error\n, %s.", native.errmsg());
              fnount.value("");
              return ;	// ERROR
    case  1:  fl_message("Text file open canceled\n .");
              fnount.value("");
              return ;		// CANCEL
    default: 								// PICKED FILE
      if ( native.filename() ) {
        fnount.value(native.filename());

      }
      break;
  }
}

  rawFile.clear();

  FILE * sfile=fl_fopen(fnount.value(),"r");



   if (sfile == NULL){
            fl_alert("File \"%s\" open error.", fnount.value());
            fnount.value("");
            return ;
   };


     char bufff [1001];
     while( fgets (bufff , 1000 , sfile) != nullptr ){
        rawFile.push_back(bufff);
     }
   fclose (sfile);
   if(rawFile.size()<2){
     fl_alert("File \"%s\" is empty or small.", fnount.value());
     fnount.value("");
     return ;

   }
   MetaData.clear();
   const std::string fn=fl_filename_name(fnount.value());
   std::string ffn=fnount.value();
   auto pte = ffn.find(fn);
   ffn.erase(pte, fn.size());
   MetaData.push_back({"Source file", fn});
   MetaData.push_back({"Path", ffn});
   Win()->deactivate();
   Fl_Double_Window* tdw=GetWin("Form1");
   auto new_cntxt=reinterpret_cast<clsGenContext *>(tdw->user_data());
   new_cntxt->ParentWin(Win());
   new_cntxt->Win()->show();

   return ;
}
