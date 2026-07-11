#include "../../../../include/Interface/UI/MainMenuRenderer.h"

#include <algorithm>
#include <cmath>

#include "raylib.h"

namespace {

    constexpr int BUTTON_WIDTH = 400;
    constexpr int BUTTON_HEIGHT = 60;
    constexpr int BUTTON_SPACING = 20;

    constexpr int PANEL_WIDTH = 500;
    constexpr int PANEL_MARGIN_RIGHT = 100;

    constexpr float TITLE_FONT_SIZE = 72.0f;
    constexpr float TITLE_SPACING = 10.0f;
    constexpr float BUTTON_FONT_SIZE = 28.0f;
    constexpr float BUTTON_TEXT_SPACING = 2.0f;
    constexpr float TITLE_POS_Y = 120.0f;

    constexpr Color PANEL_COLOR = { 10, 15, 30, 210 };
    constexpr Color TEXT_NORMAL = { 100, 120, 140, 255 };
    constexpr Color TEXT_HOVER = { 255, 255, 255, 255 };

    constexpr float HOVER_ANIMATION_SPEED = 10.0f;
    constexpr float HOVER_MAX_SHIFT = 8.0f;
    constexpr int HOVER_LINE_Y_OFFSET = 10;
    constexpr int HOVER_LINE_THICKNESS = 2;

    constexpr float PARALLAX_MAX_OFFSET_X = 20.0f;
    constexpr float PARALLAX_MAX_OFFSET_Y = 10.0f;
    constexpr float PARALLAX_SMOOTH_SPEED = 5.0f;

    constexpr float OVERLAY_ALPHA = 0.15f;
    constexpr float CURSOR_OFFSET_X = 30.0f;
    constexpr int SCANLINE_SPACING = 4;
    constexpr unsigned char SCANLINE_ALPHA = 40;

    constexpr int BUTTON_COUNT = 6;
    const char* BUTTON_LABELS[BUTTON_COUNT] = {
        "DEVAM ET",
        "YENI OYUN",
        "YUKLE",
        "AYARLAR",
        "KREDILER",
        "CIKIS"
    };

    struct ButtonState {
        Vector2 textSize;
        float hoverAmount;
    };

    ButtonState buttons[BUTTON_COUNT] = {};
    int selectedButtonIndex = 0;

    Font themeFont{};
    Font buttonFont{};
    Texture2D backgroundTexture{};

    float titleWidth = 0.0f;

    float currentParallaxX = 0.0f;
    float currentParallaxY = 0.0f;

    MouseCursor activeCursor = MOUSE_CURSOR_DEFAULT;

    constexpr float Lerp(float start, float end, float amount) {
        return start + amount * (end - start);
    }

    Color LerpColor(Color a, Color b, float t) {
        return {
            static_cast<unsigned char>(a.r + (b.r - a.r) * t),
            static_cast<unsigned char>(a.g + (b.g - a.g) * t),
            static_cast<unsigned char>(a.b + (b.b - a.b) * t),
            static_cast<unsigned char>(a.a + (b.a - a.a) * t)
        };
    }

    void SetCursorOptimized(MouseCursor cursor) {
        if (activeCursor != cursor) {
            SetMouseCursor(cursor);
            activeCursor = cursor;
        }
    }

    void CalculateTextSizes() {
        titleWidth = MeasureTextEx(
            themeFont,
            "PRAESIDIUM",
            TITLE_FONT_SIZE,
            TITLE_SPACING
        ).x;

        for (int i = 0; i < BUTTON_COUNT; ++i) {
            buttons[i].textSize = MeasureTextEx(
                buttonFont,
                BUTTON_LABELS[i],
                BUTTON_FONT_SIZE,
                BUTTON_TEXT_SPACING
            );
        }
    }

    Rectangle GetButtonBounds(int index, int screenWidth, int screenHeight) {
        const int totalButtonsHeight =
            (BUTTON_COUNT * BUTTON_HEIGHT) + ((BUTTON_COUNT - 1) * BUTTON_SPACING);

        const int startY =
            (screenHeight - totalButtonsHeight) / 2 + 100;

        const float panelX = static_cast<float>(std::max(0, screenWidth - PANEL_WIDTH - PANEL_MARGIN_RIGHT));
        const float startX = panelX + static_cast<float>((PANEL_WIDTH - BUTTON_WIDTH) / 2.0f);

        return {
            startX,
            static_cast<float>(startY + index * (BUTTON_HEIGHT + BUTTON_SPACING)),
            static_cast<float>(BUTTON_WIDTH),
            static_cast<float>(BUTTON_HEIGHT)
        };
    }

    void DrawBackground(int screenWidth, int screenHeight) {

        if (backgroundTexture.id == 0) {
            return;
        }

        const float targetWidth = static_cast<float>(screenWidth) + (PARALLAX_MAX_OFFSET_X * 2.0f);
        const float targetHeight = static_cast<float>(screenHeight) + (PARALLAX_MAX_OFFSET_Y * 2.0f);

        const float scale = std::max(
            targetWidth / static_cast<float>(backgroundTexture.width),
            targetHeight / static_cast<float>(backgroundTexture.height)
        );

        const float width = static_cast<float>(backgroundTexture.width) * scale;
        const float height = static_cast<float>(backgroundTexture.height) * scale;

        const Rectangle destination = {
            static_cast<float>(static_cast<int>(((static_cast<float>(screenWidth) - width) * 0.5f) + currentParallaxX)),
            static_cast<float>(static_cast<int>(((static_cast<float>(screenHeight) - height) * 0.5f) + currentParallaxY)),
            width,
            height
        };

        DrawTexturePro(
            backgroundTexture,
            { 0.0f, 0.0f, static_cast<float>(backgroundTexture.width), static_cast<float>(backgroundTexture.height) },
            destination,
            {0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }

    void DrawPanelScanlines(float x, float width, int height) {
        for (int y = 0; y < height; y += SCANLINE_SPACING) {
            DrawRectangle(
                static_cast<int>(x),
                y,
                static_cast<int>(width),
                1,
                { 0, 0, 0, SCANLINE_ALPHA }
            );
        }
    }

    void DrawButton(int index, const ButtonState& state, const Rectangle& bounds) {

        const float hoverOffset = state.hoverAmount * HOVER_MAX_SHIFT;
        const float currentX = bounds.x - hoverOffset;

        const Color currentColor = LerpColor(TEXT_NORMAL, TEXT_HOVER, state.hoverAmount);

        const Vector2 textPosition = {
            currentX + (bounds.width - state.textSize.x) / 2.0f,
            bounds.y + (bounds.height - state.textSize.y) / 2.0f
        };

        if (state.hoverAmount > 0.0f) {

            const float lineWidth = bounds.width * 0.5f * state.hoverAmount;
            const float lineX = currentX + (bounds.width - lineWidth) / 2.0f;

            DrawRectangle(
                static_cast<int>(std::round(lineX)),
                static_cast<int>(std::round(bounds.y + bounds.height - HOVER_LINE_Y_OFFSET)),
                static_cast<int>(std::round(lineWidth)),
                HOVER_LINE_THICKNESS,
                currentColor
            );

            const Color cursorColor = { currentColor.r, currentColor.g, currentColor.b, static_cast<unsigned char>(255 * state.hoverAmount) };

            DrawTextEx(
                buttonFont,
                ">",
                { static_cast<float>(std::round(textPosition.x - CURSOR_OFFSET_X)), static_cast<float>(std::round(textPosition.y)) },
                BUTTON_FONT_SIZE,
                0.0f,
                cursorColor
            );
        }

        DrawTextEx(
            buttonFont,
            BUTTON_LABELS[index],
            { static_cast<float>(std::round(textPosition.x)), static_cast<float>(std::round(textPosition.y)) },
            BUTTON_FONT_SIZE,
            BUTTON_TEXT_SPACING,
            currentColor
        );
    }

    void ActivateSelectedButton(Data::CoreData::AppState& currentState) {
        SetCursorOptimized(MOUSE_CURSOR_DEFAULT);

        if (selectedButtonIndex == 0 || selectedButtonIndex == 1) {
            currentState = Data::CoreData::AppState::ACTIVE_SIMULATION;
        } else if (selectedButtonIndex == 5) {
            currentState = Data::CoreData::AppState::EXIT_REQUESTED;
        } else {
            // TODO: Implement other menus.
        }
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

        CalculateTextSizes();
    }

    void Update(Data::CoreData::AppState& currentState) {

        if (!IsWindowFocused()) {
            SetCursorOptimized(MOUSE_CURSOR_DEFAULT);
            for (int i = 0; i < BUTTON_COUNT; ++i) {
                buttons[i].hoverAmount = 0.0f;
            }
            return;
        }

        const float dt = GetFrameTime();
        const Vector2 mousePosition = GetMousePosition();
        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        const float targetParallaxX = -((mousePosition.x - (screenWidth * 0.5f)) / (screenWidth * 0.5f)) * PARALLAX_MAX_OFFSET_X;
        const float targetParallaxY = -((mousePosition.y - (screenHeight * 0.5f)) / (screenHeight * 0.5f)) * PARALLAX_MAX_OFFSET_Y;

        currentParallaxX = Lerp(currentParallaxX, targetParallaxX, dt * PARALLAX_SMOOTH_SPEED);
        currentParallaxY = Lerp(currentParallaxY, targetParallaxY, dt * PARALLAX_SMOOTH_SPEED);

        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            selectedButtonIndex--;
            if (selectedButtonIndex < 0) selectedButtonIndex = BUTTON_COUNT - 1;
        }

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            selectedButtonIndex++;
            if (selectedButtonIndex >= BUTTON_COUNT) selectedButtonIndex = 0;
        }

        bool isAnyButtonHovered = false;

        for (int i = 0; i < BUTTON_COUNT; ++i) {

            auto& button = buttons[i];
            const Rectangle bounds = GetButtonBounds(i, screenWidth, screenHeight);
            const bool isHovered = CheckCollisionPointRec(mousePosition, bounds);

            if (isHovered) {
                selectedButtonIndex = i;
                isAnyButtonHovered = true;
            }

            if (i == selectedButtonIndex) {
                button.hoverAmount += dt * HOVER_ANIMATION_SPEED;
            } else {
                button.hoverAmount -= dt * HOVER_ANIMATION_SPEED;
            }

            button.hoverAmount = std::clamp(button.hoverAmount, 0.0f, 1.0f);
        }

        if (isAnyButtonHovered) {
            SetCursorOptimized(MOUSE_CURSOR_POINTING_HAND);
        } else {
            SetCursorOptimized(MOUSE_CURSOR_DEFAULT);
        }

        bool actionTriggered = false;

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            actionTriggered = true;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isAnyButtonHovered) {
            actionTriggered = true;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            SetCursorOptimized(MOUSE_CURSOR_DEFAULT);
            currentState = Data::CoreData::AppState::EXIT_REQUESTED;
            return;
        }

        if (actionTriggered) {
            ActivateSelectedButton(currentState);
        }
    }

    void Draw() {
        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        ClearBackground(BLACK);

        DrawBackground(screenWidth, screenHeight);

        DrawRectangle(
            0,
            0,
            screenWidth,
            screenHeight,
            Fade(BLACK, OVERLAY_ALPHA)
        );

        const float panelX = static_cast<float>(std::max(0, screenWidth - PANEL_WIDTH - PANEL_MARGIN_RIGHT));

        DrawRectangle(
            static_cast<int>(panelX),
            0,
            PANEL_WIDTH,
            screenHeight,
            PANEL_COLOR
        );

        DrawPanelScanlines(panelX, PANEL_WIDTH, screenHeight);

        DrawTextEx(
            themeFont,
            "PRAESIDIUM",
            {
                static_cast<float>(std::round(panelX + (PANEL_WIDTH - titleWidth) / 2.0f)),
                TITLE_POS_Y
            },
            TITLE_FONT_SIZE,
            TITLE_SPACING,
            TEXT_HOVER
        );

        for (int i = 0; i < BUTTON_COUNT; ++i) {
            const Rectangle bounds = GetButtonBounds(i, screenWidth, screenHeight);
            DrawButton(i, buttons[i], bounds);
        }
    }

    void Close() {

        if (themeFont.texture.id != 0) {
            UnloadFont(themeFont);
        }

        if (backgroundTexture.id != 0) {
            UnloadTexture(backgroundTexture);
        }

        SetCursorOptimized(MOUSE_CURSOR_DEFAULT);
    }

} // namespace Interface::UI::MainMenuRenderer