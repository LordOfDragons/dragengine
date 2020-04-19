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

#ifndef _SELOADSAVESKIN_H_
#define _SELOADSAVESKIN_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class seSkin;
class sePropertyNode;
class sePropertyNodeGroup;
class deBaseSkinModule;
class deSkinPropertyNode;
class deSkinPropertyNodeGroup;
class decBaseFileReader;
class decBaseFileWriter;
class igdeTexturePropertyList;


/**
 * \brief Load/Saves skins using an engine module.
 */
class seLoadSaveSkin{
private:
	deBaseSkinModule *pModule;
	
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new load/save object. */
	seLoadSaveSkin( deBaseSkinModule *module );
	
	/** \brief Clean up load/save object. */
	~seLoadSaveSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set pattern. */
	void SetPattern( const char *pattern );
	
	
	
	/** \brief Load skin. */
	void LoadSkin( seSkin *skin, decBaseFileReader *file, const igdeTexturePropertyList &knownPropertyList );
	
	/** \brief Load propery node group. */
	sePropertyNodeGroup *LoadPropertyNodeGroup( const deSkinPropertyNodeGroup &engNodeGroup );
	
	/** \brief Load propery node. */
	sePropertyNode *LoadPropertyNode( deSkinPropertyNode &engNode );
	
	/** \brief Load common propery node parameters. */
	void LoadPropertyNodeCommon( sePropertyNode &node, const deSkinPropertyNode &engNode );
	
	/** \brief Save skin. */
	void SaveSkin( seSkin *skin, decBaseFileWriter *file );
	/*@}*/
};

#endif
