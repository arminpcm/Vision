load("@rules_python//python:defs.bzl", "py_binary")
load("@pip_vision_deps//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

py_binary(
    name = "example_python",
    main = "PythonExample.py",
    srcs = ["PythonExample.py"],
    deps = [
        requirement("torch"),
    ],
)