/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELGAMERUNPARAMS_H_
#define _DELGAMERUNPARAMS_H_

#include "patch/delPatchList.h"
#include "profile/delGameProfile.h"

#include <dragengine/common/string/decString.h>

class delGame;


/**
 * \brief Game Run Parameters.
 * 
 * Stores the parameters for running a game. Usually the parameters are taken
 * from the game itself but in some situations temporary parameters can be
 * used. In all cases this class provides the actual parameters with which a
 * game is run no matter where they come from.
 */
class DE_DLL_EXPORT delGameRunParams{
private:
	delGameProfile::Ref pGameProfile;
	decString pRunArguments;
	int pWidth;
	int pHeight;
	bool pFullScreen;
	delPatchList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create run parameters. */
	delGameRunParams();
	
	/** \brief Clean up run parameters. */
	~delGameRunParams();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game profile. */
	inline delGameProfile *GetGameProfile() const{ return pGameProfile; }
	
	/** \brief Set game profile. */
	void SetGameProfile( delGameProfile *profile );
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArguments; }
	
	/** \brief Set run arguments. */
	void SetRunArguments( const char *arguments );
	
	/** \brief Window width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set window width. */
	void SetWidth( int width );
	
	/** \brief Window height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set window height. */
	void SetHeight( int height );
	
	/** \brief Window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if window is full screen. */
	void SetFullScreen( bool fullScreen );
	
	/** \brief Patches to apply. */
	inline delPatchList &GetPatches(){ return pPatches; }
	inline const delPatchList &GetPatches() const{ return pPatches; }
	
	/** \brief Find patches to use. */
	bool FindPatches( const delGame &game, bool useLatestPatch,
		const decUuid &useCustomPatch, decString &error );
	/*@}*/
};

#endif
