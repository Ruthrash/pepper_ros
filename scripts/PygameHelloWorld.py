
import pygame
import pygame.freetype
pygame.init()

state = "[a](!at b p1) [a](!holding a b1) [a](atBox bx1 p1) [a](atBox bx2 p1) [a](atBox bx3 p1) [a](at a p1) [a](in b1 bx1) [a](in b2 bx2) [a](in b3 bx3) [b](!at b p1) [b](!holding a b1) [b](atBox bx1 p1) [b](atBox bx2 p1) [b](atBox bx3 p1) [b](at a p1) [b](in b1 bx1) [b](in b2 bx2) [b](in b3 bx3) (atBox bx1 p1) (atBox bx2 p1) (atBox bx3 p1) (atRobot p1) (at a p1) (dummy) (in b1 bx1) (in b2 bx2) (in b3 bx3)"

#screen = pygame.display.set_mode((800, 600))
X = 500
Y = 500



#  green, blue colour .
white = (255, 255, 255)
green = (0, 255, 0)
blue = (0, 0, 128)


screen = pygame.display.set_mode([X, Y])

font = pygame.font.Font("open-sans/OpenSans-Light.ttf", 24)
text = font.render("Ruthrash", True, green, blue)
textRect = text.get_rect()##creates a text surface object

textRect.center = (X//2,Y //4)

# Run until the user asks to quit

running = True

while running:
    screen.fill((255, 255, 255))
    
    # Did the user click the window close button?
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        screen.blit(text, textRect)
        pygame.draw.circle(screen, (0, 0, 255), (250, 250), 75)
        pygame.display.update()
    # Fill the background with white
    
    # Draw a solid blue circle in the cente
    #pygame.draw.circle(screen, (0, 0, 255), (250, 250), 75)

# Done! Time to quit.

pygame.quit()