import pygame
import random
from datetime import datetime

# Initialize the pygame module
pygame.init()

# Game parameters
WIDTH, HEIGHT = 800, 600  # Screen dimensions
PLAYER_ACC = 0.9         # Acceleration for player movement
FRICTION = -0.12         # Friction affecting player movement (deceleration)
FPS = 60                 # Frames per second (game refresh rate)
WALL_SPEED = 5           # Speed at which walls move down the screen

# Color definitions
WHITE = (255, 255, 255)
RED = (213, 50, 80)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
BACKGROUND_COLOR = (0, 0, 0)

COIN_COUNTER = 0

class Player(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((30, 30))  # Create a rectangular player surface
        self.image.fill(GREEN)                 # Fill the player with green color
        self.rect = self.image.get_rect()      # Get the rectangular area covering the player
        self.rect.center = (x, y)              # Position the player at given (x, y)
        self.vel_x = 0  # Horizontal velocity
        self.vel_y = 0  # Vertical velocity <-- Added

    def update(self, keys):
        # Horizontal movement (left/right)
        if keys[pygame.K_LEFT]:
            self.vel_x -= PLAYER_ACC
        if keys[pygame.K_RIGHT]:
            self.vel_x += PLAYER_ACC

        # Vertical movement (up/down) <-- Added
        if keys[pygame.K_UP]:
            self.vel_y -= PLAYER_ACC
        if keys[pygame.K_DOWN]:
            self.vel_y += PLAYER_ACC

        # Apply friction to horizontal velocity
        self.vel_x += self.vel_x * FRICTION
        # Update player's horizontal position
        self.rect.x += self.vel_x

        # Apply friction to vertical velocity <-- Added
        self.vel_y += self.vel_y * FRICTION
        # Update player's vertical position <-- Added
        self.rect.y += self.vel_y

        # Ensure player doesn't move outside the screen horizontally
        if self.rect.right > WIDTH:
            self.rect.right = WIDTH
        if self.rect.left < 0:
            self.rect.left = 0

        # Ensure player doesn't move outside the screen vertically <-- Added
        if self.rect.bottom > HEIGHT:
            self.rect.bottom = HEIGHT
        if self.rect.top < 0:
            self.rect.top = 0

class Coin(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()
        self.image = pygame.Surface((20, 20))
        self.image.fill(YELLOW)
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y

    def update(self):
        pass

# Define Wall class
class Wall(pygame.sprite.Sprite):
    def __init__(self, x, y, w, h, existing_walls=None):
        super().__init__()
        self.image = pygame.Surface((w, h))  # Create wall surface with given width and height
        self.image.fill(RED)                 # Fill the wall with red color
        self.rect = self.image.get_rect(topleft=(x, y))  # Position the wall at the given (x, y)

        # Ensure new walls don't overlap with existing walls
        if existing_walls:
            while pygame.sprite.spritecollideany(self, existing_walls, collided=None):
                self.rect.y = random.randint(-100, -40)

    def update(self):
        # Move the wall down the screen
        self.rect.y += WALL_SPEED
        # Remove wall from game if it moves past the bottom edge of the screen
        if self.rect.top > HEIGHT:
            self.kill()

def responwn_and_collect_coin(all_sprites, coins, player, chance_per_frame=0.005):
    if random.random() < chance_per_frame:  # 0.2% chance every frame to spawn a coin
        coin_x = random.randint(0, WIDTH - 20)
        coin_y = random.randint(HEIGHT // 2, HEIGHT - 20)  # only on the bottom half
        coin = Coin(coin_x, coin_y)
        all_sprites.add(coin)
        coins.add(coin)
        
    coin_hits = pygame.sprite.spritecollide(player, coins, True)  # The True means the coin will be deleted once collected
    for coin in coin_hits:
        global COIN_COUNTER
        COIN_COUNTER += 1


# Main game loop
def game_loop():
    # Setup display and game title
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Dodge the Wall - Flight Simulator")
    clock = pygame.time.Clock()

    all_sprites = pygame.sprite.Group()  # Group containing all sprites
    walls = pygame.sprite.Group()        # Group containing all wall sprites

    player = Player(WIDTH / 2, HEIGHT - 40)  # Instantiate player
    all_sprites.add(player)                 # Add player to all_sprites group

    running = True
    last_wall_y = 0                         # Track y-position of last generated wall
    start_time = datetime.now()
    
    coins = pygame.sprite.Group()
    
    while running:
        # Calculate elapsed time to adjust wall speed dynamically
        current_time = datetime.now()
        elapsed_time = (current_time - start_time).total_seconds()
        global WALL_SPEED
        WALL_SPEED = 5 + elapsed_time/5

        clock.tick(FPS)  # Cap the game loop to the defined FPS

        responwn_and_collect_coin(all_sprites, coins, player)

        # Generate new walls if there are fewer than 5 on screen
        if len(walls) < 5:
            WALL_WIDTH = 80
            WALL_HEIGHT = random.randint(20, 40)
            random_x = random.randint(0, WIDTH - WALL_WIDTH)
            # Ensure there's a gap between consecutive walls
            gap = 70
            random_y = last_wall_y - WALL_HEIGHT - gap
            last_wall_y = random_y

            wall = Wall(random_x, random_y, WALL_WIDTH, WALL_HEIGHT, walls)
            all_sprites.add(wall)
            walls.add(wall)

        # Check for user events like closing the game window
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Update player and wall positions
        keys = pygame.key.get_pressed()
        player.update(keys)
        walls.update()

        # Check for collisions between player and walls
        hits = pygame.sprite.spritecollide(player, walls, False)
        if hits:
            running = False

        # Draw all sprites on screen and update the display
        screen.fill(BACKGROUND_COLOR)
        all_sprites.draw(screen)
        font = pygame.font.SysFont(None, 36)  # Default font, size 36
        text_surface = font.render(f"Coins: {COIN_COUNTER}", True, WHITE)
        screen.blit(text_surface, (10, 10))  # Draw at top-left
        pygame.display.flip()

    pygame.quit()  # End the game

# Start the game loop
game_loop()
