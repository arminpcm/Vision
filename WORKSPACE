load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "io_bazel_rules_skylib",
    urls = ["https://github.com/bazelbuild/rules_skylib/archive/RELEASE_TAG.tar.gz"],
    strip_prefix = "rules_skylib-RELEASE_TAG",
)


http_archive(
    name = "bazel_skylib",
    sha256 = "66ffd9315665bfaafc96b52278f57c7e2dd09f5ede279ea6d39b2be471e7e3aa",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.4.2/bazel-skylib-1.4.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.4.2/bazel-skylib-1.4.2.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

http_archive(
    name = "rules_python",
    urls = ["https://github.com/bazelbuild/rules_python/archive/RELEASE_TAG.tar.gz"],
    strip_prefix = "rules_python-RELEASE_TAG",
)

http_archive(
    name = "org_pytorch",
    urls = ["https://github.com/pytorch/pytorch/archive/v1.10.0.tar.gz"],
    strip_prefix = "pytorch-1.10.0",
)
