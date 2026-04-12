

#include "tinyml.h"
#include "inference.h"

int TinyML::predict(const uint8_t *data)
{
    // cast only if inference expects non-const
    return Inference::predict((uint8_t*)data);
}
