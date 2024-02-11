#ifndef LIGHT_H_
#define LIGHT_H_

enum LIGHT_TYPE { POINT, AREA };

class Light {
public:
  Light(LIGHT_TYPE, float *, float *);

private:
  LIGHT_TYPE type;
  float diffuse[3];  // field `id`
  float specular[3]; // field `is`
};

#endif // !LIGHT_H_
