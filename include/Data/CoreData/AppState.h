#pragma once

enum class AppState {
    SPLASH_SCREEN,      // Başlangıç yüklemeleri
    MAIN_MENU,          // Ana menü
    SAVE_LOAD_MENU,     // Kayıtlı oyunları listeleme, yükleme ve yeni kayıt alma
    SETTINGS,           // Ayarlar
    ACTIVE_SIMULATION,  // Ana oyun döngüsü
    PAUSED,             // Oyun içi duraklatma menüsü
    GAME_OVER,          // Ana üs yok edildiğinde çıkacak ekran
    EXIT_REQUESTED      // Sistemin tamamen kapatılması
};