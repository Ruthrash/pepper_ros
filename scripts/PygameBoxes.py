
import pygame
import pygame.freetype
pygame.init()



class Box:
    def __init__(self, center_x, center_y, instanceGame,  screen,name ):
        self.l = 120 
        self.b = 40
        self.x = center_x
        self.y = center_y
        self.instanceGame = instanceGame
        self.surface = screen
        self.name = name
        font = pygame.font.Font("open-sans/OpenSans-Light.ttf", 24)
        self.text = font.render(self.name, True, green, blue)
        self.textRect = self.text.get_rect()##creates a text surface object
        self.textRect.center = (self.x, self.y + self.l/2 + 25)
    
    def DrawBox(self):
        self.instanceGame.draw.line(self.surface,(0,0,255),
                                    (self.x - self.b/2 ,self.y + self.l/2 ),
                                    (self.x + self.b/2,self.y +self.l/2 ),2)
        self.instanceGame.draw.line(self.surface,(0,0,255),
                                    (self.x - self.b/2 ,self.y - self.l/2 ),
                                    (self.x - self.b/2,self.y +self.l/2 ),2)
        self.instanceGame.draw.line(self.surface,(0,0,255),
                                    (self.x + self.b/2 ,self.y - self.l/2 ),
                                    (self.x + self.b/2,self.y +self.l/2 ),2)  
        self.surface.blit(self.text, self.textRect)                                                                                                 

    def DrawCircle(self):
        self.instanceGame.draw.circle(self.surface, (0, 0, 255), (self.x, self.y), 10)

        


state = "[a](!at b p1) [a](!holding a b1) [a](atBox bx1 p1) [a](atBox bx2 p1) [a](atBox bx3 p1) [a](at a p1) [a](in b1 bx1) [a](in b2 bx2) [a](in b3 bx3) [b](!at b p1) [b](!holding a b1) [b](atBox bx1 p1) [b](atBox bx2 p1) [b](atBox bx3 p1) [b](at a p1) [b](in b1 bx1) [b](in b2 bx2) [b](in b3 bx3) (atBox bx1 p1) (atBox bx2 p1) (atBox bx3 p1) (atRobot p1) (at a p1) (dummy) (in b1 bx1) (in b2 bx2) (in b3 bx3)"

#screen = pygame.display.set_mode((800, 600))
X = 500
Y = 500




#  green, blue colour .
white = (255, 255, 255)
green = (0, 255, 0)
blue = (0, 0, 128)


screen = pygame.display.set_mode([X, Y])




#textRect.center = (X//2,Y //4)

# Run until the user asks to quit

running = True

while running:
    screen.fill((255, 255, 255))
    boxA = Box(X/4, Y/2, pygame, screen ,"A")
    boxB = Box(X/2, Y/2, pygame, screen ,"B")
    boxC = Box(3*X/4, Y/2, pygame, screen ,"C")
    
    # Did the user click the window close button?
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        boxA.DrawCircle()
        boxA.DrawBox()
        boxB.DrawBox()
        boxC.DrawBox()        

        pygame.display.update()
    # Fill the background with white
    
    # Draw a solid blue circle in the cente
    #pygame.draw.circle(screen, (0, 0, 255), (250, 250), 75)

# Done! Time to quit.

pygame.quit()