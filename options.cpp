#include "options.h"

namespace P4Dummy {

DummyOptions::DummyOptions() {
    registerOption(
        "--use-fixed", nullptr,
        [this](const char *) {
            useFixed = true;
            return true;
        },
        "Instead of parsing a real P4 program manipulate and parse a dummy program.");
}

}  // namespace P4Dummy
