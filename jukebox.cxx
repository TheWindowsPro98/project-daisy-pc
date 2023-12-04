#include "inc/includes.hxx"

const types::u8 sndAmnt = 10;
const float musX = 0;
const float musY = 0;
const float musTitleXDelta = 5;
const float musAlbumXDelta = 5.5;
const float musArtistXDelta = 8;
const float musLoopXDelta = 11.65;
const float musTypeXDelta = 9.25;
const float musDurationXDelta = 11;
const float musTotalXDelta = 9.35;
const structs::SndMData musHdr[] = 
{
    {&testTrack,"Test Track","Game","TheWindowsPro98",true,false},
    {&lvlClearTrack,"Act Complete","Sonic the Hedgehog 3","Jun Senoue",false,false},
    {&titleTrack,"Character Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&lfTrack,"Angel Island Zone (Act 2)","Sonic the Hedgehog 3","Unknown Artist",true,false},
    {&lsTrack,"Zone Select","Sonic Advance","Tatsuyuki Maeda, Yutaka Minobe, Teruhiko Nakagawa",true,false},
    {&hoverSFX,"Menu Hover","Game","TheWindowsPro98",false,true},
    {&confSFX,"Menu Select","Game","TheWindowsPro98",false,true},
    {&backSFX,"Menu Back","Game","TheWindowsPro98",false,true},
    {&crashSFX,"Macintosh IIcx Crash Chime","Chimes of Death","???",false,true},
    {&jumpSFX,"Jump","Game","TheWindowsPro98",false,true},
};
sf::SoundBuffer* sb;
sf::Sound* snd;
const std::string blankString = "";
const std::string boolStrings[] = {"No","Yes"};
const std::string musType[] = {"Music","SFX"};
const std::string timeMetric = " seconds";

static void jukeboxBack()
{
    fadeRect.setFillColor(sf::Color::White);
    float volume = music.getVolume();
    while (window.isOpen())
    {
        if (volume == 0)
        {
            music.stop();
            music.setVolume(100);
            break;
        }
        volume = music.getVolume();
        fadeMusic(true,volFadeSpeed,volMin);
        screenFade(volFadeSpeed,false,fadeLight);
        window.display();
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                updSRAM();
                window.close();
                return;
            }
        }
    }
    delete snd;
    delete sb;
    prefsScreen();
}

static void cursorMove(bool direction)
{
    if (!direction)
    {
        sndHvr.play();
        if (menuIndex == 0)
        {
            menuIndex = sndAmnt-1;
        }
        else
        {
            menuIndex--;
        }
    }
    else
    {
        sndHvr.play();
        if (menuIndex == sndAmnt-1)
        {
            menuIndex = 0;
        }
        else
        {
            menuIndex++;
        }
    }
}

static void initMusic()
{
    if (!musHdr[menuIndex].isSFX)
    {
        music.openFromFile(*musHdr[menuIndex].songPath);
        music.setLoop(musHdr[menuIndex].loop);
        music.play();
    }
    else
    {
        sb->loadFromFile(*musHdr[menuIndex].songPath);
        snd->setBuffer(*sb);
        snd->setLoop(musHdr[menuIndex].loop);
        snd->play();
    }
}

static void toggleMusicPause()
{
    if (music.getStatus() == sf::Music::Playing)
    {
        music.pause();
    }
    else
    {
        music.play();
    }
}

void jukebox()
{
    sb = new sf::SoundBuffer;
    snd = new sf::Sound;
    music.openFromFile(*musHdr[menuIndex].songPath);
    fadeRect.setFillColor(sf::Color::Black);
    sf::Text songLabel(templateText);
    songLabel.setPosition(pixelToTile(musX),pixelToTile(musY));
    sf::Text songAlbumLabel(templateText);
    songAlbumLabel.setPosition(pixelToTile(musX),pixelToTile(musY+1));
    sf::Text songArtistLabel(templateText);
    songArtistLabel.setPosition(pixelToTile(musX),pixelToTile(musY+2));
    sf::Text songLoopLabel(templateText);
    songLoopLabel.setPosition(pixelToTile(musX),pixelToTile(musY+4));
    sf::Text songTypeLabel(templateText);
    songTypeLabel.setPosition(pixelToTile(musX),pixelToTile(musY+5));
    sf::Text songTimeElapsed(templateText);
    songTimeElapsed.setPosition(pixelToTile(musX),pixelToTile(musY+7));
    sf::Text songTimeTotal(templateText);
    songTimeTotal.setPosition(pixelToTile(musX),pixelToTile(musY+8));
    std::stringstream songDurationStr;
    std::stringstream songElapsedStr;
    while(window.isOpen())
    {
        songDurationStr.str(blankString);
        songElapsedStr.str(blankString);
        songElapsedStr << std::fixed << std::showpoint << std::setprecision(3) << music.getPlayingOffset().asSeconds();
        songDurationStr << std::fixed << std::showpoint << std::setprecision(3) << music.getDuration().asSeconds();
        songLabel.setString("Title: " + musHdr[menuIndex].songTitle);
        songAlbumLabel.setString("Album: " + musHdr[menuIndex].songAlbum);
        songArtistLabel.setString("Artist(s): " + musHdr[menuIndex].songArtist);
        songLoopLabel.setString("Will Audio Loop: " + boolStrings[musHdr[menuIndex].loop]);
        songTypeLabel.setString("Audio Type: " + musType[musHdr[menuIndex].isSFX]);
        songTimeElapsed.setString("Elapsed Time: " + songElapsedStr.str() + timeMetric);
        songTimeTotal.setString("Total Time: " + songDurationStr.str() + timeMetric);
        window.clear();
        window.draw(songLabel);
        window.draw(songAlbumLabel);
        window.draw(songArtistLabel);
        window.draw(songLoopLabel);
        window.draw(songTypeLabel);
        window.draw(songTimeElapsed);
        window.draw(songTimeTotal);
        screenFade(volFadeSpeed,true,fadeDark);
        window.display();
        while(window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
            {
                updSRAM();
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (e.key.scancode == sf::Keyboard::Scan::Left)
                {
                    cursorMove(false);
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Right)
                {
                    cursorMove(true);
                }
                if (e.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    initMusic();
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Space)
                {
                    toggleMusicPause();
                }
                else if (e.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    sndBack.play();
                    jukeboxBack();
                }
                break;
            }
            case sf::Event::JoystickButtonPressed:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                switch (e.joystickButton.button)
                {
                    case buttonCross:
                    {
                        initMusic();
                        break;
                    }
                    case buttonSquare:
                    {
                        toggleMusicPause();
                        break;
                    }
                    case buttonCircle:
                    {
                        sndBack.play();
                        jukeboxBack();
                        break;
                    }
                    break;
                }
                break;
            }
            case sf::Event::JoystickMoved:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                switch (e.joystickMove.axis)
                {
                    case axisDPADX:
                    {
                        if (e.joystickMove.position == -100)
                        {
                            cursorMove(false);
                        }
                        else if (e.joystickMove.position == 100)
                        {
                            cursorMove(true);
                        }
                        break;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
}