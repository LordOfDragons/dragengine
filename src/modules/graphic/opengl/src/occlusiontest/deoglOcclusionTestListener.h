/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLOCCLUSIONTESTLISTENER_H_
#define _DEOGLOCCLUSIONTESTLISTENER_H_



/**
 * @brief Occlusion Test Listener.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new occlusion test listener. */
	deoglOcclusionTestListener();
	/** Cleans up the occlusion test listener. */
	virtual ~deoglOcclusionTestListener();
	/*@}*/
	
	/** @name Listening */
	/*@{*/
	/** The occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible() = 0;
	/*@}*/
};

#endif
