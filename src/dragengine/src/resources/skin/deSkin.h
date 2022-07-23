/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKIN_H_
#define _DESKIN_H_

#include "../deFileResource.h"

class deSkinTexture;
class deSkinManager;
class deBaseGraphicSkin;
class deBaseAudioSkin;
class deBasePhysicsSkin;


/**
 * \brief Skin with textures.
 *
 * Skins contain the textures for scene components. A skin is a collection
 * of textures. Each texture is named and composes of properties. Components
 * link to textures using their name. Properties describe where the data for
 * various types originate from.
 *
 * \par Renderables
 * Renderables are the most powerfull building stones of a texture. Such a
 * renderable can be changed during runtime at any time. What data you
 * place inside such a renderable is up to the developer. To use renderables
 * you reference them using their name from inside your code. Renderables
 * can be shared which is why they are stored outside the data trees.
 */
class DE_DLL_EXPORT deSkin : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSkin> Ref;
	
	
	
private:
	deSkinTexture **pTextures;
	int pTextureCount, pTextureSize;
	
	deBaseGraphicSkin *pPeerGraphic;
	deBaseAudioSkin *pPeerAudio;
	deBasePhysicsSkin *pPeerPhysics;
	
public:
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new skin object with the given resource manager and filename. */
	deSkin( deSkinManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up skin.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSkin();
	/*@}*/
	
	
	
public:
	/** \name Texture Management */
	/*@{*/
	/** \brief Count of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Adds the given texture. */
	void AddTexture( deSkinTexture *tex );
	
	/** \brief Texture with the given index. */
	deSkinTexture *GetTextureAt( int index ) const;
	
	/** \brief Index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed( const char *name ) const;
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicSkin *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic( deBaseGraphicSkin *peer );
	
	/** \brief Audio system peer object. */
	inline deBaseAudioSkin *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer object. */
	void SetPeerAudio( deBaseAudioSkin *peer );
	
	/** \brief Physics system peer object. */
	inline deBasePhysicsSkin *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics( deBasePhysicsSkin *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
