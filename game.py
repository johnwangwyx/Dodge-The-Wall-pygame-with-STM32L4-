import pygame
import random
from datetime import datetime
import serial

# Initialize the pygame module
pygame.init()

# Game parameters
WIDTH, HEIGHT = 800, 600  # Screen dimensions
PLAYER_ACC = 60           # Acceleration for player movement
FRICTION = -0.5           # Friction affecting player movement (deceleration)
FPS = 60                  # Frames per second (game refresh rate)
WALL_SPEED = 15           # Speed at which walls move down the screen

# Color definitions
WHITE = (255, 255, 255)
RED = (213, 50, 80)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
BACKGROUND_COLOR = (0, 0, 0)

PLAYER_HEALTH = 3

COIN_COUNTER = 0

class Player(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)

        # Load the airplane image
        self.original_image = pygame.image.load('Assets/Plane2.png').convert_alpha()
        # Scale the image
        self.original_image = pygame.transform.scale(self.original_image, (50, 50))

        # Get the rectangular area covering the player
        self.rect = self.original_image.get_rect()
        # Position the player at given (x, y)
        self.rect.center = (x, y)
        self.vel_x = 0   # Horizontal velocity
        self.vel_y = 0   # Vertical velocity
        self.health = PLAYER_HEALTH  # Initialize health to 2
        self.angle = 0   # Angle for rotation
        self.image = self.original_image.copy()

    def update_with_data(self, roll, pitch):
        pitch = pitch*1.5
        # Clamp the roll and pitch values to the range of -45 to 45 degrees
        roll = max(-45, min(45, roll))
        pitch = max(-45, min(45, pitch))
        # Map the roll and pitch to the player's velocity
        self.vel_x = roll * PLAYER_ACC / 90  # Assuming max roll of 90 maps to max PLAYER_ACC
        self.vel_y = pitch * PLAYER_ACC / 90  # Assuming max pitch of 90 maps to max PLAYER_ACC

        # Update the angle based on roll (left/right tilt)
        self.angle = -roll  # Negative to ensure correct direction of rotation

        # Rotate the triangle based on the current angle
        self.image = pygame.transform.rotate(self.original_image, self.angle)

        # Update position and bounding box
        self.rect = self.image.get_rect(center=self.rect.center)

        # Apply friction to horizontal velocity
        self.vel_x += self.vel_x * FRICTION
        # Update player's horizontal position
        self.rect.x += self.vel_x

        # Apply friction to vertical velocity
        self.vel_y += self.vel_y * FRICTION
        # Update player's vertical position
        self.rect.y += self.vel_y

        # Ensure player doesn't move outside the screen horizontally
        if self.rect.right > WIDTH:
            self.rect.right = WIDTH
        if self.rect.left < 0:
            self.rect.left = 0

        # Ensure player doesn't move outside the screen vertically
        if self.rect.bottom > HEIGHT:
            self.rect.bottom = HEIGHT
        if self.rect.top < 0:
            self.rect.top = 0


class Coin(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()

        # Load the coin image
        self.image = pygame.image.load('Assets/gold_coin.png').convert_alpha()
        # Scale the image
        self.image = pygame.transform.scale(self.image, (30, 30))

        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y

    def update(self):
        pass

class ScrollingBackground:
    def __init__(self, img_path, speed):
        self.image = pygame.image.load(img_path).convert()  # Load the landscape image
        self.speed = speed
        self.y1 = 0
        self.y2 = -HEIGHT  # Set the second image just above the screen

    def update(self):
        self.y1 += self.speed
        self.y2 += self.speed
        # Reset images when they move out of the screen
        if self.y1 >= HEIGHT:
            self.y1 = -HEIGHT
        if self.y2 >= HEIGHT:
            self.y2 = -HEIGHT

    def draw(self, screen):
        screen.blit(self.image, (0, self.y1))
        screen.blit(self.image, (0, self.y2))

class Explosion(pygame.sprite.Sprite):
    def __init__(self, center, frames):
        super().__init__()
        self.frames = frames
        self.image = self.frames[0]
        self.rect = self.image.get_rect(center=center)
        self.frame_index = 0
        self.last_update = pygame.time.get_ticks()
        self.frame_rate = 50  # Milliseconds between frames

    def update(self):
        now = pygame.time.get_ticks()
        if now - self.last_update > self.frame_rate:
            self.last_update = now
            self.frame_index += 1
            if self.frame_index == len(self.frames):
                self.kill()  # Remove the sprite after the animation is done
            else:
                self.image = self.frames[self.frame_index]

# Define Wall class
class Wall(pygame.sprite.Sprite):
    def __init__(self, x, y, w, h, existing_walls=None):
        super().__init__()
        self.image = pygame.Surface((w, h))              # Create wall surface with given width and height
        self.image.fill(RED)                             # Fill the wall with red color
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
    if random.random() < chance_per_frame:                 # 0.2% chance every frame to spawn a coin
        coin_x = random.randint(0, WIDTH - 20)
        coin_y = random.randint(HEIGHT // 2, HEIGHT - 20)  # only on the bottom half
        coin = Coin(coin_x, coin_y)
        all_sprites.add(coin)
        coins.add(coin)
        
    coin_hits = pygame.sprite.spritecollide(player, coins, True)  # The True means the coin will be deleted once collected
    for coin in coin_hits:
        global COIN_COUNTER
        COIN_COUNTER += 1

def draw_text(surf, text, size, x, y):
    font = pygame.font.Font(None, size)
    text_surface = font.render(text, True, WHITE)
    text_rect = text_surface.get_rect()
    text_rect.midtop = (x, y)
    surf.blit(text_surface, text_rect)

def show_start_screen(screen):
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return False  # Exit the game
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    running = False  # Start the game

        screen.fill(BACKGROUND_COLOR)
        draw_text(screen, "Dodge the Wall - Flight Simulator", 64, WIDTH / 2, HEIGHT / 4)
        #draw_text(screen, "Press Space to Start", 22, WIDTH / 2, HEIGHT / 2)

        pygame.display.flip()
        pygame.time.wait(100)

    return True


# Main game loop
def game_loop():
    # Setup display and game title
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Dodge the Wall - Flight Simulator")
    clock = pygame.time.Clock()

    all_sprites = pygame.sprite.Group()      # Group containing all sprites
    walls = pygame.sprite.Group()            # Group containing all wall sprites

    player = Player(WIDTH / 2, HEIGHT - 40)  # Instantiate player
    all_sprites.add(player)                  # Add player to all_sprites group

    running = True
    last_wall_y = 0                          # Track y-position of last generated wall
    start_time = datetime.now()
    
    coins = pygame.sprite.Group()

    # Load heart images
    full_heart = pygame.image.load('Assets/full_heart.png').convert_alpha()
    empty_heart = pygame.image.load('Assets/empty_heart.png').convert_alpha()

    full_heart = pygame.transform.scale(full_heart, (30, 30))
    empty_heart = pygame.transform.scale(empty_heart, (30, 30))

    background = ScrollingBackground('Assets/background.png', 0.15)

    explosion_frames = []
    for i in range(1, 26):
        filename = f'Assets/Explosion/{str(i).zfill(4)}.png'
        img = pygame.image.load(filename).convert_alpha()
        scaled_img = pygame.transform.scale(img, (120, 120))
        explosion_frames.append(scaled_img)    
    explosion_sprites = pygame.sprite.Group()

    if show_start_screen(screen):
        pygame.event.clear()
        # Setup serial port
        ser = serial.Serial('/dev/cu.usbmodem1103', 115200, timeout=0)
        while running:
            # Calculate elapsed time to adjust wall speed dynamically
            current_time = datetime.now()
            elapsed_time = (current_time - start_time).total_seconds()
            rounded_time = round(elapsed_time, 2)
            global WALL_SPEED
            WALL_SPEED = 10.0 + elapsed_time/10
            WALL_SPEED = min(WALL_SPEED, 20.0)

            clock.tick(FPS)  # Cap the game loop to the defined FPS

            responwn_and_collect_coin(all_sprites, coins, player)

            # Generate new walls if thebre are fewer than 5 on screen
            if len(walls) < 5:
                WALL_WIDTH = random.randint(70, 120)
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

            # Read data from UART
            line = ser.readline().decode('utf-8')
            if "roll and pitch," in line:
                _, roll_str, pitch_str = line.split(',')
                try:
                    roll = int(roll_str)
                    pitch = int(pitch_str)
                    player.update_with_data(roll, -pitch)
                except ValueError:
                    pass
            walls.update()

            # Check for collisions between player and walls
            hits = pygame.sprite.spritecollide(player, walls, True)
            if hits:
                expl = Explosion(hits[0].rect.center, explosion_frames)
                explosion_sprites.add(expl)
                all_sprites.add(expl)
                player.health -= 1
                if player.health <= 0:
                    running = False

            # Draw all sprites on screen and update the display
            # screen.fill(BACKGROUND_COLOR)
            background.update()  # Update the background's position
            background.draw(screen)  # Draw the background
            explosion_sprites.update()
            all_sprites.draw(screen)
            
            
            # Display Coins
            font = pygame.font.SysFont(None, 36)  # Default font, size 36
            text_surface = font.render(f"Coins: {COIN_COUNTER}", True, YELLOW)
            screen.blit(text_surface, (10, 10))  # Draw at top-left
            
            # Display Survival Time
            text_surface = font.render(f"Survival Time: {rounded_time}s", True, WHITE)
            screen.blit(text_surface, (10, 35))  # Draw at top-left

            # Display Health as Hearts
            for i in range(player.health):  # Assuming a maximum of 2 health
                if i < player.health:
                    screen.blit(full_heart, (WIDTH - (player.health*40) + (i * 35), 10))
                else:
                    screen.blit(empty_heart, (WIDTH - (player.health*40) + (i * 35), 10))

            # Display Current WALL_SPEED
            speed_text = font.render(f"Wall Speed: {round(WALL_SPEED, 2)}", True, WHITE)  # Round WALL_SPEED for display
            screen.blit(speed_text, (10, 65))

            pygame.display.flip()

    pygame.quit()  # End the game

# Start the game loop
game_loop()
