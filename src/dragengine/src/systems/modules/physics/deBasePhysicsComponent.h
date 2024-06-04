/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEBASEPHASICSCOMPONENT_H_
#define _DEBASEPHASICSCOMPONENT_H_

#include "../../../common/math/decMath.h"

class deDecal;
class deDecalList;
class decShape;


/**
 * \brief Physics module component peer.
 */
class DE_DLL_EXPORT deBasePhysicsComponent{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBasePhysicsComponent();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsComponent();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Model object changed. */
	virtual void ModelChanged();
	
	/** \brief Rig object changed. */
	virtual void RigChanged();
	
	/** \brief Visitility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Extends changed. */
	virtual void ExtendsChanged();
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
	
	/** \brief Parameter or hint changed. */
    virtual void ParametersChanged();
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * \brief Collect decals in contact with point.
	 * 
	 * The point is best taken from an earlier collision test and is in world
	 * coordinates. The point is considered to be in the same sector as the component.
	 * 
	 * \param[in] point Point on the surface to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	virtual void FindDecalsAt( const decVector &point, deDecalList &list );
	
	/**
	 * \brief Collect decals in contact with shape.
	 * 
	 * The shape is considered to be in the same sector as the component.
	 * 
	 * \param[in] shape Shape to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	virtual void FindDecalsTouching( decShape *shape, deDecalList &list );
	/*@}*/
};

#endif
