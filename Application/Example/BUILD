load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "example_application",
    srcs = ["ExampleApplication.cc"],
    deps = ["//Library/Common/Example:example_library",],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "yaml_example",
    srcs = ["YamlExample.cc"],
    deps = ["@yaml_cpp//:yaml-cpp"],
)

cc_binary(
    name = "example_config",
    srcs = ["ExampleConfigApplication.cc"],
    deps = [
        "//Library/ConfigInterface/proto:person_cc_proto",
        "//Library/ConfigInterface:config_interface",
    ],
)
