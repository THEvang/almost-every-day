#include "every_math.h"

#include <math.h>

Vector3 vec3_cross(Vector3 a, Vector3 b) {
	return (Vector3) {
		.x = a.y*b.z - a.z*b.y,
		.y = a.z*b.x - a.x*b.z,
		.z = a.x*b.y - a.y*b.x		
	};
}

Vector3 vec3_add(Vector3 a, Vector3 b) {
	return (Vector3) {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
}

Vector3 vec3_scale(float a, Vector3 b) {
	return (Vector3) {
		.x = b.x * a,
		.y = b.y * a,
		.z = b.z * a 
	};
}

float vec3_dot(Vector3 a, Vector3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Quaternion quat_add(Quaternion a, Quaternion b) {
	return (Quaternion) {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
		.w = a.w + b.w
	};
}

Quaternion quat_mult(Quaternion q, Quaternion r) {
	
	Vector3 v;
	v = vec3_cross(q.qv, r.qv);		
	v = vec3_add(v, vec3_scale(r.qw, q.qv));
	v = vec3_add(v, vec3_scale(q.qw, r.qv));

	float w = q.w * r.w - vec3_dot(q.qv, r.qv);

	return (Quaternion) {
		.qv = v,
		.qw = w 
	};
};

//Assumes Unit Quaternion
Matrix4 quat_to_matrix(Quaternion a) {
	
	Matrix4 r = {0};	
	r.e[0] = 1.0 - 2.0 * (a.y * a.y + a.z * a.z);
	r.e[1] = 2.0 * (a.x * a.y - a.w * a.z);
	r.e[2] = 2.0 * (a.x * a.z + a.w * a.y);

	r.e[4] = 2.0 * (a.x * a.y + a.w * a.z);
	r.e[5] = 1.0 - 2.0 * (a.x * a.x + a.z * a.z);
	r.e[6] = 2.0 * (a.y * a.z - a.w * a.x);

	r.e[8] = 2.0 * (a.x * a.z - a.w * a.y);
	r.e[9] = 2.0 * (a.y * a.z + a.w * a.x);
	r.e[10] = 1.0 - 2.0 * (a.x * a.x + a.y * a.y);

	r.e[15] = 1.0;
	
	return r;
};

Quaternion to_quaternion(double deg, Vector3 axis) {
	double radians = TO_RAD(deg);
	return (Quaternion) { 
		.qv = vec3_scale(sinf(radians), axis),
		.qw = cosf(radians)
	};
}

Quaternion quat_rotate(Quaternion q, double deg) {
	Quaternion r = to_quaternion(deg, (Vector3) {{0, 0, 1}});
	return quat_mult(q, r);
}

Quaternion quat_conjugate(Quaternion q) {
	return (Quaternion) {
		.x = -q.x,
		.y = -q.y,
		.z = -q.z,
		.w = q.w
	};
}

double
quat_norm(Quaternion q) {
	return sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Quaternion
quat_scale(double a, Quaternion q) {
	return (Quaternion) {
		.x = q.x * a,
		.y = q.y * a,
		.z = q.z * a,
		.w = q.w * a
	};
}

Quaternion
quat_normalize(Quaternion q) {
	
	double norm = quat_norm(q);
	return quat_scale(1 / norm, q);
}
