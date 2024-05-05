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

#ifndef _DESKINPROPERTYVISITOR_H_
#define _DESKINPROPERTYVISITOR_H_

#include "../../../dragengine_export.h"

class deSkinProperty;
class deSkinPropertyValue;
class deSkinPropertyColor;
class deSkinPropertyImage;
class deSkinPropertyVideo;
class deSkinPropertyConstructed;
class deSkinPropertyMapped;


/**
 * \brief Visitor for skin properties.
 */
class DE_DLL_EXPORT deSkinPropertyVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new visitor. */
	deSkinPropertyVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~deSkinPropertyVisitor();
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
