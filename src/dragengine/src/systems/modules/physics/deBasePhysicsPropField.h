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

#ifndef _DEBASEPHYSICSPROPFIELD_H_
#define _DEBASEPHYSICSPROPFIELD_H_

#include "../../../common/math/decMath.h"

class dePropFieldType;
class dePropFieldGround;


/**
 * \brief Physics System Prop Field Peer.
 *
 * Peer for the prop field resource used by the physics system.
 */
class DE_DLL_EXPORT deBasePhysicsPropField{
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsPropField();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Type has been added. */
	virtual void TypeAdded( int index, dePropFieldType *type );
	
	/** \brief Type has been removed. */
	virtual void TypeRemoved( int index, dePropFieldType *type );
	
	/** \brief All types have been removed. */
	virtual void AllTypesRemoved();
	
	/** \brief Type changed. */
	virtual void TypeChanged( int index, dePropFieldType *type );
	
	/** \brief Instances changed. */
	virtual void InstancesChanged( int index, dePropFieldType *type );
	
	/** \brief Project all instances to the given ground. */
	virtual void ProjectInstances( const dePropFieldGround &ground, const decVector &direction );
	/*@}*/
};

#endif
