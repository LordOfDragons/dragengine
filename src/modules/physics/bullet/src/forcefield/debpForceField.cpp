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
#include <string.h>

#include "debpForceField.h"
#include "debpFFVortex.h"

#include "../world/debpWorld.h"

#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/common/exceptions.h>



#ifndef random
#define random rand
#endif

// Class debpForceField
/////////////////////////

// Constructor, destructor
////////////////////////////

debpForceField::debpForceField(dePhysicsBullet &bullet, const deForceField &forceField) :
pBullet(bullet),
pForceField(forceField),

pVortexCount(0),
pTimeUntilNextVortex(0.5f),
pDirtyGeometry(true){
}



// Management
///////////////

void debpForceField::Update(float elapsed){
	pUpdateGeometry();
	
	// update next vortex timer and add one if elapsed
	pTimeUntilNextVortex -= elapsed;
	if(pTimeUntilNextVortex <= 0.0f){
		pTimeUntilNextVortex = 0.5f;
		
		if(pVortexCount == pVortices.GetCount()){
			pVortices.Add(deTUniqueReference<debpFFVortex>::New());
		}
		
		float fieldRadius = pForceField.GetRadius();
		float rfactor = 1.0f / (float)RAND_MAX;
		decVector position, view;
		float size, velocity, timeToLive;
		float arc1, arc2, dist;
		
		size = 0.1f + (float)random() * rfactor * 9.9f; // 0.1 -> 10m
		
		arc1 = (float)random() * rfactor * (PI * 2.0f);
		dist = (float)random() * rfactor * (fieldRadius - size);
		
		position.x = sinf(arc1) * dist;
		position.y = 0.0f;
		position.z = cosf(arc1) * dist;
		
		arc1 = (float)random() * rfactor * (PI * 2.0f);
		arc2 = (float)random() * rfactor * PI;
		
		view.x = sinf(arc1) * cosf(arc2);
		view.y = sinf(arc2);
		view.z = cosf(arc1) * cosf(arc2);
		
		velocity = (float)random() * rfactor * pForceField.GetForce();
		timeToLive = 0.5f + (float)random() * rfactor * 9.5f; // 0.5 -> 10s
		
		debpFFVortex &v = pVortices.GetAt(pVortexCount);
		v.SetPosition(position);
		v.SetView(view);
		v.SetSize(size);
		v.SetFullVelocity(velocity);
		v.SetTimeToLive(timeToLive);
		v.Update();
		pVortexCount++;
		
		//pBullet->LogInfoFormat( "Added Vortex: p=(%g,%g,%g) v=(%g,%g,%g) s=%g ve=%g ttl=%g", position.x, position.y, position.z,
		//	view.x, view.y, view.z, size, velocity, timeToLive );
	}
	
	// update vortices
	int i;
	for(i=0; i<pVortexCount; i++){
		debpFFVortex &vortex = pVortices.GetAt(i);
		vortex.IncreaseLivedTime(elapsed);
		if(vortex.HasDied()){
			//pBullet->LogInfoFormat("Vortex Died: p=(%g,%g,%g) v=(%g,%g,%g) s=%g ve=%g", vortex->GetPosition().x, vortex->GetPosition().y, vortex->GetPosition().z,
			//	vortex->GetView().x, vortex->GetView().y, vortex->GetView().z, vortex->GetSize(), vortex->GetFullVelocity());
			pVortices.Move(i, pVortexCount);
			pVortexCount--;
			
		}else{
			vortex.Update();
		}
	}
}



/*
void debpForceField::GetForceAtPoint(const decVector &position, decVector &force){
	const float flucAngle = DEG2RAD * 180.0f;
	const float ffForce = pForceField.GetForce();
	const float ffRadius = pForceField.GetRadius();
	const float ffExponent = pForceField.GetExponent();
	const float ffFluctuation = pForceField.GetFluctuation();
	const int ffType = pForceField.GetType();
	decVector direction;
	decMatrix flucmat;
	float distance;
	float addvelo, addtorque, force;
	
		direction.x = (float)(particle.position.x - ffpos.x);
		direction.y = (float)(particle.position.y - ffpos.y);
		direction.z = (float)(particle.position.z - ffpos.z);
		
		distance = direction.Length();
		
		if(distance > 1e-5f && distance < ffRadius){
			if(ffType == deForceField::eftLinear){
				direction = ffDir;
				
			}else{
				direction /= distance;
			}
			
			flucmat.SetRotationY(pFluctDir * ffFluctuation * flucAngle);
			force = (ffForce * powf(1.0f - distance / ffRadius, ffExponent))
				* ( 1.0f + ( pFluctStr * ffFluctuation ) * 0.25f ) * elapsed;
			
			if(pParticleEmitter->GetUseLocalGravity()){
				addvelo = force * 0.02f;
				addtorque = 0.0f; //force * (DEG2RAD * 45.0f);
				
			}else{
				addvelo = force * 0.2f;
				addtorque = 0.0f;
			}
			
			direction = flucmat.TransformNormal(direction);
			particle.linearVelocity += direction * addvelo;
			particle.angularVelocity.x += addtorque; // += direction * addtorque;
		}
	}
}
*/



// Vortices
/////////////

debpFFVortex *debpForceField::GetVortexAt(int index) const{
	if(index < 0 || index >= pVortexCount){
		DETHROW(deeInvalidParam);
	}
	return pVortices[index];
}



// Notifications
//////////////////

void debpForceField::PositionChanged(){
	pDirtyGeometry = true;
}

void debpForceField::OrientationChanged(){
	pDirtyGeometry = true;
}

void debpForceField::InfluenceAreaChanged(){
}

void debpForceField::RadiusChanged(){
}

void debpForceField::ExponentChanged(){
}

void debpForceField::TypeChanged(){
}

void debpForceField::DirectionChanged(){
}

void debpForceField::ForceChanged(){
}

void debpForceField::FluctuationChanged(){
}

void debpForceField::CollisionFilterChanged(){
}

void debpForceField::EnabledChanged(){
}



// Private functions
//////////////////////

void debpForceField::pUpdateGeometry(){
	if(!pDirtyGeometry){
		return;
	}
	
	const decMatrix matRot(decMatrix::CreateFromQuaternion(pForceField.GetOrientation()));
	pDirection = matRot.TransformView();
	
	pVortexMatrix = decMatrix::CreateRotationY(pForceField.GetFluctuationDirection()
		* DEG2RAD * pForceField.GetRadius() ) * matRot;
	
	pDirtyGeometry = false;
}
