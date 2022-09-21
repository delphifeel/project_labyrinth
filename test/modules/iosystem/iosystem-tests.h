#include <iostream>
#include "modules/iosystem/iosystem.h"

void IOSystem_TestsRun(void)
{
    auto ioSystem = new IOSystem(666, [](IOSystem::Stream ioStream, const uint8 data[], uint data_len) {
        (void) data;
        std::cout << "IOStream: " << (size_t) ioStream << "\n"
                  << "data len: " << data_len << "\n";
    });
    ioSystem->Start();
}