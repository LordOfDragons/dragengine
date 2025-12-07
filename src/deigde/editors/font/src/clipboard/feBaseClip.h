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

// include only once
#ifndef _FEBASECLIP_H_
#define _FEBASECLIP_H_



/**
 * @brief Base Clipboard Clip.
 * Base class for clips to be held by a clipboard object.
 */
class feBaseClip{
public:
	enum eClipTypes{
		ectObject
	};
	
private:
	int pType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new base clipboard clip. */
	feBaseClip(int type);
	/** Cleans up the clipboard clip. */
	virtual ~feBaseClip();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the type of the clip. */
	inline int GetType() const{ return pType; }
	/*@}*/
};

// end of include only once
#endif
