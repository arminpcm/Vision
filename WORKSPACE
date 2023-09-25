load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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
    name = "gtest",
    sha256 = "b4870bf121ff7795ba20d20bcdd8627b8e088f2d1dab299a031c1034eddc93d5",
    urls = ["https://github.com/google/googletest/archive/release-1.11.0.tar.gz"],
    strip_prefix = "googletest-release-1.11.0",
)


http_archive(
    name = "rules_python",
    sha256 = "5868e73107a8e85d8f323806e60cad7283f34b32163ea6ff1020cf27abef6036",
    urls = ["https://github.com/bazelbuild/rules_python/archive/0.25.0.tar.gz"],
    strip_prefix = "rules_python-0.25.0",
)

http_archive(
    name = "org_pytorch",
    urls = ["https://github.com/pytorch/pytorch/archive/v1.10.0.tar.gz"],
    strip_prefix = "pytorch-1.10.0",
)

http_archive(
    name = "yaml_cpp",
    sha256 = "43e6a9fcb146ad871515f0d0873947e5d497a1c9c60c58cb102a97b47208b7c3",
    urls = ["https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.tar.gz"],
    strip_prefix = "yaml-cpp-yaml-cpp-0.7.0",
)

http_archive(
    name = "rules_cc",
    sha256 = "35f2fb4ea0b3e61ad64a369de284e4fbbdcdba71836a5555abb5e194cf119509",
    strip_prefix = "rules_cc-624b5d59dfb45672d4239422fa1e3de1822ee110",
    urls = [
        "https://github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
    ],
)
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()
http_archive(
    name = "rules_proto",
    sha256 = "e017528fd1c91c5a33f15493e3a398181a9e821a804eb7ff5acdd1d2d6c2b18d",
    strip_prefix = "rules_proto-4.0.0-3.20.0",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0-3.20.0.tar.gz",
    ],
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

http_archive(
    name = "com_google_protobuf",
    sha256 = "c6003e1d2e7fefa78a3039f19f383b4f3a61e81be8c19356f85b6461998ad3db",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.17.3.tar.gz"],  # Adjust the version as needed
    strip_prefix = "protobuf-3.17.3",  # Adjust the version as needed
)
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

http_archive(
    name = "gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
    strip_prefix = "gflags-2.2.2",
)

load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
)

git_repository(
       name = "bazel_clang_tidy",
       commit = "783aa523aafb4a6798a538c61e700b6ed27975a7",
       remote = "https://github.com/erenon/bazel_clang_tidy.git",
)

# Add hermetic pythong tooling
http_archive(
    name = "rules_python",
    sha256 = "29a801171f7ca190c543406f9894abf2d483c206e14d6acbd695623662320097",
    strip_prefix = "rules_python-0.18.1",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.18.1/rules_python-0.18.1.tar.gz",
)

load("@rules_python//python:repositories.bzl", "py_repositories")
py_repositories()

# Add python 3.10
load("@rules_python//python:repositories.bzl", "python_register_toolchains")

python_register_toolchains(
    name = "python3_10",
    python_version = "3.10",
    register_coverage_tool = True,
)
load("@python3_10//:defs.bzl", PYTHON_INTERPRETER_TARGET = "interpreter")
load("@rules_python//python:pip.bzl", "pip_parse")


# pip_parse() is used instead of pip_install() so that we can move pip installation to the build stage
# instead of prior to the Bazel analysis phase (which is the case for pip_install()). This change will
# help to speed up the overall build time and only download the dependencies as and when needed.
#
# pip_parse() also requires us to specify a fully resolved lock file for our Python dependencies. The
# advantage to this is that it enforces fully deterministic builds. Without fully locked dependencies,
# we cannot guarantee that two builds on the same commit use the same package versions.
#
# See:
# - https://github.com/bazelbuild/rules_python#fetch-pip-dependencies-lazily
# - https://github.com/bazelbuild/rules_python/blob/main/docs/pip.md#pip_parse
# - https://github.com/bazelbuild/rules_python/blob/main/docs/pip.md#compile_pip_requirements
#
# The helper compile_pip_requirements() is used for regenerating the locked requirements.txt files.
# Steps:
# 1) Add/remove/modify package in requirements.txt.
# 2) Validate locked requirements.txt can be generated:
#     bazel test //:pip_vision_deps_test
# 3) Update requirements_lock.txt:
#     bazel run //:pip_vision_deps.update
# 4) Commit updated requirements_lock.txt

PIP_INSTALL_TIMEOUT_SECONDS = 3600  # 60 minutes

PIP_EXTRA_ARGS = [
    "--require-hashes",
    "--index-url=https://pypi.org/simple",
    "--extra-index-url=https://download.pytorch.org/whl/cu117",  # This is needed for torchvision
    "--no-cache-dir",
]

pip_parse(
    name = "pip_vision_deps",
    timeout = PIP_INSTALL_TIMEOUT_SECONDS,
    extra_pip_args = PIP_EXTRA_ARGS,
    python_interpreter_target = PYTHON_INTERPRETER_TARGET,
    requirements_lock = "//:requirements_lock.txt",
)
load("@pip_vision_deps//:requirements.bzl", "install_deps")
install_deps()

# adding pybind11
http_archive(
  name = "pybind11",
  build_file = "@pybind11_bazel//:pybind11.BUILD",
  strip_prefix = "pybind11-2.10.4",
  urls = ["https://github.com/pybind/pybind11/archive/refs/tags/v2.10.4.zip"],
  sha256 = "115bc49b69133dd8a7a7f824b669826ff6a35bc70a28ce2cf987d57c50a6543a",
)
