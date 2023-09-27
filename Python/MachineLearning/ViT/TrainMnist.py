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
from torch.nn import CrossEntropyLoss
from torch.optim import Adam
from torch.utils.data import DataLoader
from torchvision.datasets.mnist import MNIST
from torchvision.transforms import ToTensor

from Python.MachineLearning.ViT.ViT import VisionTransformer

# Constants:
BATCH_SIZE:int = 128
N_EPOCHS = 5
LEARNING_RATE = 0.005


def main():
    """
    Main function for training and testing a Vision Transformer model on MNIST dataset.
    """
    # Loading data
    transform = ToTensor()

    train_set = MNIST(
        root="./../datasets", train=True, download=True, transform=transform
    )
    test_set = MNIST(
        root="./../datasets", train=False, download=True, transform=transform
    )

    train_loader = DataLoader(train_set, shuffle=True, batch_size=BATCH_SIZE)
    test_loader = DataLoader(test_set, shuffle=False, batch_size=BATCH_SIZE)

    # Defining model and training options
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(
        "Using device: ",
        device,
        f"({torch.cuda.get_device_name(device)})" if torch.cuda.is_available() else "",
    )

    model = VisionTransformer(
        image_size=28,
        image_channels=1,
        patch_size=7,
        num_layers=2,
        num_heads=2,
        hidden_dim=8,
        mlp_dim=8,
        num_classes=10
    ).to(device)

    # Training loop
    optimizer = Adam(model.parameters(), lr=LEARNING_RATE)
    criterion = CrossEntropyLoss()
    for epoch in range(N_EPOCHS):
        print(f"Epoch: {epoch} started...")
        train_loss:float = 0.0
        for batch in train_loader:
            x, y = batch
            x, y = x.to(device), y.to(device)
            y_hat = model(x)
            loss = criterion(y_hat, y)

            train_loss += loss.detach().cpu().item() / len(train_loader)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

        print(f"Epoch {epoch + 1}/{N_EPOCHS} loss: {train_loss:.2f}")

    # Test loop
    with torch.no_grad():
        correct, total = 0, 0
        test_loss:float = 0.0
        for batch in test_loader:
            x, y = batch
            x, y = x.to(device), y.to(device)
            y_hat = model(x)
            loss = criterion(y_hat, y)
            test_loss += loss.detach().cpu().item() / len(test_loader)

            correct += torch.sum(torch.argmax(y_hat, dim=1) == y).detach().cpu().item()
            total += len(x)
        print(f"Test loss: {test_loss:.2f}")
        print(f"Test accuracy: {correct / total * 100:.2f}%")


if __name__ == '__main__':
    sys.exit(main())
