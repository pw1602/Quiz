#pragma once

#include "Funkcje.h"

class Stworz
{
public:
	Stworz(RenderWindow &win, Font &font, Sprite &background, Sprite &speaker, Texture &speaker_text, Texture &speaker_text_mute, Music &music);
	~Stworz();

	void Tworzenie();

private:
	RenderWindow *okno;
	Font *czcionka;
	Texture *glosnik_tekstura, *glosnik_wyciszony_tekstura;
	Sprite *tlo, *glosnik;
	fstream plik;
	Music *muzyka;
	int ilosc_pytan;

	struct PYTANIA
	{
		Text pytanie;
		Text odpowiedz[4];
		RectangleShape podkreslenie[4], podkreslenie_pytania;
		bool zaznaczenie[4], zaznaczenie_pytania; //Czy dana odpowiedü jest zaznaczona
		int poprawnosc[4]; //Poprawnosc kazdego pytania 1 - true
	}; //struct
	vector <PYTANIA> v_pytania;

	struct WYBOR {
		Text wybor_tekst[4];
		bool zaznaczenie[4];
	};
	vector <WYBOR> v_wybor;

	bool Wczytaj();
	void Edytuj();
	bool Zaznaczona_Odpowiedz();
	void Dodaj_Pytanie();
	void Usun_Pytanie();
	void Zapisz();
};