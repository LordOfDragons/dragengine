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

#ifndef _IGDEEDITSLIDERTEXT_H_
#define _IGDEEDITSLIDERTEXT_H_

#include "../igdeSlider.h"
#include "../igdeTextField.h"
#include "../event/igdeSliderListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>

class igdeEditSliderTextListener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit SliderText.
 * 
 * Composed widget to edit value using slider or text field. The main widget is the slider
 * widget. The text field is synchronized to the slider.
 */
class DE_DLL_EXPORT igdeEditSliderText : public igdeContainerFlow{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeEditSliderText> Ref;
	
	
protected:
	/** \brief Slider listener. */
	class DE_DLL_EXPORT cListenerSlider : public igdeSliderListener{
	protected:
		igdeEditSliderText &pSliderText;
		
	public:
		cListenerSlider( igdeEditSliderText &sliderText );
		virtual ~cListenerSlider();
		virtual void OnValueChanged( igdeSlider *slider );
		virtual void OnValueChanging(igdeSlider *slider );
	};
	
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListenerTextField : public igdeTextFieldListener{
	protected:
		igdeEditSliderText &pSliderText;
		
	public:
		cListenerTextField( igdeEditSliderText &sliderText );
		virtual ~cListenerTextField();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	igdeSlider::Ref pSlider;
	igdeTextField::Ref pText;
	
	float pValue;
	bool pPreventNotify;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit point. */
	igdeEditSliderText( igdeUIHelper &helper, float lower, float upper,
		int columns, int precision, float tickSpacing, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditSliderText();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Range lower value. */
	float GetLower() const;
	
	/** \brief Range upper value. */
	float GetUpper() const;
	
	/** \brief Set range. */
	void SetRange( float lower, float upper );
	
	/** \brief Tick spacing. */
	float GetTickSpacing() const;
	
	/** \brief Set tick spacing. */
	void SetTickSpacing( float spacing );
	
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( float value );
	
	/** \brief Precision. */
	int GetPrecision() const;
	
	/** \brief Set precision. */
	void SetPrecision( int precision );
	
	/** \brief Slider is enabled. */
	bool GetEnabled() const;
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Description shown in tool tips. */
	const decString &GetDescription() const;
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditSliderTextListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditSliderTextListener *listener );
	
	/** \brief Notify listeners value changed and has been accepted by user. */
	virtual void NotifySliderTextValueChanged();
	
	/** \brief Notify listeners value is changing but not accepted yet. */
	virtual void NotifySliderTextValueChanging();
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeUIHelper &helper, float lower, float upper, int columns,
		int precision, float tickSpacing, const char *description );
};

#endif
