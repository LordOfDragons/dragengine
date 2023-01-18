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

#ifndef _DEOGLDEVELOPERMODETESTS_H_
#define _DEOGLDEVELOPERMODETESTS_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class decUnicodeString;
class decUnicodeArgumentList;
class deException;
class deoglSPBlockUBO;



/**
 * Developer Mode Tests.
 */
class deoglDeveloperModeTests{
private:
	deoglRenderThread &pRenderThread;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new developer mode object. */
	deoglDeveloperModeTests( deoglRenderThread &renderThread );
	/** Cleans up the developer mode object. */
	~deoglDeveloperModeTests();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Print stats syntax. */
	void Help( decUnicodeString &answer );
	/** Tests command. */
	void Tests( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	/** Test 2d convex hull class. */
	void TestConvexHull2D( decUnicodeString &answer );
	
	/** Answer test passed. */
	void AnswerTestPassed( decUnicodeString &answer );
	/** Answer test failed with exception. */
	void AnswerTestFailedWithException( decUnicodeString &answer, const deException &exception );
	/*@}*/
};

#endif
