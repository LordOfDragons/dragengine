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

#ifndef _IGDELOADSAVENAVSPACE_H_
#define _IGDELOADSAVENAVSPACE_H_

#include <dragengine/common/string/decString.h>

class deNavigationSpace;
class igdeEnvironment;
class igdeGameProject;

class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load and save navigation space file in the Drag[en]gine Navigation Space format.
 */
class igdeLoadSaveNavSpace{
private:
	igdeEnvironment *pEnvironment;
	decString pLoggingSource;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load save object. */
	igdeLoadSaveNavSpace( igdeEnvironment *environment, const char *loggingSource );
	
	/** \brief Clean up load save object. */
	virtual ~igdeLoadSaveNavSpace();
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
	
	/** \brief Set default file extension. */
	void SetDefaultExtension( const char *extension );
	
	/** \brief Load navigation space using a file reader. */
	virtual void Load( deNavigationSpace &navspace, decBaseFileReader &reader );
	
	/** \brief Save navigation space using a file writer. */
	virtual void Save( const deNavigationSpace &navspace, decBaseFileWriter &writer );
	/*@}*/
};

#endif
