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

#ifndef _DECLACTIONDELGAHELPER_H_
#define _DECLACTIONDELGAHELPER_H_

#include "../game/declGameList.h"
#include "../game/patch/declPatchList.h"

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>

class declLauncher;

/**
 * \brief Action helper for handling delga files.
 */
class declActionDelgaHelper{
private:
	declLauncher &pLauncher;
	decString pFilename;
	declGameList pGames;
	declPatchList pPatches;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	declActionDelgaHelper( declLauncher &launcher, const decString &filename );
	
	/** \brief Clean up action. */
	~declActionDelgaHelper();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline declLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Games. */
	inline const declGameList &GetGames() const{ return pGames; }
	
	/** \brief Patches. */
	inline const declPatchList &GetPatches() const{ return pPatches; }
	
	/** \brief Load delga file. */
	void Load();
	
	/** \brief Unload delga file. */
	void Unload();
	
	/** \brief Has any content. */
	bool HasContent() const;
	
	/** \brief Install delga file. */
	void Install();
	
	/** \brief Uninstall delga file. */
	void Uninstall();
	/*@}*/
};

#endif
