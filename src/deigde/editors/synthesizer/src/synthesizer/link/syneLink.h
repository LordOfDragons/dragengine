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

#ifndef _SYNELINK_H_
#define _SYNELINK_H_

#include "../controller/syneController.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>

class syneLinkMapping;
class syneSynthesizer;



/**
 * Synthesizer link.
 */
class syneLink : public deObject{
private:
	syneSynthesizer *pSynthesizer;
	
	deSynthesizerLink::Ref pEngLink;
	
	decString pName;
	
	syneController::Ref pController;
	int pRepeat;
	decCurveBezier pCurve;
	
	
	
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<syneLink> Ref;
	
	/** Type holding list of objects. */
	typedef decTObjectOrderedSet<syneLink> List;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create link. */
	syneLink();
	
	/** Create copy of link. */
	syneLink(const syneLink &copy);
	
	/** Clean up link. */
protected:
	virtual ~syneLink();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**  Synthesizer. */
	inline syneSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** Set synthesizer. */
	void SetSynthesizer(syneSynthesizer *synthesizer);
	
	
	
	/** Engine controller link or \em nullptr if not managed. */
	inline const deSynthesizerLink::Ref &GetEngineLink() const{ return pEngLink; }
	
	
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName(const char *name);
	
	
	
	/** Controller or \em nullptr. */
	inline const syneController::Ref &GetController() const{ return pController; }
	
	/** Set controller or \em nullptr. */
	void SetController(syneController *controller);
	
	/** Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat(int repeat);
	
	/** Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	
	
	/** Notify engine object about changes in this link. */
	void NotifyLinkChanged();
	
	
	
	/** Update controller. */
	void UpdateController();
	
	/** Updates engine curve. */
	void UpdateCurve();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy link to this link. */
	syneLink &operator=(const syneLink &copy);
	/*@}*/
};

#endif
