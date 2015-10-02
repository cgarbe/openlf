/*
* Copyright (c) 2014 Sven Wanner
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
*/

#ifndef IMGPROC_HPP
#define	IMGPROC_HPP

#include "OpenLF/debug.hpp"
#include "OpenLF/global.hpp"
#include "OpenLF/image/io.hpp"
#include "OpenLF/image/Channel.hpp"

typedef std::map<std::string,OpenLF::image::ImageChannel> channel_map;
typedef std::map<std::string,OpenLF::image::ImageChannel>::iterator channel_map_iter;

namespace OpenLF { 
    namespace image { 
        namespace imgproc {
            
    

//! computes the difference btw channel images
/*!
 Computes the difference between channel images pixelwise for each channel and 
 sums up the resulting difference channels.
 \param channels1 input channel image 1
 \param channels2 input channel image 2
 \param result image reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
//void difference(map<string,vigra::MultiArray<2,float>> &channels1,
//                map<string,vigra::MultiArray<2,float>> &channels2,
//                vigra::MultiArray<2,float> &result);


//! computes the difference btw channel images
/*!
 Computes the difference between ImageChannel instances pixelwise for each 
 channel and sums up the resulting difference channels.
 \param channels1 input channel image 1
 \param channels2 input channel image 2
 \param result image reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
void difference(std::map<std::string,ImageChannel> &channels1,
                std::map<std::string,ImageChannel> &channels2,
                ImageChannel &result);
            
          
//! computes the difference btw images pixelwise img1-img2
/*!
 \param img1 input image 1
 \param img2 input image 2
 \param result image reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
//void difference(vigra::MultiArray<2,float> &img1, 
//                vigra::MultiArray<2,float> &img2,
//                vigra::MultiArray<2,float> &result);



//! computes the difference btw ImageChannels pixelwise img1-img2
/*!
 \param img1 input ImageChannel 1
 \param img2 input ImageChannel 2
 \param result ImageChannel reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
void difference(ImageChannel &img1, 
                ImageChannel &img2,
                ImageChannel &result);



//! computes the absolute difference btw  channel images pixelwise |img1-img2|
/*!
 \param img1 input channel image 1
 \param img2 input channel image 2
 \param result image reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
//void abs_difference(map<string,vigra::MultiArray<2,float>> &channels1,
//                    map<string,vigra::MultiArray<2,float>> &channels2,
//                    vigra::MultiArray<2,float> &result);


//! computes the absolute difference btw  ImageChannel |img1-img2|
/*!
 \param img1 input ImageChannel image 1
 \param img2 input ImageChannel image 2
 \param result ImageChannel reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
void abs_difference(std::map<std::string,ImageChannel> &channels1,
                    std::map<std::string,ImageChannel> &channels2,
                    ImageChannel &result);


//! computes the absolute difference btw images pixelwise |img1-img2|
/*!
 \param img1 input image 1
 \param img2 input image 2
 \param result image reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
//void abs_difference(vigra::MultiArray<2,float> &img1, 
//                vigra::MultiArray<2,float> &img2,
//                vigra::MultiArray<2,float> &result);


//! computes the absolute difference btw ImagesChannels pixelwise |img1-img2|
/*!
 \param img1 ImageChannel 1
 \param img2 ImageChannel 2
 \param result ImageChannel reference
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
void abs_difference(ImageChannel &img1, 
                ImageChannel &img2,
                ImageChannel &result);


//! computes the mean square error
/*!
 Computes the mean square error 1/N Sum(channels1(c,x,y)-channels2(c,x,y))^2
 for each channel separately and writes the results in the result vector. 
 \param img1 input channel image 1
 \param img2 input channel image 2
 \param results float vector storing the MSE for each channel
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
//void MSE(map<string,vigra::MultiArray<2,float>> &channels1,
//         map<string,vigra::MultiArray<2,float>> &channels2,
//         map<string,double> &results);


//! computes the mean square error
/*!
 Computes the mean square error 1/N Sum(channels1(c,x,y)-channels2(c,x,y))^2
 for each channel separately and writes the results in the result vector. 
 \param img1 input ImageChannel map 1
 \param img2 input ImageChannel map 2
 \param results float vector storing the MSE for each channel
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
void MSE(std::map<std::string,ImageChannel> &channels1,
         std::map<std::string,ImageChannel> &channels2,
         std::map<std::string,double> &results);


//! computes the mean square error 1/N Sum(img1(x,y)-img2(x,y))^2)
/*!
 \param img1 input image 1
 \param img2 input image 2
 \returns mse
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
//double MSE(vigra::MultiArray<2,float> &img1, 
//           vigra::MultiArray<2,float> &img2);


//! computes the mean square error 1/N Sum(img1(x,y)-img2(x,y))^2)
/*!
 \param img1 ImageChannel 1
 \param img2 ImageChannel 2
 \returns mse
 \author Sven Wanner (sven.wanner@iwr.uni-heidelberg.de)
*/ 
double MSE(ImageChannel &img1, 
           ImageChannel &img2);
            
}}}

#endif	/* IMGPROC_HPP */

