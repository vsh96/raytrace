#pragma once

#include <math.h>

class vec3
{
public:
    vec3() {}
    vec3(float x, float y, float z) { e[0] = x; e[1] = y; e[2] = z; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline const vec3& operator+() const { return *this ;}
    inline vec3 operator-() const { return vec3( -e[0], -e[1], -e[2]); }
    inline float operator[]( int i ) const { return e[i]; }
    inline float& operator[]( int i ) { return e[i]; }

    inline vec3& operator+=( const vec3 &v2 );
    inline vec3& operator-=( const vec3 &v2 );
    inline vec3& operator*=( const vec3 &v2 );
    inline vec3& operator/=( const vec3 &v2 );
    inline vec3& operator*=( const float t );
    inline vec3& operator/=( const float t );



    float e[3];

};

    inline vec3 operator+(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
    }

    inline vec3 operator-(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
    }

    inline vec3 operator*(const vec3 &v1, const vec3 &v2 )
    {
        return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
    }

    inline vec3 operator/(const vec3 &v1, const vec3 &v2 )
    {
        return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
    }

    inline vec3 operator*(const vec3 &v, const float &t)
    {
        return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
    }

    inline vec3 operator/(const vec3 &v, const float &t)
    {
        return vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
    }

    inline float dot(const vec3 &v1, const vec3 &v2)
    {
        return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
    }

    inline float length(vec3 v) 
    {
        return sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
    }
    inline vec3 normalize(vec3 v) { return v/length(v); }

    inline vec3 cross(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
                    -(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0]),
                    v1.e[0]*v2.e[1] -  v1.e[1]*v2.e[0]);
    }