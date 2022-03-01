#include "idle.h"

Idle::Idle(): Thread(defaultStackSize, 1) {}

void Idle::run() {
	while (1);
}
