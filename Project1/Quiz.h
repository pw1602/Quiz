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

	int liczba_pytan; //Ilo�� pyta� w quzie
	float czas, czas_koniec; //Do pokazania czasu
	float limit_czasu; //Limit na sko�czenie quizu
	bool koniec_quizu; //Je�li sko�czono quiz

	struct PYTANIA
	{
		Text pytanie;
		Text odpowiedz[4];
		RectangleShape podkreslenie[4];
		bool zaznaczenie[4]; //Czy dana odpowied� jest zaznaczona
		int poprawnosc[4]; //Poprawnosc kazdego pytania 1 - true
	}; //struct

	vector <PYTANIA> v_pytania; //Przechowuje jedno pytanie wraz z odpowiedziami

	bool Wczytaj_Dane();
	bool Ilosc_Pytan(); //Zapytaie u�ytkownika o ilo�� pyta� do wy�wietlenia
	void Wynik();
	bool Poprawnosc_Odpowiedzi();
};