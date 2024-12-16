# include <raylib.h>
# include <vector>


std::vector<Texture> anime_girls_textures;
Image fog_image;
Image fog_image_original;
Texture fog_texture;
int fog_reveal_radius = 50;
bool fog_needs_update = true;
Sound folk_music;
long int fog_pixels_remaining = 0;
int current_girl = 0;


void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(anime_girls_textures[current_girl % anime_girls_textures.size()], 0, 0, WHITE);
    DrawTexture(fog_texture, 0, 0, WHITE);
    DrawFPS(0, 0);
    EndDrawing();
}


void update()
{
    if (fog_needs_update)
    {
        UnloadTexture(fog_texture);
        fog_texture = LoadTextureFromImage(fog_image);
        fog_needs_update = false;
    }

    if (!IsSoundPlaying(folk_music))
    {
        PlaySound(folk_music);
    }
}


void check_events()
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        int center_x = GetMouseX();
        int center_y = GetMouseY();
        int radius = fog_reveal_radius;

        for (int y = -radius; y <= radius; y++)
        {
            for (int x = -radius; x <= radius; x++)
            {
                // Calcular a posição do pixel atual
                int pixel_x = center_x + x;
                int pixel_y = center_y + y;

                // Verificar se está dentro da imagem e dentro do círculo
                if (x * x + y * y <= radius * radius &&
                    pixel_x >= 0 && pixel_x < fog_image.width &&
                    pixel_y >= 0 && pixel_y < fog_image.height)
                {
                    // Verificar se o pixel ainda não foi revelado
                    Color pixel_color = GetImageColor(fog_image, pixel_x, pixel_y);
                    if (pixel_color.a != 0) // Pixel não é transparente
                    {
                        fog_pixels_remaining--;
                        ImageDrawPixel(&fog_image, pixel_x, pixel_y, BLANK); // Revela o pixel
                    }
                }
            }
        }

        fog_needs_update = true;

        // Verificar se a neblina foi completamente limpa
        if (fog_pixels_remaining <= 0)
        {
            ImageClearBackground(&fog_image, BLANK);
            ImageDraw(&fog_image, fog_image_original,
                      (Rectangle){0, 0, fog_image_original.width, fog_image_original.height},
                      (Rectangle){0, 0, fog_image.width, fog_image.height}, WHITE);

            fog_reveal_radius = (fog_reveal_radius < 5) ? 5 : (fog_reveal_radius - 5);
            current_girl++;
            fog_pixels_remaining = fog_image.width * fog_image.height;
            fog_needs_update = true;
        }
    }
}



void run()
{
    while (!WindowShouldClose())
    {
        check_events();
        update();
        draw();
    }
}


void scale_and_crop(Image * image, double min_width, double min_height)
{
    double scale_x = min_width / image->width;
    double scale_y = min_height / image->height;
    double scale = (scale_x > scale_y) ? (scale_x) : (scale_y);
    int new_width = static_cast<int> (image->width * scale);
    int new_height = static_cast<int> (image->height * scale);

    ImageResize(image, new_width, new_height);
    ImageCrop(image, (Rectangle) {0, 0, GetScreenWidth(), GetScreenHeight()});
}


void load_anime_girls()
{
    for (int i = 1; i <= 8; ++i)
    {
        Image image = LoadImage(TextFormat("../resources/graphics/anime_girl_0%i.jpg", i));
        scale_and_crop(&image, GetScreenWidth(), GetScreenHeight());
        Texture texture = LoadTextureFromImage(image);
        anime_girls_textures.push_back(texture);
    }
}


int main()
{
    InitWindow(0, 0, "Fog of War");
    ToggleFullscreen();
    InitAudioDevice();
    SetMasterVolume(0.5);
    SetTargetFPS(0);

    load_anime_girls();

    fog_pixels_remaining = GetScreenWidth() * GetScreenHeight();
    folk_music = LoadSound("../resources/sounds/folk_music.mp3");
    fog_image_original = LoadImage("../resources/graphics/fog_image.png");
    fog_image = ImageCopy(fog_image_original);
    scale_and_crop(&fog_image, GetScreenWidth(), GetScreenHeight());



    run();

    UnloadImage(fog_image);

    for (Texture& texture : anime_girls_textures)
    {
        UnloadTexture(texture);
    }

    UnloadTexture(fog_texture);
    CloseWindow();

    return 0;
}