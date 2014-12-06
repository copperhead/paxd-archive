#include <stdlib.h>

#include "flags.h"

int main(void) {
    return update_attributes() < 0 ? 1 : 0;
}
