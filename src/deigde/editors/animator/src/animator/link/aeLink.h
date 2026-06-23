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

#ifndef _AELINK_H_
#define _AELINK_H_

#include "../controller/aeController.h"
#include "../../meta/animator/aeMCLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/animator/deAnimatorLink.h>

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyContext.h>
#include <deigde/meta/property/igdeMetaPropertyCurveBezier.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>
#include <deigde/meta/property/igdeMetaPropertyObject.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>

class aeAnimator;
class aeWindowMain;
class deAnimatorLink;


/**
 * Animator link.
 */
class aeLink : public deObject{
public:
	using Ref = deTObjectReference<aeLink>;
	using List = decTObjectOrderedSet<aeLink>;
	
	
private:
	aeMCLink::Ref pMetaContext;
	aeAnimator *pAnimator;
	
	deAnimatorLink *pEngLink;
	
	bool pTempNoNotify = false;
	
public:
	igdeMetaPropertyStringStorage::Storage name;
	igdeMetaPropertyObjectStorage<aeController>::Storage controller;
	igdeMetaPropertyIntegerStorage::Storage repeat;
	igdeMetaPropertyCurveBezierStorage::Storage curve;
	igdeMetaPropertyStringStorage::Storage bone;
	igdeMetaPropertySelectionEnumStorage<deAnimatorLink::eBoneParameter>::Storage boneParameter;
	igdeMetaPropertyFloatStorage::Storage boneMinimum;
	igdeMetaPropertyFloatStorage::Storage boneMaximum;
	igdeMetaPropertyStringStorage::Storage vertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage vertexPositionSetMinimum;
	igdeMetaPropertyFloatStorage::Storage vertexPositionSetMaximum;
	igdeMetaPropertyBooleanStorage::Storage wrapY;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create link. */
	aeLink(aeWindowMain &windowMain, const char *name = "Link");
	
	/** Create copy of link. */
	aeLink(aeWindowMain &windowMain, const aeLink &copy);
	
	/** Clean up link. */
protected:
	~aeLink() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**  Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Meta context. */
	inline const aeMCLink::Ref &GetMetaContext() const{ return pMetaContext; }
	
	
	
	/** Engine controller link or \em nullptr if not managed. */
	inline deAnimatorLink *GetEngineLink() const{ return pEngLink; }
	
	
	
	/** Name. */
	inline const decString &GetName() const{ return name; }
	
	/** Set name. */
	void SetName(const char *name);
	
	
	
	/** Controller or \em nullptr. */
	inline const deTObjectReference<aeController> &GetController() const{ return controller; }
	
	/** Set controller or \em nullptr. */
	void SetController(aeController *controller, bool notify = true);
	
	/** Repeat count of input value. */
	inline int GetRepeat() const{ return repeat; }
	
	/**
	 * Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat(int repeat);
	
	/** Curve. */
	const decCurveBezier &GetCurve() const{ return curve; }
	
	/** Set curve. */
	void SetCurve(const decCurveBezier &curve);
	
	/**
	 * Bone to use parameter of as input or empty string to not use.
	 * \version 1.6
	 */
	inline const decString &GetBone() const{ return bone; }
	
	/**
	 * Set bone to use parameter of as input or empty string to not use.
	 * \version 1.6
	 */
	void SetBone(const char *bone);
	
	/**
	 * Bone parameter to use as input.
	 * \version 1.6
	 */
	inline deAnimatorLink::eBoneParameter GetBoneParameter() const{ return boneParameter; }
	
	/**
	 * Set bone parameter to use as input.
	 * \version 1.6
	 */
	void SetBoneParameter(deAnimatorLink::eBoneParameter parameter);
	
	/**
	 * Minimum bone parameter value.
	 * \version 1.6
	 */
	inline float GetBoneMinimum() const{ return boneMinimum; }
	
	/**
	 * Set minimum bone parameter value
	 * \version 1.6
	 */
	void SetBoneMinimum(float value);
	
	/**
	 * Maximum bone parameter value.
	 * \version 1.6
	 */
	inline float GetBoneMaximum() const{ return boneMaximum; }
	
	/**
	 * Set maximum bone parameter value.
	 * \version 1.6
	 */
	void SetBoneMaximum(float value);
	
	/** Vertex position set to use as input or empty string to not use. */
	inline const decString &GetVertexPositionSet() const{ return vertexPositionSet; }
	
	/** Set vertex position set to use as input or empty string to not use. */
	void SetVertexPositionSet(const char *vertexPositionSet);
	
	/** Minimum vertex position set parameter value. */
	inline float GetVertexPositionSetMinimum() const{ return vertexPositionSetMinimum; }
	
	/** Set minimum vertex position set parameter value. */
	void SetVertexPositionSetMinimum(float value);
	
	/** Maximum vertex position set parameter value. */
	inline float GetVertexPositionSetMaximum() const{ return vertexPositionSetMaximum; }
	
	/** Set maximum vertex position set parameter value. */
	void SetVertexPositionSetMaximum(float value);
	
	/**
	 * Wrap Y value instead of clamping.
	 * \version 1.9
	 */
	inline bool GetWrapY() const{ return wrapY; }
	
	/**
	 * Set to wrap Y value instead of clamping.
	 * \version 1.9
	 */
	void SetWrapY(bool wrap);
	
	
	
	/** Notify engine object about changes in this link. */
	void NotifyLinkChanged();
	
	
	
	/** Update controller. */
	void UpdateController();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy link to this link. */
	aeLink &operator=(const aeLink &copy);
	/*@}*/
	
	
	
private:
	void pUpdateBoneLimits();
};

#endif
