import pygame

class Button():
    def __init__(self, x, y, image):
        self.image = image
        self.rect = self.image.get_rect()
        self.rect.topleft = (x, y)

    def draw(self, surface):
        action = False

        #get the coursor position
        pos = pygame.mouse.get_pos()

        #checking mouse hover and conditions for clicking
        if self.rect.collidepoint(pos):
            if pygame.mouse.get_pressed()[0]: #0 - left mouse click
                action = True

        surface.blit(self.image, self.rect)
        
        return action

