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

#ifndef _IGDECAMERAINTERACTIONLISTENER_H_
#define _IGDECAMERAINTERACTIONLISTENER_H_

#include "igdeMouseDragListener.h"
#include "../igdeCamera.h"
#include "../../environment/igdeEnvironmentListener.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/smooth/decSmoothVector.h>
#include <dragengine/common/math/smooth/decSmoothFloat.h>

class igdeEnvironment;
class igdeViewRenderWindow;


/**
 * \brief Camera Interaction Listener events.
 * 
 * Adding this listener to an igdeViewRenderWindow adds support for manipulate camera using the
 * chosen interaction mode. For mouse interaction the right button manipulates the camera.
 * Shift and control modifier keys determine if the interaction is to rotate, pan, view or zoom
 * the camera. For touch pad interaction manipulation is enabled by control + right button and
 * disabled using control + right button or escape button. In this mode mouse movement rotates
 * the camera and WASD pan the camera and QE zoom the camera.
 */
class DE_DLL_EXPORT igdeCameraInteractionListener : public igdeMouseDragListener{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeCameraInteractionListener>;
	
	
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
		eiZoom,
		
		/** \brief Fly mode. */
		eiFly
	};
	
	
private:
	class MouseDragListener : public igdeMouseDragListener{
	public:
		igdeCameraInteractionListener *owner;
		
		MouseDragListener(igdeCameraInteractionListener *owner);
		bool OnDragBegin() override;
		void OnDragUpdate() override;
		void OnDragFinish(bool cancelled) override;
		void OnButtonPress(igdeWidget *widget, int button, const decPoint &position, int modifiers) override;
		void OnMouseMoved(igdeWidget *widget, const decPoint &position, int modifiers) override;
		void OnButtonRelease(igdeWidget *widget, int button,
			const decPoint &position, int modifiers) override;
		void OnMouseWheeled(igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers) override;
		void OnKeyPress(igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key) override;
		void OnKeyRelease(igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key) override;
		
	protected:
		~MouseDragListener() override;
	};
	
	class EnvironmentListener : public igdeEnvironmentListener{
	public:
		igdeCameraInteractionListener *owner;
		
		EnvironmentListener(igdeCameraInteractionListener *owner);
		void OnFrameUpdate(float elapsed) override;
		
	protected:
		~EnvironmentListener() override;
	};
	
	
	igdeEnvironment &pEnvironment;
	igdeCamera::Ref pCamera;
	
	bool pEnableRotate, pEnablePan, pEnableMove, pEnableZoom;
	
	float pSpeedRotate, pSpeedPan, pSpeedMove, pSpeedZoom, pSpeedFly;
	int pFlySpeedModifier;
	
	eInteraction pInteraction;
	
	decDVector pMoveOrigin, pMoveCurrent;
	decVector pRotateOrigin, pRotateCurrent;
	float pZoomOrigin, pZoomCurrent;
	decDMatrix pMoveMatrix;
	bool pFlyForward, pFlyBackward, pFlyLeft, pFlyRight, pFlyUp, pFlyDown;
	decSmoothVector pFlyVelocity;
	decPoint pFlyMouseLastPosition, pFlyMouseCurrentPosition;
	int pRightMouseButtonPressed;
	
	deTWeakObjectReference<igdeViewRenderWindow> pViewRenderWindow;
	
	deTObjectReference<MouseDragListener> pMouseDragListener;
	deTObjectReference<EnvironmentListener> pEnvironmentListener;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	explicit igdeCameraInteractionListener(igdeEnvironment &environment);
	explicit igdeCameraInteractionListener(igdeEnvironment &environment, igdeCamera *camera);
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~igdeCameraInteractionListener() override;
	/*@}*/
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Camera or nullptr. */
	inline const igdeCamera::Ref &GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or nullptr. */
	void SetCamera(igdeCamera *camera);
	
	
	/** \brief Enable rotate camera interaction. */
	inline bool GetEnableRotate() const{ return pEnableRotate; }
	
	/** \brief Set if rotate camera interaction is enabled. */
	void SetEnableRotate(bool enable);
	
	/** \brief Enable pan camera interaction. */
	inline bool GetEnablePan() const{ return pEnablePan; }
	
	/** \brief Set if pan camera interaction is enabled. */
	void SetEnablePan(bool enable);
	
	/** \brief Enable move camera interaction. */
	inline bool GetEnableMove() const{ return pEnableMove; }
	
	/** \brief Set if move camera interaction is enabled. */
	void SetEnableMove(bool enable);
	
	/** \brief Enable zoom camera interaction. */
	inline bool GetEnableZoom() const{ return pEnableZoom; }
	
	/** \brief Set if zoom camera interaction is enabled. */
	void SetEnableZoom(bool enable);
	
	/** \brief Enable all supported interactions. */
	virtual void SetEnabledAll(bool enable);
	
	
	/** \brief Rotation speed in degrees per drag distance pixels. */
	inline float GetSpeedRotate() const{ return pSpeedRotate; }
	
	/** \brief Set rotation speed in degrees per drag distance pixels. */
	void SetSpeedRotate(float degreesPerPixel);
	
	/** \brief Pan speed in meters per drag distance pixels. */
	inline float GetSpeedPan() const{ return pSpeedPan; }
	
	/** \brief Set pan speed in meters per drag distance pixels. */
	void SetSpeedPan(float metersPerPixel);
	
	/** \brief Move speed in meters per drag distance pixels. */
	inline float GetSpeedMove() const{ return pSpeedMove; }
	
	/** \brief Set move speed in meters per drag distance pixels. */
	void SetSpeedMove(float metersPerPixel);
	
	/** \brief Zoom speed in meters per drag distance pixels. */
	inline float GetSpeedZoom() const{ return pSpeedZoom; }
	
	/** \brief Set zoom speed in meters per drag distance pixels. */
	void SetSpeedZoom(float metersPerPixel);
	
	/** \brief Fly speed in meters per second. */
	inline float GetSpeedFly() const{ return pSpeedFly; }
	
	/** \brief Set fly speed in meters per second. */
	void SetSpeedFly(float metersPerSecond);
	
	/**
	 * \brief Fly speed modifier.
	 * 
	 * 0 is normal speed. Every +1 increases speed by 25%. Every -1 decreases speed by 25%.
	 * Minimum is -20 and maximum is 25.
	 */
	inline int GetSpeedFlyModifier() const{ return pFlySpeedModifier; }
	
	/**
	 * \brief Set fly speed modifier.
	 * 
	 * 0 is normal speed. Every +1 increases speed by 25%. Every -1 decreases speed by 25%.
	 * Minimum is -20 and maximum is 25.
	 */
	void SetSpeedFlyModifier(int multiplier);
	
	/**
	 * \brief Fly speed after applying multiplier.
	 * 
	 * The fly speed is multiplied by "pow(1.25, modifier)". For 3m/s fly speed this results
	 * in a range from 0.01m/s to 260m/s.
	 */
	float GetSpeedFlyModified() const;
	
	
	
	/** \brief Interaction in progress. */
	inline eInteraction GetInteraction() const{ return pInteraction; }
	
	/** \brief Set interaction in progress. */
	void SetInteraction(eInteraction interaction);
	
	
	/** \brief Pan or move position at start of interaction. */
	inline const decDVector &GetMoveOrigin() const{ return pMoveOrigin; }
	
	/** \brief Set pan or move position at start of interaction. */
	void SetMoveOrigin(const decDVector &position);
	
	/** \brief Current pan or move position. */
	inline const decDVector &GetMoveCurrent() const{ return pMoveCurrent; }
	
	/** \brief Set current pan or move position. */
	void SetMoveCurrent(const decDVector &position);
	
	/** \brief Rotation in degrees at start of interaction. */
	inline const decVector &GetRotateOrigin() const{ return pRotateOrigin; }
	
	/** \brief Set rotation in degrees at start of interaction. */
	void SetRotateOrigin(const decVector &rotation);
	
	/** \brief Current rotation in degrees. */
	inline const decVector &GetRotationCurrent() const{ return pRotateCurrent; }
	
	/** \brief Set current rotation in degrees. */
	void SetRotationCurrent(const decVector &rotation);
	
	/** \brief Zoom at start of interaction. */
	inline float GetZoomOrigin() const{ return pZoomOrigin; }
	
	/** \brief Set zoom at start of interaction. */
	void SetZoomOrigin(float zoom);
	
	/** \brief Current zoom. */
	inline float GetZoomCurrent() const{ return pZoomCurrent; }
	
	/** \brief Set current zoom. */
	void SetZoomCurrent(float zoom);
	
	/** \brief Matrix for pan and zoom interaction. */
	inline const decDMatrix &GetMoveMatrix() const{ return pMoveMatrix; }
	
	/** \brief Set matrix for pan and zoom interaction. */
	void SetMoveMatrix(const decDMatrix &matrix);
	
	
	/** \brief Add listeners. */
	void AddListeners(igdeViewRenderWindow &widget);
	
	/** \brief Remove listeners. */
	void RemoveListeners(igdeViewRenderWindow &widget);
	/*@}*/
	
	
	/** \brief Interaction events */
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
	virtual void OnRotateFinish(bool cancelled);
	
	
	/** \brief Panning camera begins. */
	virtual bool OnPanBegin();
	
	/** \brief Update panning. */
	virtual void OnPanUpdate();
	
	/** \brief Panning ends. */
	virtual void OnPanFinish(bool cancelled);
	
	
	/** \brief Moving camera begins. */
	virtual bool OnMoveBegin();
	
	/** \brief Update moving. */
	virtual void OnMoveUpdate();
	
	/** \brief Moving ends. */
	virtual void OnMoveFinish(bool cancelled);
	
	
	/** \brief Zooming camera begins. */
	virtual bool OnZoomBegin();
	
	/** \brief Update zooming. */
	virtual void OnZoomUpdate();
	
	/** \brief Zooming ends. */
	virtual void OnZoomFinish(bool cancelled);
	
	
	/** \brief Begin fly mode. */
	virtual void OnFlyBegin();
	
	/** \brief Update fly mode. */
	virtual void OnFlyUpdate(float elapsed);
	
	/** \brief End fly mode. */
	virtual void OnFlyEnd();
	
	
	/**
	 * \brief Begin camera interaction.
	 * 
	 * Called by On*Begin() methods when camera interaction begins. For use by subclasses to
	 * react to camera interaction without having to overrite the individual begin methods.
	 */
	virtual void OnBeginInteraction();
	
	/**
	 * \brief Camera has been modified.
	 * 
	 * Called by On*Update() methods when camera parameters have been changed. For use by
	 * subclasses to react to camera changes without having to overrite thje individual
	 * update methods. Default implementation does nothing.
	 */
	virtual void OnCameraChanged();
	
	/**
	 * \brief End camera interaction.
	 * 
	 * Called by On*Finish() methods when camera interaction ends. For use by subclasses to
	 * react to camera interaction without having to overrite the individual finish methods.
	 */
	virtual void OnEndInteraction(bool cancelled);
	/*@}*/
	
	
	
	/** \brief events */
	/*@{*/
	virtual bool OnDragBegin();
	virtual void OnDragUpdate();
	virtual void OnDragFinish(bool cancelled);
	
	void OnButtonPress(igdeWidget *widget, int button, const decPoint &position, int modifiers) override;
	void OnMouseMoved(igdeWidget *widget, const decPoint &position, int modifiers) override;
	void OnButtonRelease(igdeWidget *widget, int button, const decPoint &position, int modifiers) override;
	void OnMouseWheeled(igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers) override;
	void OnKeyPress(igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key) override;
	void OnKeyRelease(igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key) override;
	
	virtual void OnFrameUpdate(float elapsed);
	/*@}*/
};

#endif
