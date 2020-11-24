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

#ifndef _MEHELPERS_H_
#define _MEHELPERS_H_

#include <dragengine/common/math/decMath.h>

#include <deigde/gamedefinition/property/igdeGDProperty.h>


class meObject;
class igdeGDClass;
class igdeGDCComponent;
class igdeGDCLight;
class igdeGDCCTexture;
class igdeGDPropertyList;
class igdeGDCNavigationSpace;
class igdeGDCNavigationBlocker;

class deObjectReference;
class decString;
class decStringList;


namespace meHelpers{
	/** \brief Find first game definition component. */
	igdeGDCComponent *FindFirstComponent( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition component. */
	igdeGDCComponent *FindFirstComponent( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition component. */
	bool FindFirstComponent( const igdeGDClass &gdclass, igdeGDCComponent* &component );
	
	/** \brief Find first game definition component. */
	bool FindFirstComponent( const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component );
	
	
	
	/** \brief Find first game definition light. */
	igdeGDCLight *FindFirstLight( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition light. */
	igdeGDCLight *FindFirstLight( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition light. */
	bool FindFirstLight( const igdeGDClass &gdclass, igdeGDCLight* &light );
	
	/** \brief Find first game definition light. */
	bool FindFirstLight( const igdeGDClass &gdclass, decString &prefix, igdeGDCLight* &light );
	
	
	
	/** \brief Find first game definition navigationSpace. */
	igdeGDCNavigationSpace *FindFirstNavigationSpace( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition navigationSpace. */
	igdeGDCNavigationSpace *FindFirstNavigationSpace( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition navigationSpace. */
	bool FindFirstNavigationSpace( const igdeGDClass &gdclass, igdeGDCNavigationSpace* &navigationSpace );
	
	/** \brief Find first game definition navigationSpace. */
	bool FindFirstNavigationSpace( const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationSpace* &navigationSpace );
	
	
	
	/** \brief Find first game definition navigationBlocker. */
	igdeGDCNavigationBlocker *FindFirstNavigationBlocker( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition navigationBlocker. */
	igdeGDCNavigationBlocker *FindFirstNavigationBlocker( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition navigationBlocker. */
	bool FindFirstNavigationBlocker( const igdeGDClass &gdclass, igdeGDCNavigationBlocker* &navigationBlocker );
	
	/** \brief Find first game definition navigationBlocker. */
	bool FindFirstNavigationBlocker( const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationBlocker* &navigationBlocker );
	
	
	
	/** \brief Get names of all path type properties matching pattern type. */
	void GetPatternTypePropertyNames( const igdeGDClass &gdclass,
		igdeGDProperty::ePathPatternTypes patternType, decStringList &names );
	
	
	
	/** \brief Create texture. */
	void CreateTexture( deObjectReference &texture, meObject *object, const char *textureName );
	
	/** \brief Create texture. */
	void CreateTexture( deObjectReference &texture, meObject *object, const char *textureName,
		const igdeGDCCTexture * gdctexture );
};

#endif
