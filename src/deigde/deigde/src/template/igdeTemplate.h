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

#ifndef _IGDETEMPLATE_H_
#define _IGDETEMPLATE_H_

#include "igdeTemplateFileList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief Project template.
 */
class igdeTemplate : public deObject{
private:
	decString pBasePath;
	decString pName;
	decString pDescription;
	decString pScriptModule;
	decStringList pBaseGameDefinitions;
	igdeTemplateFileList pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create template. */
	igdeTemplate();
	
	/** \brief Create template. */
	igdeTemplate( const igdeTemplate &atemplate );
	
protected:
	/** \brief Clean up template. */
	virtual ~igdeTemplate();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Base path to directory containing template. */
	inline const decString &GetBasePath() const{ return pBasePath; }
	
	/** \brief Set base path to directory containing template. */
	void SetBasePath( const char *path );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Script module required for this template. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** \brief Set script module required for this template. */
	void SetScriptModule( const char *scriptModule );
	
	/** \brief Base game definition list. */
	inline decStringList &GetBaseGameDefinitions(){ return pBaseGameDefinitions; }
	inline const decStringList &GetBaseGameDefinitions() const{ return pBaseGameDefinitions; }
	
	/** \brief Files. */
	inline igdeTemplateFileList &GetFiles(){ return pFiles; }
	inline const igdeTemplateFileList &GetFiles() const{ return pFiles; }
	/*@}*/
};

#endif
