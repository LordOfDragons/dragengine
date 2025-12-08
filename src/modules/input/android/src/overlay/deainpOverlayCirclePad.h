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

#ifndef _DEAINPOVERLAYCIRCLEPAD_H_
#define _DEAINPOVERLAYCIRCLEPAD_H_

#include "deainpInputBinding.h"
#include "deainpOverlay.h"

#include <dragengine/resources/canvas/deCanvasImage.h>


/**
 * \brief Circle pad overlay.
 */
class deainpOverlayCirclePad : public deainpOverlay{
public:
	typedef deTObjectReference<deainpOverlayCirclePad> Ref;
	
	
private:
	deainpInputBinding pAxisLeftRight;
	deainpInputBinding pAxisUpDown;
	
	deainpInputBinding pKeyCodeLeft;
	deainpInputBinding pKeyCodeUp;
	deainpInputBinding pKeyCodeRight;
	deainpInputBinding pKeyCodeDown;
	
	deainpInputBinding pKeyCodeUpLeft;
	deainpInputBinding pKeyCodeUpRight;
	deainpInputBinding pKeyCodeDownLeft;
	deainpInputBinding pKeyCodeDownRight;
	
	bool pPressedLeft;
	bool pPressedUp;
	bool pPressedRight;
	bool pPressedDown;
	
	bool pPressedUpLeft;
	bool pPressedUpRight;
	bool pPressedDownLeft;
	bool pPressedDownRight;
	
	int pPadRadius;
	float pOuterRadius;
	float pInnerRadius;
	decPoint pCenter;
	
	float pLeftRightPerSecond;
	float pUpDownPerSecond;
	
	deCanvasView *pCanvas;
	deCanvasImage::Ref pCImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create overlay circle pad. */
	deainpOverlayCirclePad(deAndroidInput &androidInput);
	
	/** \brief Clean up overlay circle pad. */
	~deainpOverlayCirclePad() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Left-right axis. */
	inline const deainpInputBinding &GetAxisLeftRight() const{ return pAxisLeftRight; }
	
	/** \brief Set left-right axis. */
	void SetAxisLeftRight(const deainpInputBinding &axis);
	
	/** \brief Up-down axis. */
	inline const deainpInputBinding &GetAxisUpDown() const{ return pAxisUpDown; }
	
	/** \brief Set up-down axis. */
	void SetAxisUpDown(const deainpInputBinding &axis);
	
	
	
	/** \brief Key binding for left pad press. */
	inline const deainpInputBinding &GetKeyCodeLeft() const{ return pKeyCodeLeft; }
	
	/** \brief Set key binding for left pad press. */
	void SetKeyCodeLeft(const deainpInputBinding &binding);
	
	/** \brief Key binding for up pad press. */
	inline const deainpInputBinding &GetKeyCodeUp() const{ return pKeyCodeUp; }
	
	/** \brief Set key binding for up pad press. */
	void SetKeyCodeUp(const deainpInputBinding &binding);
	
	/** \brief Key binding for right pad press. */
	inline const deainpInputBinding &GetKeyCodeRight() const{ return pKeyCodeRight; }
	
	/** \brief Set key binding for right pad press. */
	void SetKeyCodeRight(const deainpInputBinding &binding);
	
	/** \brief Key binding for down pad press. */
	inline const deainpInputBinding &GetKeyCodeDown() const{ return pKeyCodeDown; }
	
	/** \brief Set key binding for down pad press. */
	void SetKeyCodeDown(const deainpInputBinding &binding);
	
	
	
	/** \brief Key binding for up-left pad press or empty to activate up and left binding together. */
	inline const deainpInputBinding &GetKeyCodeUpLeft() const{ return pKeyCodeUpLeft; }
	
	/** \brief Set key binding for up-left pad press or empty to activate up and left binding together. */
	void SetKeyCodeUpLeft(const deainpInputBinding &binding);
	
	/** \brief Key binding for up-right pad press or empty to activate up and right binding together. */
	inline const deainpInputBinding &GetKeyCodeUpRight() const{ return pKeyCodeUpRight; }
	
	/** \brief Set key binding for up-right pad press or empty to activate up and right binding together. */
	void SetKeyCodeUpRight(const deainpInputBinding &binding);
	
	/** \brief Key binding for down-left pad press or empty to activate down and left binding together. */
	inline const deainpInputBinding &GetKeyCodeDownLeft() const{ return pKeyCodeDownLeft; }
	
	/** \brief Set key binding for down-left pad press or empty to activate down and left binding together. */
	void SetKeyCodeDownLeft(const deainpInputBinding &binding);
	
	/** \brief Key binding for down-right pad press or empty to activate down and right binding together. */
	inline const deainpInputBinding &GetKeyCodeDownRight() const{ return pKeyCodeDownRight; }
	
	/** \brief Set key binding for down-right pad press or empty to activate down and right binding together. */
	void SetKeyCodeDownRight(const deainpInputBinding &binding);
	
	
	
	/** \brief Left button is pressed. */
	inline bool GetPressedLeft() const{ return pPressedLeft; }
	
	/** \brief Set if left button is pressed. */
	void SetPressedLeft(bool pressed);
	
	/** \brief Up button is pressed. */
	inline bool GetPressedUp() const{ return pPressedUp; }
	
	/** \brief Set if up button is pressed. */
	void SetPressedUp(bool pressed);
	
	/** \brief Right button is pressed. */
	inline bool GetPressedRight() const{ return pPressedRight; }
	
	/** \brief Set if right button is pressed. */
	void SetPressedRight(bool pressed);
	
	/** \brief Down button is pressed. */
	inline bool GetPressedDown() const{ return pPressedDown; }
	
	/** \brief Set if down button is pressed. */
	void SetPressedDown(bool pressed);
	
	
	
	/** \brief Up-left button is pressed. */
	inline bool GetPressedUpLeft() const{ return pPressedUpLeft; }
	
	/** \brief Set up-left button is pressed. */
	void SetPressedUpLeft(bool pressed);
	
	/** \brief Up-right button is pressed. */
	inline bool GetPressedUpRight() const{ return pPressedUpRight; }
	
	/** \brief Set up-right button is pressed. */
	void SetPressedUpRight(bool pressed);
	
	/** \brief Down-left button is pressed. */
	inline bool GetPressedDownLeft() const{ return pPressedDownLeft; }
	
	/** \brief Set down-left button is pressed. */
	void SetPressedDownLeft(bool pressed);
	
	/** \brief Down-right button is pressed. */
	inline bool GetPressedDownRight() const{ return pPressedDownRight; }
	
	/** \brief Set down-right button is pressed. */
	void SetPressedDownRight(bool pressed);
	
	
	
	/** \brief Set all pressed states. */
	void SetPressedAll(bool left, bool up, bool right, bool down,
		bool upLeft, bool upRight, bool downLeft, bool downRight);
	
	
	/** \brief Pad radius in pixels. */
	inline int GetPadRadius() const{ return pPadRadius; }
	
	/** \brief Set pad radius in pixels. */
	void SetPadRadius(int radius);
	
	/** \brief Outer radius in percentage of pad radius. */
	inline float GetOuterRadius() const{ return pOuterRadius; }
	
	/** \brief Set outer radius in percentage of pad radius. */
	void SetOuterRadius(float radius);
	
	/** \brief Inner radius in percentage of pad radius. */
	inline float GetInnerRadius() const{ return pInnerRadius; }
	
	/** \brief Set inner radius in percentage of pad radius. */
	void SetInnerRadius(float radius);
	
	/** \brief Center of pad. */
	inline const decPoint &GetCenter() const{ return pCenter; }
	
	/** \brief Set center of pad. */
	void SetCenter(const decPoint &center);
	
	
	
	/** \brief Axis unit per second to move for left-right axis. */
	inline float GetLeftRightPerSecond() const{ return pLeftRightPerSecond; }
	
	/** \brief Set axis unit per second to move for left-right axis. */
	void SetLeftRightPerSecond(float unitsPerSecond);
	
	/** \brief Axis unit per second to move for up-down axis. */
	inline float GetUpDownPerSecond() const{ return pUpDownPerSecond; }
	
	/** \brief Set axis unit per second to move for up-down axis. */
	void SetUpDownPerSecond(float unitsPerSecond);
	
	
	
	/** \brief Generate input events if pointer is set. */
	void GenerateEvents();
	
	
	
	/** \brief Update binding indices. */
	void UpdateBindingIndices() override;
	
	/** \brief Update canvas content. */
	void UpdateContent() override;
	
	/**
	 * \brief Touch event.
	 * \details Overlay receives upcoming move events until release if \em true is returned.
	 *          Default implementation returns \em false.
	 * \param[in] position Touch position.
	 * \returns \em true if event has been handled or \em false otherwise.
	 */
	virtual bool OnTouch(int pointerId, const decPoint &position);
	
	/**
	 * \brief Move event.
	 * \param[in] position Move distance since last move event.
	 */
	virtual void OnMove(const decPoint &position);
	
	/** \brief Release. */
	void OnRelease() override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
