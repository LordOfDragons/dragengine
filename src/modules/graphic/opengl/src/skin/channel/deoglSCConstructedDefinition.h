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

#ifndef _DEOGLSCCONSTRUCTEDDEFINITION_H_
#define _DEOGLSCCONSTRUCTEDDEFINITION_H_

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>
#include <dragengine/common/math/decMath.h>

class deSkinPropertyConstructed;
class decMemoryFileWriter;
class decMemoryFile;
class deEngine;


/**
 * Build constructed definition string.
 */
class deoglSCConstructedDefinition : public deSkinPropertyNodeVisitor{
private:
	const deEngine &pEngine;
	decMemoryFileWriter *pDefinition;
	decMemoryFileWriter *pVerify;
	bool pCacheValid;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglSCConstructedDefinition( const deEngine &engine, decMemoryFile *definition,
		decMemoryFile *verify, const deSkinPropertyConstructed &property );
	
	/** Clean up visitor. */
	virtual ~deoglSCConstructedDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Definitiojn is cache valid. */
	inline bool GetCacheValid() const{ return pCacheValid; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node );
	
	/** Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node );
	
	/** Visit shape node. */
	virtual void VisitShape( deSkinPropertyNodeShape &node );
	
	/** Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node );
	/*@}*/
	
	
	
private:
	void pWriteBasicProperties( const deSkinPropertyNode &node );
	void pWriteFloat( float value, float rounding );
	void pWriteColor( const decColor &color );
};

#endif

