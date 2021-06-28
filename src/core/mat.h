#pragma once

#include<iostream>
#include<cmath>

namespace mat{

    struct vec1{
        float x;
        vec1();
        vec1(float a);
        void log(std::string s);
    };
    struct vec2{
        float x, y;
        vec2();
        vec2(float a, float b);
        void log(std::string s);
    };
    struct vec3{
        float x, y, z;
        vec3();
        vec3(float x, float y, float c);
        void log(std::string s);
    };
    struct vec4{
        float x, y, z, w;
        vec4();
        vec4(float p, float q, float r, float s);
        void log(std::string s);
    };
    class mat4{
    public:
        vec4 a, b, c, d;

        mat4();
        mat4(float n);
        void log(std::string s);
    };

    mat4 translate(mat4 a, vec3 b);
    mat4 scale(mat4 a, vec3 b);
    mat4 rotateX(mat4 a, float theta);
    mat4 rotateY(mat4 a, float theta);
    mat4 rotateZ(mat4 a, float theta);

    mat4 perspective(float close, float far, float fov, float R);

    vec4 multiply(mat4 a, vec4 b);

    void log(std::string s, mat4 a);
    float module(vec3 a);
    float module(vec2 a);
    float dot(vec3 a, vec3 b);
    float dot(vec2 a, vec2 b);
    vec3 projection(vec3 a, vec3 b);
    vec3 normalise(vec3 a);
    vec2 normalise(vec2 a);
    vec3 cross(vec3 a, vec3 b);
    vec3 multiplyScalar(vec3 a, float b);
    vec3 multiplyScalar(vec3 a, int b);
    vec3 add(vec3 a, vec3 b);
    vec3 subtract(vec3 a, vec3 b);
    vec2 subtract(vec2 a, vec2 b);
    void log(std::string s, vec3 a);
    int sgn(int n);
    int sgn(float n);
    float clampTwoSide(float minPossibleValue, float maxPossibleValue, float n);
    float clampMin(float minPossibleValue, float n);
    float clampMax(float maxPossibleValue, float n);

    class Camera{
    public:
        vec3 position, right, up, direction;
        mat4 viewMatrix;
        float pitch, yaw;
        void setViewMatrix();
        void setViewMatrix(vec3 position, vec3 direction);
        void setViewMatrix(vec3 position, float pitch1, float yaw1);
        Camera();

    };
}
