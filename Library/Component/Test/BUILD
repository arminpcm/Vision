load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "component_test",
    srcs = ["ComponentTest.cc"],
    deps = [
        "//Library/Component:component",
        "//Library/ConfigInterface/proto:person_cc_proto",
        "@gtest//:gtest_main",
    ],
)