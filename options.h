#ifndef _TOZ3_OPTIONS_H_
#define _TOZ3_OPTIONS_H_

#include "frontends/common/options.h"
#include "frontends/common/parser_options.h"

namespace P4::P4Dummy {

class DummyOptions : public CompilerOptions {
 public:
    virtual ~DummyOptions() = default;

    DummyOptions();
    DummyOptions(const DummyOptions &) = default;
    DummyOptions(DummyOptions &&) = delete;
    DummyOptions &operator=(const DummyOptions &) = default;
    DummyOptions &operator=(DummyOptions &&) = delete;

    bool useFixed = false;
};

using P4DummyContext = P4CContextWithOptions<DummyOptions>;

}  // namespace P4::P4Dummy

#endif /* _TOZ3_OPTIONS_H_ */
