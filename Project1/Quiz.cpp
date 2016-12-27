#include "Quiz.h"

Quiz::Quiz(RenderWindow &win, Font &font, Sprite &background, Music &music, Sprite &speaker, Texture &speaker_text, Texture &speaker_text_mute)
{
	okno = &win;
	czcionka = &font;
	tlo = &background;
	muzyka = &music;
	glosnik = &speaker;
	glosnik_tekstura = &speaker_text;
	glosnik_wyciszony_tekstura = &speaker_text_mute;
	liczba_pytan = 1; //Ilosc poczatkowych pytan do wyswietlenia
	limit_czasu = liczba_pytan * 30; //Jedno pytanie to jedna minuta na odpowiedŸ
	koniec_quizu = false;
	zegar.restart(); //Zresetowanie czasu zegara

	glosnik->setPosition(okno->getSize().x - glosnik->getGlobalBounds().width - 10, 10);
}

Quiz::~Quiz()
{
	//destruktor
}

void Quiz::Rozpocznij_Quiz()
{
	int kolejne_pytanie = 0; //Numer pytania aktualnie wyœwietlanego
	bool quiz = true; //False - wyjœcie z Quizu
	Text pozostaly_czas, nastepne_pytanie, poprzednie_pytanie, zakoncz_quiz, wyjdz_z_quizu, wybor[4], numer_pytania; //wybor[4] - tekst zawieraj¹cy A, B, C, D
	int minuty = 0, rozmiar_czcionki_pytania = 0;
	string wybor_str[] = { "A. ", "B. ", "C. ", "D. " };

	//--------------------------------------------------Ustawienia tekstu i jego tla w quzie--------------------------------------------------
	//Czas
	pozostaly_czas.setFont(*czcionka);
	pozostaly_czas.setCharacterSize(30);

	//Nastêpne pytanie
	Ustawienia_tekstu(nastepne_pytanie, *czcionka, 30, 1, Color::Blue, L"Nastêpne");
	nastepne_pytanie.setPosition(750, 600);

	//Poprzednie pytanie
	Ustawienia_tekstu(poprzednie_pytanie, *czcionka, 30, 1, Color::Blue, L"Poprzednie");
	poprzednie_pytanie.setPosition(100, 600);

	//Zakoñczenie quizu
	Ustawienia_tekstu(zakoncz_quiz, *czcionka, 25, 1, Color::Blue, L"SprawdŸ Wynik");
	zakoncz_quiz.setPosition(okno->getSize().x / 2 - zakoncz_quiz.getGlobalBounds().width / 2, okno->getSize().y - zakoncz_quiz.getGlobalBounds().height - 20);

	//Wyjœcie z quizu
	Ustawienia_tekstu(wyjdz_z_quizu, *czcionka, 40, 1, Color::Blue, L"<-");
	wyjdz_z_quizu.setPosition(20, okno->getSize().y - wyjdz_z_quizu.getGlobalBounds().height - 20);

	//A, B, C, D
	for (int i = 0; i < 4; i++)
	{
		Ustawienia_tekstu(wybor[i], *czcionka, 30, 0, Color::Blue, wybor_str[i]);
		wybor[i].setPosition(20, 250 + i * 60);
	}//for
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	if (Ilosc_Pytan())
		Wczytaj_Dane();
	else quiz = false;

	//--------------------------------------------------Ustawienie w³asnoœci elementów w strukturze pytañ--------------------------------------------------
	for (int i = 0; i < v_pytania.size(); i++)
	{
		//Pytanie
		v_pytania[i].pytanie.setFont(*czcionka);
		v_pytania[i].pytanie.setFillColor(Color::Blue);
		v_pytania[i].pytanie.setStyle(Text::Bold);

		rozmiar_czcionki_pytania = 45;
		v_pytania[i].pytanie.setCharacterSize(rozmiar_czcionki_pytania);
		while (v_pytania[i].pytanie.getGlobalBounds().width >= okno->getSize().x - 35)
		{
			rozmiar_czcionki_pytania--;
			v_pytania[i].pytanie.setCharacterSize(rozmiar_czcionki_pytania);
		}//while
		v_pytania[i].pytanie.setPosition(okno->getSize().x / 2 - v_pytania[i].pytanie.getGlobalBounds().width / 2, 163);

		for (int j = 0; j < 4; j++)
		{
			//Odpowiedzi
			v_pytania[i].odpowiedz[j].setFont(*czcionka);
			v_pytania[i].odpowiedz[j].setCharacterSize(30);
			v_pytania[i].odpowiedz[j].setFillColor(Color::Blue);
			v_pytania[i].odpowiedz[j].setPosition(60, 250 + j * 60);

			//Podkreœlenie
			v_pytania[i].podkreslenie[j].setSize(Vector2f(0, 0));
			v_pytania[i].podkreslenie[j].setFillColor(Color::Red);

			//Zaznaczenie
			v_pytania[i].zaznaczenie[j] = false;
		}//for
	}//for
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	zegar.restart(); //Zresetowanie czasu zegara
	while (okno->isOpen() && quiz)
	{
		czas = zegar.getElapsedTime().asSeconds(); //Czas do float
		czas_koniec = zegar.getElapsedTime().asSeconds(); //Czas do float
		Event wyd; //Przechwytuje wciœniête klawicze etc.
		Vector2f mysz(Mouse::getPosition(*okno)); //Zapisuje pozycje myszy w oknie

		//Ustawienia tekstu wyœwietlaj¹cego czas i czasu
		limit_czasu = liczba_pytan * 30 / 60;

		if (czas >= 60)
		{
			minuty++;
			zegar.restart();
		}//if

		if (czas_koniec >= (liczba_pytan * 30))
			koniec_quizu = true;

		if (limit_czasu < 60 && limit_czasu > 0 && liczba_pytan % 2 == 0)
			pozostaly_czas.setString("Czas to " + to_string(static_cast<int>(limit_czasu)) + " min!: " + to_string(minuty) + " min " + to_string(static_cast<int>(czas)) + " sek");
		else if (limit_czasu == 0)
			pozostaly_czas.setString("Czas to 30 sek!: " + to_string(static_cast<int>(czas)) + " sek");
		else pozostaly_czas.setString("Czas to " + to_string(static_cast<int>(limit_czasu)) + " min 30 sek!: " + to_string(minuty) + " min " + to_string(static_cast<int>(czas)) + " sek");
		pozostaly_czas.setPosition(okno->getSize().x / 2 - pozostaly_czas.getGlobalBounds().width / 2, 30);
		pozostaly_czas.setFillColor(Color::Blue);

		//--------------------------------------------------Wydarzenia w oknie--------------------------------------------------
		while (okno->pollEvent(wyd))
		{
			if (wyd.type == Event::Closed || wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Escape || wyjdz_z_quizu.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) //Zamniêcie okna przez "X" lub wciœniêcie ESC lub wyjœcie w quizie
				quiz = false;

			if (nastepne_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left && kolejne_pytanie < v_pytania.size() - 1) //Zmiana pytania w prawo
				kolejne_pytanie++;

			if (wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Right && kolejne_pytanie < v_pytania.size() - 1) //Zmiana pytania w prawo
				kolejne_pytanie++;

			if (poprzednie_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left && kolejne_pytanie > 0) //Zmiana pytania w lewo
				kolejne_pytanie--;

			if (wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Left && kolejne_pytanie > 0) //Zmiana pytania w lewo
				kolejne_pytanie--;

			if (zakoncz_quiz.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) //Zakoñczenie quizu
				koniec_quizu = true;

			if (glosnik->getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) //Wy³¹czenie muzyki
			{
				muzyka->pause();
				glosnik->setTexture(*glosnik_wyciszony_tekstura);
			}//if

			if (glosnik->getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right) //W³¹czenie muzyki
			{
				muzyka->play();
				glosnik->setTexture(*glosnik_tekstura);
			}//if

			//--------------------------------------------------Najechanie mysz¹ na odpowiedzi--------------------------------------------------
			for (int i = 0; i < 4; i++)
			{
				if ((v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) && !v_pytania[kolejne_pytanie].zaznaczenie[i]) //Zaznaczenie odpowiedzi
					v_pytania[kolejne_pytanie].zaznaczenie[i] = true;

				if ((v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right) && v_pytania[kolejne_pytanie].zaznaczenie[i]) //Odznaczenie odpowiedzi
					v_pytania[kolejne_pytanie].zaznaczenie[i] = false;
			}//for
			//------------------------------------------------------------------------------------------------------------------------------------------------------
		}//while
		//------------------------------------------------------------------------------------------------------------------------------------------------------

		//--------------------------------------------------Ustawienia po najechaniu mysz¹ na dany element--------------------------------------------------
		for (int i = 0; i < 4; i++)
		{
			if (v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().contains(mysz) || v_pytania[kolejne_pytanie].zaznaczenie[i])
			{
				v_pytania[kolejne_pytanie].podkreslenie[i].setSize(Vector2f(v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().width, 2));
				v_pytania[kolejne_pytanie].podkreslenie[i].setPosition(v_pytania[kolejne_pytanie].odpowiedz[i].getPosition().x, v_pytania[kolejne_pytanie].odpowiedz[i].getPosition().y + 40);
			}//if
			else v_pytania[kolejne_pytanie].podkreslenie[i].setSize(Vector2f(0, 0));
		}//for

		//--------------------------------------------------Ustawienia tekstu w quizie--------------------------------------------------
		if (nastepne_pytanie.getGlobalBounds().contains(mysz))
		{
			nastepne_pytanie.setOutlineThickness(1);
			nastepne_pytanie.setOutlineColor(Color::Cyan);
		}//if
		else if (poprzednie_pytanie.getGlobalBounds().contains(mysz))
		{
			poprzednie_pytanie.setOutlineThickness(1);
			poprzednie_pytanie.setOutlineColor(Color::Cyan);
		}//else if
		else if (zakoncz_quiz.getGlobalBounds().contains(mysz))
		{
			zakoncz_quiz.setOutlineThickness(1);
			zakoncz_quiz.setOutlineColor(Color::Cyan);
		}//else if
		else if (wyjdz_z_quizu.getGlobalBounds().contains(mysz))
		{
			wyjdz_z_quizu.setOutlineThickness(1);
			wyjdz_z_quizu.setOutlineColor(Color::Cyan);
		}//else if
		else
		{
			nastepne_pytanie.setOutlineThickness(0);
			poprzednie_pytanie.setOutlineThickness(0);
			zakoncz_quiz.setOutlineThickness(0);
			wyjdz_z_quizu.setOutlineThickness(0);
		}//else

		//Numer pytania
		Ustawienia_tekstu(numer_pytania, *czcionka, 35, 0, Color::Blue, "Pytanie nr " + to_string(kolejne_pytanie + 1));
		numer_pytania.setPosition(okno->getSize().x / 2 - numer_pytania.getGlobalBounds().width / 2, 100);
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------------------------

		if (koniec_quizu)
		{
			Wynik();
			quiz = false;
			koniec_quizu = false;
		}//if

		//--------------------------------------------------Wyœwietlenie wszystkiego na ekranie--------------------------------------------------
		okno->clear();
		okno->draw(*tlo);
		okno->draw(v_pytania[kolejne_pytanie].pytanie);
		for (int j = 0; j < 4; j++)
		{
			okno->draw(v_pytania[kolejne_pytanie].odpowiedz[j]);
			okno->draw(v_pytania[kolejne_pytanie].podkreslenie[j]);
			okno->draw(wybor[j]);
		}//for
		okno->draw(numer_pytania);
		okno->draw(pozostaly_czas);
		okno->draw(nastepne_pytanie);
		okno->draw(poprzednie_pytanie);
		okno->draw(zakoncz_quiz);
		okno->draw(wyjdz_z_quizu);
		okno->draw(*glosnik);
		okno->display();
		//------------------------------------------------------------------------------------------------------------------------------------------------------
	}//while
}

bool Quiz::Wczytaj_Dane()
{
	int sredniki, pytanie_zaw, ilosc_wczytanych_pytan = 0; //sredniki - liczba srednikow, pytanie_zaw - +1 do liczby œredników, by nie wczytywa³ tego samego srednika
	PYTANIA pytania;
	string linia_tekstu, tekst, zawartosc_pytania[6]; //linia_tekstu - cala linia pobranego tekstu, tekst - kazdorazowy tekst miedzy srednikami w linii

	plik.open("data\\questions\\pytania.csv", ios::in);

	if (plik.good())
	{
		while (getline(plik, linia_tekstu) && ilosc_wczytanych_pytan < liczba_pytan)
		{
			sredniki = 0;
			pytanie_zaw = 1;
			tekst.clear();

			for (int i = 0; i < linia_tekstu.length(); i++)
			{
				if (linia_tekstu[i] == ';')
					sredniki++;

				if (sredniki == pytanie_zaw)
				{
					zawartosc_pytania[pytanie_zaw - 1] = tekst;
					pytanie_zaw++;
					tekst.clear();
				}//if

				if (linia_tekstu[i] != ';')
					tekst += linia_tekstu[i];
			}//for

			pytania.pytanie.setString(zawartosc_pytania[0]);
			for (int i = 0; i < 4; i++)
			{
				pytania.odpowiedz[i].setString(zawartosc_pytania[i + 1]);
				pytania.poprawnosc[i] = zawartosc_pytania[5][i] - '0'; //Odjêcie 0, aby nie by³y wyœwietlane wartoœci w ASCII
			}//for

			v_pytania.push_back(pytania);
			ilosc_wczytanych_pytan++;
		}//while
		plik.close();

		return true;
	}//if
	else return false;

	return false;
}

bool Quiz::Ilosc_Pytan()
{
	Text tekst, tekst2; //tekst - Górny z info o wybraniu pytania, tekst2 - Dolny z podaniem swojej iloœci
	bool wybor = true; //False - Zakoñczenie pobierania iloœci pytañ
	const int ilosc_prostokatow = 3; //Liczba "paneli" z wyborem pytania

	struct WYBOR_PYTAN {
		RectangleShape prostokat;
		Text prostokat_tekst; //Pokazanie liczby z iloœci¹ pytañ
		int ile_pyt = 5; //Iloœæ pytañ w danym "panelu"
	};
	WYBOR_PYTAN wybor_pytan[ilosc_prostokatow];

	//--------------------------------------------------Ustawienia elementów w strukturze wyboru pytañ--------------------------------------------------
	for (int i = 0; i < ilosc_prostokatow; i++)
	{
		//Prostokat
		wybor_pytan[i].prostokat.setSize(Vector2f(150, 150));
		wybor_pytan[i].prostokat.setPosition(230 + i * 200, 250);

		//Liczba pytañ
		if (i > 0)
			wybor_pytan[i].ile_pyt = wybor_pytan[i - 1].ile_pyt + 5;

		//Tekst "panelu"
		Ustawienia_tekstu(wybor_pytan[i].prostokat_tekst, *czcionka, 50, 0, Color::White, to_string(wybor_pytan[i].ile_pyt));
		wybor_pytan[i].prostokat_tekst.setPosition(wybor_pytan[i].prostokat.getPosition().x + wybor_pytan[i].prostokat.getGlobalBounds().width / 2 - wybor_pytan[i].prostokat_tekst.getGlobalBounds().width / 2, wybor_pytan[i].prostokat.getPosition().y + wybor_pytan[i].prostokat.getGlobalBounds().height / 2 - wybor_pytan[i].prostokat_tekst.getGlobalBounds().height + 10);
	}//for
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------Ustawienia tekstu--------------------------------------------------
	Ustawienia_tekstu(tekst2, *czcionka, 25, 0, Color::White, L"Wybierz iloœæ pytañ:");
	tekst2.setPosition(okno->getSize().x / 2 - tekst2.getGlobalBounds().width / 2, 200);

	Ustawienia_tekstu(tekst, *czcionka, 25, 0, Color::White, L"Lub podaj iloœæ pytañ:");
	tekst.setPosition(okno->getSize().x / 2 - tekst.getGlobalBounds().width / 2, okno->getSize().y - 350);

	glosnik->setPosition(okno->getSize().x - glosnik->getGlobalBounds().width - 10, 10);
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	size_t maxTextLength = 2; //Maksymalna d³ugoœæ wpisywanego tekstu

	Text wpisywany; //Aktualnie wpisywany tekst jako wyœwietlenie
	wpisywany.setFont(*czcionka);
	wpisywany.setCharacterSize(25);

	string liczba; //Wpisywany tekst

	while (okno->isOpen() && wybor)
	{
		Event wyd; //Przechwytywanie wciœniêtych klawiszy etc.
		Vector2f mysz(Mouse::getPosition(*okno)); //Pozycja myszy w oknie

		//--------------------------------------------------Wydarzenia w oknie--------------------------------------------------
		while (okno->pollEvent(wyd))
		{
			if (wyd.type == Event::TextEntered) //Jeœli coœ wpisywane
			{
				Uint32 unicode = wyd.text.unicode; //Kod wciœniêtego klawisza

				if (unicode == '\b' && liczba.size() > 0) //Jeœli backspace
				{
					liczba.erase(liczba.size() - 1, 1);
					wpisywany.setString(liczba);
				}//if
				else if (unicode >= 48 && unicode <= 57 && liczba.size() < maxTextLength) //Tylko cyfry
				{
					liczba += static_cast<char>(unicode);
					wpisywany.setString(liczba);
					liczba_pytan = stoi(liczba); //Zmiana wpisanej liczby z string na int
				}//else if
			}//if

			if (wyd.type == Event::Closed || wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Escape) //Jeœli zamkniêto okno przez "X" lub wciœniêty ESC
				return false;

			if (wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Return) //Jeœli wciœniêtym Enter
			{
				wybor = false;
				return true;
			}//if

			if (glosnik->getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
			{
				muzyka->pause();
				glosnik->setTexture(*glosnik_wyciszony_tekstura);
			}//if

			if (glosnik->getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right)
			{
				muzyka->play();
				glosnik->setTexture(*glosnik_tekstura);
			}//if

			//--------------------------------------------------Najechanie mysz¹ na "panele"--------------------------------------------------
			for (int i = 0; i < ilosc_prostokatow; i++)
			{
				if (wybor_pytan[i].prostokat.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				{
					liczba_pytan = wybor_pytan[i].ile_pyt;
					wybor = false;
					return true;
				}//if
			}//for
			//------------------------------------------------------------------------------------------------------------------------------------------------------
		}//while
		//------------------------------------------------------------------------------------------------------------------------------------------------------

		//Ustawienie pozycji tekstu wpisywanego
		wpisywany.setPosition(tekst.getPosition().x + tekst.getGlobalBounds().width + 10, tekst.getPosition().y);

		//--------------------------------------------------Ustawienia po najechaniu mysz¹ na element--------------------------------------------------
		for (int i = 0; i < ilosc_prostokatow; i++)
		{
			if (wybor_pytan[i].prostokat.getGlobalBounds().contains(mysz))
			{
				wybor_pytan[i].prostokat.setFillColor(Color(0, 0, 150, 255));
				wybor_pytan[i].prostokat.setOutlineColor(Color::Cyan);
				wybor_pytan[i].prostokat.setOutlineThickness(1);
				wybor_pytan[i].prostokat_tekst.setFillColor(Color::Green);
			}//if
			else
			{
				wybor_pytan[i].prostokat.setFillColor(Color(0, 0, 150, 150));
				wybor_pytan[i].prostokat_tekst.setFillColor(Color::White);
				wybor_pytan[i].prostokat.setOutlineColor(Color(0, 0, 150, 150));
			}//else
		}//for
		//------------------------------------------------------------------------------------------------------------------------------------------------------

		//--------------------------------------------------Wyœwietlenie na ekranie--------------------------------------------------
		okno->clear();
		okno->draw(*tlo);
		for (int i = 0; i < ilosc_prostokatow; i++)
		{
			okno->draw(wybor_pytan[i].prostokat);
			okno->draw(wybor_pytan[i].prostokat_tekst);
		}//for
		okno->draw(tekst);
		okno->draw(tekst2);
		okno->draw(wpisywany);
		okno->draw(*glosnik);
		okno->display();
		//------------------------------------------------------------------------------------------------------------------------------------------------------
	}//while

	return false;
}

void Quiz::Wynik()
{
	int kolejne_pytanie = 0, punkty = 0, poprawne_punkty = 0; //kolejne_pytanie - kolejne pytanie w vectorze, punkty - iloœæ uzyskanych pkt, poprawne_punkty - iloœæ mo¿liwych pkt za wszystkie poprawne odpowiedzi
	Text nastepne_pytanie, poprzednie_pytanie, wyjdz, ilosc_pkt, wybor[4], numer_pytania;
	string wybor_str[] = { "A. ", "B. ", "C. ", "D. " };
	bool dodaj_punkt[4];

	//--------------------------------------------------Ustawienia tekstu i jego tla w wynikach--------------------------------------------------
	//Nastêpne pytanie
	Ustawienia_tekstu(nastepne_pytanie, *czcionka, 30, 1, Color::Blue, L"Nastêpne");
	nastepne_pytanie.setPosition(750, 600);

	//Poprzednie pytanie
	Ustawienia_tekstu(poprzednie_pytanie, *czcionka, 30, 1, Color::Blue, L"Poprzednie");
	poprzednie_pytanie.setPosition(100, 600);

	//WyjdŸ
	Ustawienia_tekstu(wyjdz, *czcionka, 30, 1, Color::Blue, L"WyjdŸ");
	wyjdz.setPosition(okno->getSize().x / 2 - wyjdz.getGlobalBounds().width / 2, okno->getSize().y - wyjdz.getGlobalBounds().height - 20);

	//A, B, C, D
	for (int i = 0; i < 4; i++)
	{
		Ustawienia_tekstu(wybor[i], *czcionka, 30, 0, Color::Blue, wybor_str[i]);
		wybor[i].setPosition(20, 250 + i * 60);
	}//for
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	for (int i = 0; i < v_pytania.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (v_pytania[i].poprawnosc[j] == 1)
			{
				v_pytania[i].odpowiedz[j].setOutlineThickness(1);
				v_pytania[i].odpowiedz[j].setOutlineColor(Color::Black);
				v_pytania[i].odpowiedz[j].setStyle(Text::Bold);
				v_pytania[i].odpowiedz[j].setFillColor(Color::Green);
			}//if
		}//for
		poprawne_punkty++;

		if (Poprawnosc_Odpowiedzi())
			punkty++;
	}//for

	//Ustawienia tekstu - ci¹g dalszy
	Ustawienia_tekstu(ilosc_pkt, *czcionka, 30, 0, Color::Blue, L"Iloœæ punktów: " + to_string(punkty) + " / " + to_string(poprawne_punkty));
	ilosc_pkt.setPosition(okno->getSize().x / 2 - ilosc_pkt.getGlobalBounds().width / 2, 30);

	glosnik->setPosition(okno->getSize().x - glosnik->getGlobalBounds().width - 10, 10);

	while (okno->isOpen() && koniec_quizu)
	{
		Event wyd;
		Vector2f mysz(Mouse::getPosition(*okno));

		while (okno->pollEvent(wyd))
		{
			if (wyd.type == Event::Closed || wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Escape || wyjdz.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) //Zamniêcie okna przez "X" lub wciœniêcie ESC
				koniec_quizu = false;

			if ((nastepne_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) && kolejne_pytanie < v_pytania.size() - 1) //Zmiana pytania w prawo
				kolejne_pytanie++;

			if (wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Right && kolejne_pytanie < v_pytania.size() - 1) //Zmiana pytania w prawo
				kolejne_pytanie++;

			if ((poprzednie_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) && kolejne_pytanie > 0) //Zmiana pytania w lewo
				kolejne_pytanie--;

			if ((wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Left) && kolejne_pytanie > 0) //Zmiana pytania w lewo
				kolejne_pytanie--;

			if (glosnik->getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
			{
				muzyka->pause();
				glosnik->setTexture(*glosnik_wyciszony_tekstura);
			}//if

			if (glosnik->getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right)
			{
				muzyka->play();
				glosnik->setTexture(*glosnik_tekstura);
			}//if
		}//while

		if (nastepne_pytanie.getGlobalBounds().contains(mysz))
		{
			nastepne_pytanie.setOutlineThickness(1);
			nastepne_pytanie.setOutlineColor(Color::Cyan);
		}//if
		else if (poprzednie_pytanie.getGlobalBounds().contains(mysz))
		{
			poprzednie_pytanie.setOutlineThickness(1);
			poprzednie_pytanie.setOutlineColor(Color::Cyan);
		}//else if
		else if (wyjdz.getGlobalBounds().contains(mysz))
		{
			wyjdz.setOutlineThickness(1);
			wyjdz.setOutlineColor(Color::Cyan);
		}//else if
		else
		{
			nastepne_pytanie.setOutlineThickness(0);
			poprzednie_pytanie.setOutlineThickness(0);
			wyjdz.setOutlineThickness(0);
		}//else

		//Numer pytania
		Ustawienia_tekstu(numer_pytania, *czcionka, 35, 0, Color::Blue, "Pytanie nr " + to_string(kolejne_pytanie + 1));
		numer_pytania.setPosition(okno->getSize().x / 2 - numer_pytania.getGlobalBounds().width / 2, 100);

		okno->clear();
		okno->draw(*tlo);
		for (int j = 0; j < 4; j++)
		{
			okno->draw(v_pytania[kolejne_pytanie].odpowiedz[j]);
			okno->draw(v_pytania[kolejne_pytanie].podkreslenie[j]);
			okno->draw(wybor[j]);
		}//for
		okno->draw(numer_pytania);
		okno->draw(v_pytania[kolejne_pytanie].pytanie);
		okno->draw(nastepne_pytanie);
		okno->draw(poprzednie_pytanie);
		okno->draw(wyjdz);
		okno->draw(ilosc_pkt);
		okno->draw(*glosnik);
		okno->display();
	}//while
}

bool Quiz::Poprawnosc_Odpowiedzi()
{
	bool poprawne[4]; //Przechowuje czy dana odpowiedŸ by³a poprawna
	int poprawne_int = 0; //Iloœæ poprawnych odpowiedzi

	for (int i = 0; i < v_pytania.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (v_pytania[i].zaznaczenie[j] && v_pytania[i].poprawnosc[j])
				poprawne[j] = true;
			else if (v_pytania[i].zaznaczenie[j] && !v_pytania[i].poprawnosc[j])
				poprawne[j] = false;
			else poprawne[j] = true;
		}//for
	}//for

	for (int i = 0; i < 4; i++)
	{
		if (poprawne[i])
			poprawne_int++;
	}//for

	if (poprawne_int == 4) //Jeœli wszystko by³o zaznaczone poprawnie w pytaniu to zwraca prawdê
		return true;
	else return false;
}