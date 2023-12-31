#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    CFATAL("This is a test! %f", 3.14f);
    CERROR("This is a test! %f", 3.14f);
    CWARN("This is a test! %f", 3.14f);
    CINFO("This is a test! %f", 3.14f);
    CDEBUG("This is a test! %f", 3.14f);
    CTRACE("This is a test! %f", 3.14f);

    CASSERT(1 == 0);

    return 0;
}