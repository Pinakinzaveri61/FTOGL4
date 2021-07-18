#include "pch.h"

#include "G3DVector.h"


G3DVector6::G3DVector6()
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

G3DVector6::G3DVector6(const G3DVector6 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
}

G3DVector6::G3DVector6(ccVTvec xP, ccVTvec yP, ccVTvec zP)
{
	v[0] = xP;
	v[1] = yP;
	v[2] = zP;
}


void G3DVector6::SetV(ccVTvec xP, ccVTvec yP, ccVTvec zP)
{
	v[0] = xP;
	v[1] = yP;
	v[2] = zP;
}

void G3DVector6::SetT(ccVTvec x1P, ccVTvec y1P, ccVTvec z1P)	// Texture coordinate.
{
	t[0] = x1P;
	t[1] = y1P;
	t[2] = z1P;
}

G3DVector6& G3DVector6::operator=(G3DVector &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	return *this;
}


G3DVector6& G3DVector6::operator=(const G3DVector6 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	return *this;
}

/*
G3DVector6& G3DVector6::operator=(G3DVector6 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];

	v[3] = vec.v[3];
	v[4] = vec.v[4];
	v[5] = vec.v[5];

	return *this;
}


G3DVector6 G3DVector6::operator*(ccVTvec s)
{
	return G3DVector6(v[0]*s, v[1]*s, v[2]*s);
}

G3DVector6& G3DVector6::operator*=(ccVTvec s)
{
	v[0] *= s;
	v[1] *= s;
	v[2] *= s;
	return *this;
}

G3DVector6& G3DVector6::operator*=(G3DVector6 &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	v[2] *= vec.v[2];
	return *this;
}


G3DVector6 G3DVector6::operator*(G3DVector6 &vec)
{
	return G3DVector6(v[0]*vec.v[0], v[1]*vec.v[1], v[2]*vec.v[2]);
}




G3DVector6 G3DVector6::operator/(ccVTvec s)
{
	return G3DVector6(v[0]/s, v[1]/s, v[2]/s);
}

G3DVector6& G3DVector6::operator/=(ccVTvec s)
{
	v[0] /= s;
	v[1] /= s;
	v[2] /= s;
	return *this;
}

G3DVector6 G3DVector6::operator/(G3DVector6 &vec)
{
	return G3DVector6(v[0]/vec.v[0], v[1]/vec.v[1], v[2]/vec.v[2]);
}

G3DVector6& G3DVector6::operator/=(G3DVector6 &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	v[2] /= vec.v[2];
	return *this;
}













*/


G3DVector G3DVector6::operator+(G3DVector6& vec)
{
	return G3DVector(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2]);

}

G3DVector6 G3DVector6::operator+(G3DVector &vec)
{
	return G3DVector6(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2]);
	
}

G3DVector6& G3DVector6::operator+=(G3DVector &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	v[2] += vec.v[2];
	return *this;
}



G3DVector6 G3DVector6::operator-(G3DVector &vec)
{
	return G3DVector6(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2]);
	
}

G3DVector6& G3DVector6::operator-=(G3DVector &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	v[2] -= vec.v[2];
	return *this;
}


/*
bool G3DVector6::operator==(G3DVector6 &vec)
{
	return (v[0]==vec.v[0] && v[1]==vec.v[1] && v[2]==vec.v[2]);
}

void G3DVector6::Normalize(ccVTvec n)
{
	ccVTvec d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])/n;
	v[0]/= d;
	v[1]/= d;
	v[2]/= d;
}
*/


/*
G3DVector6 G3DVector6::MidPt(G3DVector6 vec1, G3DVector6 vec2)
{
	G3DVector6 vecMid;

	vecMid = vec2 - vec1;
	vecMid /= 2;
	return G3DVector6(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2]);
}
*/

/*
void G3DVector6::UnitVector()
{
//	normalize, makes the vector length 1
	ccVTvec length;

	length = x * x + y * y + z * z;
	length = sqrt(length);		// sqrt(x^2 + y^2 + z^2)
	//divides vector by its length to normalise		
	x /= length;
	y /= length;
	z /= length;

}

void G3DVector6::ModVector()
{
//	Conver Value to +Ve.
	if (x < 0.0) x = -x;
	if (y < 0.0) y = -y;
	if (z < 0.0) z = -z;
}


double G3DVector6::DotV(G3DVector6 v)
{
	double dotProL;

	dotProL = (x * v.x) + (y * v.y) + (z * v.z);

	return dotProL;
}


// Returns the corss product (Normal vector) of this and passed vector.
G3DVector6 G3DVector6::CrossV(G3DVector6 v)
{
	G3DVector6 crosProL;

	crosProL.x = (y * v.z) - (z * v.y);
	crosProL.y = (z * v.x) - (x * v.z);
	crosProL.z = (x * v.y) - (y * v.x);

	return crosProL;
}



// Finds the corss product of passed 3 vertices.
G3DVector6 G3DVector6::CrossVer(G3DVector6 vertx1, G3DVector6 vertx2, G3DVector6 vertx3)
{
	G3DVector6 crosProL;

	G3DVector6 vect1;
	G3DVector6 vect2;

	// Find two vector from 3 vertices.
	vect1 = vertx1 - vertx2;
	vect2 = vertx2 - vertx3;

	// Get the noraml by cross product.
	crosProL = vect1.CrossV(vect2);

	return crosProL;
}

*/


/*

//	******************		**********************


bool G3DVector6::IsMoreThanAll(G3DVector6 Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x > Vec2.x) && (y > Vec2.y) && (z > Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DVector6::IsMoreThanEqAll(G3DVector6 Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x >= Vec2.x) && (y >= Vec2.y) && (z >= Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DVector6::IsMoreThanEqAllAbs(G3DVector6 Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	G3DVector6 Vec;
	Vec.SetV(x, y, z);
	Vec.ModVector();

	Vec2.ModVector();

	if ( (Vec.x >= Vec2.x) && (Vec.y >= Vec2.y) && (Vec.z >= Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}


bool G3DVector6::IsMoreThanAny(G3DVector6 Vec2)	// i.e. x>x1 or y>y1 or z>z1
{
	if ( (x > Vec2.x) || (y > Vec2.y) || (z > Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

// ***********
bool G3DVector6::IsLessThanAll(G3DVector6 Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x < Vec2.x) && (y < Vec2.y) && (z < Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DVector6::IsLessThanEqAll(G3DVector6 Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x <= Vec2.x) && (y <= Vec2.y) && (z <= Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DVector6::IsLessThanEqAllAbs(G3DVector6 Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	G3DVector6 Vec;
	Vec.SetV(x, y, z);

	Vec.ModVector();
	Vec2.ModVector();

	Vec  *= 0.1;	// Reduce accuracy for better comparision. (Compiler sometime do not compare = for same looking value)
	Vec2 *= 0.1;

	if ( (Vec.x <= Vec2.x) && (Vec.y <= Vec2.y) && (Vec.z <= Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DVector6::IsLessThanAny(G3DVector6 Vec2)	// i.e. x>x1 or y>y1 or z>z1
{
	if ( (x < Vec2.x) || (y < Vec2.y) || (z < Vec2.z) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DVector6::IsInSameDirectionVS(G3DVector6 Vec2, int viewSide)	// i.e. x>x1 or y>y1 or z>z1
{
	bool retVal = true;

	switch(viewSide)
	{
	case VIEW_FRONT:
	case VIEW_BACK:
		if ( (x < 0) && (Vec2.x > 0)) retVal = false;
		if ( (x > 0) && (Vec2.x < 0)) retVal = false;

		if ( (y < 0) && (Vec2.y > 0)) retVal = false;
		if ( (y > 0) && (Vec2.y < 0)) retVal = false;
		break;

	case VIEW_TOP:
	case VIEW_BOTTOM:
		if ( (x < 0) && (Vec2.x > 0)) retVal = false;
		if ( (x > 0) && (Vec2.x < 0)) retVal = false;

		if ( (z < 0) && (Vec2.z > 0)) retVal = false;
		if ( (z > 0) && (Vec2.z < 0)) retVal = false;
		break;

	case VIEW_LEFT:
	case VIEW_RIGHT:
		if ( (y < 0) && (Vec2.y > 0)) retVal = false;
		if ( (y > 0) && (Vec2.y < 0)) retVal = false;

		if ( (z < 0) && (Vec2.z > 0)) retVal = false;
		if ( (z > 0) && (Vec2.z < 0)) retVal = false;
		break;
	}

	return retVal;
}


bool G3DVector6::IsInSameDirection(G3DVector6 Vec2)	// i.e. x>x1 or y>y1 or z>z1
{
	bool retVal = true;

	if ( (x < 0) && (Vec2.x > 0)) retVal = false;
	if ( (x > 0) && (Vec2.x < 0)) retVal = false;

	if ( (y < 0) && (Vec2.y > 0)) retVal = false;
	if ( (y > 0) && (Vec2.y < 0)) retVal = false;

	return retVal;
}

*/


// ***********

/*
bool G3DVector6::NotZeroVector()
{
	if ((x == 0) && (y == 0) && (z == 0)) 
	{
		return false;
	}else
	{
		return true;
	}
}


bool G3DVector6::AlmostZeroVector()
{
	if ((x < 0.0000001) && (y < 0.0000001) && (z < 0.0000001)) 
	{
		return true;
	}else
	{
		return false;
	}
}


void G3DVector6::GetModuleVerF(float* moduleVerF)
{
	*moduleVerF = 1.3f;
}


*/

































/*
G3DCameraVector::G3DCameraVector()
{
	camTranslate.SetV(0,0,0);			// Shifting of Scene so viewed properly in Camera.
	camRotation.SetV(0,0,0);			// Rotaion of Scene.

	camRotShift.SetV(0,0,0);		// Rotaional Axis of Scene.

	fov = 70.0;					// Field of View 

}
*/

