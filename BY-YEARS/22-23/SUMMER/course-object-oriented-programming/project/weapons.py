import pygame
import math
import constants
import random

class Weapon():
    def __init__(self, image, bullet_image):
        self.original_image = image
        self.angle = 0
        self.image = pygame.transform.rotate(self.original_image, self.angle)
        self.bullet_image = bullet_image
        self.rect = self.image.get_rect()
        self.fired = False # trigger for shooting
        self.last_shot = pygame.time.get_ticks() #for cooldown

    def update(self, player): # "glueing" weapon to the player
        shot_cooldown = 420
        bullet = None # in case we didnt click so the whole thing wont just blow up  
        self.rect.center = player.rect.center

        position = pygame.mouse.get_pos() # where the coursor is
        # 0 - x cords, 1 - y cords,  
        x_dist = position[0] - self.rect.centerx 
        y_dist = -(position[1] - self.rect.centery) # negative becouse in pygame y cords increase down the screen 
        self.angle = math.degrees(math.atan2(y_dist, x_dist)) # math >_<

        #mouse input - shoting
        if pygame.mouse.get_pressed()[0] and self.fired == False and (pygame.time.get_ticks() - self.last_shot) > shot_cooldown: # 0 - mouse left button
            bullet = Player_bullet(self.bullet_image, self.rect.centerx, self.rect.centery, self.angle)
            self.fired = True
            self.last_shot = pygame.time.get_ticks()
        # reseting "triger"
        if pygame.mouse.get_pressed()[0] == False:
            self.fired = False
        return bullet

    def draw(self, surface):
        self.image = pygame.transform.rotate(self.original_image, self.angle)
        surface.blit(self.image, ((self.rect.centerx - int(self.image.get_width() / 2)), self.rect.centery - int(self.image.get_height() / 2))) # here we kinda manualy update the weapon positiong, so it would look better


class Bullet(pygame.sprite.Sprite): #iheretance from pygame existing class
    def __init__(self, image, x, y, angle, bullet_speed):
        pygame.sprite.Sprite.__init__(self) #constructor from Sprite
        
        
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)

        # calculate speed based on the angle (math >_<)
        self.dx = math.cos(math.radians(self.angle)) * bullet_speed
        self.dy = -(math.sin(math.radians(self.angle)) * bullet_speed) # pygame y cords increase down the screen thats why we have it negated 

    def draw(self, surface): 
        surface.blit(self.image, ((self.rect.centerx - int(self.image.get_width() / 2)), (self.rect.centery - int(self.image.get_height() / 2)) )) # here we kinda manualy update the weapon positiong, so it would look better


class Player_bullet(Bullet):
    def __init__(self, image, x, y, angle):
        self.angle = angle
        self.original_image = image
        self.image = pygame.transform.rotate(self.original_image, self.angle - 90)
        self.speed = constants.BULLET_SPEED

        super().__init__(image, x, y, angle, self.speed)
          

    def update(self, screen_scroll, obstacle_tiles, enemy_list):
        #variables so it wont blow up
        damage = 0
        damage_pos = None

        #bullet repositiong based on speed
        self.rect.x += screen_scroll[0] + self.dx
        self.rect.y += screen_scroll[1] + self.dy

        #checking for collsions between bullet and wall
        for obstacle in obstacle_tiles:
            if obstacle[1].colliderect(self.rect):
                self.kill()

        # checking if bullets are off screen (well of course we dont want to waste memory ^-^)
        if self.rect.right < 0 or self.rect.left > constants.SCREEN_WIDTH or self.rect.bottom < 0 or self.rect.top > constants.SCREEN_HIGHT:
            self.kill # no more hidden bullets

        #chekcing collision between bullets and enemies
        for enemy in enemy_list:
            if enemy.rect.colliderect(self.rect) and enemy.alive: #was enemy hit by the bullet and is it alive?
                damage = 10 + random.randint(-3, 5) + constants.PLAYER_DMG # 7 -15 dmg dealt ^-^
                damage_pos = enemy.rect #so we would know where to put DamgeText
                enemy.health -= damage
                enemy.hit = True
                self.kill()
                break #we did hit one of them so no need to check the rest of enemies :D

        return damage, damage_pos


class Enemy_Bullet(Bullet): 
    def __init__(self, image, x, y, target_x, target_y):
        x_dist = target_x - x
        y_dist = -(target_y - y)
        self.angle = math.degrees(math.atan2(y_dist, x_dist))
        
        self.original_image = image
        self.image = pygame.transform.rotate(self.original_image, self.angle - 90)

        self.speed = constants.ENEMY_BULLET_SPEED
        
        super().__init__(image, x, y, self.angle, self.speed)
    

    def update(self, screen_scroll, player, obstacle_tiles):
        
        #bullet repositiong based on speed
        self.rect.x += screen_scroll[0] + self.dx
        self.rect.y += screen_scroll[1] + self.dy


        for obstacle in obstacle_tiles:
            if obstacle[1].colliderect(self.rect):
                self.kill()

        # checking if enemy_bullets are off screen (well of course we dont want to waste memory ^-^)
        if self.rect.right < 0 or self.rect.left > constants.SCREEN_WIDTH or self.rect.bottom < 0 or self.rect.top > constants.SCREEN_HIGHT:
            self.kill # no more hidden bullets

        #checking for collision beetwen self and player
        if player.rect.colliderect(self.rect) and player.hit == False:
            player.hit = True
            player.last_hit = pygame.time.get_ticks()
            player.health -= 10
            self.kill() 

    
