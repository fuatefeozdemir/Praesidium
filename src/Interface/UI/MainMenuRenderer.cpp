#include "../../../../include/Interface/UI/MainMenuRenderer.h"

#include "raylib.h"

namespace {

    constexpr int BUTTON_WIDTH = 400;
    constexpr int BUTTON_HEIGHT = 60;
    constexpr int BUTTON_SPACING = 20;

    constexpr int PANEL_WIDTH = 500;
    constexpr float TITLE_FONT_SIZE = 72.0f;
    constexpr float TITLE_SPACING = 10.0f;
    constexpr float BUTTON_FONT_SIZE = 28.0f;
    constexpr float BUTTON_TEXT_SPACING = 2.0f;
    constexpr float TITLE_POS_Y = 120.0f;

    constexpr Color PANEL_COLOR = { 5, 5, 5, 210 };
    constexpr Color TEXT_NORMAL = { 150, 150, 150, 255 };
    constexpr Color TEXT_HOVER = { 255, 255, 255, 255 };

    Rectangle startButton{};
    Rectangle settingsButton{};
    Rectangle exitButton{};

    Font themeFont{};
    Font buttonFont{};
    Texture2D backgroundTexture{};

    void DrawButton(
        Rectangle bounds,
        const char* text,
        Vector2 mousePosition,
        Font font) {

        const bool isHovered = CheckCollisionPointRec(mousePosition, bounds);
        const Color textColor = isHovered ? TEXT_HOVER : TEXT_NORMAL;

        const Vector2 textSize = MeasureTextEx(
            font,
            text,
            BUTTON_FONT_SIZE,
            BUTTON_TEXT_SPACING
        );

        const Vector2 textPosition = {
            bounds.x + (bounds.width - textSize.x) / 2.0f,
            bounds.y + (bounds.height - textSize.y) / 2.0f
        };

        if (isHovered) {
            DrawRectangle(
                static_cast<int>(bounds.x + 100),
                static_cast<int>(bounds.y + bounds.height - 10),
                static_cast<int>(bounds.width - 200),
                2,
                TEXT_HOVER
            );
        }

        DrawTextEx(
            font,
            text,
            textPosition,
            BUTTON_FONT_SIZE,
            BUTTON_TEXT_SPACING,
            textColor
        );
    }

    void UpdateLayout() {

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        const int totalButtonsHeight =
            (3 * BUTTON_HEIGHT) + (2 * BUTTON_SPACING);

        const int startY =
            (screenHeight - totalButtonsHeight) / 2 + 100;

        startButton = {
            static_cast<float>((screenWidth - BUTTON_WIDTH) / 2),
            static_cast<float>(startY),
            static_cast<float>(BUTTON_WIDTH),
            static_cast<float>(BUTTON_HEIGHT)
        };

        settingsButton = {
            startButton.x,
            static_cast<float>(startY + BUTTON_HEIGHT + BUTTON_SPACING),
            static_cast<float>(BUTTON_WIDTH),
            static_cast<float>(BUTTON_HEIGHT)
        };

        exitButton = {
            startButton.x,
            static_cast<float>(startY + (BUTTON_HEIGHT + BUTTON_SPACING) * 2),
            static_cast<float>(BUTTON_WIDTH),
            static_cast<float>(BUTTON_HEIGHT)
        };
    }

} // namespace

namespace Interface::UI::MainMenuRenderer {

    void Initialize() {

        themeFont = LoadFontEx(
            ASSETS_DIR "fonts/theme_font.ttf",
            96,
            nullptr,
            0
        );

        SetTextureFilter(
            themeFont.texture,
            TEXTURE_FILTER_BILINEAR
        );

        buttonFont = GetFontDefault();

        backgroundTexture = LoadTexture(
            ASSETS_DIR "textures/mainmenu_bg.png"
        );

        UpdateLayout();
    }

    void Update(Data::CoreData::AppState& currentState) {

        const Vector2 mousePosition = GetMousePosition();

        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return;
        }

        if (CheckCollisionPointRec(mousePosition, startButton)) {
            currentState = Data::CoreData::AppState::ACTIVE_SIMULATION;
            return;
        }

        if (CheckCollisionPointRec(mousePosition, settingsButton)) {
            // TODO: Implement the settings menu.
            return;
        }

        if (CheckCollisionPointRec(mousePosition, exitButton)) {
            currentState = Data::CoreData::AppState::EXIT_REQUESTED;
        }
    }

    void Draw() {
        UpdateLayout();

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();
        const Vector2 mousePosition = GetMousePosition();

        ClearBackground(BLACK);

        if (backgroundTexture.id != 0) {

            DrawTexturePro(
                backgroundTexture,
                {
                    0.0f,
                    0.0f,
                    static_cast<float>(backgroundTexture.width),
                    static_cast<float>(backgroundTexture.height)
                },
                {
                    0.0f,
                    0.0f,
                    static_cast<float>(screenWidth),
                    static_cast<float>(screenHeight)
                },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }

        DrawRectangle(
            (screenWidth - PANEL_WIDTH) / 2,
            0,
            PANEL_WIDTH,
            screenHeight,
            PANEL_COLOR
        );

        constexpr const char* TITLE = "PRAESIDIUM";

        const float titleWidth = MeasureTextEx(
            themeFont,
            TITLE,
            TITLE_FONT_SIZE,
            TITLE_SPACING
        ).x;

        DrawTextEx(
            themeFont,
            TITLE,
            {
                (screenWidth - titleWidth) / 2.0f,
                TITLE_POS_Y
            },
            TITLE_FONT_SIZE,
            TITLE_SPACING,
            TEXT_HOVER
        );

        DrawButton(
            startButton,
            "OYUNA BASLA",
            mousePosition,
            buttonFont
        );

        DrawButton(
            settingsButton,
            "AYARLAR",
            mousePosition,
            buttonFont
        );

        DrawButton(
            exitButton,
            "CIKIS",
            mousePosition,
            buttonFont
        );
    }

    void Close() {

        if (themeFont.texture.id != 0) {
            UnloadFont(themeFont);
        }

        if (backgroundTexture.id != 0) {
            UnloadTexture(backgroundTexture);
        }
    }

} // namespace Interface::UI::MainMenuRenderer