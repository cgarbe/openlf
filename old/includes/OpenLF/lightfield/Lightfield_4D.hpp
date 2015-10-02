/* 
 * File:   Lightfield4D.hpp
 * Author: kiryl
 *
 * Created on December 2, 2014, 11:53 AM
 */

#ifndef LIGHTFIELD4D_HPP
#define	LIGHTFIELD4D_HPP


#include "OpenLF/global.hpp"
#include "OpenLF/image/utils.hpp"
#include "OpenLF/lightfield/io.hpp"
#include "OpenLF/lightfield/Properties.hpp"
#include "OpenLF/lightfield/DataHandler.hpp"
#include "OpenLF/lightfield/FileHandler.hpp"
#include "OpenLF/lightfield/Lightfield.hpp"

namespace OpenLF { 
    namespace lightfield { 
 
class EpiIterator_4D;

class Lightfield_4D : public Lightfield{
public:
    friend class EpiIterator_4D;
    Lightfield_4D();
    Lightfield_4D(const std::string filename);
    //Lightfield_4D(const Lightfield_4D& orig);
    ~Lightfield_4D();
    
    //! access a single intensity value of the channel specified
    /*!
     \param v vertical camera index
     \param h horizontal camera index
     \param x position index
     \param y position index
     \param channel_name name of the channel to access
    */
    float getLoxel(int h, int v, int x, int y, const std::string channel_name);
 
    
    //! access a single image of a light field channel
    /*!
     \param h horizontal index
     \param v vertical index
     \param channel_name name of the channel
     \param img reference to MultiArrayView
    */
    void getImage(int h, int v, const std::string channel_name, vigra::MultiArrayView<2,float> &img);
    
    
public:
    
    //! get a view to a horizontal epi of the channel specified
    /*!
     \param channel_name name of the channel to extract the epi from
     \param y fixed row image domain index
     \param v vertical camera index (default=0)
     \param focus global shift parameter in pixel (default=0)
    */
    view_2D getHorizontalEpiChannel(std::string channel_name, int y, int v, int focus);
    
    view_2D getHorizontalEpiChannel(std::string channel_name, int y, int v);
  
    //! get a view to a vertical epi of the channel specified
    /*!
     \param channel_name name of the channel to extract the epi from
     \param x fixed column image domain index
     \param h fixed horizontal camera index (default=0)
     \param focus global shift paramter in pixel (default=0)
    */
    vigra::MultiArrayView<2,float> getVerticalEpiChannel(std::string channel_name, int x, int h, int focus);
    
    vigra::MultiArrayView<2,float> getVerticalEpiChannel(std::string channel_name, int x, int h);
};

class EpiIterator_4D {
    Lightfield *m_lf;
    DIRECTION m_direction;
    int m_camera_index;
    int m_epi_index;
    bool m_finished;
    
public: 
    EpiIterator_4D(Lightfield *lf, DIRECTION direction);
    //EpiIterator_4D(const EpiIterator_4D& orig);
    
    void first();
    void next();
    bool end();
    view_2D get(std::string channel_name, int focus);
    ~EpiIterator_4D();
};
    }
}



#endif	/* LIGHTFIELD4D_HPP */

