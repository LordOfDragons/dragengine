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



/**
 * @brief 4 Component Boundary for rectangular areas.
 */
class decBoundary{
public:
	/** Top Left corner X coordinate. */
	int x1;
	/** Top Left corner Y coordinate. */
	int y1;
	/** Bottom Right corner X coordinate. */
	int x2;
	/** Bottom Right corner Y coordinate. */
	int y2;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new boundary object initialized with all components to 0. */
	inline decBoundary(){
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
	}
	/** Creates a new boundary object with the given values. */
	inline decBoundary( int nx1, int ny1, int nx2, int ny2 ){
		x1 = nx1;
		y1 = ny1;
		x2 = nx2;
		y2 = ny2;
	}
	/** Creates a new boundary object with the values of another boundary object. */
	inline decBoundary( const decBoundary &b ){
		x1 = b.x1;
		y1 = b.y1;
		x2 = b.x2;
		y2 = b.y2;
	}
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Rearranges the coordinates so that x1/y1 are the top left corner and x2/y2 the bottom right corner. */
	void OrientateRect();
	/**
	 * Rearranges the coordinates so that x1/y1 are the top left corner and x2/y2 the bottom right corner.
	 * This version is designed to oriente a line. The function will though most probably be
	 * replaced with something else. You should thus not use it if possible.
	 */
	void OrientateLine();
	/** Returns the point formed by adding the given point to the top left corner point. */
	inline decPoint MapPoint( const decPoint &pt ) const{ return decPoint( x1 + pt.x, y1 + pt.y ); }
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Sets the components of this boundary object to the values of another one. */
	decBoundary &operator=( const decBoundary &b );
	/*@}*/
};
