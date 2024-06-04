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

#include <string.h>
#include <stdio.h>

#include "decShape.h"
#include "decShapeBox.h"
#include "decShapeSphere.h"
#include "decShapeCapsule.h"
#include "decShapeCylinder.h"
#include "decShapeHull.h"
#include "decShapeVisitorIdentify.h"

#include "../exceptions.h"



// Class decShapeVisitorIdentify
//////////////////////////////////

// Constructors, destructors
//////////////////////////////

decShapeVisitorIdentify::decShapeVisitorIdentify() :
pShape( NULL ),
pType( estUnknown ){
}

decShapeVisitorIdentify::~decShapeVisitorIdentify(){
}



// Visiting
/////////////

decShapeSphere &decShapeVisitorIdentify::CastToSphere() const{
	if( pType != estSphere ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeSphere* )pShape );
}

decShapeBox &decShapeVisitorIdentify::CastToBox() const{
	if( pType != estBox ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeBox* )pShape );
}

decShapeCylinder &decShapeVisitorIdentify::CastToCylinder() const{
	if( pType != estCylinder ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeCylinder* )pShape );
}

decShapeCapsule &decShapeVisitorIdentify::CastToCapsule() const{
	if( pType != estCapsule ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeCapsule* )pShape );
}

decShapeHull &decShapeVisitorIdentify::CastToHull() const{
	if( pType != estHull ){
		DETHROW( deeInvalidParam );
	}
	return *( ( decShapeHull* )pShape );
}


void decShapeVisitorIdentify::Reset(){
	pShape = NULL;
	pType = estUnknown;
}


void decShapeVisitorIdentify::VisitShape( decShape &shape ){
	pShape = &shape;
	pType = estUnknown;
}

void decShapeVisitorIdentify::VisitShapeSphere( decShapeSphere &sphere ){
	pShape = &sphere;
	pType = estSphere;
}

void decShapeVisitorIdentify::VisitShapeBox( decShapeBox &box ){
	pShape = &box;
	pType = estBox;
}

void decShapeVisitorIdentify::VisitShapeCylinder( decShapeCylinder &cylinder ){
	pShape = &cylinder;
	pType = estCylinder;
}

void decShapeVisitorIdentify::VisitShapeCapsule( decShapeCapsule &capsule ){
	pShape = &capsule;
	pType = estCapsule;
}

void decShapeVisitorIdentify::VisitShapeHull( decShapeHull &hull ){
	pShape = &hull;
	pType = estHull;
}
