/*
* Copyright (c) 2015 Heidelberg Collaboratory for Image Processing
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Author Sven Wanner, Maximilian Diebold, Hendrik Siedelmann 
*
*/
#include <vigra/impex.hxx>
#include "clif/clif_vigra.hpp"
#include "comp_epi.hpp"
#include "operators.hpp"

#include "clif/subset3d.hpp"

#include "openlf/types.hpp"

#include <omp.h>

//for printprogress
#include <cstdarg>

using namespace clif;
using namespace vigra;
using namespace std;

#define DPPT DspParameter::ParamType

template<typename T> class save_flexmav3 {
public:
void operator()(FlexMAV<3> *img, const char *name)
{    
  MultiArrayView<3,T> *i = img->get<T>();
  MultiArrayView<2,T> channel = i->bindAt(2, 1);
  exportImage(channel, ImageExportInfo(name));
}
};

namespace openlf { namespace components {
  
namespace {
  enum class P_IDX : int {Epi_Circuit,Merge_Circuit,DispStart,DispStop,DispStep,StartLine,StopLine};
}

template<typename T> void COMP_Epi::openlf_add_param(const char *name, T val, DspParameter::ParamType type, int idx)
{
  int i;
  i = AddParameter_(name, DspParameter(type, val));
  assert(i == idx);
}

void COMP_Epi::openlf_add_param(const char *name, DspParameter::ParamType type, int idx)
{
  int i;
  i = AddParameter_(name, DspParameter(type));
  assert(i == idx);
}
  
COMP_Epi::COMP_Epi()
{
  AddInput_("input");
  AddInput_("config");
  AddOutput_("output");
  
  openlf_add_param("epi circuit", (DspCircuit*)&_default_epi_circuit, DPPT::Pointer, (int)P_IDX::Epi_Circuit);
  openlf_add_param("merge circuit", (DspCircuit*)&_default_merge_circuit, DPPT::Pointer, (int)P_IDX::Merge_Circuit);
  
  openlf_add_param("DispStart", DPPT::Float, (int)P_IDX::DispStart);
  openlf_add_param("DispStop", DPPT::Float, (int)P_IDX::DispStop);
  openlf_add_param("DispStep", DPPT::Float, (int)P_IDX::DispStep);
  
  openlf_add_param("StartLine", DPPT::Int, (int)P_IDX::StartLine);
  openlf_add_param("StopLine", DPPT::Int, (int)P_IDX::StopLine);
}

template<typename T> class save_flexmav {
public:
void operator()(FlexMAV<2> *img, const char *name)
{    
  exportImage(*img->get<T>(), ImageExportInfo(name));
}
};

template<typename T> class subarray_copy {
public:
void operator()(int line, int epi_w, int epi_h, FlexMAV<3> *sink_mav, FlexMAV<4> *disp_store, float disp_scale)
{
  for(int c=0;c<sink_mav->shape()[2];c++) {
    MultiArrayView<2,T> sink = sink_mav->get<T>()->bindAt(2, c);
    MultiArrayView<3,T> store = disp_store->get<T>()->bindAt(3, c);
    
    for(int i=0;i<epi_h;i++) {
      //bind store y to epi line
      MultiArrayView<2,T> epi = store.bindAt(1, line);
      epi = sink;
      if (disp_scale != 1.0)
        epi *= 1.0/disp_scale;
    }
  }
}
};

void component_apply_config_path(DspComponent *comp, LF *config, path config_path)
{
  if (config && config->data) {
    
    for(uint i=0;i<comp->GetParameterCount();i++) {
      Attribute *attr;
      const DspParameter *param = comp->GetParameter(i);
      path param_path = config_path / comp->GetParameterName(i);
      
      attr = config->data->getMatch(param_path);
      if (attr) {  
        //TODO abstract this (add call... to DspType??)
        switch (param->Type()) {
          case DPPT::Float : 
            float fval;
            attr->convert(&fval);
            comp->SetParameter(i, DspParameter(param->Type(), fval));
            printf("DEBUG: set param %s to %f\n", param_path.c_str(), fval);
            break;
          case DPPT::Int : 
            int ival;
            attr->convert(&ival);
            comp->SetParameter(i, DspParameter(param->Type(), ival));
            printf("DEBUG: set param %s to %d\n", param_path.c_str(), ival);
            break;
          default:
            printf("FIXME: unhandled parameter type! (comp_epi)\n");
        }
      }
      else
        printf("no match for %s\n", param_path.c_str());
    }
  }
}


void component_child_apply_config(DspComponent *comp, LF *config, std::string parent)
{
  path config_path;
  std::string name = comp->GetComponentName();
  
  if (!parent.size())
    parent = "default";
  
  if (!name.size())
    name = "default";
  
  component_apply_config_path(comp, config, path("openlf") / parent / name);
}

void component_apply_config(DspComponent *comp, LF *config)
{
  path config_path;
  std::string name = comp->GetComponentName();
  
  if (!name.size())
    name = "default";
  
  component_apply_config_path(comp, config, path("openlf") / name);
}

std::string GetComponentNameDefault(DspComponent *comp, std::string default_name)
{
  std::string name = comp->GetComponentName();
  
  if (!name.size())
    return default_name;
  
  return name;
}

template<typename T> void get_float_param(DspComponent *comp, T &val, int idx)
{
  const float *ptr;
  const DspParameter *p = comp->GetParameter(idx);

  if (!p) return;
  ptr = p->GetFloat();
  if (!ptr) return;
  val = *ptr;
  
  printf("got %d copm %p\n", val, comp);
}

template<typename T> void get_int_param(DspComponent *comp, T &val, int idx)
{
  const int *ptr;
  const DspParameter *p = comp->GetParameter(idx);

  if (!p) return;
  ptr = p->GetInt();
  if (!ptr) return;
  val = *ptr;
  
  printf("got %d copm %p\n", val, comp);
}

static void printprogress(int curr, int max, int &last, const char *fmt = NULL, ...)
{
  last = (last + 1) % 4;
  int pos = curr*60/max;
  char unf[] = "                                                             ]";
  char fin[] = "[============================================================]";
  char buf[100];
  
  char cs[] = "-\\|/";
  memcpy(buf, fin, pos+1);
  buf[pos+1] = cs[last];
  memcpy(buf+pos+2, unf+pos+2, 62-pos-2+1);
  if (!fmt) {
    printf("%s\r", buf);
  }
  else {
    printf("%s", buf);
    va_list arglist;
    va_start(arglist, fmt);
    vprintf(fmt, arglist);
    va_end(arglist);
    printf("\r");
  }
  fflush(NULL);
}

void COMP_Epi::Process_(DspSignalBus& inputs, DspSignalBus& outputs)
{
  LF *in = NULL;
  LF *config = NULL;
  LF *out = NULL;
  bool res;
  
  assert(inputs.GetValue(0, in));
  res = inputs.GetValue(0, in);
  assert(res);
  
  inputs.GetValue(1, config);
  
  out = new LF();
  out->data = new Dataset();
  out->data->memory_link(in->data);
  
  outputs.SetValue(0, out);
  
  assert(in);
  assert(out);
  
  int subset_idx = 0; //we could also loop over all subsets or specify subset using string name
  
  //subset_idx -- extrinsics path
  Subset3d subset(in->data, subset_idx);
  
  FlexMAV<4> *disp_store = NULL;
  
  int epi_w = subset.EPIWidth();
  int epi_h = subset.EPIHeight();
  
  float scale = 1.0;
  
  float disp_start = 3.0, disp_stop = 7.0, disp_step = 1.0;
  int start_line = 0, stop_line = subset.EPICount();
  
  //setup circuit and threading
  
  int t_count = omp_get_max_threads();

  const DspParameter *p;
  DspCircuit *epi_circuit;
  DspCircuit *merge_circuit;
  
  p = GetParameter((int)P_IDX::Epi_Circuit);
  assert(p);
  p->GetPointer(epi_circuit);
  
  p = GetParameter((int)P_IDX::Merge_Circuit);
  assert(p);
  p->GetPointer(merge_circuit);
  
  vector<FlexMAVSource<3>> comp_source(t_count);
  vector<FlexMAVSink<3>>   comp_sink(t_count);
  vector<clif::FlexMAV<3>> mav_source(t_count);
  
  component_child_apply_config(epi_circuit, config, GetComponentName());
  component_child_apply_config(merge_circuit, config, GetComponentName());
  component_apply_config(this, config);
  
  get_float_param(this, disp_start, (int)P_IDX::DispStart);
  get_float_param(this, disp_step, (int)P_IDX::DispStep);
  get_float_param(this, disp_stop, (int)P_IDX::DispStop);
  
  get_int_param(this, start_line, (int)P_IDX::StartLine);
  get_int_param(this, stop_line, (int)P_IDX::StopLine);
  
  printf("%f %f %f\n", disp_start, disp_step, disp_stop);
  
  //FIXME delete!
  vector<DspComponent*>  epi_circuits(t_count);
  vector<DspComponent*>  merge_circuits(t_count);
  vector<DspCircuit>   outer_circuit(t_count);
  for(int i=0;i<t_count;i++) {
    epi_circuits[i] = epi_circuit->clone();
    merge_circuits[i] = merge_circuit->clone();
    assert(epi_circuits[i]);
    assert(merge_circuits[i]);
    
    outer_circuit[i].AddComponent(comp_source[i], "source");
    outer_circuit[i].AddComponent(epi_circuits[i], "epi");  
    outer_circuit[i].AddComponent(comp_sink[i], "sink");
    //temp
    outer_circuit[i].AddComponent(merge_circuits[i], "merge");  
    
    outer_circuit[i].ConnectOutToIn(comp_source[i], 0, epi_circuits[i], 0);
    //temp
    //outer_circuit[i].ConnectOutToIn(epi_circuits[i], 0, comp_sink[i], 0);
    outer_circuit[i].ConnectOutToIn(epi_circuits[i], 0, merge_circuits[i], 0);
    outer_circuit[i].ConnectOutToIn(epi_circuits[i], 1, merge_circuits[i], 1);
    outer_circuit[i].ConnectOutToIn(merge_circuits[i], 0, comp_sink[i], 0);
    
    comp_source[i].set(&mav_source[i]);
  }
  
  FlexMAV<3> *sink_mav;
      
  int progress = 0;
  int done = 0;
  printf("\n");
  #pragma omp parallel for private(sink_mav)
  for(int i=start_line;i<stop_line;i++) {
#pragma omp critical 
    {
      if (done % 10 == 0)
        printprogress(done, stop_line-start_line, progress);
      done++;
    }
    
    merge_circuits[omp_get_thread_num()]->SetParameter(0, DspParameter(DspParameter::ParamType::Bool, true));
    
    for(float d=disp_start;d<=disp_stop;d+=disp_step) {
      epi_circuits[omp_get_thread_num()]->SetParameter(0, DspParameter(DspParameter::ParamType::Float, d));
      
      #pragma omp critical
      readEPI(&subset, mav_source[omp_get_thread_num()], i, d, ClifUnit::PIXELS, UNDISTORT, Interpolation::LINEAR, scale);
      
      //tick the circuit to fill _sink_mav using _source_mav and the circuit
      outer_circuit[omp_get_thread_num()].Tick();
      outer_circuit[omp_get_thread_num()].Reset();
      
      merge_circuits[omp_get_thread_num()]->SetParameter(0, DspParameter(DspParameter::ParamType::Bool, false));
    }
    
    sink_mav = comp_sink[omp_get_thread_num()].get();
    assert(sink_mav->type() == BaseType::FLOAT);
    
    //TODO this should not be necessary
    #pragma omp critical
    if (!disp_store)
      disp_store = new FlexMAV<4>(Shape4(subset.EPIWidth(), subset.EPICount(), subset.EPIHeight(), sink_mav->shape()[2]), sink_mav->type());
    
    disp_store->call<subarray_copy>(i,epi_w,epi_h,sink_mav,disp_store,scale);
    
  }
  
  out->path = std::string("disparity/default/data");
  Datastore *datastore = out->data->addStore("disparity/default/data");
  disp_store->write(datastore);
  
  delete disp_store;
}

bool COMP_Epi::ParameterUpdating_(int index, const DspParameter& param)
{  
  //just store parameter 
  SetParameter_(index, param);
  
  return true;      
}

}} //namespace openlf::components
