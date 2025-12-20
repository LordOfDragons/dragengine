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
#ifndef _RECLIPBOARD_H_
#define _RECLIPBOARD_H_

// predefinitions
class feBaseClip;



/**
 * @brief Clipboard.
 * Manages a clipboard holding data copied by the user. The data is stored
 * as a clip object which can be subclasses. At most one clip object can
 * be in the clipboard at every time.
 */
class feClipboard{
private:
	feBaseClip *pClip;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new clipboard. */
	feClipboard();
	/** Cleans up the clipboatd. */
	~feClipboard();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if a clip is held by the clipboatd. */
	bool HasClip() const;
	/** Retrieves the clip or nullptr if not set. */
	inline feBaseClip *GetClip() const{ return pClip; }
	/** Sets the held clip replacing the old one if existing. */
	void SetClip(feBaseClip *clip);
	/*@}*/
};

// end of include only once
#endif
