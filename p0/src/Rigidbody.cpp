#include "Rigidbody.h"
#include "ComponentHash.h"

uint32_t Rigidbody::Hash()
{
	return ComponentHash::RIGIDBODY;
}
