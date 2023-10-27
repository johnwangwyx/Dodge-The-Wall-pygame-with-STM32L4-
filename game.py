import pygame
import random

# Pygame initialization
pygame.init()

# Game parameters
WIDTH, HEIGHT = 800, 600
PLAYER_ACC = 0.9
FRICTION = -0.12
FPS = 60
WALL_SPEED = 5

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
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((random.randint(50, 150), 30))
        self.image.fill(RED)
        self.rect = self.image.get_rect()
        self.rect.x = random.randint(0, WIDTH - self.rect.width)
        self.rect.y = 0 - self.rect.height

    def update(self):
        self.rect.y += WALL_SPEED
        if self.rect.top > HEIGHT:
            self.kill()

# Game loop
def game_loop():
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Flight Simulator")
    clock = pygame.time.Clock()

    all_sprites = pygame.sprite.Group()
    walls = pygame.sprite.Group()

    player = Player(WIDTH / 2, HEIGHT - 40)
    all_sprites.add(player)

    running = True
    while running:
        clock.tick(FPS)

        if random.random() > 0.95:
            wall = Wall()
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
