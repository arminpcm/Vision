load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "example_application",
    srcs = ["ExampleApplication.cc"],
    deps = ["//Library/Common/Example:example_library",],
    visibility = ["//visibility:public"],
)
