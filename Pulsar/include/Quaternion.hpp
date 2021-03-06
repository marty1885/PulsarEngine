#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#include <iostream>
#include <glm/matrix.hpp>

/*  Operator overloading
	http://stackoverflow.com/questions/4421706/operator-overloading/4421715#4421715
	Typecasting
	http://www.cplusplus.com/doc/tutorial/typecasting/
	Implicit conversion when overloading operators for template classes
	http://stackoverflow.com/questions/8890051/implicit-conversion-when-overloading-operators-for-template-classes
*/

namespace Pulsar
{
template <class scalar> class Quat
{
public:

	// Components
	scalar w, x, y, z;

	// Constructors
	Quat(){ w = 0; x = 0; y = 0; z = 0; }
	Quat(const scalar& w0){ w = w0; x = 0; y = 0; z = 0; } // Scalar
	Quat(scalar x0, scalar y0, scalar z0){ w = 0; x = x0; y = y0; z = z0; }	// Vector
	Quat(scalar w0, scalar x0, scalar y0, scalar z0) { w = w0; x = x0; y = y0; z = z0; } // Quaternion
	Quat(glm::vec4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
	Quat(glm::vec3 V) { x = V.x; y = V.y; z = V.z; }

	// Assignment (Conversion from scalar to Quat)
	Quat& operator= (const scalar& w0) { return *this; }

	// Typecast
	operator Quat<double>()	 {return Quat(w,x,y,z);}
	explicit operator Quat<float>() {return Quat(w,x,y,z);}

	// += operator (Addition)
	Quat& operator+=(const Quat& R) { w += R.w; x += R.x; y += R.y; z += R.z; return *this; }

	// -= operator (Subtraction)
	Quat& operator-=(const Quat& R) { w -= R.w; x -= R.x; y -= R.y; z -= R.z; return *this; }

	// *= operator (Quaternion product)
	Quat& operator*=(const Quat& R)
	{
		Quat L(w,x,y,z);
		w = L.w*R.w - L.x*R.x - L.y*R.y - L.z*R.z;
		x = L.w*R.x + L.x*R.w + L.y*R.z - L.z*R.y;
		y = L.w*R.y + L.y*R.w + L.z*R.x - L.x*R.z;
		z = L.w*R.z + L.z*R.w + L.x*R.y - L.y*R.x;
		return *this;
	}

	// &= operator (Inner product)
	Quat& operator&=(const Quat& R)
	{
		w = x*R.x + y*R.y + z*R.z;
		x = 0; y = 0; z = 0;
		return *this;
	}

	// %= operator (Outer product)
	Quat& operator%=(const Quat& R)
	{
		Quat L(w,x,y,z);
		w = 0;
		x = L.y*R.z - L.z*R.y;
		y = L.z*R.x - L.x*R.z;
		z = L.x*R.y - L.y*R.x;
		return *this;
	}

	// /= operator (Divide by scalar)
	Quat& operator/=(const scalar& S) { w /= S; x /= S; y /= S; z /= S; return *this; }

	// Norm squared
	scalar norm2() { return w*w + x*x + y*y + z*z; }
	// Norm
	scalar norm() { return sqrt(norm2()); }

	// Vector part
	Quat vect() { return Quat(x,y,z); }

	// == operator
	friend bool operator==(const Quat& L, const Quat& R){ return (L.w==R.w && L.x==R.x && L.y==R.y && L.z==R.z) ? true : false;}

	// != operator
	friend bool operator!=(const Quat& L, const Quat& R){ return !operator==(L,R); }

	// + operator
	friend Quat operator+(Quat R) { return R; }

	// - operator
	friend Quat operator-(Quat R) { return Quat(-R.w, -R.x, -R.y, -R.z); }

	// ~ operator (Conjugate)
	friend Quat operator~(Quat R) { return Quat(R.w, -R.x, -R.y, -R.z); }

	// + operator (Add)
	friend Quat operator+(Quat L, const Quat& R) { return L += R; }

	// - operator (Subtract)
	friend Quat operator-(Quat L, const Quat& R) { return L -= R; }

	// * operator (Quaternion Product)
	friend Quat operator*(Quat L, const Quat& R) { return L *= R; }

	// & operator (Inner product)
	friend scalar operator&(Quat L, const Quat& R) { return L &= R; }

	// % operator (Outer product)
	friend Quat operator%(Quat L, const Quat& R) { return L %= R; }

	// / operator (Divide by scalar)
	friend Quat operator/(Quat Q, const scalar& S) { return Q /= S; }

	// ^ operator (Power)
	friend Quat operator^(Quat Q, const int& N)
	{
		if(N == 0) return Quat(1);
		else if(N > 0) { for(int i=1; i<N; i++) Q *= Q; return Q; }
		else { Quat P = inv(Q); for(int i=1; i<-N; i++) P *= P; return P; }
	}
	friend Quat operator^(Quat Q, const Quat& P) { return exp(log(Q)*P); }

	// cout
	friend std::ostream& operator<< (std::ostream& out, const Quat& Q)
	{
		if (Q==0) return out << 0;

		bool leading = true;
		if (Q.w!=0) { out << Q.w; leading = false; }
		if (Q.x>0 && leading == false) out << "+";
		if (Q.x!=0) { out << Q.x << "i"; leading = false; }
		if (Q.y>0 && leading == false) out << "+";
		if (Q.y!=0) { out << Q.y << "j"; leading = false; }
		if (Q.z>0 && leading == false) out << "+";
		if (Q.z!=0) out << Q.z << "k";
		return out;
	}

	// GLM typecasts
	operator glm::vec4() const { return glm::vec4(x,y,z,w); }
	operator glm::vec3() const { return glm::vec3(x,y,z); }	 // Vector part

};

	// Normalize
	template <class scalar>
	Quat<scalar> normalize(Quat<scalar> Q) { return Q!=0 ? Q/Q.norm() : Q; }

	// Inverse
	template <class scalar>
	Quat<scalar> inv(Quat<scalar> Q) { return Q!=0 ? ~Q/Q.norm2() : Q; }

	// Exponential
	template <class scalar>
	Quat<scalar> exp(Quat<scalar> Q)
	{
		scalar v = Q.vect().norm();
		return v!= 0 ? std::exp(Q.w) * ( cos(v) + sin(v)*normalize(Q.vect()) ) : std::exp(Q.w);
	}

	// Log
	template <class scalar>
	Quat<scalar> log(Quat<scalar> Q)
	{
		return log(Q.norm()) + Q.vect()!=0 ? acos(Q.w/Q.norm())*normalize(Q.vect()) : 0;
	}

	// Rotor
	template <class scalar>
	Quat<scalar> rotor(scalar angle, Quat<scalar> axis)
	{
		return exp(angle*normalize(axis)/2);
	}

	typedef Quat<float>	 Quatf;
	typedef Quat<double> Quatd;
}
#endif // QUATERNION_H_INCLUDED
