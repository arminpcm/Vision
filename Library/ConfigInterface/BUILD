load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "config_interface",
    hdrs = [
        "ConfigInterface.hpp",
        "ConfigInterface.cc"
    ],
    srcs = [
        "ConfigInterface.cc"
    ],
    deps = ["@com_google_protobuf//:protobuf", "//Library/ConfigInterface/proto:person_cc_proto",],
    visibility = ["//visibility:public"],
)
