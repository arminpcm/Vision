load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "clock_tests",
    srcs = ["TestClock.cc"],
    deps = [
        "//Library/Common/Time:clock",
        "@gtest//:gtest_main",
    ],
    timeout = "long",
)
