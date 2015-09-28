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


#ifndef _OPENLF_OP_LOADIMAGE_H
#define _OPENLF_OP_LOADIMAGE_H

#include <iostream>
#include "vigra/impex.hxx"
#include <vigra/stdimage.hxx>
#include "vigra/imageinfo.hxx"
#include "vigra/multi_array.hxx"

#include "openlf.hpp"



namespace openlf { 
    namespace components {

    class OP_LoadImage : public DspComponent {
        public:       
            int pFilePath;  // FilePath
            const char* filePath = std::nullptr_t();
            OP_LoadImage();
        protected:
            virtual void Process_(DspSignalBus& inputs, DspSignalBus& outputs);
            virtual bool ParameterUpdating_(int index, DspParameter const& param);
    };

}} //namespace openlf::components

#endif