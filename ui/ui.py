import copy
import tkinter as tk
import time
import sys
from PIL import Image,ImageTk

screen_width = 520
screen_height = 600

board = [
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, -8, -6, -4, -2, -1, -3, -5, -7, -9,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,-10,  0,  0,  0,  0,  0,-11,  0,  0,  0,  0,  0,
    0,  0,  0,-12,  0,-13,  0,-14,  0,-15,  0,-16,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 12,  0, 13,  0, 14,  0, 15,  0, 16,  0,  0,  0,  0,
    0,  0,  0,  0, 10,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  8,  6,  4,  2,  1,  3,  5,  7,  9,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
]

piece_to_type_vec = [1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 7]
piece_name = ["K","A","B","N","R","C","P"]

def piece_to_type(piece):
    side = 1 if piece > 0 else -1
    return side,piece_to_type_vec[abs(piece) - 1]

def piece_to_picture_name(piece):
    side,type = piece_to_type(piece)
    side_str = "R" if side == 1 else "B"
    type_str = piece_name[type - 1]
    return side_str + type_str + ".GIF"

def to_picture_board(board):
    picture_board = []
    for x in range(10):
        picture_board.append([])
        for y in range(9):
            picture_board[-1].append(None)
    for i,p in enumerate(board):
        if p:
            x = (i >> 4) - 3
            y = (i & 15) - 3
            piece_img = Image.open(f"./resource/{piece_to_picture_name(p)}")
            piece_img = ImageTk.PhotoImage(piece_img)
            picture_board[x][y] = [piece_img,0 + y * 58,20 + x * 57]
    return picture_board

def print_picture_board(picture_board,canvas):
    for x in range(10):
        for y in range(9):
            instance = picture_board[x][y]
            if instance is not None:
                [img,img_x,img_y] = picture_board[x][y]
                canvas.create_image(img_x,img_y,image=img,anchor='nw')

def mount_xy(event):
    y = (event.x - 32) // 55
    x = (event.y - 52) // 52
    x = min(max(0,x),9)
    y = min(max(0,y),8)
    print(event.x, event.y)
    print(x, y)
    return x,y
def main():
    root = tk.Tk()
    root.title("ways49")
    root.geometry(f"{screen_width}x{screen_height}")
    canvas = tk.Canvas(root,width=screen_width,height=screen_height)
    canvas.pack()

    img_board = Image.open("./resource/BOARD.GIF")
    img_board = ImageTk.PhotoImage(img_board)
    canvas.create_image(0,20,image=img_board,anchor='nw')
    print_picture_board(to_picture_board(board),canvas)
    root.bind('<Button-1>', mount_xy)
    root.mainloop()

if __name__ == "__main__":
    main()

