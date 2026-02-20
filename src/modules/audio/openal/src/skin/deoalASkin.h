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

#include "../deoalBasics.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoalSkinTexture;
class deoalAudioThread;

class deSkin;



/**
 * Skin resource peer.
 */
class deoalASkin : public deObject{
private:
	deoalAudioThread &pAudioThread;
	decString pFilename;
	
	decTList<deoalSkinTexture> pTextures;
	
	bool pAffectsSound;
	
	
	
public:
	/** Type holding strong reference. */
	using Ref = deTObjectReference<deoalASkin>;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin peer. */
	deoalASkin(deoalAudioThread &audioThread, const deSkin &skin);
	
protected:
	/** Clean up skin peer. */
	~deoalASkin() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Filename of resource. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	
	
	/** Textures. */
	inline const decTList<deoalSkinTexture> &GetTextures() const{ return pTextures; }
	
	
	
	/** Material affects sound. */
	inline bool GetAffectsSound() const{ return pAffectsSound; }
	/*@}*/
	
	
	
private:
	void pCreateTextures(const deSkin &skin);
};

#endif
