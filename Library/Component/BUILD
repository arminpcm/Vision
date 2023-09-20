load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "base_component",
    hdrs = [
        "BaseComponent.hpp",
        "BaseComponent.h",
        "BaseComponent.cc",
    ],
    srcs = ["BaseComponent.cc"],
    deps = [
        "//Library/ConfigInterface:config_interface",
        "//Library/Common/Time:clock",
        "@gflags"
    ],
    visibility = ["//visibility:public"],
)
