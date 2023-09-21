load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "example_application",
    srcs = ["ExampleApplication.cc"],
    deps = ["//Library/Common/Example:example_library",],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "example_yaml",
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

cc_binary(
    name = "example_parser",
    srcs = ["ExampleArgumentsParser.cc"],
    deps = [
        "@gflags"
    ],
)

cc_binary(
    name = "example_component",
    srcs = ["ExampleComponent.cc",],
    deps = [
        "//Application/Example/ExampleComponentLibrary:example_component_library",
        "//Library/Component:component",
    ],
)
