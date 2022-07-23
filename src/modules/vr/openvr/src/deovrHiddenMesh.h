/* 
 * Drag[en]gine OpenVR VR Module
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
	deovrHiddenMesh( deVROpenVR &ovr, vr::Hmd_Eye eye );
	
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
	inline deModel *GetModel() const{ return pModel; }
	/*@}*/
	
	
	
private:
	void pLoadHiddenMesh();
};

#endif
