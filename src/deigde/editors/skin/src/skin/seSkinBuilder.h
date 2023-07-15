/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SESKINBUILDER_H_
#define _SESKINBUILDER_H_

#include <dragengine/resources/skin/deSkinBuilder.h>

class seSkin;
class sePropertyNode;
class sePropertyNodeGroup;

class deSkinPropertyNode;
class deSkinPropertyNodeGroup;



/**
 * Skin builder.
 */
class seSkinBuilder : public deSkinBuilder{
private:
	const seSkin &pSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin builder. */
	seSkinBuilder( const seSkin &skin );
	
	/** Clean up skin builder. */
	virtual ~seSkinBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Build skin into the provided skin object. */
    virtual void BuildSkin( deSkin *engSkin );
	
	/** Create skin property node group. */
	void CreateNodeGroup( deSkinPropertyNodeGroup &engNodeGroup, const sePropertyNodeGroup &nodeGroup );
	
	/** Create skin property node. */
	deSkinPropertyNode *CreateNode( const sePropertyNode &node );
	/*@}*/
	
	
	
protected:
	void pAddMapped( deSkin &engSkin );
	void pAddTextures( deSkin &engSkin );
};

#endif
