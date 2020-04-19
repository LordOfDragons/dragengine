/* 
 * Drag[en]gine Skin Module
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

#ifndef _DESMWRITEPROPERTY_H_
#define _DESMWRITEPROPERTY_H_

#include <dragengine/resources/skin/property/deSkinPropertyVisitor.h>
#include <dragengine/common/math/decMath.h>

class decXmlWriter;



/**
 * \brief Write property visitor.
 */
class desmWriteProperty : public deSkinPropertyVisitor{
private:
	decXmlWriter &pWriter;
	const char * const pPropertyType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	desmWriteProperty( decXmlWriter &writer, const char *propertyType );
	
	/** \brief Clean up visitor. */
	virtual ~desmWriteProperty();
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
