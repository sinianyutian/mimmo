/*---------------------------------------------------------------------------*\
 *
 *  mimmo
 *
 *  Copyright (C) 2015-2017 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of mimmo.
 *
 *  mimmo is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  mimmo is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mimmo. If not, see <http://www.gnu.org/licenses/>.
 *
\*---------------------------------------------------------------------------*/
#ifndef __MIMMOPIERCEDVECTOR_HPP__
#define __MIMMOPIERCEDVECTOR_HPP__

#include "MimmoObject.hpp"
#include <piercedVector.hpp>

namespace mimmo{

/*!
   \ingroup core
 * \brief Define data location for the MimmoPiercedVector field.
 */
enum class MPVLocation: long {
    UNDEFINED = 0, /**< unknown location data */
    POINT = 1    , /**< data on geometry vertices */
    CELL = 2     , /**< data on geometry cells */
    INTERFACE = 3  /**< data on geometry interfaces */
};

/*!
 * @class MimmoPiercedVector
   \ingroup core
 * @brief MimmoPiercedVector is the basic data container for mimmo library
 *
 * MimmoPiercedVector is the basic container for data attached to a geometry
 * defined as a MimmoObject.
 * It is based on bitpit::PiercedVector container.
 * It supports interface methods to recover the related geometric object.
 * It supports a string name attribute to mark the field as well as a location enum to
 * understand to which structures of geometry refers the data (UNDEFINED no-info, POINT-vertices,
 * CELL-cells, INTERFACE-interfaces).
 */
template<typename mpv_t>
class MimmoPiercedVector: public bitpit::PiercedVector<mpv_t, long int> {
private:
    MimmoObject*                             m_geometry;            /**<Pointer to geometry. */
    MPVLocation                              m_loc;                 /**< MPVLocation enum */
    bitpit::Logger*                          m_log;          /**<Pointer to logger.*/

public:
    MimmoPiercedVector(MimmoObject* geo = NULL, MPVLocation loc = MPVLocation::UNDEFINED);
    virtual ~MimmoPiercedVector();
    //copy constructors and operators
    MimmoPiercedVector(const MimmoPiercedVector<mpv_t> & other);
    MimmoPiercedVector & operator=(MimmoPiercedVector<mpv_t> other);
    MimmoPiercedVector & operator=(bitpit::PiercedVector<mpv_t, long int> other);

    void  clear();

    // get/set methods of the class;
    MimmoObject*            getGeometry() const;
    MPVLocation             getConstDataLocation() const;
    MPVLocation             getDataLocation();
    std::vector<mpv_t>    getDataAsVector(bool ordered=false);
    std::vector<mpv_t>    getInternalDataAsVector(bool ordered=false, bool squeeze = true);
    std::vector<mpv_t>    getRawDataAsVector(bool ordered=false);
    bool                    isEmpty();

    bool                   completeMissingData(const mpv_t & defValue);
    void                   initialize(MimmoObject *, MPVLocation, const mpv_t &);
    void                   setGeometry(MimmoObject* geo);
    void                   setDataLocation(MPVLocation loc);
    void                   setDataLocation(int loc);
    void                   setData(std::vector<mpv_t> &rawdata);

    bool checkDataSizeCoherence();
    bool checkDataIdsCoherence();
    MimmoPiercedVector<mpv_t> resizeToCoherentDataIds();
    bool intIsValidLocation(int &);

    void  swap(MimmoPiercedVector<mpv_t>& x) noexcept;

    MimmoPiercedVector pointDataToCellData(double p = 0.);
    MimmoPiercedVector cellDataToPointData(double p = 0.);
    MimmoPiercedVector cellDataToPointData(const MimmoPiercedVector<mpv_t> & cellGradientsX, const MimmoPiercedVector<mpv_t> & cellGradientsY, const MimmoPiercedVector<mpv_t> & cellGradientsZ, bool maximum = false);
    MimmoPiercedVector pointDataToBoundaryInterfaceData(double p = 0.);

    std::size_t getDataFrom(const MimmoPiercedVector<mpv_t> & other, bool strict = false);
    void squeezeOutExcept(const std::vector<long int> & list, bool keepOrder = false);

private:
    livector1D getGeometryIds(bool ordered=false);
};

};

#include "MimmoPiercedVector.tpp"

#endif /* __MIMMOPIERCEDVECTOR_HPP__ */
