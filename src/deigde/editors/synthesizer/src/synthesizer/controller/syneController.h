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

#ifndef _SYNECONTROLLER_H_
#define _SYNECONTROLLER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/curve/decCurveBezier.h>

class syneSynthesizer;
class deSynthesizerController;



/**
 * \brief Synthesizer controller.
 */
class syneController : public deObject{
private:
	syneSynthesizer *pSynthesizer;
	int pEngControllerIndex;
	
	decString pName;
	
	float pMinValue;
	float pMaxValue;
	bool pClamp;
	decCurveBezier pCurve;
	
	float pEditConstantValue;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<syneController> Ref;
	
	/** \brief Type holding list of objects. */
	typedef decTObjectOrderedSet<syneController> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller. */
	syneController(const char *name = "Controller");
	
	/** \brief Create copy of controller. */
	syneController(const syneController &copy);
	
	/** \brief Clean up controller. */
protected:
	virtual ~syneController();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	inline syneSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set parent synthesizer. */
	void SetSynthesizer(syneSynthesizer *synthesizer);
	
	
	
	/** \brief Index of engine synthesizer instance controller. */
	inline int GetEngineControllerIndex() const{ return pEngControllerIndex; }
	
	/** \brief Set index of engine synthesizer instance controller. */
	void SetEngineControllerIndex(int index);
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue(float value);
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue(float value);
	
	/** \brief Value is clamped instead of wraped around. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if value is clamped instead of wraped around. */
	void SetClamp(bool clamp);
	
	/** \brief Value curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set value curve. */
	void SetCurve(const decCurveBezier &curve);
	
	
	
	/** \brief Edit constant value. */
	inline float GetEditConstantValue() const{ return pEditConstantValue; }
	
	/** \brief Set edit constant value. */
	void SetEditConstantValue(float value);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set controller. */
	syneController &operator=(const syneController &controller);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
