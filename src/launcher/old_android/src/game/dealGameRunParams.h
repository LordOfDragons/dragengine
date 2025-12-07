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

#ifndef _DEALGAMERUNPARAMS_H_
#define _DEALGAMERUNPARAMS_H_

#include "../common/string/decString.h"

class dealGameProfile;



/**
 * \brief Game Run Parameters.
 * 
 * Stores the parameters for running a game. Usually the parameters are taken
 * from the game itself but in some situations temporary parameters can be
 * used. In all cases this class provides the actual parameters with which a
 * game is run no matter where they come from.
 * 
 */
class dealGameRunParams{
private:
	dealGameProfile *pGameProfile;
	decString pRunArguments;
	int pWidth;
	int pHeight;
	bool pFullScreen;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game run parameters. */
	dealGameRunParams();
	
	/** \brief Cleans up the game run parameters. */
	~dealGameRunParams();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game profile. */
	inline dealGameProfile *GetGameProfile() const{ return pGameProfile; }
	
	/** \brief Set game profile. */
	void SetGameProfile(dealGameProfile *profile);
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArguments; }
	
	/** \brief Set run arguments. */
	void SetRunArguments(const char *arguments);
	
	/** \brief Window width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set window width. */
	void SetWidth(int width);
	
	/** \brief Window height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set window height. */
	void SetHeight(int height);
	
	/** \brief Window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if window is full screen. */
	void SetFullScreen(bool fullScreen);
	/*@}*/
};

#endif
