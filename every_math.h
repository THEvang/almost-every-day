#ifndef EVERY_MATH_H
#define EVERY_MATH_H

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
//Assumes Unit Quaternion
Matrix4 quat_to_matrix(Quaternion a);


#endif
