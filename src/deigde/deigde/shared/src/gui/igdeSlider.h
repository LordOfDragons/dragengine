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

#ifndef _IGDESLIDER_H_
#define _IGDESLIDER_H_

#include "igdeWidget.h"
#include "event/igdeActionReference.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>


class igdeSliderListener;


/**
 * \brief IGDE UI Slider.
 */
class DE_DLL_EXPORT igdeSlider : public igdeWidget{
public:
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
