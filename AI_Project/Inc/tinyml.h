#ifndef TINYML_H_
#define TINYML_H_

#include <stdint.h>

class TinyML
{
public:
    int predict(const uint8_t *data);  // input should not be modified
};

#endif /* TINYML_H_ */
