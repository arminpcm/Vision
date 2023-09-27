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

import torch
from typing import Callable, List, Optional

class MLP(torch.nn.Sequential):
    """This block implements the multi-layer perceptron (MLP) module.
    """
    def __init__(
        self,
        in_channels: int,
        hidden_channels: List[int],
        norm_layer: Optional[Callable[..., torch.nn.Module]] = None,
        activation_layer: Optional[Callable[..., torch.nn.Module]] = torch.nn.ReLU,
        inplace: Optional[bool] = None,
        bias: bool = True,
        dropout: float = 0.0,
    ):
        """
        :param in_channels: Number of channels of the input
        :param hidden_channels: List of the hidden channel dimensions
        :param norm_layer: Norm layer that will be stacked on top of the linear layer. If ``None`` this layer won't be used. Default: ``None``
        :param activation_layer: Activation function which will be stacked on top of the normalization layer (if not None), otherwise on top of the linear layer. If ``None`` this layer won't be used. Default: ``torch.nn.ReLU``
        :param inplace: Parameter for the activation layer, which can optionally do the operation in-place.
                Default is ``None``, which uses the respective default values of the ``activation_layer`` and Dropout layer.
        :param bias: Whether to use bias in the linear layer. Default ``True``
        :param dropout: The probability for the dropout layer. Default: 0.0
        """
        # The addition of `norm_layer` is inspired from the implementation of TorchMultimodal:
        # https://github.com/facebookresearch/multimodal/blob/5dec8a/torchmultimodal/modules/layers/mlp.py
        params = {} if inplace is None else {"inplace": inplace}

        layers = []
        in_dim = in_channels
        for hidden_dim in hidden_channels[:-1]:
            layers.append(torch.nn.Linear(in_dim, hidden_dim, bias=bias))
            if norm_layer is not None:
                layers.append(norm_layer(hidden_dim))
            layers.append(activation_layer(**params))
            layers.append(torch.nn.Dropout(dropout, **params))
            in_dim = hidden_dim

        layers.append(torch.nn.Linear(in_dim, hidden_channels[-1], bias=bias))
        layers.append(torch.nn.Dropout(dropout, **params))

        super().__init__(*layers)


class MLPBlock(MLP):
    """Transformer MLP block."""
    def __init__(self, in_dim: int, mlp_dim: int, dropout: float):
        super().__init__(in_dim, [mlp_dim, in_dim], activation_layer=torch.nn.GELU, inplace=None, dropout=dropout)
        
        for m in self.modules():
            if isinstance(m, torch.nn.Linear):
                torch.nn.init.xavier_uniform_(m.weight)
                if m.bias is not None:
                    torch.nn.init.normal_(m.bias, std=1e-6)

    def _load_from_state_dict(
        self,
        state_dict,
        prefix,
        local_metadata,
        strict,
        missing_keys,
        unexpected_keys,
        error_msgs,
    ):
        super()._load_from_state_dict(
            state_dict,
            prefix,
            local_metadata,
            strict,
            missing_keys,
            unexpected_keys,
            error_msgs,
        )