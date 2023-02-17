/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLVSRETAINIMAGEDATA_H_
#define _DEOGLVSRETAINIMAGEDATA_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitor.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>

class deSkin;
class deSkinProperty;
class deImage;



/**
 * Visitor retaining or releasing image data.
 */
class deoglVSRetainImageData : public deSkinPropertyVisitor, deSkinPropertyNodeVisitor {
public:
	decPointerList pImages;
	bool pForceLock;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglVSRetainImageData();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Retain images in property.
	 * \param[in] forceLock Force locking even if not necessary. Required to deal with
	 *                      combined images until they are removed
	 */
	void RetainPropertyImages( deSkinProperty &property, bool forceLock );
	
	/** Release all retained images. */
	void ReleaseImages();
	
	/** Process image. */
	void ProcessImage( deImage &image, bool forceLock );
	/*@}*/
	
	
	
	/** \name Visiting Property */
	/*@{*/
	/** Visit property. */
	virtual void VisitProperty( deSkinProperty &property );
	
	/** Visit single value property. */
	virtual void VisitValue( deSkinPropertyValue &property );
	
	/** Visit uniform color property. */
	virtual void VisitColor( deSkinPropertyColor &property );
	
	/** Visit image property. */
	virtual void VisitImage( deSkinPropertyImage &property );
	
	/** Visit video property. */
	virtual void VisitVideo( deSkinPropertyVideo &property );
	
	/** Visit complex property. */
	virtual void VisitConstructed( deSkinPropertyConstructed &property );
	/*@}*/
	
	
	
	/** \name Visiting Node */
	/*@{*/
	/** Visit node. */
	virtual void VisitNode( deSkinPropertyNode &node );
	
	/** Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node );
	
	/** Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node );
	
	/** Visit shape node. */
	virtual void VisitShape( deSkinPropertyNodeShape &node );
	
	/** Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node );
	/*@}*/
};

#endif
 
