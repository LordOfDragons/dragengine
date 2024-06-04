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

#ifndef _DEBASEGRAPHICPROPFIELD_H_
#define _DEBASEGRAPHICPROPFIELD_H_

#include "../../../dragengine_export.h"


class dePropField;
class dePropFieldType;
class deDecal;


/**
 * \brief Graphic Module Prop Field Peer
 *
 * Peer for the prop field resource used by graphic modules.
 */
class DE_DLL_EXPORT deBaseGraphicPropField{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicPropField();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicPropField();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Ground changed. */
	virtual void GroundChanged();
	
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
	
	/** \brief Instances to Bend States assignments changed. */
	virtual void AssignmentsChanged( int index, dePropFieldType *type );
	
	/** \brief Bend States changed. */
	virtual void BendStatesChanged( int index, dePropFieldType *type );
	/*@}*/
};

#endif
