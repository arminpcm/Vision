load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "example_component_library",
    hdrs = [
        "ExampleComponentLibrary.h",
    ],
    deps = [
        "//Library/ConfigInterface/proto:person_cc_proto",
    ],
    visibility = ["//visibility:public"],
)