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

#ifndef _IGDEMOUSECAMERALISTENER_H_
#define _IGDEMOUSECAMERALISTENER_H_

#include "igdeMouseDragListener.h"

#include <dragengine/common/math/decMath.h>

class igdeViewRenderWindow;
class igdeCamera;


/**
 * \brief Camera Mouse IGDE UI Mouse/Key Listener.
 * 
 * Adding this listener to an igdeViewRenderWindow adds support for draging camera with
 * right button. Shift and control modifier keys determine if the interaction is to
 * rotate, pan, view or zoom the camera.
 */
class igdeMouseCameraListener : public igdeMouseDragListener{
public:
	/** \brief Interaction. */
	enum eInteraction{
		/** \brief No interaction. */
		eiNone,
		
		/** \brief Rotate camera using orbit distance. */
		eiRotate,
		
		/** \brief Panning camera along view plane. */
		eiPan,
		
		/** \brief Moving camera formard and backward. */
		eiMove,
		
		/** \brief Increase or decrease camera orbit distance. */
		eiZoom
	};
	
	
	
private:
	igdeCamera *pCamera;
	
	bool pEnableRotate;
	bool pEnablePan;
	bool pEnableMove;
	bool pEnableZoom;
	
	float pSpeedRotate;
	float pSpeedPan;
	float pSpeedMove;
	float pSpeedZoom;
	
	eInteraction pInteraction;
	
	decDVector pMoveOrigin;
	decDVector pMoveCurrent;
	decVector pRotateOrigin;
	decVector pRotateCurrent;
	float pZoomOrigin;
	float pZoomCurrent;
	decDMatrix pMoveMatrix;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeMouseCameraListener( igdeCamera *camera = NULL );
	
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMouseCameraListener();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Camera or NULL. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or NULL. */
	void SetCamera( igdeCamera *camera );
	
	
	
	/** \brief Enable rotate camera interaction. */
	inline bool GetEnableRotate() const{ return pEnableRotate; }
	
	/** \brief Set if rotate camera interaction is enabled. */
	void SetEnableRotate( bool enable );
	
	/** \brief Enable pan camera interaction. */
	inline bool GetEnablePan() const{ return pEnablePan; }
	
	/** \brief Set if pan camera interaction is enabled. */
	void SetEnablePan( bool enable );
	
	/** \brief Enable move camera interaction. */
	inline bool GetEnableMove() const{ return pEnableMove; }
	
	/** \brief Set if move camera interaction is enabled. */
	void SetEnableMove( bool enable );
	
	/** \brief Enable zoom camera interaction. */
	inline bool GetEnableZoom() const{ return pEnableZoom; }
	
	/** \brief Set if zoom camera interaction is enabled. */
	void SetEnableZoom( bool enable );
	
	/** \brief Enable all supported interactions. */
	virtual void SetEnabledAll( bool enable );
	
	
	
	/** \brief Rotation speed in degrees per drag distance pixels. */
	inline float GetSpeedRotate() const{ return pSpeedRotate; }
	
	/** \brief Set rotation speed in degrees per drag distance pixels. */
	void SetSpeedRotate( float degreesPerPixel );
	
	/** \brief Pan speed in meters per drag distance pixels. */
	inline float GetSpeedPan() const{ return pSpeedPan; }
	
	/** \brief Set pan speed in meters per drag distance pixels. */
	void SetSpeedPan( float metersPerPixel );
	
	/** \brief Move speed in meters per drag distance pixels. */
	inline float GetSpeedMove() const{ return pSpeedMove; }
	
	/** \brief Set move speed in meters per drag distance pixels. */
	void SetSpeedMove( float metersPerPixel );
	
	/** \brief Zoom speed in meters per drag distance pixels. */
	inline float GetSpeedZoom() const{ return pSpeedZoom; }
	
	/** \brief Set zoom speed in meters per drag distance pixels. */
	void SetSpeedZoom( float metersPerPixel );
	
	
	
	/** \brief Interaction in progress. */
	inline eInteraction GetInteraction() const{ return pInteraction; }
	
	/** \brief Set interaction in progress. */
	void SetInteraction( eInteraction interaction );
	
	
	
	/** \brief Pan or move position at start of interaction. */
	inline const decDVector &GetMoveOrigin() const{ return pMoveOrigin; }
	
	/** \brief Set pan or move position at start of interaction. */
	void SetMoveOrigin( const decDVector &position );
	
	/** \brief Current pan or move position. */
	inline const decDVector &GetMoveCurrent() const{ return pMoveCurrent; }
	
	/** \brief Set current pan or move position. */
	void SetMoveCurrent( const decDVector &position );
	
	/** \brief Rotation in degrees at start of interaction. */
	inline const decVector &GetRotateOrigin() const{ return pRotateOrigin; }
	
	/** \brief Set rotation in degrees at start of interaction. */
	void SetRotateOrigin( const decVector &rotation );
	
	/** \brief Current rotation in degrees. */
	inline const decVector &GetRotationCurrent() const{ return pRotateCurrent; }
	
	/** \brief Set current rotation in degrees. */
	void SetRotationCurrent( const decVector &rotation );
	
	/** \brief Zoom at start of interaction. */
	inline float GetZoomOrigin() const{ return pZoomOrigin; }
	
	/** \brief Set zoom at start of interaction. */
	void SetZoomOrigin( float zoom );
	
	/** \brief Current zoom. */
	inline float GetZoomCurrent() const{ return pZoomCurrent; }
	
	/** \brief Set current zoom. */
	void SetZoomCurrent( float zoom );
	
	/** \brief Matrix for pan and zoom interaction. */
	inline const decDMatrix &GetMoveMatrix() const{ return pMoveMatrix; }
	
	/** \brief Set matrix for pan and zoom interaction. */
	void SetMoveMatrix( const decDMatrix &matrix );
	/*@}*/
	
	
	
	/** \text Interaction events */
	/*@{*/
	/**
	 * \brief Choose what interaction to use using current drag state.
	 * 
	 * The default implementation chooses the interaction according to the table below
	 * if the right mouse button is pressed
	 * 
	 * ^ Shift ^ Control ^ Interaction ^
	 * | - | - | eiRotate |
	 * | X | - | eiPan |
	 * | - | X | eiMove |
	 * | X | X | eiZoom |
	 */
	virtual eInteraction ChooseInteraction();
	
	
	
	/** \brief Rotating camera begins. */
	virtual bool OnRotateBegin();
	
	/** \brief Update rotating. */
	virtual void OnRotateUpdate();
	
	/** \brief Rotating ends. */
	virtual void OnRotateFinish( bool cancelled );
	
	
	
	/** \brief Panning camera begins. */
	virtual bool OnPanBegin();
	
	/** \brief Update panning. */
	virtual void OnPanUpdate();
	
	/** \brief Panning ends. */
	virtual void OnPanFinish( bool cancelled );
	
	
	
	/** \brief Moving camera begins. */
	virtual bool OnMoveBegin();
	
	/** \brief Update moving. */
	virtual void OnMoveUpdate();
	
	/** \brief Moving ends. */
	virtual void OnMoveFinish( bool cancelled );
	
	
	
	/** \brief Zooming camera begins. */
	virtual bool OnZoomBegin();
	
	/** \brief Update zooming. */
	virtual void OnZoomUpdate();
	
	/** \brief Zooming ends. */
	virtual void OnZoomFinish( bool cancelled );
	
	
	
	/**
	 * \brief Camera has been modified.
	 * 
	 * Called by On*Update() methods when camera parameters have been changed. For use by
	 * subclasses to react to camera changes without having to overrite thje individual
	 * update methods. Default implementation does nothing.
	 */
	virtual void OnCameraChanged();
	/*@}*/
	
	
	
	/** \text Dragging events */
	/*@{*/
	/**
	 * \brief Dragging begins.
	 * 
	 * Calls ChooseInteraction() to figure out what interaction to use.
	 */
	virtual bool OnDragBegin();
	
	/**
	 * \brief Update dragging.
	 * 
	 * Called if state is not edsNone and mouse has been moved. Subclass has to update the
	 * dragging process.
	 */
	virtual void OnDragUpdate();
	
	/**
	 * \brief Dragging ends.
	 * 
	 * Called if state is not edsNone and mouse button used to start draggins has been
	 * released. Subclass has to finish dragging operation.
	 */
	virtual void OnDragFinish( bool cancelled );
	/*@}*/
};

#endif
