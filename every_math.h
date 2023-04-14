#ifndef EVERY_MATH_H
#define EVERY_MATH_H

#define TO_RAD(deg) deg * M_PI / 180.0


typedef union {
	struct {
		float x,y,z;
	};
	float e[3];
} Vector3;

typedef union {
	struct {
		float x,y,z,w;
	};
	struct {
		Vector3 qv;
		float qw;
	};
	float e[4];
} Quaternion;
typedef struct {
	float e[4*4];
} Matrix4;



Vector3 vec3_cross(Vector3 a, Vector3 b);
Vector3 vec3_add(Vector3 a, Vector3 b);
Vector3 vec3_scale(float a, Vector3 b);
float vec3_dot(Vector3 a, Vector3 b);

Quaternion quat_add(Quaternion a, Quaternion b);
Quaternion quat_mult(Quaternion q, Quaternion r);
double quat_norm(Quaternion q);
Quaternion quat_normalize(Quaternion q);
//Assumes Unit Quaternion
Matrix4 quat_to_matrix(Quaternion a);

Quaternion to_quaternion(double deg, Vector3 axis);
Quaternion quat_rotate(Quaternion q, double deg);

#endif
