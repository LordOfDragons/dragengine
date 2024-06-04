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
