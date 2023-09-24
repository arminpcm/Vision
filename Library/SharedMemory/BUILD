load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "channel_header",
    hdrs = ["ChannelHeader.hpp"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "publisher",
    hdrs = ["Publisher.hpp"],
    deps = [":channel_header"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "subscriber",
    hdrs = ["Subscriber.hpp"],
    deps = [":channel_header"],
    visibility = ["//visibility:public"],
)