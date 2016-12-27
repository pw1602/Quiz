#pragma once

#include "Quiz.h"
#include "Stworz.h"
#include "Funkcje.h"

class Menu
{
public:
	Menu();
	~Menu();

	void Menu_Glowne(); //Sprawdzanie aktualnego typu wyliczeniowego

private:
	const int ile_obrazkow = 7;
	RenderWindow okno;
	Font czcionka;
	Texture teksturki[7]; //Tekstury do menu "POMOC"
	Music muzyka;
	Texture tlo_tekstura_menu, tlo_tekstura_pomoc, glosnik_tekstura, glosnik_wyciszony_tekstura, kartka_papieru_tekstura;
	Sprite tlo_menu, tlo_pomoc, glosnik;

	struct POMOC_TEKST {
		Sprite sprajt;
		Text tekst;
	};
	vector <POMOC_TEKST> v_pomoc;
	POMOC_TEKST pomoc_tekst;

	enum WYBOR_MENU { MENU, QUIZ, TWORZENIE, POMOC, NIC }; //Aktualne okno w quizie
	WYBOR_MENU wybor_menu;

	void menu(); //Ca³e menu - wyœwietlanie etc.
	void rozwiaz();
	void stworz();
	void Jak_Grac();
	void Zaladuj_Obrazki();
};