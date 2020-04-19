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

#ifndef _IGDETEMPLATEFILE_H_
#define _IGDETEMPLATEFILE_H_

#include "igdeTemplateReplaceList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief Project template file.
 */
class igdeTemplateFile : public deObject{
public:
	/** \brief Directories. */
	enum eDirectories{
		/** \brief Project data directory. */
		edData,
		
		/** \brief Project directory. */
		edProject
	};
	
	
	
private:
	decString pPath;
	decString pPattern;
	eDirectories pDirectory;
	igdeTemplateReplaceList pReplacements;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create template. */
	igdeTemplateFile();
	
	/** \brief Create template. */
	igdeTemplateFile( const igdeTemplateFile &file );
	
protected:
	/** \brief Clean up template. */
	virtual ~igdeTemplateFile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File path relative to template base path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set file path relative to template base path. */
	void SetPath( const char *path );
	
	/** \brief File pattern relative to template base path. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern relative to template base path. */
	void SetPattern( const char *pattern );
	
	/** \brief Driectory. */
	inline eDirectories GetDirectory() const{ return pDirectory; }
	
	/** \brief Set directory. */
	void SetDirectory( eDirectories directory );
	
	/** \brief Replacements. */
	inline igdeTemplateReplaceList &GetReplacements(){ return pReplacements; }
	inline const igdeTemplateReplaceList &GetReplacements() const{ return pReplacements; }
	/*@}*/
};

#endif
