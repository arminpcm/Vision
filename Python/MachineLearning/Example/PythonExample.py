"""
Copyright 2023 Armin Parchami. All rights reserved.

This software is provided "as is," without warranty of any kind, express or implied,
including but not limited to the warranties of merchantability,
fitness for a particular purpose, and noninfringement.
In no event shall the authors or copyright holders be liable for any claim,
damages, or other liability, whether in an action of contract,
tort, or otherwise, arising from, out of, or in connection with
the software or the use or other dealings in the software.
"""

import sys
import torch

def main() -> None:
    """ The main funtion that prints the torch object to show it is working through Bazel
    """
    try:
        print(torch)
        return 1
    except:
        print("An error has occured")
        return 0

if __name__ == '__main__':
    sys.exit(main())