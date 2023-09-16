#include "inc/includes.hxx"

const u8 titleX = 6;
const u8 titleY = 22;
const u8 titleOptAmnt = 4;

const structs::Option titleMenu[] = 
{
    {titleX,titleY,"Start Game"},
    {titleX+10,titleY,"Continue Game"},
    {titleX+22,titleY,"Statistics"},
    {titleX+31,titleY,"Preferences"},
};

static void selectMenuTitle()
{
    float volume;
    while (window.isOpen())
    {
        if (volume == 0)
        {
            music.stop();
            music.setVolume(100);
            break;
        }
        sf::Event e;
        volume = music.getVolume();
        fadeMusic(true,volFadeSpeed);
        screenFade(volFadeSpeed,false);
        window.draw(fadeRect);
        window.display();
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
        }
    }
    switch (*menuIndex)
    {
        case 3:
        {
            prefsScreen();
            break;
        }
        default:
        {
            fprintf(stderr,"Feature will be added soon!");
            exit(1);
            break;
        }
    }
}

void title()
{
    std::string vmajStr = std::to_string(versionMajor);
    std::string vminStr = std::to_string(versionMinor);
    std::string vrevStr = std::to_string(versionRevision);
    std::string vcStr = vmajStr + "." + vminStr + "." + vrevStr;
    std::string versionCombined = "Version " + releaseStageStringsS[releaseStage] + vcStr;
    music.openFromFile(titleTrack);
    music.setLoop(true);
    music.play();
    sf::Text copyInfo,versionText;
    copyInfo.setString(L"©TheWindowsPro98 2023");
    copyInfo.setCharacterSize(fontSize);
    copyInfo.setFont(font);
    copyInfo.setPosition(0,696);
    versionText.setString(versionCombined);
    versionText.setFont(font);
    versionText.setCharacterSize(fontSize);
    versionText.setPosition(160,144);
    sf::Texture titleTexture;
    sf::Image titlePixels;
    titlePixels.loadFromFile(titleImg);
    titleTexture.loadFromFile(titleImg,sf::IntRect(0,0,960,144));
    titleTexture.update(titlePixels);
    titleTexture.setSmooth(true);
    sf::Sprite titleSprite;
    titleSprite.setTexture(titleTexture);
    titleSprite.setPosition(160,0);
    float volume = 100;
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::Black);
        window.draw(copyInfo);
        window.draw(versionText);
        window.draw(titleSprite);
        drawMenu(titleMenu,4);
        window.display();
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (!window.hasFocus())
                {
                    break;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Left)
                {
                    sndHvr.play();
                    if (*menuIndex == 0)
                    {
                        *menuIndex = titleOptAmnt-1;
                    }
                    else
                    {
                        *menuIndex -= 1;
                    }
                }
                else if (event.key.scancode == sf::Keyboard::Scan::Right)
                {
                    sndHvr.play();
                    if (*menuIndex >= titleOptAmnt-1)
                    {
                        *menuIndex = 0;
                    }
                    else
                    {
                        *menuIndex += 1;
                    }
                }
                if (event.key.scancode == sf::Keyboard::Scan::Enter)
                {
                    sndCnf.play();
                    selectMenuTitle();
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