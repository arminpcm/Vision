load("@rules_python//python:defs.bzl", "py_library")
load("@rules_python//python:pip.bzl", "compile_pip_requirements")

filegroup(
    name = "clang_tidy_config_default",
    srcs = [
        ".clang-tidy",
    ],
)

label_flag(
    name = "clang_tidy_config",
    build_setting_default = ":clang_tidy_config_default",
    visibility = ["//visibility:public"],
)

filegroup(
    name = "clang_tidy_executable_default",
    srcs = [],  # empty list: system clang-tidy
)

label_flag(
    name = "clang_tidy_executable",
    build_setting_default = ":clang_tidy_executable_default",
    visibility = ["//visibility:public"],
)

filegroup(
    name = "clang_tidy_additional_deps_default",
    srcs = [],
)

label_flag(
    name = "clang_tidy_additional_deps",
    build_setting_default = ":clang_tidy_additional_deps_default",
    visibility = ["//visibility:public"],
)

# Python deps
package(default_visibility = ["//visibility:public"])
filegroup(
    name = "requirements",
    data = [
        "requirements.txt",
    ],
)

# See the 'pip_deps' pip_parse() in WORKSPACE for further details
# Validate: bazel test //:pip_vision_deps_test
# Generate: bazel run //:pip_vision_deps.update
compile_pip_requirements(
    name = "pip_vision_deps",
    timeout = "long",  # Increase timeout for underlying test
    extra_args = [
        "--allow-unsafe",
        "--index-url=https://pypi.org/simple",
        "--extra-index-url=https://download.pytorch.org/whl/cu117",  # Need this for torchvision
        "--resolver=backtracking",

    ],
    requirements_in = "requirements.txt",
    requirements_txt = "requirements_lock.txt",
    tags = [
        "long_test",
    ],
)
