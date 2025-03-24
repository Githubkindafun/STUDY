import pygame
import constants
from character import *
from items import Item

class World():
    def __init__(self):
        self.map_tiles = []
        self.obstacle_tiles = []
        self.exit_tile = None
        self.end_tile = None
        self.item_list = []
        self.player = None
        self.charachter_list = []

    def process_data(self, data, tile_list, item_images, character_dict):
        self.level_length = len(data)
        #iterating through each level in level data file
        for y, row in enumerate(data):
            for x, tile in enumerate(row):
                image = tile_list[tile]
                image_rect = image.get_rect()
                image_x = x * constants.TILE_SIZE
                image_y = y * constants.TILE_SIZE
                image_rect.center = (image_x, image_y)
                tile_data = [image, image_rect, image_x, image_y]
                
                if tile == 7: #------ walls
                    self.obstacle_tiles.append(tile_data)
                elif tile == 8: #------exit-tile
                    self.exit_tile = tile_data
                elif tile == 9: #------coins
                    coin = Item(image_x, image_y, 0, item_images[0])
                    self.item_list.append(coin)
                    tile_data[0] = tile_list[0]
                elif tile == 17: #------hp-potion
                    hp_potion = Item(image_x, image_y, 1, [item_images[1]])
                    self.item_list.append(hp_potion)
                    tile_data[0] = tile_list[0]
                elif tile == 18: #------speed-potion
                    speed_potion = Item(image_x, image_y, 2, [item_images[2]])
                    self.item_list.append(speed_potion)
                    tile_data[0] = tile_list[0]
                elif tile == 19: #------dmg-potion
                    dmg_potion = Item(image_x, image_y, 3, [item_images[3]])
                    self.item_list.append(dmg_potion)
                    tile_data[0] = tile_list[0]
                elif tile == character_dict.get("player"): #-------player
                    player = Player(image_x-2, image_y, constants.PLAYER_HEALTH, 1, constants.SPEED)
                    self.player = player
                    tile_data[0] = tile_list[1]
                elif tile == character_dict.get("tiny_ninja") : #------tiny-ninja
                    enemy = Tiny_ninja(image_x, image_y, 100, 1 , 10, constants.ENEMY_SPEED)
                    self.charachter_list.append(enemy)
                    tile_data[0] = tile_list[1]
                elif tile == character_dict.get("snake_demon"): #------snake-demon
                    enemy = Snake_demon(image_x, image_y, 120, 1 , 15, constants.ENEMY_SPEED + 0.5)
                    self.charachter_list.append(enemy)
                    tile_data[0] = tile_list[1]
                elif tile == character_dict.get("tiny_demon"): #------tiny-demon
                    enemy = Tiny_demon(image_x, image_y, 150, 1 , 15, constants.ENEMY_SPEED - 0.25)
                    self.charachter_list.append(enemy)
                    tile_data[0] = tile_list[1]
                elif tile == character_dict.get("shield_ninja"): #------shield_ninja
                    enemy = Shield_ninja(image_x, image_y, 200, 1 , 20, constants.ENEMY_SPEED - 0.25)
                    self.charachter_list.append(enemy)
                    tile_data[0] = tile_list[1]
                elif tile == character_dict.get("strong_ninja"): #------strong-ninja
                    enemy = Strong_ninja(image_x, image_y, 150, 1 , 30, constants.ENEMY_SPEED + 1)
                    self.charachter_list.append(enemy)
                    tile_data[0] = tile_list[1]
                elif tile == character_dict.get("huge_demon"): #-------huge-demon
                    enemy = Huge_demon(image_x, image_y, 1000, 2 , 20 ,constants.ENEMY_SPEED + 0.25, True)
                    self.charachter_list.append(enemy)
                    tile_data[0] = tile_list[1]
                elif tile == 20: #------end-tile
                    self.end_tile = tile_data
                
                    
                

                #add image data to main tles list
                if tile >= 0:
                    self.map_tiles.append(tile_data)

    def update(self, screen_scroll):
        for tile in self.map_tiles:
            tile[2] += screen_scroll[0]
            tile[3] += screen_scroll[1]
            tile[1].center = (tile[2], tile[3])



    def draw(self, surface):
        for tile in self.map_tiles:
            surface.blit(tile[0], tile[1]) # what (iamge), where (image_rect)
