load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "example_component_library",
    hdrs = [
        "ExampleComponentLibrary.hpp",
    ],
    deps = [
        "//Library/ConfigInterface/proto:example_config_cc_proto",
        "//Library/ConfigInterface/proto:example_message_cc_proto",
    ],
    visibility = ["//visibility:public"],
)