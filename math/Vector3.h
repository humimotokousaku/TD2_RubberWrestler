#pragma once
struct Vector3 final {
	float x;
	float y;
	float z;
};

struct Vector3Double final {
	double x;
	double y;
	double z;
};

//グローバル算術演算子のオーバーロード
inline Vector3 operator+(const Vector3& num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1.x + num2.x;
	num3.y = num1.y + num2.y;
	num3.z = num1.z + num2.z;

	return num3;
}

inline Vector3 operator-(const Vector3& num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1.x - num2.x;
	num3.y = num1.y - num2.y;
	num3.z = num1.z - num2.z;

	return num3;
}

inline Vector3 operator*(const Vector3& num1, const float num2) {
	Vector3 num3;

	num3.x = num1.x * num2;
	num3.y = num1.y * num2;
	num3.z = num1.z * num2;

	return num3;
}

inline Vector3 operator*(const float num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1 * num2.x;
	num3.y = num1 * num2.y;
	num3.z = num1 * num2.z;

	return num3;
}

inline Vector3 operator*(const Vector3& num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1.x * num2.x;
	num3.y = num1.y * num2.y;
	num3.z = num1.z * num2.z;

	return num3;
}

inline Vector3 operator/(const Vector3& num1, const float num2) {
	Vector3 num3;

	num3.x = num1.x / num2;
	num3.y = num1.y / num2;
	num3.z = num1.z / num2;

	return num3;
}

inline Vector3 operator+=(Vector3& num1, const Vector3& num2) {

	num1.x += num2.x;
	num1.y += num2.y;
	num1.z += num2.z;

	return num1;
}

inline Vector3 operator-=(Vector3& num1, const Vector3& num2) {

	num1.x -= num2.x;
	num1.y -= num2.y;
	num1.z -= num2.z;

	return num1;
}

inline Vector3 operator*=(Vector3& num1, const float num2) {

	num1.x *= num2;
	num1.y *= num2;
	num1.z *= num2;

	return num1;
}

inline Vector3 operator*=(Vector3& num1, const Vector3& num2) {

	num1.x *= num2.x;
	num1.y *= num2.y;
	num1.z *= num2.z;

	return num1;
}

inline Vector3 operator/=(Vector3& num1, const Vector3& num2) {

	num1.x /= num2.x;
	num1.y /= num2.y;
	num1.z /= num2.z;

	return num1;
}

inline Vector3 operator/=(Vector3& num1, const float num2) {

	num1.x = num1.x / num2;
	num1.y = num1.y / num2;
	num1.z = num1.z / num2;

	return num1;
}

inline Vector3Double operator+(const Vector3Double& num1, const Vector3Double& num2) {
	Vector3Double num3;

	num3.x = num1.x + num2.x;
	num3.y = num1.y + num2.y;
	num3.z = num1.z + num2.z;

	return num3;
}

inline Vector3Double operator-(const Vector3Double& num1, const Vector3Double& num2) {
	Vector3Double num3;

	num3.x = num1.x - num2.x;
	num3.y = num1.y - num2.y;
	num3.z = num1.z - num2.z;

	return num3;
}

inline Vector3Double operator*(const Vector3Double& num1, const double num2) {
	Vector3Double num3;

	num3.x = num1.x * num2;
	num3.y = num1.y * num2;
	num3.z = num1.z * num2;

	return num3;
}

inline Vector3Double operator*(const double num1, const Vector3Double& num2) {
	Vector3Double num3;

	num3.x = num1 * num2.x;
	num3.y = num1 * num2.y;
	num3.z = num1 * num2.z;

	return num3;
}

inline Vector3Double operator*(const Vector3Double& num1, const Vector3Double& num2) {
	Vector3Double num3;

	num3.x = num1.x * num2.x;
	num3.y = num1.y * num2.y;
	num3.z = num1.z * num2.z;

	return num3;
}

inline Vector3Double operator/(const Vector3Double& num1, const double num2) {
	Vector3Double num3;

	num3.x = num1.x / num2;
	num3.y = num1.y / num2;
	num3.z = num1.z / num2;

	return num3;
}

inline Vector3Double operator+=(Vector3Double& num1, const Vector3Double& num2) {

	num1.x += num2.x;
	num1.y += num2.y;
	num1.z += num2.z;

	return num1;
}

inline Vector3Double operator-=(Vector3Double& num1, const Vector3Double& num2) {

	num1.x -= num2.x;
	num1.y -= num2.y;
	num1.z -= num2.z;

	return num1;
}

inline Vector3Double operator*=(Vector3Double& num1, const double num2) {

	num1.x *= num2;
	num1.y *= num2;
	num1.z *= num2;

	return num1;
}

inline Vector3Double operator*=(Vector3Double& num1, const Vector3Double& num2) {

	num1.x *= num2.x;
	num1.y *= num2.y;
	num1.z *= num2.z;

	return num1;
}

inline Vector3Double operator/=(Vector3Double& num1, const Vector3Double& num2) {

	num1.x /= num2.x;
	num1.y /= num2.y;
	num1.z /= num2.z;

	return num1;
}

inline Vector3Double operator/=(Vector3Double& num1, const double num2) {

	num1.x = num1.x / num2;
	num1.y = num1.y / num2;
	num1.z = num1.z / num2;

	return num1;
}