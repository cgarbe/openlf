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
* Author Sven Wanner, Maximilian Diebold, Hendrick Siedelmann 
*
*/
#include <vigra/multi_convolution.hxx>
#include <vigra/convolution.hxx>
#include "operators.hpp"


#define OPENLF_OP_CONSTRUCT_PARAMS \
    AddParameter_("x blur", DspParameter(DspParameter::ParamType::Float, 0.0f)); \
    AddParameter_("y blur", DspParameter(DspParameter::ParamType::Float, 0.0f)); \

OPENLF_OP_START(OP_VigraGauss, 1, 1, 3, 3)
        
    for (int i=0; i < in[0]->shape()[2]; ++i){
        vigra::MultiArrayView<2, T> channel_in = in[0]->bindAt(2, i);
        vigra::MultiArrayView<2, T> channel_out = out[0]->bindAt(2, i);
        gaussianSmoothing(channel_in, channel_out, *op->GetParameter(0)->GetFloat(), *op->GetParameter(1)->GetFloat());
    }

OPENLF_OP_END(OP_VigraGauss, 1, 1, 3, 3)