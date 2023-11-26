#include "util.h"

namespace util {
    template <typename T>
    void qPrint(T output) {
        std::cout << output << "\n";
    }

    template void qPrint<int>(int);
    template void qPrint<float>(float);
    template void qPrint<char const*>(char const*);
}
