/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDEEDITSLIDERTEXT_H_
#define _IGDEEDITSLIDERTEXT_H_

#include "../igdeSliderReference.h"
#include "../igdeTextFieldReference.h"
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
protected:
	/** \brief Slider listener. */
	class cListenerSlider : public igdeSliderListener{
	protected:
		igdeEditSliderText &pSliderText;
		
	public:
		cListenerSlider( igdeEditSliderText &sliderText );
		virtual ~cListenerSlider();
		virtual void OnValueChanged( igdeSlider *slider );
		virtual void OnValueChanging(igdeSlider *slider );
	};
	
	/** \brief Text field listener. */
	class cListenerTextField : public igdeTextFieldListener{
	protected:
		igdeEditSliderText &pSliderText;
		
	public:
		cListenerTextField( igdeEditSliderText &sliderText );
		virtual ~cListenerTextField();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	igdeSliderReference pSlider;
	igdeTextFieldReference pText;
	
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
