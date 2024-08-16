import copy
import tkinter as tk
import subprocess
import time
import sys
from PIL import Image, ImageTk

p_height = 20

screen_width = 520
screen_height = 600 + p_height

init_board = [
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -8, -6, -4, -2, -1, -3, -5, -7, -9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -10, 0, 0, 0, 0, 0, -11, 0, 0, 0, 0, 0,
    0, 0, 0, -12, 0, -13, 0, -14, 0, -15, 0, -16, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 12, 0, 13, 0, 14, 0, 15, 0, 16, 0, 0, 0, 0,
    0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 8, 6, 4, 2, 1, 3, 5, 7, 9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
]

board = [
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -8, -6, -4, -2, -1, -3, -5, -7, -9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -10, 0, 0, 0, 0, 0, -11, 0, 0, 0, 0, 0,
    0, 0, 0, -12, 0, -13, 0, -14, 0, -15, 0, -16, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 12, 0, 13, 0, 14, 0, 15, 0, 16, 0, 0, 0, 0,
    0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 8, 6, 4, 2, 1, 3, 5, 7, 9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
]

piece_to_type_vec = [1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 7]
piece_name = ["K", "A", "B", "N", "R", "C", "P"]

x1 = -1
y1 = -1
side = 1
show_side = 1

from_select = None
to_select = None

root = tk.Tk()
root.title("ways49")
root.geometry(f"{screen_width}x{screen_height}")
canvas = tk.Canvas(root, width=screen_width, height=screen_height)
canvas.pack()

select_img = ImageTk.PhotoImage(Image.open(f"./resource/OOS.GIF"))
img_board = ImageTk.PhotoImage(Image.open("./resource/BOARD.GIF"))

def write_data(data):
    with open(f"./ui.txt","w+",encoding='utf-8') as f:
        f.write(data)
        f.close()

def read_data():
    with open(f"./ai.txt","r+",encoding='utf-8') as f:
        result = f.readlines()
        f.truncate()
        f.close()
    return result

def clear_data():
    with open(f"./ai.txt", "r+", encoding='utf-8') as f:
        f.truncate()
        f.close()
    with open(f"./ui.txt","w+",encoding='utf-8') as f:
        f.truncate()
        f.close()

def clear_select_label():
    canvas.delete(from_select)
    canvas.delete(to_select)


def I_first_init():
    init_game(1)


def You_first_init():
    init_game(-1)


def regret_move():
    global board, side
    if len(board_pool):
        board = copy.deepcopy(board_pool[-1])
        board_pool.pop()
        side = -side
        clear_select_label()
        write_data("regret\n")


def init_game(set_show_side):
    global board, board_pool, side, show_side
    board = copy.deepcopy(init_board)
    board_pool = []
    side = 1
    show_side = set_show_side
    clear_select_label()
    if show_side == side:
        write_data("init and ai is black\n")
    else:
        write_data("init and ai is red\n")


I_first_go = tk.Button(root, text='我先走', width=8, command=I_first_init)
You_first_go = tk.Button(root, text='它先走', width=8, command=You_first_init)
I_regret = tk.Button(root, text='我悔棋', width=8, command=regret_move)

I_first_go.pack()
I_first_go.place(x=0, y=0)
You_first_go.pack()
You_first_go.place(x=67, y=0)
I_regret.place(x=134, y=0)

board_pool = []


def piece_to_type(piece):
    side = 1 if piece > 0 else -1
    return side, piece_to_type_vec[abs(piece) - 1]


def piece_to_picture_name(piece):
    side, type = piece_to_type(piece)
    side_str = "R" if side == 1 else "B"
    type_str = piece_name[type - 1]
    return side_str + type_str + ".GIF"


def to_picture_board(board):
    picture_board = []
    for x in range(10):
        picture_board.append([])
        for y in range(9):
            picture_board[-1].append(None)
    for i, p in enumerate(board):
        if p:
            x = (i >> 4) - 3
            y = (i & 15) - 3
            if show_side != 1:
                x = 9 - x
            piece_img = Image.open(f"./resource/{piece_to_picture_name(p)}")
            piece_img = ImageTk.PhotoImage(piece_img)
            picture_board[x][y] = [piece_img, y * 58, 20 + p_height + x * 57]
    return picture_board


def print_picture_board(picture_board, canvas):
    for x in range(10):
        for y in range(9):
            instance = picture_board[x][y]
            if instance is not None:
                [img, img_x, img_y] = picture_board[x][y]
                canvas.create_image(img_x, img_y, image=img, anchor='nw')


def get_pos(x, y):
    if show_side == 1:
        return (x + 3) * 16 + y + 3
    convert_x = 9 - x
    return (convert_x + 3) * 16 + y + 3


def set_from_default():
    global x1, y1
    x1 = y1 = -1


def mount_xy(event):
    y = (event.x - 10) / 55.6
    x = (event.y - 70) / 55.5
    x = min(max(0, x), 9)
    y = min(max(0, y), 8)
    x = int(x)
    y = int(y)
    global side
    global x1, y1
    global from_select, to_select
    print(x1, y1)
    if x1 == -1:
        x1 = x
        y1 = y
        clear_select_label()
        if board[get_pos(x1, y1)] * side > 0:
            from_select = canvas.create_image(y * 58, 20 + p_height + x * 57, image=select_img, anchor='nw')
        else:
            set_from_default()
    elif board[get_pos(x1, y1)] * side > 0:
        if board[get_pos(x, y)] * side <= 0:
            to_select = canvas.create_image(y * 58, 20 + p_height + x * 57, image=select_img, anchor='nw')
            from_pos = get_pos(x1, y1)
            to_pos = get_pos(x, y)
            if board[from_pos] * side > 0 and board[from_pos] * board[to_pos] <= 0:
                board_pool.append(copy.deepcopy(board))
                board[from_pos], board[to_pos] = board[to_pos], board[from_pos]
                board[from_pos] = 0
                side = -side
                set_from_default()
                write_data(f"{from_pos}->{to_pos}\n")
        else:
            x1, y1 = x, y
            clear_select_label()
            from_select = canvas.create_image(y * 58, 20 + p_height + x * 57, image=select_img, anchor='nw')
    else:
        set_from_default()
    root.update()


def response_from_ai():
    ai_listen_result = read_data()
    if len(ai_listen_result):
        print(ai_listen_result)


def main():
    clear_data()
    canvas.create_image(0, 20 + p_height, image=img_board, anchor='nw')
    root.bind('<Button-1>', mount_xy)
    while True:
        pic_board = to_picture_board(board)
        print_picture_board(pic_board, canvas)
        root.update()
        time.sleep(0.01)


if __name__ == "__main__":
    main()
