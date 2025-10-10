#ifndef CLSAPPCONTEX_H
#define CLSAPPCONTEX_H
#include <vector>
#include <array>
#include <map>
#include <Fl/Fl_Table_Row.h>
#include <FL/user/clsGenContext.h>
#include <FL/user/fl_small_stuff.h>
#include "clsMetaDataTable/clsMetaDataTable.h"
#include "main.h"



class clsAppContext:public clsGenContext
{
    public:
      std::vector<std::string>  rawFile;
      std::vector<std::array<double,3> >  fReIm;
      double vArea;
      std::vector<std::pair<std::string, std::string>> MetaData;
      void init_windows();
      clsAppContext();
      virtual ~clsAppContext();
      void Win_activate(const std::vector<std::array<double,3> > &data);
   //cb
     void cbSaveCnf();
     void cbImport();
     void cbSelectArea(const int swc);
     void cbClose();
    protected:




    private:
     std::vector<double>  areas;
     const std::string FIDsls="EIS Data v.9.5";
};

#endif // CLSAPPCONTEX_H
