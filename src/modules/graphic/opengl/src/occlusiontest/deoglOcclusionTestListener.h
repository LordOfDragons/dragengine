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

#ifndef _DEOGLOCCLUSIONTESTLISTENER_H_
#define _DEOGLOCCLUSIONTESTLISTENER_H_



/**
 * Occlusion Test Listener.
 * Listener for occlusion testing called during evaluation of test results. This listener might
 * look strange as it contains only one function. This is due to speed reasons. Occlusion testing
 * is used to flag elements as invisible. Hence elements are considered visible unless proven
 * otherwise. This allows to avoid a bunch of function calls. While starting an occlusion test
 * the element sets itself visible. This lines up with the case of no occlusion testing done to
 * begin with. In this situation we are only interested in a notification if the element is
 * found to be invisible. Sending a notification if the element is found to be visible is a waste
 * of time. Hence only one function exists.
 */
class deoglOcclusionTestListener{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new occlusion test listener. */
	deoglOcclusionTestListener();
	/** Cleans up the occlusion test listener. */
	virtual ~deoglOcclusionTestListener();
	/*@}*/
	
	/** \name Listening */
	/*@{*/
	/** The occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible() = 0;
	/*@}*/
};

#endif
