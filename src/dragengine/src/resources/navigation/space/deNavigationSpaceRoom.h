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

#ifndef _DENAVIGATIONSPACEROOM_H_
#define _DENAVIGATIONSPACEROOM_H_

#include "../../../dragengine_export.h"


/**
 * \brief Navigation Space Room.
 * Room in a navigation space. A valid room composes of 4 or more faces and has to be closed.
 */
class DE_DLL_EXPORT deNavigationSpaceRoom{
private:
	unsigned short pFrontWallCount;
	unsigned short pBackWallCount;
	unsigned short pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space room. */
	deNavigationSpaceRoom();
	
	/** \brief Clean up navigation space room. */
	~deNavigationSpaceRoom();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of front walls. */
	inline unsigned short GetFrontWallCount() const{return pFrontWallCount;}
	
	/** \brief Set number of front walls. */
	void SetFrontWallCount(unsigned short count);
	
	/** \brief Number of back walls. */
	inline unsigned short GetBackWallCount() const{return pBackWallCount;}
	
	/** \brief Set number of back walls. */
	void SetBackWallCount(unsigned short count);
	
	/** \brief Type number. */
	inline unsigned short GetType() const{return pType;}
	
	/** \brief Set type number. */
	void SetType(unsigned short type);
	/*@}*/
};

#endif
