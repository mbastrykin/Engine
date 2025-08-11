#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

struct Music {
    sf::Music music;
    Music(const std::string& File) {
        music.openFromFile("SoundMusic\\Music\\" + File);
        //music.play();
    }
    void play() {
        music.setVolume(100);
        music.play();
    }
};

struct Sounds {
public:
   /* sf::SoundBuffer buffer;
    sf::Sound sound;*/
    Sounds(std::string File) {
        
        
        buffer.loadFromFile("SoundMusic\\Sounds\\" + File);
        sound.setBuffer(buffer);
       
    }
    ~Sounds() {
        sound.stop();
    }
	void play() {
		sound.setVolume(100);
		sound.play();
	}
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
};