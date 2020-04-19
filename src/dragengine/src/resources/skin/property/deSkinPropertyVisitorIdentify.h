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

#ifndef _DESKINPROPERTYVISITORIDENTIFY_H_
#define _DESKINPROPERTYVISITORIDENTIFY_H_

#include "deSkinPropertyVisitor.h"


/**
 * \brief Skin Property Identify Visitor.
 * Skin property visitor used to identify the type of a visited skin property. After
 * visiting a skin property the visitor can be asked what kind of skin property has
 * been visited. Provides also save casting. If the cast is not valid an exception
 * is raised.
 */
class deSkinPropertyVisitorIdentify : public deSkinPropertyVisitor{
public:
	/** \brief Property types. */
	enum ePropertyTypes{
		/** \brief Single value property. */
		eptValue,
		
		/** \brief Uniform color property. */
		eptColor,
		
		/** \brief Image property. */
		eptImage,
		
		/** \brief Video property. */
		eptVideo,
		
		/** \brief Constructed property. */
		eptConstructed,
		
		/** \brief Mapped. */
		eptMapped,
		
		/** \brief Unknown property. */
		eptUnknown = -1
	};
	
	
	
private:
	deSkinProperty *pProperty;
	ePropertyTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator property visitor identify object. */
	deSkinPropertyVisitorIdentify();
	
	/** \brief Clean up animator property visitor identify. */
	virtual ~deSkinPropertyVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Property type. */
	inline ePropertyTypes GetType() const{ return pType; }
	
	/** \brief Determines if this is an unknown property. */
	inline bool IsUnknown() const{ return pType == eptUnknown; }
	
	/** \brief Determines if this is a single value property. */
	inline bool IsValue() const{ return pType == eptValue; }
	
	/** \brief Determines if this is a color property. */
	inline bool IsColor() const{ return pType == eptColor; }
	
	/** \brief Determines if this is an image property. */
	inline bool IsImage() const{ return pType == eptImage; }
	
	/** \brief Determines if this is a video property. */
	inline bool IsVideo() const{ return pType == eptVideo; }
	
	/** \brief Determines if this is a constructed property. */
	inline bool IsConstructed() const{ return pType == eptConstructed; }
	
	/** \brief Determines if this is a mapped property. */
	inline bool IsMapped() const{ return pType == eptMapped; }
	
	/** \brief Cast to a single value property. */
	deSkinPropertyValue &CastToValue() const;
	
	/** \brief Cast to a uniform color property. */
	deSkinPropertyColor &CastToColor() const;
	
	/** \brief Cast to an image property. */
	deSkinPropertyImage &CastToImage() const;
	
	/** \brief Cast to a video property. */
	deSkinPropertyVideo &CastToVideo() const;
	
	/** \brief Cast to a constructed property. */
	deSkinPropertyConstructed &CastToConstructed() const;
	
	/** \brief Cast to a mapped property. */
	deSkinPropertyMapped &CastToMapped() const;
	
	/** \brief Resets the visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void VisitProperty( deSkinProperty &property );
	
	/** \brief Visit single value property. */
	virtual void VisitValue( deSkinPropertyValue &property );
	
	/** \brief Visit uniform color property. */
	virtual void VisitColor( deSkinPropertyColor &property );
	
	/** \brief Visit image property. */
	virtual void VisitImage( deSkinPropertyImage &property );
	
	/** \brief Visit video property. */
	virtual void VisitVideo( deSkinPropertyVideo &property );
	
	/** \brief Visit constructed property. */
	virtual void VisitConstructed( deSkinPropertyConstructed &property );
	
	/** \brief Visit mapped property. */
	virtual void VisitMapped( deSkinPropertyMapped &property );
	/*@}*/
};

#endif
