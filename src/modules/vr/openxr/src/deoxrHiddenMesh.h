/* 
 * Drag[en]gine OpenXR VR Module
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

#ifndef _DEOXRHIDDENMESH_H_
#define _DEOXRHIDDENMESH_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/resources/model/deModel.h>

class deoxrSession;


/**
 * VR hidden mesh.
 */
class deoxrHiddenMesh : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoxrHiddenMesh> Ref;
	
	
	
private:
	deoxrSession &pSession;
	
	const XrViewConfigurationType pViewConfig;
	const uint32_t pViewIndex;
	
	XrFovf pFov;
	
	deModel::Ref pModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Load hidden mesh. */
	deoxrHiddenMesh( deoxrSession &session, XrViewConfigurationType viewConfig, uint32_t viewIndex );
	
protected:
	/** Clean up hidden mesh. */
	virtual ~deoxrHiddenMesh();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** View configuration. */
	inline XrViewConfigurationType GetViewConfig() const{ return pViewConfig; }
	
	/** View index. */
	inline uint32_t GetViewIndex() const{ return pViewIndex; }
	
	/** Fov. */
	inline XrFovf GetFov() const{ return pFov; }
	
	/** Set fov. */
	void SetFov( XrFovf fov );
	
	/** Model. */
	inline deModel *GetModel() const{ return pModel; }
	
	/** Update model. */
	void UpdateModel();
	/*@}*/
	
	
	
private:
	void pFetchData( XrVisibilityMaskKHR &mask ) const;
	void pProjectVertices( XrVisibilityMaskKHR &mask ) const;
	void pMapVerticesToWindow( XrVisibilityMaskKHR &mask ) const;
	void pFitVertices( XrVisibilityMaskKHR &mask ) const;
};

#endif
