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

#ifndef _DEOVRTEXTUREMAP_H_
#define _DEOVRTEXTUREMAP_H_

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/skin/deSkin.h>

class deVROpenVR;


/**
 * VR texture map.
 */
class deovrTextureMap : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrTextureMap> Ref;
	
	
private:
	deVROpenVR &pOvr;
	
	vr::TextureID_t pID;
	deImage::Ref pImageColor;
	deImage::Ref pImageSolidity;
	deSkin::Ref pSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Load texture map. */
	deovrTextureMap(deVROpenVR &ovr, vr::TextureID_t id);
	
protected:
	/** Clean up texture map. */
	virtual ~deovrTextureMap();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** OpenVR ovr. */
	inline deVROpenVR &GetOvr() const{ return pOvr; }
	
	/** ID. */
	inline vr::TextureID_t GetID() const{ return pID; }
	
	/** Skin. */
	inline const deSkin::Ref &GetSkin() const{ return pSkin; }
	/*@}*/
	
	
	
private:
	void pLoadTextureMap();
};

#endif
