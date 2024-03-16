#include <stdlib.h>
#include <iostream>

#include "frontends/common/constantFolding.h"
#include "frontends/common/parseInput.h"
#include "frontends/p4/frontend.h"
#include "frontends/p4/toP4/toP4.h"
#include "ir/ir.h"
#include "options.h"
#include "test/gtest/helpers.h"
#include "frontends/common/options.h"
#include "frontends/common/parser_options.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "frontends/p4/typeChecking/typeChecker.h"
#include "frontends/p4/typeMap.h"
#include "ir/pass_manager.h"
#include "ir/visitor.h"
#include "lib/compile_context.h"
#include "lib/cstring.h"
#include "lib/error.h"

namespace P4Dummy {

class MidEnd : public PassManager {
    P4::ReferenceMap refMap;
    P4::TypeMap typeMap;

 public:
    explicit MidEnd() {
        addPasses({
            new P4::TypeChecking(&refMap, &typeMap, true),
            new P4::ConstantFolding(&refMap, &typeMap),
        });
    }
};

/// A dummy visitor which prints out the parser and parser states in preorder.
class ParserVisitor : public Inspector {
    bool preorder(const IR::P4Parser *parser) override {
        std::cout << "I am visiting parser " << parser->controlPlaneName().c_str() << "\n";
        return true;
    }

    bool preorder(const IR::ParserState *state) override {
        std::cout << "I am visiting parser state " << state->controlPlaneName().c_str() << ":\n";
        state->dbprint(std::cout);
        std::cout << "\n";
        return true;
    }
};

const IR::P4Program *parseDummyP4(const ParserOptions &options) {
    std::stringstream streamTest;
    streamTest << R"(
header ethernet_t {
    bit<48> dst_addr;
    bit<48> src_addr;
    bit<16> eth_type;
}

header H {
    bit<8> a;
    bit<8> b;
}

struct Headers {
    ethernet_t eth_hdr;
    H          h;
}

struct Meta { }

parser p(packet_in pkt, out Headers h, inout Meta meta, inout standard_metadata_t stdmeta) {
    state start {
        pkt.extract(h.eth_hdr);
        transition parse_h;
    }
    state parse_h {
        pkt.extract(h.h);
        transition accept;
    }
}

control vrfy(inout Headers h, inout Meta meta) {
    apply { }
}

control ingress(inout Headers h, inout Meta m, inout standard_metadata_t s) {

    action MyAction1() {
        h.h.b = 1;
    }

    table da_table {
        key = {
            h.h.a : exact;
        }

        actions = {
            MyAction1;
        }

        size = 1024;
        default_action = MyAction1();
    }

    apply {
        da_table.apply();
    }
}
control egress(inout Headers h, inout Meta m, inout standard_metadata_t s) {
    apply { }
}

control update(inout Headers h, inout Meta m) {
    apply { }
}

control deparser(packet_out pkt, in Headers h) {
    apply {
        pkt.emit(h);
    }
}
V1Switch(p(), vrfy(), ingress(), egress(), update(), deparser()) main;)";
    auto source = P4_SOURCE(P4Headers::V1MODEL, streamTest.str().c_str());

    return P4::parseP4String(source, options.langVersion);
}

}  // namespace P4Dummy

int main(int argc, char *const argv[]) {
    AutoCompileContext autoP4DummyContext(new P4Dummy::P4DummyContext);
    auto &options = P4Dummy::P4DummyContext::get().options();
    options.langVersion = CompilerOptions::FrontendVersion::P4_16;

    if (options.process(argc, argv) == nullptr) {
        return EXIT_FAILURE;
    }

    options.file = argv[0];

    const IR::P4Program *program = nullptr;
    if (options.file == nullptr && !options.useFixed) {
        options.usage();
        return EXIT_FAILURE;
    }

    if (options.useFixed) {
        program = P4Dummy::parseDummyP4(options);
    } else {
        options.setInputFile();
        if (::errorCount() > 0) {
            return EXIT_FAILURE;
        }
        program = P4::parseP4File(options);
    }

    if (program == nullptr && ::errorCount() != 0) {
        return EXIT_FAILURE;
    }

    // This compiler pass can print IR nodes.
    auto top4 = P4::ToP4(&std::cout, false);

    std::cout << "\n############################## INITIAL ##############################\n";
    // Print the program before running front end passes.
    program->apply(top4);

    std::cout
        << "\n############################## AFTER FRONT END ##############################\n";
    // Apply the front end passes. These are usually fixed.
    P4::FrontEnd fe;
    program = fe.run(options, program);

    // Print the program after running front end passes.
    program->apply(top4);

    std::cout << "\n############################## AFTER MID END ##############################\n";
    // Apply the mid end passes.
    program = program->apply(P4Dummy::MidEnd());

    // Print the program after running front end passes.
    program->apply(top4);

    std::cout << "\n############################## CUSTOM VISITOR ##############################\n";
    // Apply a custom visitor that prints the parser states for the respective program.
    program->apply(P4Dummy::ParserVisitor());

    return ::errorCount() > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
