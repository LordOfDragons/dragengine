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

#ifndef _SEBODY_H_
#define _SEBODY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/skin/deSkin.h>

class seLayer;

class deEngine;



/**
 * \brief Sky body.
 */
class seBody : public deObject{
private:
	deEngine *pEngine;
	
	seLayer *pLayer;
	
	decVector pOrientation;
	decVector2 pSize;
	decColor pColor;
	decString pPathSkin;
	deSkin::Ref pEngSkin;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seBody> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky body. */
	seBody(deEngine *engine);
	
protected:
	/** \brief Clean up sky body. */
	virtual ~seBody();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine *GetEngine() const{return pEngine;}
	
	/** \brief Parent layer or \em NULL. */
	inline seLayer *GetLayer() const{return pLayer;}
	
	/** \brief Set parent layer or \em NULL. */
	void SetLayer(seLayer *layer);
	
	/** \brief Orientation. */
	inline const decVector &GetOrientation() const{return pOrientation;}
	
	/** \brief Set orientation. */
	void SetOrientation(const decVector &orientation);
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{return pSize;}
	
	/** \brief Set size. */
	void SetSize(const decVector2 &size);
	
	/** \brief Color. */
	inline const decColor &GetColor() const{return pColor;}
	
	/** \brief Set color. */
	void SetColor(const decColor &color);
	
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{return pPathSkin;}
	
	/** \brief Set skin path. */
	void SetSkinPath(const char *skinPath);
	
	/** \brief Engine skin or \em NULL. */
	inline deSkin *GetEngineSkin() const{return pEngSkin;}
	
	/** \brief Body is the active one. */
	inline bool GetActive() const{return pActive;}
	
	/** \brief Set if body is the active one. */
	void SetActive(bool active);
	
	/** \brief Body is selected. */
	inline bool GetSelected() const{return pSelected;}
	
	/** \brief Set if body is selected. */
	void SetSelected(bool selected);
	
	/** \brief Notify listeners body parameters changed. */
	void NotifyBodyChanged();
	
	/** \brief Update relative resources after changing the base path. */
	void UpdateRelativeResources();
	/*@}*/
	
	
	
private:
	void pUpdateSkin();
};

#endif
