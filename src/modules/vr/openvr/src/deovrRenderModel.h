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

#ifndef _DEOVRRENDERMODEL_H_
#define _DEOVRRENDERMODEL_H_

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/skin/deSkin.h>

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
	deovrRenderModel( deVROpenVR &ovr, const char *name );
	
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
