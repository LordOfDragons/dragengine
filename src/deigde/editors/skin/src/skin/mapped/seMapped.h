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

#ifndef _SEMAPPED_H_
#define _SEMAPPED_H_

#include "../property/sePropertyList.h"

#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/deSkinMapped.h>

class seSkin;
class seMappedList;


/**
 * Skin mapped.
 */
class seMapped : public deObject{
public:
	typedef deTObjectReference<seMapped> Ref;
	
	
private:
	seSkin *pSkin;
	
	decString pName;
	
	decCurveBezier pCurve;
	deSkinMapped::eInputTypes pInputType;
	float pInputLower;
	float pInputUpper;
	bool pInputClamped;
	float pOutputLower;
	float pOutputUpper;
	decString pBone;
	decString pRenderable;
	deSkinMapped::eRenderableComponent pRenderableComponent;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new mapped. */
	seMapped(const char *name = "Mapped");
	
	/** Create copy of mapped. */
	seMapped(const seMapped &mapped);
	
protected:
	/** Clean up mapped. */
	virtual ~seMapped();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent skin or nullptr. */
	inline seSkin *GetSkin() const{return pSkin;}
	
	/** Set parent skin or nullptr. */
	void SetSkin(seSkin *skin);
	
	
	
	/** Name. */
	inline const decString &GetName() const{return pName;}
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Make name uniqueu. */
	void MakeNameUnique(const seMappedList &list);
	
	/** Curve. */
	inline const decCurveBezier &GetCurve() const{return pCurve;}
	
	/** Set curve. */
	void SetCurve(const decCurveBezier &curve);
	
	/** Input type. */
	inline deSkinMapped::eInputTypes GetInputType() const{return pInputType;}
	
	/** Set input type. */
	void SetInputType(deSkinMapped::eInputTypes inputType);
	
	/** Lower input range. */
	inline float GetInputLower() const{return pInputLower;}
	
	/** Set lower input range. */
	void SetInputLower(float lower);
	
	/** Upper input range. */
	inline float GetInputUpper() const{return pInputUpper;}
	
	/** Set upper input range. */
	void SetInputUpper(float upper);
	
	/** Input value is clamped to range instead of wrapping around. */
	inline bool GetInputClamped() const{return pInputClamped;}
	
	/** Set if input value is clamped to range instead of wrapping around. */
	void SetInputClamped(bool inputClamped);
	
	/** Lower output range. */
	inline float GetOutputLower() const{return pOutputLower;}
	
	/** Set lower output range. */
	void SetOutputLower(float lower);
	
	/** Upper output range. */
	inline float GetOutputUpper() const{return pOutputUpper;}
	
	/** Set upper output range. */
	void SetOutputUpper(float upper);
	
	/** Bone name if bone related input type is used. */
	inline const decString &GetBone() const{return pBone;}
	
	/** Set bone name if bone related input type is used. */
	void SetBone(const char *bone);
	
	/** Renderable name if renderable related input type is used. */
	inline const decString &GetRenderable() const{return pRenderable;}
	
	/** Set renderable name if renderable related input type is used. */
	void SetRenderable(const char *renderable);
	
	/** Color component to use if renderable is of color type. */
	inline deSkinMapped::eRenderableComponent GetRenderableComponent() const{return pRenderableComponent;}
	
	/** Set color component to use if renderable is of color type. */
	void SetRenderableComponent(deSkinMapped::eRenderableComponent component);
	
	
	
	/** Mapped is active. */
	inline bool GetActive() const{return pActive;}
	
	/** Set if mapped is active. */
	void SetActive(bool active);
	
	/** Mapped is selected. */
	inline bool GetSelected() const{return pSelected;}
	
	/** Set if mapped is selected. */
	void SetSelected(bool selected);
	
	
	
	/** Notify listeners mapped changed. */
	void NotifyChanged();
	
	/** Notify listeners name changed. */
	void NotifyNameChanged();
	/*@}*/
};

#endif
