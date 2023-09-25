load("@rules_python//python:defs.bzl", "py_library")
load("@pip_vision_deps//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

py_binary(
    name = "example_python",
    main = "python_example.py",
    srcs = ["python_example.py"],
    deps = [
        requirement("torch"),
    ],
)