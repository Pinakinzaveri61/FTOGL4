/*
 * G3DVector - ConsCAD utility library (Reduced, Limited functions Library)
 *
 * Copyright (c) 2019 Pinakin Zaveri <Pinakin.Zaveri@rediff.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */



#include "pch.h"

#include "G3DVector.h"
#include "LibComn.h"


G3DVector::G3DVector()
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}


G3DVector::G3DVector(ccVTvec xP, ccVTvec yP, ccVTvec zP)
{
	v[0] = xP;
	v[1] = yP;
	v[2] = zP;
}

// Special copy constructor
G3DVector::G3DVector(const G3DVector &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}



// Special = operator 
G3DVector& G3DVector::operator=(const G3DVector &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	return *this;
}

bool G3DVector::operator==(G3DVector &vec)
{
	return (v[0]==vec.v[0] && v[1]==vec.v[1] && v[2]==vec.v[2]);
}

void G3DVector::Normalize(ccVTvec n)
{
	ccVTvec d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])/n;
	v[0]/= d;
	v[1]/= d;
	v[2]/= d;
}


// ***********

void G3DVector::SetV(ccVTvec xP, ccVTvec yP, ccVTvec zP)
{
	v[0] = xP;
	v[1] = yP;
	v[2] = zP;
}

void G3DVector::GetModuleVerF(float* moduleVerF)
{
	*moduleVerF = 1.3f;
}
