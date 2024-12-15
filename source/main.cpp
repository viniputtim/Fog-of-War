# include <raylib.h>


Image background_image;
Image fog_image;
Texture background_texture;
Texture fog_texture;
int fog_reveal_radius = 100;
Sound folk_music;


void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(background_texture, 0, 0, WHITE);
    DrawTexture(fog_texture, 0, 0, WHITE);
    DrawFPS(0, 0);
    EndDrawing();
}


void update()
{
    UnloadTexture(fog_texture);
    fog_texture = LoadTextureFromImage(fog_image);

    if (!IsSoundPlaying(folk_music))
    {
        PlaySound(folk_music);
    }
}


void check_events()
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        ImageDrawCircle(&fog_image, GetMouseX(), GetMouseY(), fog_reveal_radius, BLANK);
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


void scale_and_center(Image * image, double max_width, double max_height)
{
    double scale_x = max_width / image->width;
    double scale_y = max_height / image->height;
    double scale = (scale_x > scale_y) ? (scale_x) : (scale_y);
    int new_width = static_cast<int> (image->width * scale);
    int new_height = static_cast<int> (image->height * scale);

    ImageResize(image, new_width, new_height);
}


int main()
{
    InitWindow(0, 0, "Fog of War");
    ToggleFullscreen();
    InitAudioDevice();
    SetMasterVolume(0.5);
    SetTargetFPS(60);

    folk_music = LoadSound("../resources/sounds/folk_music.mp3");
    background_image = LoadImage("../resources/graphics/background.jpg");
    fog_image = LoadImage("../resources/graphics/fog_image.png");
    scale_and_center(&background_image, GetScreenWidth(), GetScreenHeight());
    scale_and_center(&fog_image, GetScreenWidth(), GetScreenHeight());

    background_texture = LoadTextureFromImage(background_image);
    UnloadImage(background_image);

    run();

    UnloadImage(fog_image);
    UnloadTexture(background_texture);
    UnloadTexture(fog_texture);
    CloseWindow();

    return 0;
}