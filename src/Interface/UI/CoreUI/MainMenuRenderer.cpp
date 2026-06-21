#include "../../../../include/Interface/UI/CoreUI/MainMenuRenderer.h"
#include "raylib.h"

namespace Interface::UI::CoreUI::MainMenuSystem {

    const int BUTTON_WIDTH = 400;
    const int BUTTON_HEIGHT = 60;
    const int BUTTON_SPACING = 20;

    const Color PANEL_COLOR = { 5, 5, 5, 210 };
    const Color TEXT_NORMAL = { 150, 150, 150, 255 };
    const Color TEXT_HOVER = { 255, 255, 255, 255 };

    Rectangle startButton;
    Rectangle settingsButton;
    Rectangle exitButton;

    Font themeFont;
    Font buttonFont;
    Texture2D bgTexture;

    void Initialize() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        themeFont = LoadFontEx(ASSETS_DIR "fonts/theme_font.ttf", 96, 0, 0);
        SetTextureFilter(themeFont.texture, TEXTURE_FILTER_BILINEAR);

        buttonFont = GetFontDefault();

        bgTexture = LoadTexture(ASSETS_DIR "textures/bg_wood.png");

        int totalButtonsHeight = (3 * BUTTON_HEIGHT) + (2 * BUTTON_SPACING);
        int startY = (screenHeight - totalButtonsHeight) / 2 + 100;

        startButton = { (float)(screenWidth - BUTTON_WIDTH) / 2, (float)startY, BUTTON_WIDTH, BUTTON_HEIGHT };
        settingsButton = { (float)(screenWidth - BUTTON_WIDTH) / 2, (float)(startY + BUTTON_HEIGHT + BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT };
        exitButton = { (float)(screenWidth - BUTTON_WIDTH) / 2, (float)(startY + (BUTTON_HEIGHT + BUTTON_SPACING) * 2), BUTTON_WIDTH, BUTTON_HEIGHT };
    }

    void Update(AppState& currentState) {
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, startButton)) currentState = AppState::ACTIVE_SIMULATION;
            else if (CheckCollisionPointRec(mousePos, settingsButton)) { /* Ayarlar AppState::SETTINGS */ }
            else if (CheckCollisionPointRec(mousePos, exitButton)) currentState = AppState::EXIT_REQUESTED;
        }
    }

    void Draw() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        Vector2 mousePos = GetMousePosition();

        ClearBackground(BLACK);

        if (bgTexture.id != 0) {
            Rectangle source = { 0.0f, 0.0f, (float)bgTexture.width, (float)bgTexture.height };
            Rectangle dest = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
            DrawTexturePro(bgTexture, source, dest, Vector2{ 0, 0 }, 0.0f, WHITE);
        }

        int panelWidth = 500;
        int panelX = (screenWidth - panelWidth) / 2;
        DrawRectangle(panelX, 0, panelWidth, screenHeight, PANEL_COLOR);

        const char* title = "PRAESIDIUM";
        float fontSize = 72;
        float fontSpacing = 10;
        float titleWidth = MeasureTextEx(themeFont, title, fontSize, fontSpacing).x;
        Vector2 titlePos = { (screenWidth - titleWidth) / 2.0f, 120.0f };
        DrawTextEx(themeFont, title, titlePos, fontSize, fontSpacing, TEXT_HOVER);

        auto DrawModernButton = [](Rectangle bounds, const char* text, Vector2 mouse, Font font) {
            bool isHovered = CheckCollisionPointRec(mouse, bounds);
            Color currentText = isHovered ? TEXT_HOVER : TEXT_NORMAL;

            float textFontSize = 28;
            float spacing = 2;
            Vector2 textDim = MeasureTextEx(font, text, textFontSize, spacing);
            Vector2 textPos = { bounds.x + (bounds.width - textDim.x) / 2.0f, bounds.y + (bounds.height - textDim.y) / 2.0f };

            if (isHovered) {
                DrawRectangle(bounds.x + 100, bounds.y + bounds.height - 10, bounds.width - 200, 2, TEXT_HOVER);
            }

            DrawTextEx(font, text, textPos, textFontSize, spacing, currentText);
        };

        DrawModernButton(startButton, "OYUNA BASLA", mousePos, buttonFont);
        DrawModernButton(settingsButton, "AYARLAR", mousePos, buttonFont);
        DrawModernButton(exitButton, "CIKIS", mousePos, buttonFont);
    }

    void Close() {
        UnloadFont(themeFont);
        UnloadTexture(bgTexture);
    }
}