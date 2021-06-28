#include "mat.h"

mat::mat4 identity(1);

namespace mat{

    vec2::vec2(float a, float b) : x(a), y(b) {}
    vec2::vec2() { x = y = 0; }
    vec3::vec3(float a, float b, float c) : x(a), y(b), z(c) {}
    vec3::vec3():x(0), y(0), z(0){}
    void vec3::log(std::string s){
        std::cout << s << std::endl;
        std::cout << x << " " << y << " " << z << std::endl;
    }
    vec4::vec4(): x(0), y(0), z(0), w(0){}
    vec4::vec4(float p, float q, float r, float s): x(p), y(q), z(r), w(s){}

    mat4::mat4(){}
    mat4::mat4(float n){
        a.x = b.y = c.z = d.w = n;
    }
    void mat4::log(std::string s){
            std::cout << s << std::endl;
            std::cout << a.x << " " << b.x << " " << c.x << " " << d.x << std::endl;
            std::cout << a.y << " " << b.y << " " << c.y << " " << d.y << std::endl;
            std::cout << a.z << " " << b.z << " " << c.z << " " << d.z << std::endl;
            std::cout << a.w << " " << b.w << " " << c.w << " " << d.w << std::endl;
    }
    void vec2::log(std::string s) {
        std::cout << s << std::endl;
        std::cout << x << " " << y << std::endl;
    }
    mat4 translate(mat4 a, vec3 b){
        a.d.x += b.x;
        a.d.y += b.y;
        a.d.z += b.z;
        return a;
    }
    mat4 scale(mat4 a, vec3 b) {
        a.a.x *= b.x;
        a.b.y *= b.y;
        a.c.z *= b.z;
        return a;
    }
    mat4 rotateX(mat4 a, float theta){
        a.a.x = 1;
        a.b.y = cos(theta);
        a.b.z = sin(theta);
        a.c.y = -sin(theta);
        a.c.z = cos(theta);
        a.d.w = 1;
        return a;
    }
    mat4 rotateY(mat4 a, float theta){

        a.a.x = cos(theta);
        a.a.z = -sin(theta);
        a.b.y = 1;
        a.c.x = sin(theta);
        a.c.z = cos(theta);
        a.d.w = 1;
        return a;
    }
    mat4 rotateZ(mat4 a, float theta){
        a.a.x = cos(theta);
        a.a.y = sin(theta);
        a.b.x = -sin(theta);
        a.b.y = cos(theta);
        a.c.z = 1;
        a.c.w = 1;
        return a;
    }

    mat4 perspective(float close, float far, float fov, float R){

        mat4 p(0);
        float L = 2.0f * tan(fov / 2) * close;

        float H = L / R;

        p.a.x = close * 2.0f / L;                   
        p.b.y = close * 2.0f / H;                   
        p.c.z = -(far + close) / (far - close);        
        p.d.z = 2.0f * far * close / (close - far); 

        p.c.w = -1;

        return p;
    }

    vec4 multiply(mat4 a, vec4 b){

        //std::cout << "BBB";
        vec4 rez;
        rez.x = a.a.x * b.x + a.b.x * b.y + a.c.x * b.z + a.d.x * b.w;
        rez.y = a.a.y * b.x + a.b.y * b.y + a.c.y * b.z + a.d.y * b.w;
        rez.z = a.a.z * b.x + a.b.z * b.y + a.c.z * b.z + a.d.z * b.w;
        rez.w = a.a.w * b.x + a.b.w * b.y + a.c.w * b.z + a.d.w * b.w;

        return rez;

    }

    float module(vec3 a){
        return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }
    float module(vec2 a) {
        return sqrt(a.x * a.x + a.y * a.y);
    }
    float dot(vec3 a, vec3 b){
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    float dot(vec2 a, vec2 b) {
        return a.x * b.x + a.y * b.y;
    }
    vec3 projection(vec3 a, vec3 b){
        float d = (module(b)) * (module(b)) / dot(a, b);
        vec3 c;
        c.x = a.x * d;
        c.y = a.y * d;
        c.z = a.z * d;
        return c;
    }
    vec3 normalise(vec3 a){
        float b = module(a);
        a.x /= b;
        a.y /= b;
        a.z /= b;
        return a;
    }
    vec2 normalise(vec2 a) {
        float b = module(a);
        a.x /= b;
        a.y /= b;
        return a;
    }
    vec3 cross(vec3 a, vec3 b){

        vec3 rez;
        rez.x = a.y * b.z - a.z * b.y;
        rez.y = -(a.x * b.z - a.z * b.x);
        rez.z = a.x * b.y - a.y * b.x;
        return rez;
    }
    vec3 multiplyScalar(vec3 a, float b){
        a.x *= b;
        a.y *= b;
        a.z *= b;
        return a;
    }
    vec3 multiplyScalar(vec3 a, int b) {
        a.x *= b;
        a.y *= b;
        a.z *= b;
        return a;
    }
    vec3 add(vec3 a, vec3 b){
        vec3 c;
        c.x = a.x + b.x;
        c.y = a.y + b.y;
        c.z = a.z + b.z;
        return c;
    }
    vec3 subtract(vec3 a, vec3 b) {
        vec3 c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        c.z = a.z - b.z;
        return c;
    }
    vec2 subtract(vec2 a, vec2 b) {
        vec2 c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        return c;
    }
    int sgn(int n) {
        if (n > 0)
            return 1;
        else return -1;
    }
    int sgn(float n) {
        if (n > 0)
            return 1;
        else return -1;
    }
    float clampTwoSide(float minPossibleValue, float maxPossibleValue, float n) {
        if (n < minPossibleValue)
            return minPossibleValue;
        if (n > maxPossibleValue)
            return maxPossibleValue;
        return n;

    }
    float clampMin(float minPossibleValue, float n) {
        if (n < minPossibleValue)
            return minPossibleValue;
        else return n;
    }
    float clampMax(float maxPossibleValue, float n) {
        if (n > maxPossibleValue)
            return maxPossibleValue;
        else return n;
    }
  
    void Camera::setViewMatrix(){
        viewMatrix.a.x = right.x;
        viewMatrix.b.x = right.y;
        viewMatrix.c.x = right.z;
        viewMatrix.d.x = -dot(position, right);

        viewMatrix.a.y = up.x;
        viewMatrix.b.y = up.y;
        viewMatrix.c.y = up.z;
        viewMatrix.d.y = -dot(position, up);

        viewMatrix.a.z = direction.x;
        viewMatrix.b.z = direction.y;
        viewMatrix.c.z = direction.z;
        viewMatrix.d.z = -dot(position, direction);

        viewMatrix.a.w = 0;
        viewMatrix.b.w = 0;
        viewMatrix.c.w = 0;
        viewMatrix.d.w = 1;

    }
    void Camera::setViewMatrix(vec3 position, vec3 direction){
        right = normalise(cross(direction, vec3(0, 1, 0)));
        up = normalise(cross(right, up));
        setViewMatrix();
    }
    void Camera::setViewMatrix(vec3 position, float pitch1, float yaw1){
        pitch = pitch1;
        yaw = yaw1;
        direction.x = -cos(pitch) * cos(yaw);
        direction.y = -sin(pitch);
        direction.z = -cos(pitch) * sin(yaw);
        direction = normalise(direction);

        right = normalise(cross(direction, vec3(0, 1, 0)));
        up = normalise(cross(right, direction));
        setViewMatrix();
    }
    Camera::Camera(){
        pitch = yaw = 0;
    }

};
