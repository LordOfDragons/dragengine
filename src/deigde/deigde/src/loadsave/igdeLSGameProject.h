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

#ifndef _IGDELSGAMEPROJECT_H_
#define _IGDELSGAMEPROJECT_H_

#include <dragengine/common/string/decString.h>

class igdeGameProject;
class igdeLoadSaveSystem;

class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load and save game project files in the Drag[en]gine Game Project format.
 */
class igdeLSGameProject{
private:
	igdeLoadSaveSystem *pLoadSaveSystem;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save game project. */
	igdeLSGameProject( igdeLoadSaveSystem *lssys );
	
	/** \brief Clean up load/save game project. */
	~igdeLSGameProject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in a file dialog. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name to display in a file dialog. */
	void SetName( const char *name );
	
	/** \brief File pattern to display in a file dialog. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern to display in a file dialog. */
	void SetPattern( const char *pattern );
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set he default file extension. */
	void SetDefaultExtension( const char *extension );
	
	
	
	/** \brief Load game project from a file reader. */
	void Load( const char *filename, igdeGameProject *project, decBaseFileReader *file );
	
	/** \brief Save game project from a file reader. */
	void Save( igdeGameProject *project, decBaseFileWriter *file );
	/*@}*/
};

#endif
