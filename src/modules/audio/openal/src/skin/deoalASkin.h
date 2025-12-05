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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalASkin> Ref;


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
