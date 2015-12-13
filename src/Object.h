#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm.hpp>
#include <string>

struct KeplerianElements {
    float e; /* eccentricity */
    float a; /* semi major axis */
    float inc; /* Inclination */
    float LAN; /* Longitude of Ascending Node */
    float AP; /* Argument of Periapsis */
    float LP; /* Longitude of Periapsis */
    float M0; /* Mean anomaly at epoch */
    float epoch;
    float t;
};

class Object {
public:
    Object
    (std::string name,
     glm::vec3 r,
     glm::vec3 v,
     glm::vec3 thrust,
     float epoch, float m, float Cd, float A);
    Object
    (std::string name,
     KeplerianElements ke,
     float m, float Cd, float A);
    Object(const Object &obj);
    Object operator= (const Object &obj);
    ~Object();
    void tick(float dt);
    void set_thrust(glm::vec3 thrust);
    glm::vec3 get_r() const;
    glm::vec3 get_v() const;
    glm::vec3 get_thrust() const;
    float get_m() const;
    float get_Cd() const;
    float get_A() const;
    bool isActive() const;
    bool isCrashed() const;
    KeplerianElements get_ke() const;
    void deactivate();
    void activate();
private:
    void recompute_ke(float epoch);
    void recompute_sv();
    glm::vec3 force();
    std::string name;
    KeplerianElements ke;
    glm::vec3 r;
    glm::vec3 v;
    glm::vec3 thrust;
    float m;
    float Cd;
    float A;
    bool active;
    bool crashed;
};
#endif
