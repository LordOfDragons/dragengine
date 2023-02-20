/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLMODEL_H_
#define _DEOGLMODEL_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicModel.h>

class deoglRModel;

class deGraphicOpenGl;
class deModel;



/**
 * Model peer.
 */
class deoglModel : public deBaseGraphicModel{
public:
	deGraphicOpenGl &pOgl;
	const deModel &pModel;
	deoglRModel *pRModel;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create model peer. */
	deoglModel( deGraphicOpenGl &ogl, const deModel &model );
	
	/** Cleans up the opengl model. */
	virtual ~deoglModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Model resource. */
	inline const deModel &GetModel() const{ return pModel; }
	
	/** Render model. */
	inline deoglRModel *GetRModel() const{ return pRModel; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
