#pragma once

#include "Funkcje.h"

class Quiz
{
public:
	Quiz(RenderWindow &win, Font &font, Sprite &background, Music &music, Sprite &speaker, Texture &speaker_text, Texture &speaker_text_mute);
	~Quiz();

	void Rozpocznij_Quiz();

private:
	RenderWindow *okno;
	Font *czcionka;
	Sprite *tlo, *glosnik;
	Texture *glosnik_tekstura, *glosnik_wyciszony_tekstura;
	Music *muzyka;
	fstream plik;
	Clock zegar; //Mierzy czas

	int liczba_pytan; //Iloœæ pytañ w quzie
	float czas, czas_koniec; //Do pokazania czasu
	float limit_czasu; //Limit na skoñczenie quizu
	bool koniec_quizu; //Jeœli skoñczono quiz

	struct PYTANIA
	{
		Text pytanie;
		Text odpowiedz[4];
		RectangleShape podkreslenie[4];
		bool zaznaczenie[4]; //Czy dana odpowiedŸ jest zaznaczona
		int poprawnosc[4]; //Poprawnosc kazdego pytania 1 - true
	}; //struct

	vector <PYTANIA> v_pytania; //Przechowuje jedno pytanie wraz z odpowiedziami

	bool Wczytaj_Dane();
	bool Ilosc_Pytan(); //Zapytaie u¿ytkownika o iloœæ pytañ do wyœwietlenia
	void Wynik();
	bool Poprawnosc_Odpowiedzi();
};