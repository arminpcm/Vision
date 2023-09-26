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

import pytorch_lightning as pl
import sys
import torch

from Python.MachineLearning.Layers.Transformers.Attention import MultiheadAttention

def main():
    """Main test function
    """
    # Define input size
    batch_size: np.uint16 = 1
    seq_length: np.uint16 = 10
    embedding_dim: np.uint16 = 6
    num_heads: np.uint16 = 3

    # Set the seed
    pl.seed_everything(42)

    # Set the device
    device = torch.device("cpu")
    print("Device:", device)

    # Generate random input
    input_tensor = torch.randn(batch_size, seq_length, embedding_dim)

    multi_head_attention_layer = MultiheadAttention(embedding_dim, num_heads)

    output, attention = multi_head_attention_layer(input_tensor)

    print(output, attention)

    return 0


if __name__ == '__main__':
    sys.exit(main())  # next section explains the use of sys.exit