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

#ifndef _IGDEVIEWCURVEBEZIER_H_
#define _IGDEVIEWCURVEBEZIER_H_

#include "../igdeWidget.h"
#include "../event/igdeAction.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/math/decMath.h>

class igdeEnvironment;
class igdeViewCurveBezierListener;


/**
 * \brief View and edit decCurveBezier.
 */
class DE_DLL_EXPORT igdeViewCurveBezier : public igdeWidget{
public:
	class DE_DLL_EXPORT cActionResetView : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionResetView( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionFitToCurve : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionFitToCurve( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionEditSelectedPoint : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionEditSelectedPoint( igdeViewCurveBezier &view );
		virtual void OnAction();
		virtual void Update();
	};
	
	class DE_DLL_EXPORT cActionSetInterpolationMode : public igdeAction{
		igdeViewCurveBezier &pView;
		decCurveBezier::eInterpolationModes pMode;
	public:
		cActionSetInterpolationMode( igdeViewCurveBezier &view,
			decCurveBezier::eInterpolationModes mode, const char *text, igdeIcon *icon,
			const char *description, deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined );
		virtual void OnAction();
		virtual void Update();
	};
	
	class DE_DLL_EXPORT cActionCopyCurve : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionCopyCurve( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionPasteCurve : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionPasteCurve( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionClearCurve : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionClearCurve( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionSetDefaultConstant : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionSetDefaultConstant( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionSetDefaultLinear : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionSetDefaultLinear( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionSetDefaultBezier : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionSetDefaultBezier( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionInvertCurveX : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionInvertCurveX( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionInvertCurveY : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionInvertCurveY( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionEditClamp : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionEditClamp( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	class DE_DLL_EXPORT cActionAutoHandles : public igdeAction{
		igdeViewCurveBezier &pView;
	public:
		cActionAutoHandles( igdeViewCurveBezier &view );
		virtual void OnAction();
	};
	
	
	
private:
	bool pEnabled;
	decCurveBezier pCurve;
	
	int pSelPoint;
	
	bool pClamp;
	decVector2 pClampMin;
	decVector2 pClampMax;
	
	decPoint pDefaultSize;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	igdeViewCurveBezier( igdeEnvironment &environment );
	
	
	
protected:
	/** \brief Clean up view. */
	virtual ~igdeViewCurveBezier();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Default size. */
	inline const decPoint &GetDefaultSize() const{ return pDefaultSize; }
	
	/** \brief Set default size. */
	void SetDefaultSize( const decPoint &size );
	
	
	
	/** \brief TextField is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Bezier curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set bezier curve. */
	void SetCurve( const decCurveBezier &curve, bool changing = false );
	
	/** \brief Clear curve. */
	void ClearCurve();
	
	/** \brief Invert curve along X axis. */
	void InvertCurveX();
	
	/** \brief Invert curve along Y axis. */
	void InvertCurveY();
	
	/**
	 * \brief Set curve to default constant curve.
	 * \details Fits curve into clamp minimum/maximum range.
	 * \version 1.9
	 */
	void SetDefaultConstant();
	
	/**
	 * \brief Set curve to default linear curve.
	 * \details Fits curve into clamp minimum/maximum range.
	 */
	void SetDefaultLinear();
	
	/**
	 * \brief Set curve to default bezier curve.
	 * \details Fits curve into clamp minimum/maximum range.
	 */
	void SetDefaultBezier();
	
	/**
	 * \brief Set handles to default position for interpolation mode.
	 * \version 1.15
	 */
	void SetAutoHandles();
	
	
	
	/** \brief Selected point or -1. */
	inline int GetSelectedPoint() const{ return pSelPoint; }
	
	/** \brief Set selected point or -1. */
	void SetSelectedPoint( int index );
	
	
	
	/** \brief Clamp curve. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if curve is clamped. */
	void SetClamp( bool clamp );
	
	/** \brief Clamp minimum coordinates. */
	inline const decVector2 &GetClampMin() const{ return pClampMin; }
	
	/** \brief Set clamp minimum coordinates. */
	void SetClampMin( const decVector2 &clamp );
	
	/** \brief Clamp maximum coordinates. */
	inline const decVector2 &GetClampMax() const{ return pClampMax; }
	
	/** \brief Set clamp maximum coordinates. */
	void SetClampMax( const decVector2 &clamp );
	
	
	
	/** \brief Reset view to range from clamp minimum to maximum. */
	void ResetView();
	
	/** \brief Fit view to curve. */
	void FitViewToCurve();
	
	/** \brief Show context menu at position. */
	void ShowContextMenu( const decPoint &position );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeViewCurveBezierListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeViewCurveBezierListener *listener );
	
	/** \brief Notify listeners changed. */
	virtual void NotifyCurveChanged();
	
	/** \brief Notify listeners changing. */
	virtual void NotifyCurveChanging();
	
	/** \brief Notify listeners selected point changed. */
	virtual void NotifyCurveSelectPoint();
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
	/** \brief Default size changed. */
	virtual void OnDefaultSizeChanged();
	
	/** \brief Curve changed. */
	virtual void OnCurveChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Selected point changed. */
	virtual void OnSelectedPointChanged();
	
	/** \brief Clamp changed. */
	virtual void OnClampChanged();
	
	/** \brief Reset view. */
	virtual void OnResetView();
	
	/** \brief Fit to curve. */
	virtual void OnFitToCurve();
	/*@}*/
};

#endif
