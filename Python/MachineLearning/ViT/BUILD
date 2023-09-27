load("@rules_python//python:defs.bzl", "py_library")
load("@rules_python//python:defs.bzl", "py_binary")
load("@pip_vision_deps//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])


py_library(
    name = "vit",
    srcs = ["ViT.py"],
    deps = [
        requirement("torch"),
        "//Python/MachineLearning/Layers/Transformers:mlp"
    ],
)

py_binary(
    name = "train_mnist",
    main = "TrainMnist.py",
    srcs = ["TrainMnist.py"],
    deps = [
        requirement("torch"),
        requirement("torchvision"),
        requirement("pytorch_lightning"),
        ":vit"
    ],
)
