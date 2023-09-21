load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "component",
    hdrs = [
        "Component.hpp",
    ],
    srcs = ["ComponentImpl.hpp",],
    deps = [
        "//Library/ConfigInterface:config_interface",
        "//Library/Common/Time:clock",
        "@gflags"
    ],
    visibility = ["//visibility:public"],
)
