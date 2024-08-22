import copy
import os
import numpy as np

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
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,r,n,b,a,k,a,b,n,r,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,c,0,0,0,0,0,c,0,0,0,0,0,
    0,0,0,p,0,p,0,p,0,p,0,p,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,P,0,P,0,P,0,P,0,P,0,0,0,0,
    0,0,0,0,C,0,0,0,0,0,C,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,R,N,B,A,K,A,B,N,R,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
])

# init_game_board = np.asarray([
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,r,n,b,a,k,a,b,n,r,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,0,c,0,0,0,0,0,c,0,0,0,0,0,
#     0,0,0,p,0,p,0,p,0,p,0,p,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,P,0,P,0,P,0,P,0,P,0,0,0,0,
#     0,0,0,0,C,0,0,0,0,0,C,0,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,R,N,B,A,K,A,B,N,R,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
# ])

def make_move(game_board,move):
    from_pos = move & 255
    to_pos = move >> 8
    game_board[to_pos] = copy.deepcopy(game_board[from_pos])
    game_board[from_pos] = 0

def make_mirror_move(game_board,move):
    from_pos = move & 255
    to_pos = move >> 8
    from_x = from_pos >> 4
    from_y = from_pos & 15
    mirror_from_y = 14 - from_y
    mirror_from_pos = (from_x << 4) + mirror_from_y
    to_x = to_pos >> 4
    to_y = to_pos & 15
    mirror_to_y = 14 - to_y
    mirror_to_pos = (to_x << 4) + mirror_to_y
    game_board[mirror_to_pos] = copy.deepcopy(game_board[mirror_from_pos])
    game_board[mirror_from_pos] = 0

def convert_file_to_data(filepath):
    data = []
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()
        game_board = copy.deepcopy(init_game_board)
        mirror_game_board = copy.deepcopy(init_game_board)
        side = 1
        for line in lines:
            line = line.strip()
            split_str = line.split(" ")

            basic_eva = int(split_str[1])
            promote_eva = int(split_str[2])

            _dict = {
                'game_board' : copy.deepcopy(game_board),
                'mirror_game_board' : copy.deepcopy(mirror_game_board),
                'side' : side,
                'basic_eva' : basic_eva,
                'promote_eva' : promote_eva
            }
            data.append(_dict)

            # step
            move = int(split_str[0])
            make_move(game_board,move)
            make_mirror_move(mirror_game_board,move)
            side = -side
    return data


if __name__ == "__main__":
    data = convert_file_to_data("E:\\Projects_chess\\dump_3\\file_0.txt")
    print(np.reshape(data[2]['game_board'],(16,16)))

