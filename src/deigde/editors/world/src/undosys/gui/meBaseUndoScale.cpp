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

#include <stdio.h>
#include <stdlib.h>

#include "meBaseUndoScale.h"
#include "../../worldedit.h"



// class meBaseUndoScale
//////////////////////////

// Constructor, destructor
////////////////////////////

meBaseUndoScale::meBaseUndoScale(){
	pModifyPosition = true;
	pModifySize = true;
	pFactors.Set(1.0f, 1.0f, 1.0f);
	pUniformFactor = 1.0f;
	pScaleUniform = false;
	
	Update();
}

meBaseUndoScale::~meBaseUndoScale(){
}



// Management
///////////////

void meBaseUndoScale::SetModifyPosition(bool modifyPosition){
	pModifyPosition = modifyPosition;
}

void meBaseUndoScale::SetModifySize(bool modifySize){
	pModifySize = modifySize;
}

void meBaseUndoScale::SetFactors(const decVector &factors){
	pFactors = factors;
	Update();
}

void meBaseUndoScale::SetUniformFactor(float factor){
	pUniformFactor = factor;
	Update();
}

void meBaseUndoScale::SetScaleUniform(bool scaleUniform){
	pScaleUniform = scaleUniform;
	Update();
}

void meBaseUndoScale::SetPivot(const decDVector &pivot){
	pPivot = pivot;
	Update();
}

void meBaseUndoScale::Update(){
	// matrix... TODO
	/*
	return decMatrix::CreateTranslation(-p_center)
		* decMatrix::CreateRotationZ( -p_viewRot.z )
		* decMatrix::CreateRotationY( -p_viewRot.y )
		* decMatrix::CreateRotationX( -p_viewRot.x )
		* decMatrix::CreateScale( pScaleX, pScaleY, 1 )
		* decMatrix::CreateRotationX( p_viewRot.x )
		* decMatrix::CreateRotationY( p_viewRot.y )
		* decMatrix::CreateRotationZ( p_viewRot.z )
		* decMatrix::CreateTranslation( p_center );
	*/
	
	// set information
	decString info;
	info.Format("factors(%g,%g,%g;%g) pivot(%g,%g,%g)",
		pFactors.x, pFactors.y, pFactors.z, pUniformFactor, pPivot.x, pPivot.y, pPivot.z);
	SetLongInfo(info);
}

void meBaseUndoScale::TransformElement(decDVector &position, decVector &scaling){
	if(pModifySize){
		if(pScaleUniform){
			scaling.x *= pUniformFactor;
			scaling.y *= pUniformFactor;
			scaling.z *= pUniformFactor;
			
		}else{
			scaling.x *= pFactors.x;
			scaling.y *= pFactors.y;
			scaling.z *= pFactors.z;
		}
	}
	
	if(pModifyPosition){
		position.x = pPivot.x + (position.x - pPivot.x) * (double)pFactors.x;
		position.y = pPivot.y + (position.y - pPivot.y) * (double)pFactors.y;
		position.z = pPivot.z + (position.z - pPivot.z) * (double)pFactors.z;
	}
}



// Undo and Redo actions
//////////////////////////

void meBaseUndoScale::ProgressiveRedo(){
	Undo();
	Redo();
}
