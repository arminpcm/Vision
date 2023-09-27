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
"""NOTE: This file is taken and modified from torchvision"""

import math
import torch
import torch.nn as nn

from collections import OrderedDict
from typing import Callable, Optional, List
from functools import partial

from Python.MachineLearning.Layers.Transformers.MLP import MLPBlock


class EncoderBlock(nn.Module):
    """Transformer encoder block."""
    def __init__(
        self,
        num_heads: int,
        hidden_dim: int,
        mlp_dim: int,
        dropout: float,
        attention_dropout: float,
        norm_layer: Callable[..., torch.nn.Module] = partial(nn.LayerNorm, eps=1e-6),
    ):
        super().__init__()
        self._num_heads = num_heads

        # Attention block
        self._layer_norm_1 = norm_layer(hidden_dim)
        self._self_attention = nn.MultiheadAttention(hidden_dim, num_heads, dropout=attention_dropout, batch_first=True)
        self._dropout = nn.Dropout(dropout)

        # MLP block
        self._layer_norm_2 = norm_layer(hidden_dim)
        self._mlp = MLPBlock(hidden_dim, mlp_dim, dropout)

    def forward(self, input: torch.Tensor):
        torch._assert(input.dim() == 3, f"Expected (batch_size, seq_length, hidden_dim) got {input.shape}")
        x = self._layer_norm_1(input)
        x, _ = self._self_attention(x, x, x, need_weights=False)
        x = self._dropout(x)
        x = x + input

        y = self._layer_norm_2(x)
        y = self._mlp(y)
        return x + y


class Encoder(nn.Module):
    """Transformer Model Encoder for sequence to sequence translation."""
    def __init__(
        self,
        seq_length: int,
        num_layers: int,
        num_heads: int,
        hidden_dim: int,
        mlp_dim: int,
        dropout: float,
        attention_dropout: float,
        norm_layer: Callable[..., torch.nn.Module] = partial(nn.LayerNorm, eps=1e-6),
    ):
        super().__init__()
        # Note that batch_size is on the first dim because
        # we have batch_first=True in nn.MultiAttention() by default
        self._pos_embedding = nn.Parameter(torch.empty(1, seq_length, hidden_dim).normal_(std=0.02))  # from BERT
        self._dropout = nn.Dropout(dropout)
        layers: OrderedDict[str, nn.Module] = OrderedDict()
        for i in range(num_layers):
            layers[f"encoder_layer_{i}"] = EncoderBlock(
                num_heads,
                hidden_dim,
                mlp_dim,
                dropout,
                attention_dropout,
                norm_layer,
            )
        self._layers = nn.Sequential(layers)
        self._layer_norm = norm_layer(hidden_dim)

    def forward(self, input: torch.Tensor):
        assert(input.dim() == 3, f"Expected (batch_size, seq_length, hidden_dim) got {input.shape}")
        input = input + self._pos_embedding
        return self._layer_norm(self._layers(self._dropout(input)))


class VisionTransformer(nn.Module):
    """Vision Transformer as per https://arxiv.org/abs/2010.11929."""
    def __init__(
        self,
        image_size: int,
        image_channels: int,
        patch_size: int,
        num_layers: int,
        num_heads: int,
        hidden_dim: int,
        mlp_dim: int,
        dropout: float = 0.0,
        attention_dropout: float = 0.0,
        num_classes: int = 1000,
        representation_size: Optional[int] = None,
        norm_layer: Callable[..., torch.nn.Module] = partial(nn.LayerNorm, eps=1e-6)
    ):
        super().__init__()
        assert(image_size % patch_size == 0, "Input shape indivisible by patch size!")
        self._image_size = image_size
        self._patch_size = patch_size
        self._hidden_dim = hidden_dim
        self._mlp_dim = mlp_dim
        self._attention_dropout = attention_dropout
        self._dropout = dropout
        self._num_classes = num_classes
        self._representation_size = representation_size
        self._norm_layer = norm_layer
        self._conv_proj = nn.Conv2d(in_channels=image_channels, out_channels=hidden_dim, kernel_size=patch_size, stride=patch_size)

        seq_length = (image_size // patch_size) ** 2

        # Add a class token
        self._class_token = nn.Parameter(torch.zeros(1, 1, hidden_dim))
        seq_length += 1

        self._encoder = Encoder(
            seq_length,
            num_layers,
            num_heads,
            hidden_dim,
            mlp_dim,
            dropout,
            attention_dropout,
            norm_layer,
        )
        self._seq_length = seq_length

        heads_layers: OrderedDict[str, nn.Module] = OrderedDict()
        if representation_size is None:
            heads_layers["head"] = nn.Linear(hidden_dim, num_classes)
        else:
            heads_layers["pre_logits"] = nn.Linear(hidden_dim, representation_size)
            heads_layers["act"] = nn.Tanh()
            heads_layers["head"] = nn.Linear(representation_size, num_classes)

        self._heads = nn.Sequential(heads_layers)

        if isinstance(self._conv_proj, nn.Conv2d):
            # Init the patchify stem
            fan_in = self._conv_proj.in_channels * self._conv_proj.kernel_size[0] * self._conv_proj.kernel_size[1]
            nn.init.trunc_normal_(self._conv_proj.weight, std=math.sqrt(1 / fan_in))
            if self._conv_proj.bias is not None:
                nn.init.zeros_(self._conv_proj.bias)
        elif self._conv_proj.conv_last is not None and isinstance(self._conv_proj.conv_last, nn.Conv2d):
            # Init the last 1x1 conv of the conv stem
            nn.init.normal_(
                self._conv_proj.conv_last.weight, mean=0.0, std=math.sqrt(2.0 / self._conv_proj.conv_last.out_channels)
            )
            if self._conv_proj.conv_last.bias is not None:
                nn.init.zeros_(self._conv_proj.conv_last.bias)

        if hasattr(self._heads, "pre_logits") and isinstance(self._heads.pre_logits, nn.Linear):
            fan_in = self._heads.pre_logits.in_features
            nn.init.trunc_normal_(self._heads.pre_logits.weight, std=math.sqrt(1 / fan_in))
            nn.init.zeros_(self._heads.pre_logits.bias)

        if isinstance(self._heads.head, nn.Linear):
            nn.init.zeros_(self._heads.head.weight)
            nn.init.zeros_(self._heads.head.bias)

    def _process_input(self, x: torch.Tensor) -> torch.Tensor:
        n, c, h, w = x.shape
        p = self._patch_size
        torch._assert(h == self._image_size, f"Wrong image height! Expected {self._image_size} but got {h}!")
        torch._assert(w == self._image_size, f"Wrong image width! Expected {self._image_size} but got {w}!")
        n_h = h // p
        n_w = w // p

        # (n, c, h, w) -> (n, hidden_dim, n_h, n_w)
        x = self._conv_proj(x)
        # (n, hidden_dim, n_h, n_w) -> (n, hidden_dim, (n_h * n_w))
        x = x.reshape(n, self._hidden_dim, n_h * n_w)

        # (n, hidden_dim, (n_h * n_w)) -> (n, (n_h * n_w), hidden_dim)
        # The self attention layer expects inputs in the format (N, S, E)
        # where S is the source sequence length, N is the batch size, E is the
        # embedding dimension
        x = x.permute(0, 2, 1)

        return x

    def forward(self, x: torch.Tensor):
        # Reshape and permute the input tensor
        x = self._process_input(x)
        n = x.shape[0]

        # Expand the class token to the full batch
        batch_class_token = self._class_token.expand(n, -1, -1)
        x = torch.cat([batch_class_token, x], dim=1)

        x = self._encoder(x)

        # Classifier "token" as used by standard language architectures
        x = x[:, 0]

        x = self._heads(x)

        return x
