load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "example_component_library",
    hdrs = [
        "ExampleComponentLibrary.hpp",
    ],
    deps = [
        "//Library/ConfigInterface/proto:example_config_cc_proto",
        "//Library/SharedMemory/messages:example_message"
    ],
    visibility = ["//visibility:public"],
)