#pragma once

#include "includes.hxx"

extern sf::RenderWindow window;
extern sf::Font font;
extern sf::Music music;
extern sf::RectangleShape fadeRect;
extern const types::u8 fontSize;
extern sf::SoundBuffer sbHvr;
extern sf::Sound sndHvr;
extern sf::SoundBuffer sbCnf;
extern sf::Sound sndCnf;
extern sf::SoundBuffer sbBack;
extern sf::Sound sndBack;
extern sf::Text templateText;
extern const float volFadeSpeed;
extern const sf::Color playerColors[];
enum errorCodes
{
    genericErr,
    missingFuncErr,
    badLevelErr,
    missingAssetsErr,
};

void screenFade(float speed, bool direction);
void drawMenu(const structs::Option* option, types::u8 length);
void fadeMusic(bool direction, float speed);
float pixelToTile(float pos);
void printerr(types::u8 error);