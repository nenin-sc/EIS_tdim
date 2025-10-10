 #include <FL/fl_ask.H>
 #include <FL/Fl_Menu_Item.h>
#include "clsForm1Context.h"

void cbmnu_brw_set_hdr(Fl_Widget * wg, void* userdata){
 auto brw = reinterpret_cast<Fl_Multi_Browser1 *>(wg);
 auto cntxt= reinterpret_cast<clsForm1Context*>(userdata);
 auto hr = brw->GetHDR();
 cntxt->GetWg<Fl_Value_Output>("vout_frm1_hdr")->value(hr);
}


//------------------------------------------
void cbmnu_brw_set_1stLn(Fl_Widget * wg, void* userdata){
 auto brw = reinterpret_cast<Fl_Multi_Browser1 *>(wg);
 auto cntxt= reinterpret_cast<clsForm1Context*>(userdata);
  int ln=brw->GetFirst();
 if(0==ln){
   fl_message("No mark for first line found\n .");
   return;
 }
 auto sp= cntxt->GetWg<Fl_Value_Output>("vout_frm1_1stLn",ERR_PLACE);
 sp->value(ln);
}


//------------------------------------------
void cbmnu_tb_set_LastLn(Fl_Widget * wg, void* userdata){
 auto brw = reinterpret_cast<Fl_Multi_Browser1 *>(wg);
 auto cntxt= reinterpret_cast<clsForm1Context*>(userdata);

  int ln=brw->GetLast();
 if(0==ln){
   fl_message("No mark for last line found\n .");
   return;
 }
 auto sp= cntxt->GetWg<Fl_Value_Output>("vout_frm1_LastLn",ERR_PLACE);
 sp->value(ln);
}

//-------------------------------------------------
void cbmnu_brw_set_M1stLn(Fl_Widget * wg, void* userdata){
 auto brw = reinterpret_cast<Fl_Multi_Browser1 *>(wg);
 auto cntxt= reinterpret_cast<clsForm1Context*>(userdata);
  int ln=brw->GetFirst();
 if(0==ln){
   fl_message("No mark for first meta data line found\n .");
   return;
 }
 auto sp= cntxt->GetWg<Fl_Value_Output>("vout_frm1_Meta1stLn",ERR_PLACE);
 sp->value(ln);
}


//------------------------------------------
void cbmnu_tb_set_MLastLn(Fl_Widget * wg, void* userdata){
 auto brw = reinterpret_cast<Fl_Multi_Browser1 *>(wg);
 auto cntxt= reinterpret_cast<clsForm1Context*>(userdata);

  int ln=brw->GetLast();
 if(0==ln){
   fl_message("No mark for last meta data line found\n .");
   return;
 }
 auto sp= cntxt->GetWg<Fl_Value_Output>("vout_frm1_MetaLastLn",ERR_PLACE);
 sp->value(ln);
}

//-------------------------------------------------

int Fl_Multi_Browser1::GetLast(){
  int i=0;
 for ( int t=1; t<=size(); ++t )
    if(selected(t)) i=t;
return i;
};

int Fl_Multi_Browser1::GetHDR(){
hposition(0);
int i=0;
for ( int t=1; t<=size(); t++ ) {
    if(selected(t)){
      i=t;
    break;
    };
}

return i;
};


int Fl_Multi_Browser1::GetFirst(){

for ( int t=1; t<=size(); ++t )
    if(selected(t)){
    return t;
    };

  return 0;
        };


Fl_Menu_Item row_sel_brw[] = {
  {"Header Line",0,cbmnu_brw_set_hdr},
  {"1st Line",0,cbmnu_brw_set_1stLn},
  {"Last Line",0,cbmnu_tb_set_LastLn},
  {"Start Metadata",0,cbmnu_brw_set_M1stLn},
  {"End Metadata",0,cbmnu_tb_set_MLastLn},
  {0}
};

int Fl_Multi_Browser1::handle(int e) {
 clsForm1Context *cntxt= (clsForm1Context*) user_data();
        int ret = Fl_Multi_Browser::handle(e);
        switch (e)    {
    case FL_PUSH: //Any mouse click
    if (Fl::event_button () == FL_RIGHT_MOUSE)
   {
     const Fl_Menu_Item *m = row_sel_brw->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
     if ( m ) m->do_callback(this, cntxt);
     }
   else
   {
      if (Fl::event_button () == FL_LEFT_MOUSE){


   }

   }
break;
}
        return(ret);
    }
