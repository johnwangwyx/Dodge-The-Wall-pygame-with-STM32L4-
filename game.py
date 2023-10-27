import pygame
import random

# Pygame initialization
pygame.init()

# Game parameters
WIDTH, HEIGHT = 800, 600
PLAYER_ACC = 0.9
FRICTION = -0.12
FPS = 60
WALL_SPEED = 10

# Colors
WHITE = (255, 255, 255)
RED = (213, 50, 80)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
BACKGROUND_COLOR = (0, 0, 0)

# Player class
class Player(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((30, 30))
        self.image.fill(GREEN)
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)
        self.vel_x = 0

    def update(self, keys):
        if keys[pygame.K_LEFT]:
            self.vel_x -= PLAYER_ACC
        if keys[pygame.K_RIGHT]:
            self.vel_x += PLAYER_ACC

        self.vel_x += self.vel_x * FRICTION
        self.rect.x += self.vel_x

        if self.rect.right > WIDTH:
            self.rect.right = WIDTH
        if self.rect.left < 0:
            self.rect.left = 0

# Wall class
class Wall(pygame.sprite.Sprite):
    def __init__(self, x, y, w, h, existing_walls=None):
        super().__init__()
        self.image = pygame.Surface((w, h))
        self.image.fill(RED)
        self.rect = self.image.get_rect(topleft=(x, y))

        if existing_walls:
            while pygame.sprite.spritecollideany(self, existing_walls, collided=None):
                self.rect.y = random.randint(-100, -40)

    def update(self):
        self.rect.y += WALL_SPEED
        if self.rect.top > HEIGHT:
            self.kill()

# Game loop
def game_loop():
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Dodge the Wall - Flight Simulator")
    clock = pygame.time.Clock()

    all_sprites = pygame.sprite.Group()
    walls = pygame.sprite.Group()

    player = Player(WIDTH / 2, HEIGHT - 40)
    all_sprites.add(player)

    running = True
    last_wall_y = 0
    while running:
        clock.tick(FPS)

        if len(walls) < 5:
            WALL_WIDTH = 80
            WALL_HEIGHT = random.randint(20, 40)
            random_x = random.randint(0, WIDTH - WALL_WIDTH)
            
            # Make sure there's a gap between this wall and the last
            gap = 100
            random_y = last_wall_y - WALL_HEIGHT - gap
            last_wall_y = random_y
            
            wall = Wall(random_x, random_y, WALL_WIDTH, WALL_HEIGHT, walls)
            all_sprites.add(wall)
            walls.add(wall)

        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Update
        keys = pygame.key.get_pressed()
        player.update(keys)

        walls.update()

        hits = pygame.sprite.spritecollide(player, walls, False)
        if hits:
            running = False

        # Draw / render
        screen.fill(BACKGROUND_COLOR)
        all_sprites.draw(screen)
        pygame.display.flip()

    pygame.quit()

# Run the game
game_loop()
