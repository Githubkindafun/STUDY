import pygame
import constants
import math
import weapons


def loadsprites(character_type):
    #loading character image
    mob = character_type
    #character image_type
    animation_types = ["idle", "run"]
    print("hehe")
    #laoding images
    animation_list = []

    for animation in animation_types:
        #restet tmp list of images
        tmp_list = []
        for i in range(4):
            image = pygame.image.load(f"assets/images/characters/{mob}/{animation}/{i}.png").convert_alpha()
            w = image.get_width()
            h = image.get_height()
            image = pygame.transform.scale(image, (w * constants.SCALE, h * constants.SCALE))
            tmp_list.append(image)  
        animation_list.append(tmp_list)
    return animation_list

class Character():
    #def __init__(self, x, y, health, mob_animations, character_type, boss, size, speed=0):
    def __init__(self, x, y, size, dmg, speed=0):
        #self.character_type = character_type #to zbędne tutaj bo kazda klasa będzie poprostu innym characterem
        #self.boss = boss # tego się pozbywam i poprostu w przypadku bossa wprowadze poprawki
        #self.score = 0 #tylko player
        self.flip = False
        ##self.animation_list = mob_animations[character_type] #główna rozkmina
        self.frame_index = 0 #podchodzi pod główna rozkmine
        self.action = 0 # 0 - idle, 1 - running 
        self.update_time = pygame.time.get_ticks()
        self.running = False # Flase becouse we start as idle
        #self.health = health
        self.alive = True #dead or alive :o
        #self.hit = False #do ai
        #self.last_hit = pygame.time.get_ticks() #do ai
        self.last_attack = pygame.time.get_ticks() #do ai bossa
        #self.stunned = False #enemy do ai
        #self.speed = speed # to pod kazda postac
        #self.dmg = 0 #to jest tylko pod gracza 

        #self.player_seen = False #to tylko pod metode ai
        #self.last_time_player_seen = None #to tylko pod metode ai
        #self.last_seen_player_pos_x = None #to tylko pod metode ai
        #self.last_seen_player_pos_y = None #to tylko pod metode ai
        
        self.animation_list = loadsprites(self.character_type)
        
        self.image = self.animation_list[self.action][self.frame_index]
        self.rect = pygame.Rect(0, 0, constants.TILE_SIZE * size, constants.TILE_SIZE * size)
        self.rect.center = (x, y)
    


    def move(self, dx, dy, obstacle_tiles, exit_tile = None, end_tile = None):
    #def move(self, dx, dy, obstacle_tiles):
        #screen_scroll = [0, 0]
        
        #level_complete = False
        #end = False

        self.running = False
        if dx != 0 or dy != 0: # if there is any movemnt then player is "runnig"
            self.running = True

        if dx < 0:
            self.flip = True
        if dx > 0:
            self.flip = False

       
        velocity = math.sqrt(dx ** 2 + dy ** 2)
        if velocity > 0:
            dx =  dx / velocity * self.speed
            dy = dy / velocity * self.speed

        #checking for collsions with map in x directions
        self.rect.x += dx
        for obstacle in obstacle_tiles:
            #checking for collsions 
            if obstacle[1].colliderect(self.rect):
                #check wich side the collsion is from
                if dx > 0:
                    self.rect.right = obstacle[1].left
                if dx < 0:
                    self.rect.left = obstacle[1].right     

        #checking for collsions with map in y directions
        self.rect.y += dy
        for obstacle in obstacle_tiles:
            #checking for collsions 
            if obstacle[1].colliderect(self.rect):
                #check wich side the collsion is from
                if dy > 0:
                    self.rect.bottom = obstacle[1].top
                if dy < 0:
                    self.rect.top = obstacle[1].bottom  
        '''
        #logic only for player
        if self.character_type == 0:
            #cheking collision with exit tile
            if exit_tile[1].colliderect(self.rect):
                #cheking if player is close to the center of the exit tile
                exit_dist = math.sqrt(((self.rect.centerx - exit_tile[1].centerx) ** 2) + ((self.rect.centery - exit_tile[1].centery) ** 2))
                if exit_dist < 20:
                    level_complete = True
            
            #cheking collision with end tile
            if end_tile[1].colliderect(self.rect):
                #cheking if player is close to the center of the exit tile
                end_dist = math.sqrt(((self.rect.centerx - end_tile[1].centerx) ** 2) + ((self.rect.centery - end_tile[1].centery) ** 2))
                if end_dist < 20:
                    end = True

            #update scorll based on player position
            #move camera left and right
            if self.rect.right > (constants.SCREEN_WIDTH - constants.SCROOL_TRESH):
                screen_scroll[0] = (constants.SCREEN_WIDTH - constants.SCROOL_TRESH) - self.rect.right
                self.rect.right = constants.SCREEN_WIDTH - constants.SCROOL_TRESH
            if self.rect.left < constants.SCROOL_TRESH:
                screen_scroll[0] = constants.SCROOL_TRESH - self.rect.left
                self.rect.left = constants.SCROOL_TRESH

            #move camera up and down
            if self.rect.bottom > (constants.SCREEN_HIGHT - constants.SCROOL_TRESH):
                screen_scroll[1] = (constants.SCREEN_HIGHT - constants.SCROOL_TRESH) - self.rect.bottom
                self.rect.bottom = constants.SCREEN_HIGHT - constants.SCROOL_TRESH
            if self.rect.top < constants.SCROOL_TRESH:
                screen_scroll[1] = constants.SCROOL_TRESH - self.rect.top
                self.rect.top = constants.SCROOL_TRESH
        '''
        #return screen_scroll, level_complete, end

    '''
    def ai(self, player, obstacle_tiles, screen_scroll, enemy_bullet_image):
        clipped_line = ()
        stun_cooldown = 100
        ai_cooldown = 1000
        ai_dx = 0
        ai_dy = 0
        enemy_bullet = None

        

        #repositioning enemys based on screen scroll
        self.rect.x += screen_scroll[0]
        self.rect.y += screen_scroll[1]

        #creating line of sight from enemy to the player
        line_of_sight = ((self.rect.centerx , self.rect.centery), (player.rect.centerx , player.rect.centery) )
        #checking if line of sights passes obstacle tile
        for obstacle in obstacle_tiles:
            if obstacle[1].clipline(line_of_sight):
                clipped_line = obstacle[1].clipline(line_of_sight)
        
        #cheking disctance from player
        dist = math.sqrt(((self.rect.centerx - player.rect.centerx) ** 2) + ((self.rect.centery - player.rect.centery) ** 2))

        if not clipped_line:
           self.player_seen = True
           self.last_seen_player_pos_x = player.rect.centerx
           self.last_seen_player_pos_y = player.rect.centery
           self.last_time_player_seen = pygame.time.get_ticks()


        if self.player_seen:
            if pygame.time.get_ticks() - self.last_time_player_seen < ai_cooldown:
                ai_dx = self.last_seen_player_pos_x - self.rect.centerx
                ai_dy = self.last_seen_player_pos_y - self.rect.centery
      
        if self.alive:
            if not self.stunned:
                #moving towards player
                self.move(ai_dx, ai_dy, obstacle_tiles)
                #attacking player
                if dist < constants.ATTACK_RANGE and player.hit == False:
                    player.health -= self.dmg
                    player.hit = True
                    player.last_hit = pygame.time.get_ticks()
             
            
            
            #boss shoots bullets too
            enemy_bullet_cooldown = 750
            
            if self.boss:
                if dist < 400:
                    if pygame.time.get_ticks() - self.last_attack >= enemy_bullet_cooldown:
                        enemy_bullet = weapons.Enemy_Bullet(enemy_bullet_image, self.rect.centerx, self.rect.centery, player.rect.centerx, player.rect.centery)
                        self.last_attack = pygame.time.get_ticks()
            

            #checking if hit
            if self.hit == True:
                self.hit = False
                self.last_hit = pygame.time.get_ticks()
                self.stunned = True
                self.running = False
                self.update_action(0)
            
            if(pygame.time.get_ticks() - self.last_hit > stun_cooldown):
                self.stunned = False
              

        return enemy_bullet
    '''

    def update(self):
        #checking if character died ?
        if self.health <= 0:
            self.health = 0
            self.alive = False

        '''
        #timer to reset player taking hit
        hit_cooldown = 1000
        if self.character_type == 0:
            if self.hit == True and (pygame.time.get_ticks() - self.last_hit) > hit_cooldown:
                self.hit = False
        '''

        # what action player is performing
        if self.running == True:
            self.update_action(1) # 1 - running
        else:
            self.update_action(0) # 0 - idle

        animation_cooldown = 70
        #handle animation
        #update animation
        self.image = self.animation_list[self.action][self.frame_index]
        #chcekcing if enaugh time has passed since the last update
        if pygame.time.get_ticks() - self.update_time > animation_cooldown:
            self.frame_index += 1
            self.update_time = pygame.time.get_ticks()
        #cheking if animation has ended
        if self.frame_index >= len(self.animation_list[self.action]):
            self.frame_index = 0

    def update_action(self, new_action):
        #if new action is diffrent to the previous
        if new_action != self.action:
            self.action = new_action
            #updating animation settings
            self.frame_index = 0
            self.update_time = pygame.time.get_ticks()    

    '''
    def draw(self, surface):
        flipped_image = pygame.transform.flip(self.image, self.flip, False)

        if self.character_type == 0: 
            surface.blit(flipped_image, (self.rect.x, self.rect.y - constants.SCALE * constants.OFFSET))   
        elif self.character_type == 5:
            surface.blit(flipped_image, (self.rect.x, self.rect.y - constants.SCALE * (constants.OFFSET - 5)))
        else:
            surface.blit(flipped_image, self.rect)
    '''
            
class Player(Character):
    def __init__(self, x, y, health, size, speed=0):
        self.character_type = "player"
        self.score = 0
        self.health = health
        self.speed = speed
        self.hit = False

        #animation_list = loadsprites(self.character_type)

        super().__init__(x, y, size, speed)

    def move(self, dx, dy, obstacle_tiles, exit_tile = None, end_tile = None):
        super(Player, self).move( dx, dy, obstacle_tiles, exit_tile = None, end_tile = None)

        screen_scroll = [0, 0]
        level_complete = False
        end = False
        #logic only for player
        if self.character_type == "player":
            #cheking collision with exit tile
            if exit_tile[1].colliderect(self.rect):
                #cheking if player is close to the center of the exit tile
                exit_dist = math.sqrt(((self.rect.centerx - exit_tile[1].centerx) ** 2) + ((self.rect.centery - exit_tile[1].centery) ** 2))
                if exit_dist < 20:
                    level_complete = True
            
            #cheking collision with end tile
            if end_tile[1].colliderect(self.rect):
                #cheking if player is close to the center of the exit tile
                end_dist = math.sqrt(((self.rect.centerx - end_tile[1].centerx) ** 2) + ((self.rect.centery - end_tile[1].centery) ** 2))
                if end_dist < 20:
                    end = True

            #update scorll based on player position
            #move camera left and right
            if self.rect.right > (constants.SCREEN_WIDTH - constants.SCROOL_TRESH):
                screen_scroll[0] = (constants.SCREEN_WIDTH - constants.SCROOL_TRESH) - self.rect.right
                self.rect.right = constants.SCREEN_WIDTH - constants.SCROOL_TRESH
            if self.rect.left < constants.SCROOL_TRESH:
                screen_scroll[0] = constants.SCROOL_TRESH - self.rect.left
                self.rect.left = constants.SCROOL_TRESH

            #move camera up and down
            if self.rect.bottom > (constants.SCREEN_HIGHT - constants.SCROOL_TRESH):
                screen_scroll[1] = (constants.SCREEN_HIGHT - constants.SCROOL_TRESH) - self.rect.bottom
                self.rect.bottom = constants.SCREEN_HIGHT - constants.SCROOL_TRESH
            if self.rect.top < constants.SCROOL_TRESH:
                screen_scroll[1] = constants.SCROOL_TRESH - self.rect.top
                self.rect.top = constants.SCROOL_TRESH

        return screen_scroll, level_complete, end

    def update(self):
        super(Player, self).update()
        #timer to reset player taking hit
        hit_cooldown = 1000
        if self.character_type == "player":
            if self.hit == True and (pygame.time.get_ticks() - self.last_hit) > hit_cooldown:
                self.hit = False

    def draw(self, surface):
        flipped_image = pygame.transform.flip(self.image, self.flip, False)
        surface.blit(flipped_image, (self.rect.x, self.rect.y - constants.SCALE * constants.OFFSET))   


class Mob(Character):
    def __init__(self, x, y, health, size, dmg, speed=0, boss=False):
            #self.character_type = "tniy_ninja"
            self.boss = boss
            self.dmg = dmg
            self.health = health
            self.speed = speed
            self.stunned = False
            self.hit = False
            self.last_hit = pygame.time.get_ticks()
            self.stunned = False
            self.player_seen = False #to tylko pod metode ai
            self.last_time_player_seen = None #to tylko pod metode ai
            self.last_seen_player_pos_x = None #to tylko pod metode ai
            self.last_seen_player_pos_y = None #to tylko pod metode ai

            
            super().__init__(x, y, size, dmg ,speed)

    def ai(self, player, obstacle_tiles, screen_scroll, enemy_bullet_image):
        clipped_line = ()
        stun_cooldown = 100
        ai_cooldown = 1000
        ai_dx = 0
        ai_dy = 0
        enemy_bullet = None

        

        #repositioning enemys based on screen scroll
        self.rect.x += screen_scroll[0]
        self.rect.y += screen_scroll[1]

        #creating line of sight from enemy to the player
        line_of_sight = ((self.rect.centerx , self.rect.centery), (player.rect.centerx , player.rect.centery) )
        #checking if line of sights passes obstacle tile
        for obstacle in obstacle_tiles:
            if obstacle[1].clipline(line_of_sight):
                clipped_line = obstacle[1].clipline(line_of_sight)
        
        #cheking disctance from player
        dist = math.sqrt(((self.rect.centerx - player.rect.centerx) ** 2) + ((self.rect.centery - player.rect.centery) ** 2))

        if not clipped_line:
           self.player_seen = True
           self.last_seen_player_pos_x = player.rect.centerx
           self.last_seen_player_pos_y = player.rect.centery
           self.last_time_player_seen = pygame.time.get_ticks()


        if self.player_seen:
            if pygame.time.get_ticks() - self.last_time_player_seen < ai_cooldown:
                ai_dx = self.last_seen_player_pos_x - self.rect.centerx
                ai_dy = self.last_seen_player_pos_y - self.rect.centery
      
        if self.alive:
            if not self.stunned:
                #moving towards player
                self.move(ai_dx, ai_dy, obstacle_tiles)
                #attacking player
                if dist < constants.ATTACK_RANGE and player.hit == False:
                    player.health -= self.dmg
                    player.hit = True
                    player.last_hit = pygame.time.get_ticks()
             
            
            
            #boss shoots bullets too
            enemy_bullet_cooldown = 750
            
            if self.boss:
                if dist < 400:
                    if pygame.time.get_ticks() - self.last_attack >= enemy_bullet_cooldown:
                        enemy_bullet = weapons.Enemy_Bullet(enemy_bullet_image, self.rect.centerx, self.rect.centery, player.rect.centerx, player.rect.centery)
                        self.last_attack = pygame.time.get_ticks()
            

            #checking if hit
            if self.hit == True:
                self.hit = False
                self.last_hit = pygame.time.get_ticks()
                self.stunned = True
                self.running = False
                self.update_action(0)
            
            if(pygame.time.get_ticks() - self.last_hit > stun_cooldown):
                self.stunned = False
              

        return enemy_bullet

    def draw(self, surface):
        flipped_image = pygame.transform.flip(self.image, self.flip, False)
        surface.blit(flipped_image, self.rect)


class Tiny_ninja(Mob):
    def __init__(self, x, y, health, size, dmg, speed=0):
        self.character_type = "tiny_ninja"
 
        super().__init__(x, y, health, size, dmg ,speed)

class Snake_demon(Mob):
    def __init__(self, x, y, health, size, dmg, speed=0):
        self.character_type = "snake_demon"

        super().__init__(x, y, health, size, dmg ,speed)

class Tiny_demon(Mob):
    def __init__(self, x, y, health, size, dmg, speed=0):
        self.character_type = "tiny_demon"

        super().__init__(x, y, health, size, dmg ,speed)

class Shield_ninja(Mob):
    def __init__(self, x, y, health, size, dmg, speed=0):
        self.character_type = "shield_ninja"

        super().__init__(x, y, health, size, dmg ,speed)

class Strong_ninja(Mob):
    def __init__(self, x, y, health, size, dmg, speed=0):
        self.character_type = "strong_ninja"

        super().__init__(x, y, health, size, dmg ,speed)

class Huge_demon(Mob):
    def __init__(self, x, y, health, size, dmg, speed=0, boss=True):
        self.character_type = "huge_demon"
        self.boss = boss
        super().__init__(x, y, health, size, dmg , speed, self.boss)

    def draw(self, surface):
        flipped_image = pygame.transform.flip(self.image, self.flip, False)
        surface.blit(flipped_image, (self.rect.x, self.rect.y - constants.SCALE * (constants.OFFSET - 5)))

