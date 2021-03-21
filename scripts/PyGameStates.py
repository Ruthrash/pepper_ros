
import pygame
import pygame.freetype
pygame.init()

def GetPlaceofAgent(belief_list,agent_name):
    for belief in belief_list:#[if "_"+agent_name"_" in belief]:
        print(agent_name , belief)
        if(" "+agent_name+" " in belief):
            print(agent_name , belief)
            if("at" in belief.split(" ") or "!at" in belief.split(" ") ):
                print(belief)
    return


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


class Agent:
    def __init__(self,name_,X,Y,x_max, y_max, place_, tot_agents, instanceGame, screen):
        self.name = name_
        self.x = X 
        self.y = Y 
        self.place = place_
        self.max_x = x_max 
        self.max_y = y_max
        self.tot_agents = tot_agents
        self.objects = dict()
        self.instanceGame = instanceGame
        self.surface = screen
    
    def Addobjects(self, object_, name):
        self.objects[name] = object_                                

    def DrawPlace(self):
        l_y = self.max_y/(2*self.tot_agents)*0.9
        l_x = (self.max_x/2)*0.9
        #print(self.x, self.y, l_x, l_y)
        self.instanceGame.draw.line(self.surface,(255,0,0),
                                    (self.x -l_x ,self.y + l_y),
                                    (self.x + l_x, self.y +l_y) ,2)
        self.instanceGame.draw.line(self.surface,(255,0,0),
                                    (self.x - l_x,self.y - l_y),
                                    (self.x - l_x,self.y +l_y ),2)
        self.instanceGame.draw.line(self.surface,(255,0,0),
                                    (self.x + l_x,self.y - l_y),
                                    (self.x + l_x,self.y +l_y ),2)  
        self.instanceGame.draw.line(self.surface,(255,0,0),
                                    (self.x - l_x ,self.y - l_y ),
                                    (self.x + l_x,self.y - l_y ),2)
    def RenderBeliefs(self):
        return  






state = "[a](!at b p1) [a](!holding a b1) [a](atBox bx1 p1) [a](atBox bx2 p1) [a](atBox bx3 p1) [a](at a p1) [a](in b1 bx1) [a](in b2 bx2) [a](in b3 bx3) [b](!at b p1) [b](!holding a b1) [b](atBox bx1 p1) [b](atBox bx2 p1) [b](atBox bx3 p1) [b](at a p1) [b](in b1 bx1) [b](in b2 bx2) [b](in b3 bx3) (atBox bx1 p1) (atBox bx2 p1) (atBox bx3 p1) (atRobot p1) (at a p1) (dummy) (in b1 bx1) (in b2 bx2) (in b3 bx3)"
states = state.split(')')
state_dict = dict()
for state in states:
    if(len(state.split(']')) > 1):
        if(state.split(']')[0][-1] not in list(state_dict.keys())):
            state_dict[state.split(']')[0][-1]] = list()
        else:
            state_dict[state.split(']')[0][-1]].append(state.split(']')[-1][1:])
    else:
        if("pepper" not in list(state_dict.keys())  ):
            state_dict["pepper"] = list()
        else:
            state_dict["pepper"].append(state.split(']')[-1][1:])
#screen = pygame.display.set_mode((800, 600))

"""
@brief Dimension of rendered window
"""
X = 300 
Y = 300*len(list(state_dict.keys()))

#  green, blue colour .
white = (255, 255, 255)
green = (0, 255, 0)
blue = (0, 0, 128)

screen = pygame.display.set_mode([X, Y])

tot_agents = len(list(state_dict.keys()))
agent1 = Agent(name_="a",X=X/2,Y=Y/(2*tot_agents),x_max=X, y_max=Y, place_="p1", tot_agents=tot_agents, instanceGame=pygame, screen=screen)
agent2 = Agent(name_="b",X=X/2,Y=3*Y/(2*tot_agents),x_max=X, y_max=Y, place_="p1", tot_agents=tot_agents, instanceGame=pygame, screen=screen)
agent2 = Agent(name_="pepper",X=X/2,Y=3*Y/(2*tot_agents),x_max=X, y_max=Y, place_="p1", tot_agents=tot_agents, instanceGame=pygame, screen=screen)

all_agents = list()
for idx,agent in enumerate(list(state_dict.keys())):
    #for belief in state_dict[agent]:
    GetPlaceofAgent(state_dict[agent],agent)  
    print(agent) 
    all_agents.append(Agent(name_=agent,
                            X=X/2,Y=Y/(2*tot_agents),#centroid for agent's belief rendering
                            x_max=X, y_max=Y,#Total screen size
                            place_="p1", # place of events
                            tot_agents=tot_agents, #Total no. of agents
                            instanceGame=pygame,
                            screen=screen))








#textRect.center = (X//2,Y //4)

# Run until the user asks to quit

running = True

while running:
    screen.fill((255, 255, 255))
    #boxA = Box(X/4, Y/2, pygame, screen ,"A")
    #boxB = Box(X/2, Y/2, pygame, screen ,"B")
    #boxC = Box(3*X/4, Y/2, pygame, screen ,"C")
    
    
    # Did the user click the window close button?
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        agent1.DrawPlace()
        agent2.DrawPlace()
        #boxA.DrawCircle()
        #boxA.DrawBox()
        #boxB.DrawBox()
        #boxC.DrawBox()        

        pygame.display.update()
    # Fill the background with white
    
    # Draw a solid blue circle in the cente
    #pygame.draw.circle(screen, (0, 0, 255), (250, 250), 75)

# Done! Time to quit.

pygame.quit()