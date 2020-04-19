/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLPATCH_H_
#define _DECLPATCH_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/common/utils/decUuidSet.h>



/**
 * \brief Game patch.
 */
class declPatch : public deObject{
private:
	decUuid pIdentifier;
	decUnicodeString pName;
	decUnicodeString pDescription;
	decUnicodeString pCreator;
	decString pHomepage;
	decString pPatchDirectory;
	decString pDataDirectory;
	
	decUuid pGameID;
	decUuidSet pRequiresPatches;
	
	decString pDelgaFile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch. */
	declPatch( );
	
protected:
	/** \brief Clean up game aptch. */
	virtual ~declPatch();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decUuid &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set identifier. */
	void SetIdentifier( const decUuid &identifier );
	
	/** \brief Display name. */
	inline const decUnicodeString &GetName() const{ return pName; }
	
	/** \brief Set display name. */
	void SetName( const decUnicodeString &name );
	
	/** \brief Description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const decUnicodeString &description );
	
	/** \brief Creator. */
	inline const decUnicodeString &GetCreator() const{ return pCreator; }
	
	/** \brief Set creator. */
	void SetCreator( const decUnicodeString &creator );
	
	/** \brief Homepage. */
	inline const decString &GetHomepage() const{ return pHomepage; }
	
	/** \brief Set homepage. */
	void SetHomepage( const char *homepage );
	
	/** \brief Patch directory. */
	inline const decString &GetPatchDirectory() const{ return pPatchDirectory; }
	
	/** \brief Set patch dir. */
	void SetPatchDirectory( const char *directory );
	
	/** \brief Data directory. */
	inline const decString &GetDataDirectory() const{ return pDataDirectory; }
	
	/** \brief Set data dir. */
	void SetDataDirectory( const char *directory );
	
	/** \brief Game identifier. */
	inline const decUuid &GetGameID() const{ return pGameID; }
	
	/** \brief Set game identifier. */
	void SetGameID( const decUuid &id );
	
	/** \brief Required patch identifiers. */
	inline decUuidSet &GetRequiredPatches(){ return pRequiresPatches; }
	inline const decUuidSet &GetRequiredPatches() const{ return pRequiresPatches; }
	
	/** \brief Delga file or \em NULL. */
	inline const decString &GetDelgaFile() const{ return pDelgaFile; }
	
	/** \brief Set delga file or \em NULL. */
	void SetDelgaFile( const char *file );
	/*@}*/
};

#endif
