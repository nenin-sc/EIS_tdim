#include "clsForm1Context.h"
using namespace FL_PlotXY3_13;
void clsForm1Context::PrepPlot(){
auto  pxy_test = GetWg<Fl_PlotXY3>("pltxy3_main");
    auto & crdOrigL=pxy_test->GetOriginBL();
    auto & crdOrigR=pxy_test->GetOriginBR();
    crdOrigR.Off();
    crdOrigL.On();
    pxy_LineStyle axsY2={FL_RED,FL_SOLID,2};
    pxy_LineStyle axsXY={FL_BLACK,FL_SOLID,2};
    pxy_LineStyle tgst={FL_GREEN,FL_DOT,0};

    pxy_Axis & xAx =crdOrigL.GetXAxis();
    xAx.SetScaleType(xAx.SCL_PLAIN).SetTiksN(11).TickGridOff().SetAxStyle(axsXY).SetAutoTicks(true).SetAutoScale(true);
    pxy_Axis & yAxL =crdOrigL.GetYAxis();
    yAxL.SetScaleType(yAxL.SCL_PLAIN).SetTiksN(7).TickGridOff().SetAxStyle(axsXY).SetAutoTicks(true).SetAutoScale(true).LGridOn();

    pxy_PlotStyle sty1, sty2, sty3;
    sty1.SetSym("XCross_3x3").SetSymColor(FL_BLACK).SetJoined(false);
    sty2.SetSym("Cross_3x3").SetSymColor(FL_BLUE).SetJoined(false);
    sty3.SetSym("Box_3x3").SetSymColor(FL_RED).SetJoined(false);

    auto lnxy_2 =   crdOrigL.NewPlot("lnNy",&sty2);
    auto & grd=yAxL.GetLGrid();
    grd.clear();
    pxy_GridLine gl(0.0);
    gl.On();
    grd.insert(std::make_pair("zeroY",gl));

};

void clsForm1Context::Plot(const std::vector<std::array<double,3> > & fzdata){

if(fzdata.size()<2) return;
  auto  pxy_test = GetWg<Fl_PlotXY3>("pltxy3_main");
  auto & crdOrigL=pxy_test->GetOriginBL();

  auto lnxy_1 =   crdOrigL.GetPlot("lnNy");

  lnxy_1->Clear();
  for(auto const &vr:fzdata){
    lnxy_1->AddXY(vr[1],vr[2]);

  }

  pxy_test->redraw();
}
