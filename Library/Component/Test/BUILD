load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "component_test",
    srcs = ["ComponentTest.cc"],
    deps = [
        "//Library/Component:component",
        "//Library/ConfigInterface/proto:example_config_cc_proto",
        "//Library/SharedMemory/messages:example_message",
        "@gtest//:gtest_main",
    ],
)