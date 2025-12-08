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

#ifndef _SELINK_H_
#define _SELINK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class seSky;
class seController;



/**
 * \brief Sky link.
 */
class seLink : public deObject{
private:
	seSky *pSky;
	int pIndex;
	
	decString pName;
	
	seController::Ref pController;
	decCurveBezier pCurve;
	int pRepeat;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seLink> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	seLink();
	
	/** \brief Clean up link. */
	virtual ~seLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent sky or \em NULL. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set parent sky or \em NULL. */
	void SetSky(seSky *sky);
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex(int index);
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Controller or \em NULL. */
	inline const seController::Ref &GetController() const{ return pController; }
	
	/** \brief Set controller or \em NULL. */
	void SetController(seController *controller);
	
	/** \brief Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Notify curve changed. */
	void NotifyCurveChanged();
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat(int repeat);
	
	/** \brief Link is active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if link is active one. */
	void SetActive(bool active);
	
	/** \brief Link is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if link is selected. */
	void SetSelected(bool selected);
	/*@}*/
};

#endif
