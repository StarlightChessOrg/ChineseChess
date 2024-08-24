import torch
import numpy as np
import torch.nn as nn
import torch.nn.functional as F


class net(nn.Module):
    def __init__(self):
        super(net, self).__init__()
        self.fc1 = nn.Linear(14 * 256, 128, dtype=torch.float)
        self.fc2 = nn.Linear(128,1,dtype=torch.float)
    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = self.fc2(x)
        return x


if __name__ == "__main__":
    model = net()
    print(model)
