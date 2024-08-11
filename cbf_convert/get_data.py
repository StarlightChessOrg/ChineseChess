import copy
import json

import numpy as np
import os

import multiprocessing as mp

def get_filepaths(dir,extension="cbf"):
    filepaths = []
    for file in os.listdir(dir):
        if file.endswith(extension):
            filepaths.append(os.path.join(dir,file))
        # if len(filepaths) > 8000:
        #     break
    return filepaths

[K,A,B,N,R,C,P] = [1,2,3,4,5,6,7]
[k,a,b,n,r,c,p] = [-1,-2,-3,-4,-5,-6,-7]

init_game_board = np.asarray([
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,r,n,b,a,k,a,b,n,r,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,c,0,0,0,0,0,c,0,0,0,0],
    [0,0,0,p,0,p,0,p,0,p,0,p,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,P,0,P,0,P,0,P,0,P,0,0,0],
    [0,0,0,0,C,0,0,0,0,0,C,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,R,N,B,A,K,A,B,N,R,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
])

#print(init_game_board.shape)

def get_all_legal_moves():
    _move_id2move_action = {}
    _move_action2move_id = {}
    row = ['0', '1', '2', '3', '4', '5', '6', '7', '8']
    column = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    # 士的全部走法
    advisor_labels = ['0314', '1403', '0514', '1405', '2314', '1423', '2514', '1425',
                      '9384', '8493', '9584', '8495', '7384', '8473', '7584', '8475']
    # 象的全部走法
    bishop_labels = ['2002', '0220', '2042', '4220', '0224', '2402', '4224', '2442',
                     '2406', '0624', '2446', '4624', '0628', '2806', '4628', '2846',
                     '7052', '5270', '7092', '9270', '5274', '7452', '9274', '7492',
                     '7456', '5674', '7496', '9674', '5678', '7856', '9678', '7896']
    idx = 0
    for l1 in range(10):
        for n1 in range(9):
            destinations = [(t, n1) for t in range(10)] + \
                           [(l1, t) for t in range(9)] + \
                           [(l1 + a, n1 + b) for (a, b) in
                            [(-2, -1), (-1, -2), (-2, 1), (1, -2), (2, -1), (-1, 2), (2, 1), (1, 2)]]  # 马走日
            for (l2, n2) in destinations:
                if (l1, n1) != (l2, n2) and l2 in range(10) and n2 in range(9):
                    action = column[l1] + row[n1] + column[l2] + row[n2]
                    _move_id2move_action[idx] = action
                    _move_action2move_id[action] = idx
                    idx += 1

    for action in advisor_labels:
        _move_id2move_action[idx] = action
        _move_action2move_id[action] = idx
        idx += 1

    for action in bishop_labels:
        _move_id2move_action[idx] = action
        _move_action2move_id[action] = idx
        idx += 1

    return _move_id2move_action, _move_action2move_id


move_id2move_action, move_action2move_id = get_all_legal_moves()

red = 1
black = -1

def make_move(game_board,x1,y1,x2,y2):
    game_board[x2 + 3][y2 + 3] = game_board[x1 + 3][y1 + 3]
    game_board[x1 + 3][y1 + 3] = 0


def flip180(arr):
    new_arr = arr.reshape(arr.size)
    new_arr = new_arr[::-1]
    new_arr = new_arr.reshape(arr.shape)
    return new_arr


def flip90_left(arr):
    new_arr = np.transpose(arr)
    new_arr = new_arr[::-1]
    return new_arr


def flip90_right(arr):
    new_arr = arr.reshape(arr.size)
    new_arr = new_arr[::-1]
    new_arr = new_arr.reshape(arr.shape)
    new_arr = np.transpose(new_arr)[::-1]
    return new_arr

def convert_to_256_data(filepaths):
    for i,path in enumerate(filepaths):
        with open(path,"r",encoding="utf-8") as f:
            lines = f.readlines()
            for line in lines:
                if "<Move value=" in line: #'  <Move value="44-64" end="1" />
                    if "00-00" not in line:
                        line = line.strip()
                        line = line.replace("<Move value=\"", "")
                        if "end=" not in line:
                            line = line.replace("\" />", "")
                        else:
                            line = line[:5]

                        line = line.replace("-", "")
                        x1 = int(line[1]) + 3
                        y1 = int(line[0]) + 3
                        x2 = int(line[3]) + 3
                        y2 = int(line[2]) + 3
                        from_pos = (x1 << 4) + y1
                        to_pos = (x2 << 4) + y2
                        combinate_move = (to_pos << 8) + from_pos
                        with open(f"E:\\Projects_chess\\dump_2\\{os.path.basename(path)}.txt", "a+", encoding="utf-8") as f:
                            f.write(str(combinate_move) + "\n")
                            print(combinate_move)

def convert_data(filepaths,start_id,end_id,debug=False):
    cnt = 0
    for i,path in enumerate(filepaths):
        with open(path,"r",encoding="utf-8") as f:
            lines = f.readlines()
            game_board = copy.deepcopy(init_game_board)
            y_mirror_game_board = copy.deepcopy(init_game_board)
            flop_180_game_board = -flip180(game_board)
            y_mirror_flop_180_game_board = copy.deepcopy(flop_180_game_board)
            side = red
            y_mirror_side = red
            flop_180_side = black
            y_mirror_flop_180_side = black
            for line in lines:
                line = line.strip()
                if "<Move value=" in line:
                    line = line.replace("<Move value=\"","")
                    line = line.replace("\" />","")
                    line = line.replace("-","")
                    x1 = int(line[1])
                    y1 = int(line[0])
                    x2 = int(line[3])
                    y2 = int(line[2])

                    data = []
                    if "0000" not in line:
                        move_str = f"{x1}{y1}{x2}{y2}"
                        move_id = move_action2move_id[move_str]
                        data.append({
                            "board" : game_board.tolist(),
                            "move_str" : move_str,
                            "move_id" : move_id,
                            "now_go_side" : side
                        })

                        y_mirror_x1 = int(line[1])
                        y_mirror_y1 = 8 - int(line[0])
                        y_mirror_x2 = int(line[3])
                        y_mirror_y2 = 8 - int(line[2])
                        y_mirror_move_str = f"{y_mirror_x1}{y_mirror_y1}{y_mirror_x2}{y_mirror_y2}"
                        y_mirror_move_id = move_action2move_id[y_mirror_move_str]
                        data.append({
                            "board": y_mirror_game_board.tolist(),
                            "move_str": y_mirror_move_str,
                            "move_id": y_mirror_move_id,
                            "now_go_side": y_mirror_side
                        })

                        flop_180_x1 = 9 - int(line[1])
                        flop_180_y1 = 8 - int(line[0])
                        flop_180_x2 = 9 - int(line[3])
                        flop_180_y2 = 8 - int(line[2])
                        flop_180_move_str = f"{flop_180_x1}{flop_180_y1}{flop_180_x2}{flop_180_y2}"
                        flop_180_move_id = move_action2move_id[flop_180_move_str]
                        data.append({
                            "board": flop_180_game_board.tolist(),
                            "move_str": flop_180_move_str,
                            "move_id": flop_180_move_id,
                            "now_go_side": flop_180_side
                        })

                        y_mirror_flop_180_x1 = 9 - int(line[1])
                        y_mirror_flop_180_y1 = int(line[0])
                        y_mirror_flop_180_x2 = 9 - int(line[3])
                        y_mirror_flop_180_y2 = int(line[2])
                        y_mirror_flop_180_move_str = f"{y_mirror_flop_180_x1}{y_mirror_flop_180_y1}{y_mirror_flop_180_x2}{y_mirror_flop_180_y2}"
                        y_mirror_flop_180_move_id = move_action2move_id[y_mirror_flop_180_move_str]
                        data.append({
                            "board": y_mirror_flop_180_game_board.tolist(),
                            "move_str": y_mirror_flop_180_move_str,
                            "move_id": y_mirror_flop_180_move_id,
                            "now_go_side": y_mirror_flop_180_side
                        })

                        make_move(game_board, x1, y1, x2, y2)
                        make_move(y_mirror_game_board,y_mirror_x1,y_mirror_y1,y_mirror_x2,y_mirror_y2)
                        make_move(flop_180_game_board,flop_180_x1,flop_180_y1,flop_180_x2,flop_180_y2)
                        make_move(y_mirror_flop_180_game_board,y_mirror_flop_180_x1,y_mirror_flop_180_y1,y_mirror_flop_180_x2,y_mirror_flop_180_y2)
                        side = -side
                        y_mirror_side = -y_mirror_side
                        flop_180_side = -flop_180_side
                        y_mirror_flop_180_side = -y_mirror_flop_180_side
                        if debug:
                            print(game_board, move_str, side)
                            print()
                            print()
                        if debug:
                            print(y_mirror_game_board, y_mirror_move_str, y_mirror_side)
                            print()
                            print()
                        if debug:
                            print(flop_180_game_board,flop_180_move_str,flop_180_side)
                            print()
                            print()
                        if debug:
                            print(y_mirror_flop_180_game_board, y_mirror_flop_180_move_str, y_mirror_flop_180_side)
                            print()
                            print()
                        cnt += 1
                        with open(f"E:\\Projects_chess\\dump\\{start_id}_{cnt + 1}.json","w+",encoding="utf-8") as f:
                            json.dump(data,f)
    print(f"{start_id} -> {end_id} done")

def parallel_convert_data(filepaths):
    job_sum = 1024
    inv = len(filepaths) // job_sum

    pool = mp.Pool(mp.cpu_count())
    for i in range(job_sum):
        start_index = i * inv
        end_index = min((i + 1) * inv, len(filepaths) - 1)
        pool.apply_async(convert_data, (filepaths[start_index:end_index], start_index, end_index,False))
    pool.close()
    pool.join()

if __name__ == "__main__":
    #filepaths = get_filepaths("E:\\Projects_chess\\imsa-cbf")
    #parallel_convert_data(filepaths)

    filepaths = [os.path.join("E:\\Projects_chess\\imsa-cbf","2013年广东东莞凤岗镇象棋公开赛 {EF70C879-A151-4B84-864A-38BB991418AE}.cbf")]
    convert_to_256_data(filepaths)

