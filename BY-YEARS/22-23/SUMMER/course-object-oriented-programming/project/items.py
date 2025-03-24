import pygame
import constants

class Item(pygame.sprite.Sprite):
    def __init__(self, x, y, item_type, animation_list, dummy_coin = False):
        pygame.sprite.Sprite.__init__(self)
        self.item_type = item_type # 0 - coin, 1 - health potion
        self.animation_list = animation_list
        self.frame_index = 0
        self.update_time = pygame.time.get_ticks()
        self.image = self.animation_list[self.frame_index]
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)
        self.dummy_coin = dummy_coin

    def update(self, screen_scroll, player):
        #does not apply to the dummy coin that is alwyas at the top of the screen
        if not self.dummy_coin:
            #reposition beased on screen scroll
            self.rect.x += screen_scroll[0]
            self.rect.y += screen_scroll[1]

        #lests check if player has collected an item :D
        if self.rect.colliderect(player.rect):
            #coin collected
            if self.item_type == 0:
                player.score += 1
            elif self.item_type == 1:
                player.health += 20
                if player.health > 100:
                    player.health = 100
            elif self.item_type == 2:
                player.speed += 0.12
            elif self.item_type == 3:
                constants.PLAYER_DMG += 1
            self.kill()


        #hadling animation
        animation_cooldown = 150
        #updating image
        self.image = self.animation_list[self.frame_index]
        #cheking if enaught tieme have passed since the last update
        if pygame.time.get_ticks() - self.update_time > animation_cooldown:
            self.frame_index += 1
            self.update_time = pygame.time.get_ticks()
            #wraping up the animation
            if self.frame_index >= len(self.animation_list):
                self.frame_index = 0
            
    def draw(self, surface):
        surface.blit(self.image, self.rect)