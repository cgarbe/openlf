#include <vigra/imageinfo.hxx>
#include <vigra/impex.hxx>

#include "clif/clif.hpp"
#include "clif/clif_vigra.hpp"
#include "clif/flexmav.hpp"
#include "openlf.hpp"
#include "dspatch/DspCircuit.h"

using namespace clif;
using namespace vigra;

template<typename T> class save_flexmav {
public:
void operator()(FlexMAV<2> *img, const char *name)
{    
  exportImage(*img->get<T>(), ImageExportInfo(name));
}
};

#define DPPT DspParameter::ParamType


int main(const int argc, const char *argv[])
{
  assert(argc == 3);
  
  DspCircuit graph;
  
  DspComponent *in = OpenLF::getComponent("readCLIF");
  DspComponent *config = OpenLF::getComponent("readCLIF");
  DspComponent *out = OpenLF::getComponent("writeMesh");
  DspComponent *epi = OpenLF::getComponent("procEPI2D");
  
  //WKF_StructureTensor circuit;
  
  graph.AddComponent(in, "in");
  graph.AddComponent(config, "config");
  graph.AddComponent(out, "out");
  graph.AddComponent(epi, "epi");
  graph.ConnectOutToIn(in, 0, epi, 0);
  graph.ConnectOutToIn(config, 0, epi, 1);
  graph.ConnectOutToIn(epi, 0, out, 0);
  
  //epi.set(&circuit);
  
  //epi.set(&gauss);
  //epi->SetParameter(0, DspParameter(DPPT::Pointer, (DspCircuit*)&circuit));
  //epi->SetParameter(1, DspParameter(DPPT::String, "st"));
  
  in->SetParameter(0, DspParameter(DspParameter::ParamType::String, argv[1]));
  config->SetParameter(0, DspParameter(DspParameter::ParamType::String, "config.ini"));
  out->SetParameter(0, DspParameter(DspParameter::ParamType::String, argv[2]));
  
  graph.Tick();
  graph.Reset();
  
  return EXIT_SUCCESS;
}