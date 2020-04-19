/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALASKIN_H_
#define _DEOALASKIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoalSkinTexture;
class deoalAudioThread;

class deSkin;



/**
 * \brief Skin resource peer.
 */
class deoalASkin : public deObject{
private:
	deoalAudioThread &pAudioThread;
	decString pFilename;
	
	deoalSkinTexture *pTextures;
	int pTextureCount;
	
	bool pAffectsSound;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin peer. */
	deoalASkin( deoalAudioThread &audioThread, const deSkin &skin );
	
protected:
	/** \brief Clean up skin peer. */
	virtual ~deoalASkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Filename of resource. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	
	
	/** \brief Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture at index. */
	const deoalSkinTexture &GetTextureAt( int index ) const;
	
	
	
	/** \brief Material affects sound. */
	inline bool GetAffectsSound() const{ return pAffectsSound; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateTextures( const deSkin &skin );
};

#endif
