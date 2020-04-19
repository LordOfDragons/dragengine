/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MELSHEIGHTTERRAIN_H_
#define _MELSHEIGHTTERRAIN_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class meHeightTerrain;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;
class deLogger;



/**
 * \brief Load Height Terrain Interface.
 */
class meLSHeightTerrain : public igdeBaseXML{
private:
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	meLSHeightTerrain( deLogger *logger, const char *loggerSource );
	/** Cleans up the loader. */
	virtual ~meLSHeightTerrain();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const char *GetName() const{ return pName.GetString(); }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the pattern. */
	inline const char *GetPattern() const{ return pPattern.GetString(); }
	/** Sets the pattern. */
	void SetPattern( const char *pattern );
	
	/** Loads a height terrain. */
	virtual void LoadFromFile( meHeightTerrain &heightTerrain, decBaseFileReader &file ) = 0;
	/** Saves the height terrain. */
	virtual void SaveToFile( meHeightTerrain &heightTerrain, decBaseFileWriter &file ) = 0;
	/*@}*/
};

// end of include only once
#endif
