
#ifndef VARIABLESCOPING_H
#define VARIABLESCOPING_H

#include <molecular/util/Hash.h>
#include "Uniform.h"
#include <molecular/util/DynamicScoping.h>

namespace molecular
{
namespace Gfx
{
typedef DynamicScoping<Hash, Variable> Scoping;
}
}
#endif // VARIABLESCOPING_H

