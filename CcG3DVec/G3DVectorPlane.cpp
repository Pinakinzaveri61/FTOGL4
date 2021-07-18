#include "pch.h"

#include "G3DVector.h"
#include "LibComn.h"

G3DPlane::G3DPlane()
{

	x = 0.0;
	y = 0.0;
	z = 0.0;
	d = 0.0;
}


G3DPlane::G3DPlane(const G3DPlane &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	v[3] = vec.v[3];
}


G3DPlane::G3DPlane(ccVTvec xP, ccVTvec yP, ccVTvec zP, ccVTvec dP)
{
	A = xP;
	B = yP;
	C = zP;
	D = dP;
}




G3DPlane& G3DPlane::operator=(const G3DPlane &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	v[3] = vec.v[3];
	return *this;
}


G3DPlane G3DPlane::operator*(ccVTvec s)
{
	return G3DPlane(v[0]*s, v[1]*s, v[2]*s, v[3]*s);
	// return G3DVector(v[0]*s, v[1]*s, v[2]*s);


}

G3DPlane& G3DPlane::operator*=(ccVTvec s)
{
	v[0] *= s;
	v[1] *= s;
	v[2] *= s;
	v[3] *= s;
	return *this;
}


G3DPlane& G3DPlane::operator*=(G3DPlane &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	v[2] *= vec.v[2];
	v[3] *= vec.v[3];
	return *this;
}


G3DPlane G3DPlane::operator*(G3DPlane &vec)
{
	return G3DPlane(v[0]*vec.v[0], v[1]*vec.v[1], v[2]*vec.v[2], v[3]*vec.v[3]);

	
}




G3DPlane G3DPlane::operator/(ccVTvec s)
{
	 return G3DPlane(v[0]/s, v[1]/s, v[2]/s, v[3]/s);

	
}

G3DPlane& G3DPlane::operator/=(ccVTvec s)
{
	v[0] /= s;
	v[1] /= s;
	v[2] /= s;
	v[3] /= s;
	return *this;
}

G3DPlane G3DPlane::operator/(G3DPlane &vec)
{
	return G3DPlane(v[0]/vec.v[0], v[1]/vec.v[1], v[2]/vec.v[2], v[3]/vec.v[3]);

	
}

G3DPlane& G3DPlane::operator/=(G3DPlane &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	v[2] /= vec.v[2];
	v[3] /= vec.v[3];
	return *this;
}


G3DPlane G3DPlane::operator+(G3DPlane &vec)
{
	return G3DPlane(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2], v[3]+vec.v[3]);
	
}

G3DPlane& G3DPlane::operator+=(G3DPlane &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	v[2] += vec.v[2];
	v[3] += vec.v[3];
	return *this;
}

G3DPlane G3DPlane::operator-(G3DPlane &vec)
{
	return G3DPlane(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2], v[3]-vec.v[3]);
	
}


G3DPlane& G3DPlane::operator-=(G3DPlane &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	v[2] -= vec.v[2];
	v[3] -= vec.v[3];
	return *this;
}

bool G3DPlane::operator==(G3DPlane &vec)
{
	return (v[0]==vec.v[0] && v[1]==vec.v[1] && v[2]==vec.v[2] && v[3]==vec.v[3]);
}

void G3DPlane::Normalize(ccVTvec n)
{
	ccVTvec d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])/n;
	v[0]/= d;
	v[1]/= d;
	v[2]/= d;
	v[3]/= d;
}

/*
G3DPlane G3DPlane::MidPt(G3DPlane vec1, G3DPlane vec2)
{
	G3DPlane vecMid;

	vecMid = vec2 - vec1;
	vecMid /= 2;
	return G3DPlane(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2]);
}
*/

void G3DPlane::UnitVector()
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

void G3DPlane::ModVector()
{
//	Conver Value to +Ve.
	if (x < 0.0) x = -x;
	if (y < 0.0) y = -y;
	if (z < 0.0) z = -z;
	if (d < 0.0) d = -d;
}


double G3DPlane::DotV(G3DPlane vec)
{
	double dotProL;

	dotProL = (x * vec.x) + (y * vec.y) + (z * vec.z);

	return dotProL;
}


// Returns the corss product (Normal vector) of this and passed vector.
G3DPlane G3DPlane::CrossV(G3DPlane vec)
{
	G3DPlane crosProL;

	crosProL.x = (y * vec.z) - (z * vec.y);
	crosProL.y = (z * vec.x) - (x * vec.z);
	crosProL.z = (x * vec.y) - (y * vec.x);

	return crosProL;
}



// Finds the corss product of passed 3 vertices.
G3DPlane G3DPlane::CrossVer(G3DPlane vertx1, G3DPlane vertx2, G3DPlane vertx3)
{
	G3DPlane crosProL;

	G3DPlane vect1;
	G3DPlane vect2;

	// Find two vector from 3 vertices.
	vect1 = vertx1 - vertx2;
	vect2 = vertx2 - vertx3;

	// Get the noraml by cross product.
	crosProL = (G3DPlane&) vect1.CrossV(vect2);

	//crosProL.x = (vect1.y * vect2.z) - (vect1.z * vect2.y);
	//crosProL.y = (vect1.z * vect2.x) - (vect1.x * vect2.z);
	//crosProL.z = (vect1.x * vect2.y) - (vect1.y * vect2.x);

	return crosProL;
}




//	******************		**********************


bool G3DPlane::IsMoreThanAll(G3DPlane Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x > Vec2.x) && (y > Vec2.y) && (z > Vec2.z) && (d > Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DPlane::IsMoreThanEqAll(G3DPlane Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x >= Vec2.x) && (y >= Vec2.y) && (z >= Vec2.z) && (d >= Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DPlane::IsMoreThanEqAllAbs(G3DPlane Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	G3DPlane Vec;
	Vec.SetPlaneV(x, y, z, d);
	Vec.ModVector();

	Vec2.ModVector();

	if ( (Vec.x >= Vec2.x) && (Vec.y >= Vec2.y) && (Vec.z >= Vec2.z) && (Vec.d >= Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}


bool G3DPlane::IsMoreThanAny(G3DPlane Vec2)	// i.e. x>x1 or y>y1 or z>z1
{
	if ( (x > Vec2.x) || (y > Vec2.y) || (z > Vec2.z) || (d > Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

// ***********
bool G3DPlane::IsLessThanAll(G3DPlane Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x < Vec2.x) && (y < Vec2.y) && (z < Vec2.z) && (d < Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DPlane::IsLessThanEqAll(G3DPlane Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	if ( (x <= Vec2.x) && (y <= Vec2.y) && (z <= Vec2.z)  && (d <= Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DPlane::IsLessThanEqAllAbs(G3DPlane Vec2)	// i.e. x>x1 and y>y1 and z>z1
{
	G3DPlane Vec;
	Vec.SetPlaneV(x, y, z, d);

	Vec.ModVector();
	Vec2.ModVector();

	Vec  *= 0.1;	// Reduce accuracy for better comparision. (Compiler sometime do not compare = for same looking value)
	Vec2 *= 0.1;

	if ( (Vec.x <= Vec2.x) && (Vec.y <= Vec2.y) && (Vec.z <= Vec2.z) && (Vec.d <= Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DPlane::IsLessThanAny(G3DPlane Vec2)	// i.e. x>x1 or y>y1 or z>z1
{
	if ( (x < Vec2.x) || (y < Vec2.y) || (z < Vec2.z) || (d < Vec2.d) )
	{
		return true;
	}else
	{
		return false;
	}
}

bool G3DPlane::IsInSameDirectionVS(G3DPlane Vec2, int viewSide)	// i.e. x>x1 or y>y1 or z>z1
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


bool G3DPlane::IsInSameDirection(G3DPlane Vec2)	// i.e. x>x1 or y>y1 or z>z1
{
	bool retVal = true;

	if ( (x < 0) && (Vec2.x > 0)) retVal = false;
	if ( (x > 0) && (Vec2.x < 0)) retVal = false;

	if ( (y < 0) && (Vec2.y > 0)) retVal = false;
	if ( (y > 0) && (Vec2.y < 0)) retVal = false;

	return retVal;
}


// ***********

void G3DPlane::SetV(ccVTvec x, ccVTvec y, ccVTvec z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

bool G3DPlane::NotZeroVector()
{
	if ((x == 0) && (y == 0) && (z == 0) && (d == 0)) 
	{
		return false;
	}else
	{
		return true;
	}
}


bool G3DPlane::AlmostZeroVector()
{
	if ((x < 0.0000001) && (y < 0.0000001) && (z < 0.0000001) && (d < 0.0000001)) 
	{
		return true;
	}else
	{
		return false;
	}
}




void G3DPlane::SetPlaneV(ccVTvec xP, ccVTvec yP, ccVTvec zP, ccVTvec dP)
{
	v[0] = xP;
	v[1] = yP;
	v[2] = zP;
	v[3] = dP;
}




// Calcs Plane coefficients from Planes's 3 vertices.
void G3DPlane::CalcPlaneCo(G3DVector vertx1, G3DVector vertx2, G3DVector vertx3)
{
	G3DVector vecNor;

	// Find the first three coefficients the same way we calc the Normal to the plane.
//	vecNor = G3DVector::CrossVer(vertx1, vertx2, vertx3);
	G3DVector crosProL;

	G3DVector vect1;
	G3DVector vect2;

	// Find two vector from 3 vertices.
	vect1 = (G3DVector&) (vertx1 - vertx2);
	vect2 = (G3DVector&) (vertx2 - vertx3);

	// Get the noraml by cross product.
	// crosProL = vect1.CrossV(vect2);

	// try indirect way , as above gives some error
	// Copy math. of CrossV
	//v[0] = vect1.v[0]; v[1] = vect1.v[1]; v[2] = vect1.v[2];
	//crosProL = CrossV(vect2);

	crosProL.x = (vect1.y * vect2.z) - (vect1.z * vect2.y);
	crosProL.y = (vect1.z * vect2.x) - (vect1.x * vect2.z);
	crosProL.z = (vect1.x * vect2.y) - (vect1.y * vect2.x);

	vecNor = crosProL;

	A =  vecNor.x;
	B =  vecNor.y;
	C =  vecNor.z;
	D =  -(A * vertx1.x + B * vertx1.y + C * vertx1.z);
}


// Returns the normal of this plane.
G3DVector G3DPlane::GetNormal()
{
	G3DVector vecNorL;

	vecNorL.x = A;
	vecNorL.y = B;
	vecNorL .z = C;

	return vecNorL;
}

double G3DPlane::GetD()
{
	return D;
}

