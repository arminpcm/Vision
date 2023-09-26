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

import math
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F

from torch import Tensor
from typing import Optional, Tuple

# Dimensions of qkv linear mapping layer, 3 for each of the q, k, and v vectors
QKV_DIMS: np.uint16 = 3
EPSILON_LOGIT: np.float32 = -9e15

class MultiheadAttention(nn.Module):
    """Allow the model to attend to various parts of input"""
    def __init__(self, embed_dim: np.uint16, num_heads: np.uint16) -> None:
        """Initialize the module
        :param embed_dim: The total embedding dimenstion of the tokens, should be divisible by the number of heads
        :param num_heads: Number of heads used in multi headed attention layer
        """
        super().__init__()
        assert embed_dim % num_heads == 0, "Embedding dimension must be divisible by the number of heads."

        self._embed_dim = embed_dim
        self._num_heads = num_heads
        self._head_dim = embed_dim // num_heads

        # Stack weight matrices for heads in 1...h for efficiency
        self._qkv_linear_mappings = nn.Linear(embed_dim, QKV_DIMS*embed_dim)
        self._output_linear_mapping = nn.Linear(embed_dim, embed_dim)

        self._reset_parameters()

    def _reset_parameters(self) -> None:
        """Initialize the weights for the linear layers
        """
        nn.init.xavier_uniform_(self._qkv_linear_mappings.weight)
        self._qkv_linear_mappings.bias.data.fill_(0)
        nn.init.xavier_uniform_(self._output_linear_mapping.weight)
        self._output_linear_mapping.bias.data.fill_(0)

    def _expand_mask(self, mask: Tensor) -> Tensor:
        """Convert mask to have 4 dimensions
        :param mask: If specified, a 2D, 3D, or 4D mask preventing attention to certain positions. If 2D, it will be
                     broadcasted over batch size and number of heads. If 3D, it will be broadcasted over
                     over number of heads.
        :return: Output mask of shape (batch_size, num_heads, seq_length, seq_length)
        """
        assert mask.ndim > 2, "Mask must be at least 2D with seq_length X seq_length"
        if mask.ndim == 3:
            mask = mask.unsqueeze(1)
        return mask
    
    def _scaled_dot_product(self, q:Tensor, k:Tensor, v:Tensor, mask: Optional[Tensor]=None):
        """
        :param q: The query embedding tensor of shape (batch_size, num_heads, seq_length, embedding_dim)
        :param k: The key embedding tensor (batch_size, num_heads, seq_length, embedding_dim)
        :param v: The value embedding tensor (batch_size, num_heads, seq_length, embedding_dim)
        :param mask: Optional mask tensor (batch_size, num_heads, sequence_length, sequence_length)
        :return: The scaled dot product values and attention
        """
        d_k = q.size()[-1]
        attn_logits = torch.matmul(q, k.transpose(-2, -1))
        attn_logits = attn_logits / math.sqrt(d_k)
        if mask is not None:
            attn_logits = attn_logits.masked_fill(mask == 0, EPSILON_LOGIT)
        attention = F.softmax(attn_logits, dim=-1)
        values = torch.matmul(attention, v)
        return values, attention

    def forward(self, x:Tensor, mask:Tensor=None) -> Tuple[Tensor]:
        """The forward pass of the layer
        :param x: The input tensor of shape (batch_size, seq_length, embed_dim)
        :param mask: 2D, 3D or 4D mask, has to be broadcastable to (batch_size, num_heads, seq_length, seq_length)
        :return: The output of the layer in shape of (batch_size, seq_length, embed_dim)
        """
        batch_size, seq_length, _ = x.size()
        if mask is not None:
            mask = self._expand_mask(mask)
        qkv = self._qkv_linear_mappings(x)

        # Separate Q, K, V from linear output
        qkv = qkv.reshape(batch_size, seq_length, self._num_heads, QKV_DIMS*self._head_dim)
        qkv = qkv.permute(0, 2, 1, 3) # [Batch, Head, SeqLen, Dims]
        q, k, v = qkv.chunk(QKV_DIMS, dim=-1)

        # Determine value outputs
        values, attention = self._scaled_dot_product(q, k, v, mask=mask)
        values = values.permute(0, 2, 1, 3) # [Batch, SeqLen, Head, Dims]
        values = values.reshape(batch_size, seq_length, self._embed_dim)
        output_embeddings = self._output_linear_mapping(values)

        return output_embeddings, attention
