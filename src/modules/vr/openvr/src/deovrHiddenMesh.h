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

#ifndef _DEOVRHIDDENMESH_H_
#define _DEOVRHIDDENMESH_H_

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/resources/model/deModel.h>

class deVROpenVR;


/**
 * VR hidden mesh.
 */
class deovrHiddenMesh : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrHiddenMesh> Ref;
	
	
private:
	deVROpenVR &pOvr;
	
	const vr::Hmd_Eye pEye;
	deModel::Ref pModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Load hidden mesh. */
	deovrHiddenMesh(deVROpenVR &ovr, vr::Hmd_Eye eye);
	
protected:
	/** Clean up hidden mesh. */
	virtual ~deovrHiddenMesh();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** OpenVR ovr. */
	inline deVROpenVR &GetOvr() const{ return pOvr; }
	
	/** Eye. */
	inline vr::Hmd_Eye GetEye() const{ return pEye; }
	
	/** Model. */
	inline const deModel::Ref &GetModel() const{ return pModel; }
	/*@}*/
	
	
	
private:
	void pLoadHiddenMesh();
};

#endif
