import os
import random
from PIL import Image, ImageDraw, ImageFont

# =========================
# CONFIGURATION
# =========================

IMAGE_HEIGHT = 28
DIGIT_WIDTH = 28
DIGIT_COUNT = 100
IMAGE_WIDTH = DIGIT_WIDTH * DIGIT_COUNT

RIBBON_DIR = "images/training/ribbon"
ANSWERS_DIR = "images/training/ribbon_ans"


# =========================
# FONT
# =========================

def get_random_font(size):
    font_paths = [
        # DejaVu
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",

        # Liberation
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf",

        # FreeFont
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
    ]

    available_fonts = [path for path in font_paths if os.path.exists(path)]

    return ImageFont.truetype(random.choice(available_fonts), size)

# =========================
# RIBBON
# =========================

def generate_ribbon():
    digits = []
    for i in range(DIGIT_COUNT):
        digits.append(random.randint(1, 9))
    random.shuffle(digits)

    return digits


def draw_ribbon(digits, output_path):
    image = Image.new("L", (IMAGE_WIDTH, IMAGE_HEIGHT), 255)

    draw = ImageDraw.Draw(image)

    for i, digit in enumerate(digits):
        font = get_random_font(24)

        text = str(digit)

        bbox = draw.textbbox((0, 0), text, font=font)

        text_width = bbox[2] - bbox[0]
        text_height = bbox[3] - bbox[1]

        x = (i * DIGIT_WIDTH + (DIGIT_WIDTH - text_width) / 2)

        y = ((IMAGE_HEIGHT - text_height) / 2 - bbox[1])

        draw.text((x, y), text, fill=0, font=font)

    image.save(output_path, "PNG")


# =========================
# ANSWER
# =========================

def ribbon_to_string(digits):
    return "".join(str(digit) for digit in digits)


def save_answer(digits, output_path):
    answer = ribbon_to_string(digits)

    with open(output_path, "w") as file:
        file.write(answer)


# =========================
# DATASET
# =========================

def generate_dataset(n):
    os.makedirs(RIBBON_DIR, exist_ok=True)
    os.makedirs(ANSWERS_DIR, exist_ok=True)

    generated = set()

    i = 0

    while i < n:
        digits = generate_ribbon()

        answer = ribbon_to_string(digits)

        if answer in generated:
            continue

        generated.add(answer)

        image_path = os.path.join(RIBBON_DIR, f"{i}.png")

        answer_path = os.path.join(ANSWERS_DIR, f"{i}.txt")

        draw_ribbon(digits, image_path)
        save_answer(digits, answer_path)

        i += 1

        print(f"{i}/{n} generated")


# =========================
# MAIN
# =========================

if __name__ == "__main__":
    n = int(input("Nb of ribbons to generate: "))

    generate_dataset(n)
