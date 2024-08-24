import copy
import torch
import model
import get_data
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import train

[K,A,B,N,R,C,P] = [1,2,3,4,5,6,7]
[k,a,b,n,r,c,p] = [-1,-2,-3,-4,-5,-6,-7]

single_game_board = np.asarray([
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,r,n,b,a,k,a,b,n,r,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,c,0,0,0,0,0,c,0,0,0,0,0,
    0,0,0,p,0,0,0,p,0,p,0,p,0,0,0,0,
    0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,P,0,P,0,P,0,P,0,P,0,0,0,0,
    0,0,0,0,C,N,0,0,0,0,C,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,R,0,B,A,K,A,B,N,R,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
])

model_path = "../model_31__132.pkl"

def single_forward():
    nnue = torch.load(model_path)
    game_board = copy.deepcopy(single_game_board)
    side = 1
    x_numpy = train.convert_to_x_label(game_board, side)
    x_label = torch.from_numpy(x_numpy).to(train.device)
    y = nnue(x_label)
    print(y)

def hand_forward():
    nnue = torch.load(model_path)
    p1 = 1
    p2 = 1
    layer_weight_1 = nnue.fc1.weight.detach().cpu().numpy() * p1
    layer_bias_1 = nnue.fc1.bias.detach().cpu().numpy() * p1
    layer_weight_2 = nnue.fc2.weight.detach().cpu().numpy() * p2
    layer_bias_2 = nnue.fc2.bias.detach().cpu().numpy() * p2
    # layer_weight_1 = layer_weight_1.astype(np.int32)
    # layer_bias_1 = layer_bias_1.astype(np.int32)
    # layer_weight_2 = layer_weight_2.astype(np.int32)
    # layer_bias_2 = layer_bias_2.astype(np.int32)
    print(layer_weight_1)
    print(layer_weight_2)
    #print(layer_weight_1.shape,layer_bias_1.shape)
    game_board = copy.deepcopy(single_game_board)
    #print(game_board.reshape(16,16))
    side = 1
    x_numpy = train.convert_to_x_label(game_board)
    x_label = torch.from_numpy(x_numpy).to(train.device)
    #y = nnue(x_label)
    #print(y)
    _input_layer_1 = np.zeros(shape=(128),dtype=np.float32)
    for i in range(len(x_numpy)):
        if x_numpy[i] != 0:
            for c in range(128):
                _input_layer_1[c] += layer_weight_1[c][i]
    _input_layer_1 += layer_bias_1

    for c in range(128):
        if _input_layer_1[c] < 0:
            _input_layer_1[c] = 0
        #else:
            #_input_layer_1[c] = int(_input_layer_1[c]) >> 6

    _output_layer_2 = 0
    for i in range(128):
        _output_layer_2 += _input_layer_1[i] * layer_weight_2[0][i]
    _output_layer_2 += layer_bias_2

    print(_output_layer_2 * 128)

def pop_param():
    nnue = torch.load(model_path)
    layer_weight_1 = nnue.fc1.weight.detach().cpu().numpy()
    layer_bias_1 = nnue.fc1.bias.detach().cpu().numpy()

    with open("E:\\layer_1_weight.txt","w+",encoding="utf-8") as f:
        for i in range(len(layer_weight_1)):
            for a in range(len(layer_weight_1[i])):
                f.write(str(round(layer_weight_1[i][a],8)))
                f.write(" ")
            f.write("\n")
        f.close()

    with open("E:\\layer_1_bias.txt","w+",encoding="utf-8") as f:
        for i in range(len(layer_bias_1)):
            f.write(str(round(layer_bias_1[i],8)))
            f.write(" ")
        f.write("\n")
        f.close()

    layer_weight_2 = nnue.fc2.weight.detach().cpu().numpy()
    layer_bias_2 = nnue.fc2.bias.detach().cpu().numpy()

    with open("E:\\layer_2_weight.txt","w+",encoding="utf-8") as f:
        for i in range(len(layer_weight_2)):
            for a in range(len(layer_weight_2[i])):
                f.write(str(round(layer_weight_2[i][a],8)))
                f.write(" ")
            f.write("\n")
        f.close()

    with open("E:\\layer_2_bias.txt","w+",encoding="utf-8") as f:
        for i in range(len(layer_bias_2)):
            f.write(str(round(layer_bias_2[i],8)))
            f.write(" ")
        f.write("\n")
        f.close()

if __name__ == "__main__":
    pop_param()
    #hand_forward()
    #single_forward()