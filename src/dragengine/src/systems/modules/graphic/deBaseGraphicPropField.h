/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEBASEGRAPHICPROPFIELD_H_
#define _DEBASEGRAPHICPROPFIELD_H_


class dePropField;
class dePropFieldType;
class deDecal;


/**
 * \brief Graphic Module Prop Field Peer
 *
 * Peer for the prop field resource used by graphic modules.
 */
class deBaseGraphicPropField{
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
