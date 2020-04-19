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

// include only once
#ifndef _RELSRIG_H_
#define _RELSRIG_H_



// includes
#include "reLSRig.h"
#include "dragengine/common/math/decMath.h"



// predefinitions
class reRig;
class deBaseRigModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * Loads and saves rigs in the Drag[en]gine Actor Rig XML format.
 */
class reLSRig{
private:
	deBaseRigModule *pModule;
	
	char *pName;
	char *pPattern;
	
public:
	// constructor, destructor
	reLSRig( deBaseRigModule *module );
	~reLSRig();
	
	// management
	inline const char *GetName() const{ return ( const char * )pName; }
	void SetName( const char *name );
	inline const char *GetPattern() const{ return ( const char * )pPattern; }
	void SetPattern( const char *pattern );
	
	// loading and saving
	void LoadRig( reRig *rig, decBaseFileReader *file );
	void SaveRig( reRig *rig, decBaseFileWriter *file );
	
private:
	void pCleanUp();
};

// end of include only once
#endif
