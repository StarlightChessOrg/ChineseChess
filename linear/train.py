import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
import get_data
import model

device = "cuda" if torch.cuda.is_available() else "cpu"

def scale_tanh(x):
    x = float(x)
    x /= 64.0
    return np.tanh(x)

def cal_distance(root_path,start_file_index):
    filepaths = get_data.get_filepaths(root_path,extension='txt')
    delta_origin = 0
    delta = 0
    cnt = 0
    for i,path in enumerate(filepaths[:start_file_index]):
        with open(path,"r",encoding="utf-8") as f:
            lines = f.readlines()
            for line in lines:
                line = line.strip()
                split_str = line.split(" ")
                basic_eva = float(split_str[1])
                promote_eva = float(split_str[2])
                #print(scale_tanh(basic_eva),scale_tanh(promote_eva))
                delta_origin += abs(basic_eva - promote_eva)
                delta += abs(scale_tanh(basic_eva) - scale_tanh(promote_eva))
                cnt += 1
        #print(f"{i} ; {len(filepaths)}")
    #print(f"delta_origin is {round(delta_origin / cnt, 3)}")
    print(f"basic delta : {round(delta / cnt,3)}")
    return int(delta * 1000)

def convert_to_x_label(game_board,side):
    x_label = np.zeros(shape=(28 * 256),dtype=np.float32)
    for i,p in enumerate(game_board):
        if p > 0 and side > 0:
            x_label[(abs(p) - 1) * 256 + i] = 1
        elif p > 0 and side < 0:
            x_label[(abs(p) - 1 + 14) * 256 + i] = 1
        elif p < 0 and side < 0:
            x_label[(abs(p) + 6) * 256 + i] = 1
        elif p < 0 and side > 0:
            x_label[(abs(p) + 6 + 14) * 256 + i] = 1
    return x_label

def convert_to_xy_labels(game_board,side,promote_eva):
    x_label = np.zeros(shape=(28 * 256),dtype=np.float32)
    for i,p in enumerate(game_board):
        if p > 0 and side > 0:
            x_label[(abs(p) - 1) * 256 + i] = 1
        elif p > 0 and side < 0:
            x_label[(abs(p) - 1 + 14) * 256 + i] = 1
        elif p < 0 and side < 0:
            x_label[(abs(p) + 6) * 256 + i] = 1
        elif p < 0 and side > 0:
            x_label[(abs(p) + 6 + 14) * 256 + i] = 1
    y_label = np.array([scale_tanh(promote_eva)],dtype=np.float32)
    return x_label,y_label

def cal_test_data(start_file_index,model):
    delta = 0
    delta_cnt = 0
    for i,path in enumerate(filepaths[:start_file_index]):
        data_set = get_data.convert_file_to_data(path)
        data_x = []
        data_y = []
        for data in data_set:
            game_board = data['game_board']
            mirror_game_board = data['mirror_game_board']
            side = data['side']
            promote_eva = data['promote_eva']
            #
            x,y = convert_to_xy_labels(game_board, side, promote_eva)
            data_x.append(x)
            data_y.append(y)
            #
            mirror_x,mirror_y = convert_to_xy_labels(mirror_game_board,side,promote_eva)
            data_x.append(mirror_x)
            data_y.append(mirror_y)
        x_batch = torch.from_numpy(np.array(data_x)).to(device,dtype=torch.float)
        y_batch = torch.from_numpy(np.array(data_y)).to(device,dtype=torch.float)
        pred = model(x_batch)
        delta += np.sum(np.abs(pred.cpu().detach().numpy() - y_batch.cpu().numpy())) / len(y_batch)
        delta_cnt += 1
    print(f"test delta : {round(float(delta) / delta_cnt,3)}")
    return int((delta / delta_cnt) * 1000)

def train_one_circle(filepaths,start_file_index,model):
    optimizer = torch.optim.Adam(model.parameters(), lr=2e-5)
    loss_fn = nn.MSELoss()
    learn_delta = 0
    train_sum = 0
    train_loss = 0
    for i,path in enumerate(filepaths[start_file_index:]):
        try:
            data_set = get_data.convert_file_to_data(path)
            data_x = []
            data_y = []
            for data in data_set:
                game_board = data['game_board']
                mirror_game_board = data['mirror_game_board']
                side = data['side']
                promote_eva = data['promote_eva']
                #
                x, y = convert_to_xy_labels(game_board, side, promote_eva)
                data_x.append(x)
                data_y.append(y)
                #
                mirror_x, mirror_y = convert_to_xy_labels(mirror_game_board, side, promote_eva)
                data_x.append(mirror_x)
                data_y.append(mirror_y)
            x_batch = torch.from_numpy(np.array(data_x)).to(device, dtype=torch.float)
            y_batch = torch.from_numpy(np.array(data_y)).to(device, dtype=torch.float)
            pred = model(x_batch)
            loss = loss_fn(pred, y_batch)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            train_loss += loss.item()
            learn_delta += np.sum(np.abs(pred.cpu().detach().numpy() - y_batch.cpu().numpy())) / len(y_batch)
            train_sum += 1
            #print(f"{i} ; {len(filepaths)}")
        except:
            pass

    delta = round(float(learn_delta) / train_sum,3)
    print("learn loss : ",round(train_loss / train_sum,3))
    print("learn delta : ",delta)
    return int(delta * 1000)



if __name__ == "__main__":
    root_path = "E:\\Projects_chess\\dump_3"
    filepaths = get_data.get_filepaths(root_path,extension="txt")
    #filepaths = filepaths[:256]
    model = model.net().to(device)
    start_file_index = 64
    for epoch in range(1,128):
        print("---------------------------")
        print(f"epoch : {epoch}")
        cal_distance(root_path,start_file_index)
        train_one_circle(filepaths,start_file_index,model)
        test_delta = cal_test_data(start_file_index, model)
        torch.save(model, f"../model_{epoch}__{test_delta}.pkl")