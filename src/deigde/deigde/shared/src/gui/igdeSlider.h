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

#ifndef _IGDESLIDER_H_
#define _IGDESLIDER_H_

#include "igdeWidget.h"
#include "event/igdeAction.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>


class igdeSliderListener;


/**
 * \brief IGDE UI Slider.
 */
class DE_DLL_EXPORT igdeSlider : public igdeWidget{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeSlider> Ref;
	
	
	/** \brief Orientation. */
	enum eOrientation{
		/** \brief Horizontal. */
		eoHorizontal,
		
		/** \brief Vertical. */
		eoVertical
	};
	
	
	
private:
	eOrientation pOrientation;
	float pLower;
	float pUpper;
	float pTickSpacing;
	float pValue;
	int pPrecision;
	bool pEnabled;
	decString pDescription;
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create slider. */
	igdeSlider( igdeEnvironment &environment, eOrientation orientation,
		const char *description = "" );
	
	/** \brief Create slider. */
	igdeSlider( igdeEnvironment &environment, eOrientation orientation, float lower,
		float upper, int precision, float tickSpacing, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeSlider();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Orientation. */
	inline eOrientation GetOrientation() const{ return pOrientation; }
	
	/** \brief Range lower value. */
	inline float GetLower() const{ return pLower; }
	
	/** \brief Range upper value. */
	inline float GetUpper() const{ return pUpper; }
	
	/** \brief Set range. */
	void SetRange( float lower, float upper );
	
	/** \brief Tick spacing. */
	inline float GetTickSpacing() const{ return pTickSpacing; }
	
	/** \brief Set tick spacing. */
	void SetTickSpacing( float spacing );
	
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( float value );
	
	/** \brief Precision. */
	inline int GetPrecision() const{ return pPrecision; }
	
	/** \brief Set precision. */
	void SetPrecision( int precision );
	
	/** \brief Slider is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeSliderListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeSliderListener *listener );
	
	/** \brief Notify listeners value changed. */
	void NotifyValueChanged();
	
	/** \brief Notify listeners value is changing. */
	void NotifyValueChanging();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Range or tick spacing changed. */
	virtual void OnRangeChanged();
	
	/** \brief Value changed. */
	virtual void OnValueChanged();
	
	/** \brief Precision changed. */
	virtual void OnPrecisionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	/*@}*/
};

#endif
