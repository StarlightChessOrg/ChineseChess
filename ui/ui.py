import copy
import time
import pygame
import sys

screen_width = 600
screen_height = 700

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
            piece_img = pygame.image.load(f"./resource/{piece_to_picture_name(p)}").convert()
            piece_img.set_colorkey((255,255,255))
            piece_img = pygame.transform.scale(piece_img, (55, 55))
            picture_board[x][y] = [piece_img,12 + y * 66,12 + x * 69]
    return picture_board

def print_picture_board(picture_board,screen):
    for x in range(10):
        for y in range(9):
            instance = picture_board[x][y]
            if instance is not None:
                [img,img_x,img_y] = picture_board[x][y]
                screen.blit(img,(img_x,img_y))

def mount_xy():
    left,middle,right = pygame.mouse.get_pressed()
    x = None
    y = None
    if left:
        pos = pygame.mouse.get_pos()
        y_pot,x_pot = pos
        y = (y_pot - 12) // 66
        x = (x_pot - 12) // 69
        x = min(max(0,x),9)
        y = min(max(0,y),8)
        print(x,y)
    return x,y

def exit():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

def main():
    pygame.init()
    screen = pygame.display.set_mode((screen_width, screen_height))
    back_ground_img = pygame.image.load("./resource/BOARD.BMP")
    back_ground_img = pygame.transform.scale(back_ground_img,(screen_width, screen_height))

    running = True
    while running:
        time.sleep(0.05)
        screen.blit(back_ground_img, (0, 0))
        picture_board = to_picture_board(board)
        print_picture_board(picture_board,screen)
        pygame.display.update()

        mount_xy()
        exit()

    pygame.quit()

if __name__ == "__main__":
    main()

