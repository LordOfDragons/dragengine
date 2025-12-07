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

// include only once
#ifndef _MEHTVVARIATION_H_
#define _MEHTVVARIATION_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>
#include <dragengine/src/resources/model/deModel.h>
#include <dragengine/src/resources/skin/deSkin.h>

// predefinitions
class meHTVegetationLayer;

class deEngine;



/**
 * @brief Height Terrain Vegetation Variation.
 *
 * Defines a variation in a layer of vegetation on a height terrain. A
 * vegetation layer variation composes of a model, skin and optional a
 * rig defining the plant to render.
 */
class meHTVVariation : public deObject{
private:
	meHTVegetationLayer *pVLayer;
	
	deEngine *pEngine;
	
	decString pPathModel;
	deModel::Ref pModel;
	decString pPathSkin;
	deSkin::Ref pSkin;
	float pRotationPerForce;
	float pRestitution;
	
	float pRandomScaleMin;
	float pRandomScaleMax;
	float pRandomRotationMin;
	float pRandomRotationMax;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVVariation> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meHTVVariation(deEngine *engine);
	/** Cleans up the object. */
	virtual ~meHTVVariation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent height terrain vegetation layer or NULL. */
	inline meHTVegetationLayer *GetVLayer() const{ return pVLayer; }
	/** Sets the parent height terrain vegetation layer or NULL. */
	void SetVLayer(meHTVegetationLayer *vlayer);
	
	/** Retrieves the model path. */
	inline const decString &GetPathModel() const{ return pPathModel; }
	/** Sets the model path. */
	void SetPathModel(const char *path);
	/** Retrieves the engine model or NULL if not valid. */
	inline const deModel::Ref &GetModel() const{ return pModel; }
	/** Retrieves the skin path. */
	inline const decString &GetPathSkin() const{ return pPathSkin; }
	/** Sets the skin path. */
	void SetPathSkin(const char *path);
	/** Retrieves the engine skin or NULL if not valid. */
	inline const deSkin::Ref &GetSkin() const{ return pSkin; }
	/** Retrieves the rotation per force. */
	inline float GetRotationPerForce() const{ return pRotationPerForce; }
	/** Sets the rotation per force. */
	void SetRotationPerForce(float rotationPerForce);
	/** Retrieves the restitution. */
	inline float GetRestitution() const{ return pRestitution; }
	/** Sets the restitution. */
	void SetRestitution(float resitution);
	
	/** Retrieves the minimum random scaling. */
	inline float GetMinimumRandomScaling() const{ return pRandomScaleMin; }
	/** Sets the minimum random scaling. */
	void SetMinimumRandomScaling(float scaling);
	/** Retrieves the maximum random scaling. */
	inline float GetMaximumRandomScaling() const{ return pRandomScaleMax; }
	/** Sets the maximum random scaling. */
	void SetMaximumRandomScaling(float scaling);
	/** Retrieves the minimum random rotation. */
	inline float GetMinimumRandomRotation() const{ return pRandomRotationMin; }
	/** Sets the minimum random rotation. */
	void SetMinimumRandomRotation(float rotation);
	/** Retrieves the maximum random rotation. */
	inline float GetMaximumRandomRotation() const{ return pRandomRotationMax; }
	/** Sets the maximum random rotation. */
	void SetMaximumRandomRotation(float rotation);
	
	/** Notifies that this variation changed. */
	void NotifyChanged();
	/*@}*/
};

// end of include only once
#endif
