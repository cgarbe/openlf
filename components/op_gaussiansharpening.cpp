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
#include <vigra/convolution.hxx>
#include "openlf/operator_macro.hpp"

//char only ?
/*
#define OPENLF_OP_CONSTRUCT_PARAMS \
    AddParameter_("sharpening_factor", DspParameter(DspParameter::ParamType::Float, 0)); \
    AddParameter_("scale", DspParameter(DspParameter::ParamType::Float, 0));

OPENLF_VIGRA_OP_START(OP_GaussianSharpening, 1, 1, 3, 3)

  //TODO vigra requires double values, but DSPatch has only the parameter type Float.
  double sharpening_factor = (double)*op->GetParameter(0)->GetFloat();
  double scale = (double)*op->GetParameter(1)->GetFloat();

    for (int i=0; i < in[0].shape()[2]; ++i){
        vigra::MultiArrayView<2, T> channel_in = in[0].bindAt(2, i);
        vigra::MultiArrayView<2, T> channel_out = out[0].bindAt(2, i);
        vigra::gaussianSharpening(channel_in,channel_out, sharpening_factor, scale);
    }

OPENLF_OP_END

#undef OPENLF_OP_CONSTRUCT_PARAMS
*/
