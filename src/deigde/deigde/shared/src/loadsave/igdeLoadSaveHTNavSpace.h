/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDELOADSAVEHTNAVSPACE_H_
#define _IGDELOADSAVEHTNAVSPACE_H_

#include <dragengine/common/string/decString.h>

class igdeEnvironment;

class deHeightTerrainNavSpace;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load and save height terrain navigation space file.
 */
class igdeLoadSaveHTNavSpace{
private:
	igdeEnvironment &pEnvironment;
	decString pLoggingSource;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load save object. */
	igdeLoadSaveHTNavSpace( igdeEnvironment &environment, const char *loggingSource );
	
	/** \brief Clean up load save object. */
	virtual ~igdeLoadSaveHTNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in file dialog. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name to display in file dialog. */
	void SetName( const char *name );
	
	/** \brief File pattern to display in file dialog. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern to display in file dialog. */
	void SetPattern( const char *pattern );
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set default file extension. */
	void SetDefaultExtension( const char *extension );
	
	
	
	/** \brief Load height terrain navigation space. */
	virtual void Load( deHeightTerrainNavSpace &navspace, decBaseFileReader &reader );
	
	/** \brief Save height terrain navigation space. */
	virtual void Save( const deHeightTerrainNavSpace &navspace, decBaseFileWriter &writer );
	/*@}*/
};

#endif
