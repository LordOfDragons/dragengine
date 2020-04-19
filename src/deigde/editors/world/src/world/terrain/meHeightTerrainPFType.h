/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHEIGHTTERRAINPFTYPE_H_
#define _MEHEIGHTTERRAINPFTYPE_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class meHeightTerrainPFLayer;

class deSkin;
class deModel;
class deEngine;



/**
 * \brief Height terrain prop field type.
 */
class meHeightTerrainPFType : public deObject{
private:
	meHeightTerrainPFLayer *pPFLayer;
	
	deEngine *pEngine;
	
	decString pPathModel;
	deModel *pModel;
	decString pPathSkin;
	deSkin *pSkin;
	float pRotationPerForce;
	float pRestitution;
	
	int pCoverageDensity;
	float pRandomScaleMin;
	float pRandomScaleMax;
	decVector pRandomRotationMin;
	decVector pRandomRotationMax;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meHeightTerrainPFType( deEngine *engine );
	
	/** \brief Clean up object. */
	virtual ~meHeightTerrainPFType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent prop field or \em NULL. */
	inline meHeightTerrainPFLayer *GetPFLayer() const{ return pPFLayer; }
	
	/** \brief Set parent prop field or \em NULL. */
	void SetPFLayer( meHeightTerrainPFLayer *propField );
	
	
	
	/** \brief Model path. */
	inline const decString &GetPathModel() const{ return pPathModel; }
	
	/** \brief Set model path. */
	void SetPathModel( const char *path );
	
	/** \brief Engine model or \em NULL if not valid. */
	inline deModel *GetModel() const{ return pModel; }
	
	/** \brief Skin path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	
	/** \brief Set skin path. */
	void SetPathSkin( const char *path );
	
	/** \brief Engine skin or \em NULL if not valid. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Rotation per force. */
	inline float GetRotationPerForce() const{ return pRotationPerForce; }
	
	/** \brief Set rotation per force. */
	void SetRotationPerForce( float rotationPerForce );
	
	/** \brief Restitution. */
	inline float GetRestitution() const{ return pRestitution; }
	
	/** \brief Set restitution. */
	void SetRestitution( float resitution );
	
	
	
	/** \brief Coverage density. */
	inline int GetCoverageDensity() const{ return pCoverageDensity; }
	
	/** \brief Set coverage density. */
	void SetCoverageDensity( int density );
	
	/** \brief Minimum random scaling. */
	inline float GetMinimumRandomScaling() const{ return pRandomScaleMin; }
	
	/** \brief Set minimum random scaling. */
	void SetMinimumRandomScaling( float scaling );
	
	/** \brief Maximum random scaling. */
	inline float GetMaximumRandomScaling() const{ return pRandomScaleMax; }
	
	/** \brief Set maximum random scaling. */
	void SetMaximumRandomScaling( float scaling );
	
	/** \brief Minimum random rotation. */
	inline const decVector &GetMinimumRandomRotation() const{ return pRandomRotationMin; }
	
	/** \brief Set minimum random rotation. */
	void SetMinimumRandomRotation( const decVector &rotation );
	
	/** \brief Maximum random rotation. */
	inline const decVector &GetMaximumRandomRotation() const{ return pRandomRotationMax; }
	
	/** \brief Set maximum random rotation. */
	void SetMaximumRandomRotation( const decVector &rotation );
	
	
	
	/** \brief Notify engine object that type changed. */
	void NotifyTypeChanged();
	/*@}*/
};

#endif
