/* 
 * FBX Model Module
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

#ifndef _FBXMODELMODULE_H_
#define _FBXMODELMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/model/deBaseModelModule.h>
#include <dragengine/threading/deMutex.h>

class fbxScene;
class fbxNode;
class fbxProperty;

class deCacheHelper;
class deModelLOD;
class decPointerList;


/**
 * \brief FBX Model Module.
 */
class fbxModelModule : public deBaseModelModule{
private:
	deMutex pCacheMutex;
	deCacheHelper *pCacheTCSorter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	fbxModelModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~fbxModelModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load model. */
	virtual void LoadModel( decBaseFileReader &reader, deModel &model );
	
	/** \brief Save model. */
	virtual void SaveModel( decBaseFileWriter &writer, const deModel &model );
	/*@}*/
	
	
	
private:
	void pLoadModel( deModel &model, const fbxScene &scene, const fbxNode &nodeGeometry );
	
	void pLoadModelTextures( deModel &model, const fbxScene &scene, const fbxNode &nodeModel );
	
	void pLoadModelTexture( deModel &model, const fbxScene &scene, const fbxNode &nodeMaterial );
	
	void pLoadModelLod( deModel &model, deModelLOD &lod, const fbxScene &scene,
		const fbxNode &nodeGeometry, const decMatrix &transformation );
	
	void pLoadModelVertices( deModel &model, deModelLOD &lod, const fbxScene &scene,
		const fbxNode &nodeGeometry, const decMatrix &transformation );
	
	void pLoadModelFaces( deModel &model, deModelLOD &lod, const fbxScene &scene,
		const fbxNode &nodeGeometry );
};

#endif
