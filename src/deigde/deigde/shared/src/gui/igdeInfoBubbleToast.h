/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEINFOBUBBLETOAST_H_
#define _IGDEINFOBUBBLETOAST_H_

#include "igdeInfoBubble.h"
#include "../environment/igdeEnvironmentListener.h"


/**
 * \brief Toast information bubble.
 */
class igdeInfoBubbleToast : public igdeInfoBubble{
public:
	/** \brief Strong reference. */
	using Ref = deTObjectReference<igdeInfoBubbleToast>;
	
	
	/** \brief State. */
	enum class State{
		/** \brief Hidden. */
		hidden,
		
		/** \brief Fading in. */
		fadeIn,
		
		/** \brief Visible. */
		visible,
		
		/** \brief Fading out. */
		fadeOut
	};
	
	
private:
	class EnvironmentListener : public igdeEnvironmentListener{
	public:
		igdeInfoBubbleToast *toast;
		
		EnvironmentListener(igdeInfoBubbleToast *toast);
		void OnFrameUpdate(float elapsed) override;
		
	protected:
		~EnvironmentListener() override;
	};
	
	
	float pFadeInTime, pFadeOutTime, pVisibleTimePerCharacter;
	float pMaxVisibleTime, pVisibleTime, pElapsed;
	float pToastX, pToastY, pToastTransparency;
	State pState;
	deTObjectReference<EnvironmentListener> pEnvironmentListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create toast information bubble. */
	explicit igdeInfoBubbleToast(igdeViewRenderWindow &view, float order = 2000.0f);
	
protected:
	/** \brief Clean up toast information bubble. */
	~igdeInfoBubbleToast() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Fade-in time in seconds. */
	inline float GetFadeInTime() const{ return pFadeInTime; }
	
	/** \brief Set fade-in time in seconds. */
	void SetFadeInTime(float time);

	/** \brief Fade-out time in seconds. */
	inline float GetFadeOutTime() const{ return pFadeOutTime; }
	
	/** \brief Set fade-out time in seconds. */
	void SetFadeOutTime(float time);
	
	/** \brief Visible time per character in seconds. */
	inline float GetVisibleTimePerCharacter() const{ return pVisibleTimePerCharacter; }
	
	/** \brief Set visible time per character in seconds. */
	void SetVisibleTimePerCharacter(float time);
	
	/** \brief Maximum visible time in seconds. */
	inline float GetMaxVisibleTime() const{ return pMaxVisibleTime; }

	/** \brief Set maximum visible time in seconds. */
	void SetMaxVisibleTime(float time);
	
	/** \brief Visible time in seconds. */
	inline float GetVisibleTime() const{ return pVisibleTime; }
	
	/** \brief Toast position X placement. */
	inline float GetToastX() const{ return pToastX; }
	
	/** \brief Set toast position X placement. */
	void SetToastX(float x);
	
	/** \brief Toast position Y placement. */
	inline float GetToastY() const{ return pToastY; }
	
	/** \brief Set toast position Y placement. */
	void SetToastY(float y);
	
	/** \brief Toast transparency. */
	inline float GetToastTransparency() const{ return pToastTransparency; }

	/** \brief Set toast transparency. */
	void SetToastTransparency(float transparency);
	
	
	/**
	 * \brief Show toast.
	 * 
	 * Supports translation if starting with "@".
	 */
	void ShowToast(const decString &text);
	
	/** \brief Hide toast. */
	void HideToast();
	
	
	/** \brief Calculate visible time based on the number of characters. */
	float CalculateVisibleTime(const decString &text) const;
	/*@}*/
	
	
protected:
	void pUpdateToast(float elapsed);
};

#endif
