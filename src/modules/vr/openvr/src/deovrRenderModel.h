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

#ifndef _DEOVRRENDERMODEL_H_
#define _DEOVRRENDERMODEL_H_

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/resources/model/deModel.h>

class deVROpenVR;


/**
 * VR render model.
 */
class deovrRenderModel : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrRenderModel> Ref;
	
	
private:
	deVROpenVR &pOvr;
	
	decString pName;
	deModel::Ref pModel;
	vr::TextureID_t pTextureID;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Load render model. */
	deovrRenderModel(deVROpenVR &ovr, const char *name);
	
protected:
	/** Clean up render model. */
	virtual ~deovrRenderModel();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** OpenVR ovr. */
	inline deVROpenVR &GetOvr() const{ return pOvr; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Model. */
	inline deModel *GetModel() const{ return pModel; }
	
	/** Texture ID. */
	inline vr::TextureID_t GetTextureID() const{ return pTextureID; }
	/*@}*/
	
	
	
private:
	void pLoadModel();
};

#endif
