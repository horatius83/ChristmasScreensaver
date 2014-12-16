#ifndef VECTOR_H
#define VECTOR_H

namespace Math {
	class Vector {
	public:
		Vector(float v[3]) { Set(v[0],v[1],v[2]); }
		Vector(float x, float y, float z) { Set(x,y,z);}
		Vector(const Vector& point0, const Vector& point1) {
			Set(point0);
			Sub(point1);
		}

		static Vector Normalize(const Vector& v) {
			Vector vP(v);
			float mag = sqrt(v.Dot(v));
			if(mag != 0) {
				vP.Mul(1.0f / mag);
			}
			return vP;
		}

		void Normalize() {
			float mag = sqrt(this->Dot(*this));
			if(mag != 0) {
				this->Mul(1.0f / mag);
			}
		}

		static float Dot(const Vector& v0, const Vector& v1) {
			return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
		}

		float Dot(const Vector& v) const {
			return Dot(*this,v);
		}

		static Vector Cross(const Vector& v0, const Vector& v1) {
			Vector v(v0);
			v.Cross(v1);
			return v;
		}

		void Cross(const Vector & v) {
			//x = b*f - c*e, y = c*d - a*f, z = a*e - b*d
			//x = b*f - c*e, y = c*d - x*f, z = x*e - b*d
			//x = y*f - c*e, y = c*d - x*f, z = x*e - y*d
			//x = y*f - z*e, y = z*d - x*f, z = x*e - y*d
			//x = y*f - z*e, y = z*v.x - x*f, z = x*e - y*v.x
			//x = y*f - z*v.y, y = z*v.x - x*f, z = x*v.y - y*v.x
			//x = y*v.z - z*v.y, y=z*v.x - x*v.z, z = x.vy - y*vx
			
			float xPrime = y * v.z - v.y * z;
			float yPrime = -1.0f * (x * v.z - v.x * z);
			float zPrime = x * v.y - v.x * y;
			x = xPrime;
			y = yPrime;
			z = zPrime;
		}

		void Set(const Vector& v) {
			Set(v.x, v.y, v.z);
		}
		void Set(float x, float y, float z) {
			this->x = x; this->y = y; this->z = z;
		}

		static Vector Sub(const Vector& v0, const Vector& v1) {
			Vector v(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
			return v;
		}

		void Sub(const Vector& v) {
			x = v.x - x;
			y = v.y - y;
			z = v.z - z;
		}

		static Vector Add(const Vector& v0, const Vector& v1) {
			Vector v(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z);
			return v;
		}

		void Add(const Vector& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		static Vector Mul(float s, const Vector& v0) {
			Vector v(s * v0.x, s * v0.y, s * v0.z);
			return v;
		}

		void Mul(float s) {
			x *= s;
			y *= s;
			z *= s;
		}
		float x;
		float y;
		float z;
	};
}

#endif