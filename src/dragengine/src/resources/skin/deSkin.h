/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DESKIN_H_
#define _DESKIN_H_

#include "../deFileResource.h"
#include "../../common/collection/decObjectOrderedSet.h"

class deSkinTexture;
class deSkinManager;
class deSkinMapped;

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
	
	decObjectOrderedSet pMapped;
	
	deBaseGraphicSkin *pPeerGraphic;
	deBaseAudioSkin *pPeerAudio;
	deBasePhysicsSkin *pPeerPhysics;
	
public:
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new skin object with the given resource manager and filename. */
	deSkin(deSkinManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime);
	
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
	void AddTexture(deSkinTexture *tex);
	
	/** \brief Texture with the given index. */
	deSkinTexture *GetTextureAt(int index) const;
	
	/** \brief Index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed(const char *name) const;
	/*@}*/
	
	
	
	/** \name Mapped Values */
	/*@{*/
	/** \brief Count of mapped values. */
	int GetMappedCount() const;
	
	/** \brief Mapped value at index. */
	deSkinMapped *GetMappedAt(int index) const;
	
	/** \brief Named mapped value or nullptr. */
	deSkinMapped *GetMappedNamed(const char *name) const;
	
	/** \brief Index of mapped value or -1 if absent. */
	int IndexOfMapped(deSkinMapped *mapped) const;
	
	/** \brief Index of named mapped value or -1 if absent. */
	int IndexOfMappedNamed(const char *name) const;
	
	/** \brief Mapped value is present. */
	bool HasMapped(deSkinMapped *mapped) const;
	
	/** \brief Named mapped value is present. */
	bool HasMappedNamed(const char *name) const;
	
	/** \brief Add mapped value. */
	void AddMapped(deSkinMapped *mapped);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicSkin *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic(deBaseGraphicSkin *peer);
	
	/** \brief Audio system peer object. */
	inline deBaseAudioSkin *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer object. */
	void SetPeerAudio(deBaseAudioSkin *peer);
	
	/** \brief Physics system peer object. */
	inline deBasePhysicsSkin *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics(deBasePhysicsSkin *peer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
