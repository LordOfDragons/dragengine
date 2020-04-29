/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RELSRIG_H_
#define _RELSRIG_H_

#include "reLSRig.h"

#include <dragengine/common/string/decString.h>


class reRig;
class deBaseRigModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Loads and saves rigs in the Drag[en]gine Actor Rig XML format.
 */
class reLSRig{
private:
	deBaseRigModule *pModule;
	
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load-save. */
	reLSRig( deBaseRigModule *module );
	
	/** \brief Clean up load-save. */
	~reLSRig();
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
	/*@}*/
	
	
	
	/** \name Loading and Saving */
	/*@{*/
	/** \brief Load from file. */
	void LoadRig( reRig *rig, decBaseFileReader *file );
	
	/** \brief Write to file. */
	void SaveRig( reRig *rig, decBaseFileWriter *file );
	/*@}*/
};

#endif
