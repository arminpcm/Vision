# Vision
Things I enjoy working on
[![CircleCI](https://dl.circleci.com/status-badge/img/circleci/R8zai1J9naVSzP7dj9y8sZ/MHnBLw1QFyeWRkGKppxrEg/tree/main.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/circleci/R8zai1J9naVSzP7dj9y8sZ/MHnBLw1QFyeWRkGKppxrEg/tree/main)

## Supported platform
Currently only windows is supported by the Container tools

## How to get started?
***NOTE:*** You have to be inside `Vision` directory for all the instructions provided here.


First, build the docker container using:
```
.\Container\Build.ps1
```

Next, start the container:
```
.\Container\Start.ps1
```

To get inside the container, simply run:
```
.\Container\Into.ps1
```

To return to the host, enter:
```
exit
```

To stop the running container run:
```
.\Container\Stop.ps1
```

Now, to build the code use:
```
bazel build //... --config clang-tidy
```
Note that this will run clang-tidy tests and would throw errors if the code is not compliant.

To run applications:
```
bazel run Application/Example/example_application
```

Or
```
bazel-bin/Application/Example/example_application
```