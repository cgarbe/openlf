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
#include <vigra/multi_convolution.hxx>
#include <vigra/convolution.hxx>
#include "openlf/operator_macro.hpp"

#include "opencv2/imgproc/imgproc.hpp"

#define OPENLF_OP_CONSTRUCT_PARAMS \
    AddParameter_("x_blur", DspParameter(DspParameter::ParamType::Float, 0.0f)); \
    AddParameter_("y_blur", DspParameter(DspParameter::ParamType::Float, 0.0f));
    
OPENLF_VIGRA_OP_START(OP_Gauss, 1, 1, 3, 3)

  float sx = *op->GetParameter(0)->GetFloat();
  float sy = *op->GetParameter(1)->GetFloat();
        
    for (int i=0; i < in[0].shape()[2]; ++i){
        vigra::MultiArrayView<2, T> channel_in = in[0].bindAt(2, i);
        vigra::MultiArrayView<2, T> channel_out = out[0].bindAt(2, i);
        
        //gaussianSmoothing(channel_in, channel_out, *op->GetParameter(0)->GetFloat(), *op->GetParameter(1)->GetFloat());
        
        //Sorry vigra but OpenCv is 6x faster!
        cv::Mat cv_in(cv::Size(channel_in.shape(0), channel_in.shape(1)), BaseType2CvDepth(in_mat[0]->type()), channel_in.data());
        cv::Mat cv_out(cv::Size(channel_out.shape(0), channel_out.shape(1)), BaseType2CvDepth(out_mat[0]->type()), channel_out.data());
        int kx = int(sx*3+0.99)*2+1;
        //FIXME hack, add ignore border flag?
        int ky = std::min(int(sy*3+0.99)*2+1, (int(channel_in.shape(1))-4)/2*2+1);
        cv::GaussianBlur(cv_in, cv_out, cv::Size(kx, ky), sx, sy);
    }

OPENLF_OP_END

#undef OPENLF_OP_CONSTRUCT_PARAMS
