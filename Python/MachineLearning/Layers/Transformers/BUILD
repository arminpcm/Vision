load("@rules_python//python:defs.bzl", "py_library")
load("@rules_python//python:defs.bzl", "py_binary")
load("@pip_vision_deps//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

py_library(
    name = "attention",
    srcs = ["Attention.py"],
    deps = [
        requirement("torch"),
        requirement("numpy"),
    ],
)

py_binary(
    name = "attention_main",
    main = "AttentionMain.py",
    srcs = ["AttentionMain.py"],
    deps = [
        requirement("torch"),
        requirement("numpy"),
        requirement("pytorch_lightning"),
        ":attention"
    ],
)

py_library(
    name = "mlp",
    srcs = ["MLP.py"],
    deps = [
        requirement("torch"),
    ],
)
