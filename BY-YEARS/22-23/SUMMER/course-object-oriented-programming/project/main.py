import pygame
from pygame import mixer
import csv
import constants
from character import Character
from weapons import Weapon
from items import Item
from world import World
from button import Button

pygame.init()

screen = pygame.display.set_mode((constants.SCREEN_WIDTH, constants.SCREEN_HIGHT))
pygame.display.set_caption("name of the game - to be changed, any ideas?")

#createing clock for frame rate
clock = pygame.time.Clock()

#define game variables
level = 4
start_game = False
pause_game = False
start_intro = False
screen_scroll = [0, 0] #x, y

#define player movement variables
moving_left = False
moving_right = False
moving_up = False
moving_down = False

#defining font 
font = pygame.font.Font("assets/fonts/AtariClassic.ttf", 21)

#-------------IMAGES HANDLING-------------#

#helper function to scale image
def scale_image(image, scale):
    w = image.get_width()
    h = image.get_height()
    return pygame.transform.scale(image, (w * scale, h * scale)) #new image

#msuic
pygame.mixer.music.load("assets/audio/kolejne_lofi.wav")
pygame.mixer.music.set_volume(0.05)
pygame.mixer.music.play(-1, 0.0, 5000)

#loading buttons images
restart_img = scale_image(pygame.image.load("assets/images/buttons/button_restart.png").convert_alpha(), constants.BUTTON_SCALE)
start_img = scale_image(pygame.image.load("assets/images/buttons/button_start.png").convert_alpha(), constants.BUTTON_SCALE)
exit_img = scale_image(pygame.image.load("assets/images/buttons/button_exit.png").convert_alpha(), constants.BUTTON_SCALE)
resume_img = scale_image(pygame.image.load("assets/images/buttons/button_resume.png").convert_alpha(), constants.BUTTON_SCALE)

#loading heart images (health)
heart_empty = scale_image(pygame.image.load("assets/images/items/heart_empty.png").convert_alpha(), constants.ITEM_SCALE)
heart_half = scale_image(pygame.image.load("assets/images/items/heart_half.png").convert_alpha(), constants.ITEM_SCALE)
heart_full = scale_image(pygame.image.load("assets/images/items/heart_full.png").convert_alpha(), constants.ITEM_SCALE)

#loading coin images
coin_images = []
for x in range(4):
    coin_image = scale_image(pygame.image.load(f"assets/images/items/coin_f{x}.png").convert_alpha(), constants.ITEM_SCALE)
    coin_images.append(coin_image)

#loading potions images
health_potion = scale_image(pygame.image.load(f"assets/images/items/potion_hp.png").convert_alpha(), constants.POTION_SCALE)
speed_potion = scale_image(pygame.image.load(f"assets/images/items/potion_speed.png").convert_alpha(), constants.POTION_SCALE)
dmg_potion = scale_image(pygame.image.load(f"assets/images/items/potion_dmg.png").convert_alpha(), constants.POTION_SCALE)

item_images = []
item_images.append(coin_images)
item_images.append(health_potion)
item_images.append(speed_potion)
item_images.append(dmg_potion)


#loading weapon images
weapon_image = scale_image(pygame.image.load("assets/images/weapons/bow.png").convert_alpha(), constants.WEAPON_SCALE)
bullet_image = scale_image(pygame.image.load("assets/images/weapons/arrow.png").convert_alpha(), constants.BULLET_SCALE)
enemy_bullet_image = scale_image(pygame.image.load("assets/images/weapons/fireball.png").convert_alpha(), constants.ENEMY_BULLET_SCALE)

#loading tilemap images
tile_list = []
for x in range(constants.TILE_TYPES):
    tile_image = pygame.image.load(f"assets/images/tiles/{x}.png").convert_alpha()
    tile_image = pygame.transform.scale(tile_image, (constants.TILE_SIZE, constants.TILE_SIZE))
    tile_list.append(tile_image)






character_dict = {
    "player" : 10 ,
    "tiny_ninja" : 11 ,
    "snake_demon" : 12 ,
    "tiny_demon" : 13 ,
    "shield_ninja" : 14 ,
    "strong_ninja" : 15 ,
    "huge_demon" : 16 ,
}






#outputting text on the screen
def draw_text(text, font, color, x, y):
    text_image = font.render(text, True, color)
    screen.blit(text_image, (x, y))

#function for displaying game info
def draw_info():
    #drawing banner
    pygame.draw.rect(screen, constants.BANNER_COLOR, (0, 0, constants.SCREEN_WIDTH, 50))
    pygame.draw.line(screen, constants.WHITE, (0, 50), (constants.SCREEN_WIDTH, 50))
    #drawing lives
    half_heart_drawn = False
    for i in range(5): #player hp: 100 = 5 hearts = each 20hp = each half-heart is 10 hp
        if player.health >= ((i + 1) * 20):
            screen.blit(heart_full, (10 + i * 50, 0))
        elif player.health % 20 > 0 and half_heart_drawn == False:
            screen.blit(heart_half, (10 + i * 50, 0))
            half_heart_drawn = True
        else:
            screen.blit(heart_empty, (10 + i * 50, 0))

    #showing current level
    draw_text("LEVEL: " + str(level), font, constants.WHITE, constants.SCREEN_WIDTH / 3 + 100 , 15)

    #showing score
    draw_text(f"X{player.score}", font, constants.WHITE, constants.SCREEN_WIDTH - 70, 15)

#------------- -------------#



#-------------WORLD-------------#



#tiny class: damge text (we show how much damage has been dealt)
class DamageText(pygame.sprite.Sprite):
    def __init__(self, x, y, damage, color):
        pygame.sprite.Sprite.__init__(self)
        self.image = font.render(damage, True, color)
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)
        self.timer = 0 # for deleteing dmg text 

    def update(self):
        #repositioning based on the screen scroll
        self.rect.x += screen_scroll[0]
        self.rect.y += screen_scroll[1]
        #moving dmg text up 
        self.rect.y -= 1
        #removing text after few seconds
        self.timer += 1
        if self.timer > 42:
            self.kill()

#class for handling screen fade
class ScreenFade():
    def __init__(self, direction, colour, speed):
        self.direction = direction
        self.colour = colour
        self.speed = speed
        self.fade_counter = 0

    def fade(self):
        fade_complete = False
        self.fade_counter += self.speed 
        if self.direction == 1: #whole screen fade
            pygame.draw.rect(screen, self.colour, (0 - self.fade_counter, 0, constants.SCREEN_WIDTH // 2, constants.SCREEN_HIGHT))
            pygame.draw.rect(screen, self.colour, (constants.SCREEN_WIDTH // 2 + self.fade_counter, 0, constants.SCREEN_WIDTH, constants.SCREEN_HIGHT))
            pygame.draw.rect(screen, self.colour, (0, 0 - self.fade_counter, constants.SCREEN_WIDTH, constants.SCREEN_HIGHT // 2))
            pygame.draw.rect(screen, self.colour, (0, constants.SCREEN_HIGHT // 2 + self.fade_counter, constants.SCREEN_WIDTH, constants.SCREEN_HIGHT ))
        elif self.direction == 2: #vertical screen fade
            pygame.draw.rect(screen, self.colour, (0, 0, constants.SCREEN_WIDTH, 0 + self.fade_counter))
        
        if self.fade_counter >= constants.SCREEN_WIDTH:
            fade_complete = True

        return fade_complete

#function for reseting level
def reset_level():
    damage_text_group.empty()
    bullet_group.empty()
    item_group.empty()
    enemy_bullet_group.empty()

    #creating empty tile list
    data = []
    for r in range(constants.ROWS): 
        r = [-1] * constants.COLS
        data.append(r)

    return data


#create empty tile list
world_data = []
for row in range(constants.ROWS): 
    row = [-1] * constants.COLS
    world_data.append(row)
#loading in level data and creating world
with open(f"levels/level{level}_data.csv", newline="") as csvfile:
    reader = csv.reader(csvfile, delimiter = ",")
    for x, r in enumerate(reader):
       for y, tile in enumerate(r):
            world_data[x][y] = int(tile)
            
world = World()
#world.process_data(world_data, tile_list, item_images, mob_animations)
world.process_data(world_data, tile_list, item_images, character_dict)

#------------- -------------#




#-------------CREATING ALL-------------#

#create player
player = world.player

#create player weapon
weapon = Weapon(weapon_image, bullet_image)

#creating enemy - extract enemies from world data
enemy_list = world.charachter_list

#creating sprite grups (here we will use the cool stuff from "Sprite")
damage_text_group = pygame.sprite.Group()
bullet_group = pygame.sprite.Group()
item_group = pygame.sprite.Group()
enemy_bullet_group = pygame.sprite.Group()

score_coin = Item(constants.SCREEN_WIDTH - 80, 24, 0, coin_images, True)
item_group.add(score_coin)
#add the items from the level data
for item in world.item_list:
    item_group.add(item)

#create screen fades
intro_fade = ScreenFade(1, constants.SALMON_COLORED, 4)
death_fade = ScreenFade(2, constants.BLACK, 4)

#creating buttons
restart_button = Button(constants.SCREEN_WIDTH // 2 - 175, constants.SCREEN_HIGHT // 2 - 50, restart_img)
start_button = Button(constants.SCREEN_WIDTH // 2 - 140, constants.SCREEN_HIGHT // 2 - 150, start_img)
exit_button = Button(constants.SCREEN_WIDTH // 2 - 95, constants.SCREEN_HIGHT // 2 + 50, exit_img)
resume_button = Button(constants.SCREEN_WIDTH // 2 - 180, constants.SCREEN_HIGHT // 2 - 150, resume_img)

#------------- -------------#



#------------- MAIN LOOOP -------------#

run = True

while run:

    #controling frame rate
    clock.tick(constants.FPS)

    if start_game == False:
        screen.fill(constants.MENU_BG)
        if start_button.draw(screen):
            start_game = True
            start_intro = True
        if exit_button.draw(screen):
            run = False
    else:
        if pause_game == True:
            screen.fill(constants.PAUSE_BG)
            if resume_button.draw(screen):
                pause_game = False
            if exit_button.draw(screen):
                run = False

        else:
            screen.fill(constants.BACKGROUND_COLOR)

            if player.alive:
                #calculate player movemnt (how fast)
                dx = 0 # how much to left or right
                dy = 0 # how much down or up

                if moving_left == True:
                    dx = -constants.SPEED
                if moving_right == True:
                    dx = constants.SPEED
                if moving_up == True:
                    dy = -constants.SPEED
                if moving_down == True:
                    dy = constants.SPEED    
                
                #moving player
                
                screen_scroll, level_complete, end = player.move(dx, dy, world.obstacle_tiles, world.exit_tile, world.end_tile)
                
                #------------- UPDATING ALL -------------#

                #update world
                world.update(screen_scroll)
                
                #enemy update
                for enemy in enemy_list:
                    enemy_bullet = enemy.ai(player, world.obstacle_tiles, screen_scroll, enemy_bullet_image)
                    if enemy_bullet:
                        enemy_bullet_group.add(enemy_bullet)
                    if enemy.alive:
                        enemy.update()

                #update player
                player.update()

                #update bullet
                bullet = weapon.update(player)
                if bullet:
                    bullet_group.add(bullet)
                for bullet in bullet_group:
                    damage, damage_pos = bullet.update(screen_scroll, world.obstacle_tiles, enemy_list)
                    if damage: #if any dmg has been dealt
                        damage_text = DamageText(damage_pos.centerx, damage_pos.y, str(damage), constants.DMG_COLOR)
                        damage_text_group.add(damage_text)
                #update damage text
                damage_text_group.update()
                #enemy bullet
                enemy_bullet_group.update(screen_scroll, player, world.obstacle_tiles)
                #update items
                item_group.update(screen_scroll, player)

            #------------- -------------# 

            #------------- DRAWING ALL -------------#
            #drawing world
            world.draw(screen)
            #drawing enemy on screen
            for enemy in enemy_list:
                enemy.draw(screen)
            #drawing player on screen
            player.draw(screen)
            weapon.draw(screen)
            for bullet in bullet_group:
                bullet.draw(screen)
            for enemy_bullet in enemy_bullet_group:
                enemy_bullet.draw(screen)
            damage_text_group.draw(screen)
            item_group.draw(screen)
            draw_info()
            score_coin.draw(screen)
            #------------- -------------#

            #cheking if level is completedsd
            if level_complete == True:
                start_intro = True
                level += 1
                world_data = reset_level()
                #loading in level data and creating world
                with open(f"levels/level{level}_data.csv", newline="") as csvfile:
                    reader = csv.reader(csvfile, delimiter = ",")
                    for x, r in enumerate(reader):
                        for y, tile in enumerate(r):
                                world_data[x][y] = int(tile)   
                world = World()
                #world.process_data(world_data, tile_list, item_images, mob_animations)
                world.process_data(world_data, tile_list, item_images, character_dict)

                tmp_hp = player.health
                tmp_score = player.score
                
                player = world.player
                
                player.score = tmp_score
                player.health = tmp_hp
                
                enemy_list = world.charachter_list
                score_coin = Item(constants.SCREEN_WIDTH - 80, 24, 0, coin_images, True)
                item_group.add(score_coin)
                #add the items from the level data
                for item in world.item_list:
                    item_group.add(item)

            if end == True:
                run = False

            #show intro
            if start_intro == True:
                if intro_fade.fade():
                    start_intro = False
                    intro_fade.fade_counter = 0
                    
            
            #show death_screen
            if player.alive == False:
                if death_fade.fade():
                    if restart_button.draw(screen):
                        death_fade.fade_counter = 0
                        start_intro = True
                        level = 1
                        world_data = reset_level()
                        #loading in level data and creating world
                        with open(f"levels/level{level}_data.csv", newline="") as csvfile:
                            reader = csv.reader(csvfile, delimiter = ",")
                            for x, r in enumerate(reader):
                                for y, tile in enumerate(r):
                                        world_data[x][y] = int(tile)   
                        world = World()
                        #world.process_data(world_data, tile_list, item_images, mob_animations)
                        world.process_data(world_data, tile_list, item_images, character_dict)

                        #temp_score = player.score #do rozważenia co z tym zrobić
                        player = world.player
                        #player.score = temp_score


                        enemy_list = world.charachter_list
                        score_coin = Item(constants.SCREEN_WIDTH - 80, 24, 0, coin_images, True)
                        item_group.add(score_coin)
                        #add the items from the level data
                        for item in world.item_list:
                            item_group.add(item)



    #event handler
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        #taking keaybord input(press)
        if event.type == pygame.KEYDOWN:
            #left
            if event.key == pygame.K_a:
                moving_left = True
            #right
            if event.key == pygame.K_d:
                moving_right = True
            #up
            if event.key == pygame.K_w:
                moving_up = True
            #down
            if event.key == pygame.K_s:
                moving_down = True
            #pause
            if event.key == pygame.K_ESCAPE:
                pause_game = True

        #relesing keaybord input(press)
        if event.type == pygame.KEYUP:
            #left
            if event.key == pygame.K_a:
                moving_left = False
            #right       
            if event.key == pygame.K_d:
                moving_right = False
            #up       
            if event.key == pygame.K_w:
                moving_up = False
            #down
            if event.key == pygame.K_s:
                moving_down = False

    pygame.display.update()
    
pygame.quit()
