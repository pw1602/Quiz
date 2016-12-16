#include "Stworz.h"

int kolejne_pytanie = 0;

Stworz::Stworz(RenderWindow &win, Font &font, Sprite &background, Sprite &speaker, Texture &speaker_text, Texture &speaker_text_mute, Music &music)
{
	okno = &win;
	czcionka = &font;
	tlo = &background;
	glosnik = &speaker;
	glosnik_tekstura = &speaker_text;
	glosnik_wyciszony_tekstura = &speaker_text_mute;
	muzyka = &music;

	ilosc_pytan = 0;

	glosnik->setPosition(okno->getSize().x - glosnik->getGlobalBounds().width - 10, 10);
}

Stworz::~Stworz()
{
	//destruktor
}

void Stworz::Tworzenie()
{
	if (Wczytaj())
		Edytuj();
	else MessageBox(NULL, "Problem z edytowaniem quizu! Mo�liwy brak pliku!", "B��d", MB_OK);
}

bool Stworz::Wczytaj()
{
	int sredniki, pytanie_zaw; //sredniki - liczba srednikow w tek�cie, pytanie_zaw - +1 do liczby �rednik�w, by nie wczytywa� tego samego srednika
	PYTANIA pytania;
	string linia_tekstu, tekst, zawartosc_pytania[6];

	plik.open("data\\questions\\pytania.csv", ios::in);

	if (plik.good())
	{
		while (getline(plik, linia_tekstu))
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
				pytania.poprawnosc[i] = zawartosc_pytania[5][i] - '0'; //Odj�cie 0, aby nie by�y wy�wietlane warto�ci w ASCII
			}//for

			v_pytania.push_back(pytania);
		}//while
		plik.close();

		return true;
	}//if
	else return false;

	return false;
}

void Stworz::Edytuj()
{
	int rozmiar_czcionki_pytania;
	bool tworzenie = true, mozliwosc_zaznaczenia = true;
	Text wyjdz, zapisz, nastepne_pytanie, poprzednie_pytanie, ilosc_pytan_tekst, dodaj_pytanie, usun_pytanie;
	size_t maxTextLength = 50; //Maksymalna d�ugo�� wpisywanego tekstu
	wstring tekst; //Tekst jaki zostaje wpisywany

	//--------------------------------------------------Ustawienia tekstu i jego tla w quzie--------------------------------------------------
	//Nast�pne pytanie
	Ustawienia_tekstu(nastepne_pytanie, *czcionka, 30, 1, Color::Blue, L"Nast�pne");
	nastepne_pytanie.setPosition(600, 600);

	//Poprzednie pytanie
	Ustawienia_tekstu(poprzednie_pytanie, *czcionka, 30, 1, Color::Blue, L"Poprzednie");
	poprzednie_pytanie.setPosition(100, 600);

	//Zako�czenie quizu
	Ustawienia_tekstu(zapisz, *czcionka, 25, 1, Color::Blue, L"Zapisz pytania");
	zapisz.setPosition(okno->getSize().x / 2 - zapisz.getGlobalBounds().width / 2, okno->getSize().y - zapisz.getGlobalBounds().height - 20);

	//Wyj�cie z quizu
	Ustawienia_tekstu(wyjdz, *czcionka, 25, 1, Color::Blue, L"Wyjd�");
	wyjdz.setPosition(20, okno->getSize().y - wyjdz.getGlobalBounds().height - 20);

	//Ilo�� pyta�
	ilosc_pytan = v_pytania.size();
	Ustawienia_tekstu(ilosc_pytan_tekst, *czcionka, 30, 0, Color::Blue, L"Ilo�� pyta�: " + to_string(ilosc_pytan));
	ilosc_pytan_tekst.setPosition(okno->getSize().x / 2 - ilosc_pytan_tekst.getGlobalBounds().width / 2, 20);

	//Dodaj pytanie
	Ustawienia_tekstu(dodaj_pytanie, *czcionka, 25, 1, Color::Blue, L"Dodaj pytanie");
	dodaj_pytanie.setPosition(okno->getSize().x - dodaj_pytanie.getGlobalBounds().width - 20, okno->getSize().y - dodaj_pytanie.getGlobalBounds().height - 20);

	//Usu� pytanie
	Ustawienia_tekstu(usun_pytanie, *czcionka, 25, 1, Color::Blue, L"Usu� pytanie");
	usun_pytanie.setPosition(okno->getSize().x - usun_pytanie.getGlobalBounds().width - 20, dodaj_pytanie.getPosition().y - usun_pytanie.getGlobalBounds().height - 10);
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------Ustawienie w�asno�ci element�w w strukturze pyta�--------------------------------------------------
	for (int i = 0; i < v_pytania.size(); i++)
	{
		//Pytanie
		v_pytania[i].pytanie.setFont(*czcionka);
		v_pytania[i].pytanie.setFillColor(Color::Blue);
		v_pytania[i].pytanie.setStyle(Text::Bold);
		v_pytania[i].zaznaczenie_pytania = true;

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
			Ustawienia_tekstu(v_pytania[i].odpowiedz[j], *czcionka, 45, 0, Color::Blue, v_pytania[i].odpowiedz[j].getString());
			v_pytania[i].odpowiedz[j].setPosition(150, 250 + j * 60);

			//Podkre�lenie
			v_pytania[i].podkreslenie[j].setSize(Vector2f(0, 0));
			v_pytania[i].podkreslenie[j].setFillColor(Color::Red);

			v_pytania[i].podkreslenie_pytania.setSize(Vector2f(0, 0));
			v_pytania[i].podkreslenie_pytania.setFillColor(Color::Red);

			//Zaznaczenie
			v_pytania[i].zaznaczenie[j] = false;
		}//for
	}//for
	 //------------------------------------------------------------------------------------------------------------------------------------------------------

	while (okno->isOpen() && tworzenie)
	{
		Event wyd; //Przechwytuje wci�ni�te klawicze etc.
		Vector2f mysz(Mouse::getPosition(*okno)); //Zapisuje pozycje myszy w oknie

		//--------------------------------------------------Wydarzenia w oknie--------------------------------------------------
		while (okno->pollEvent(wyd))
		{
			if (wyd.type == Event::TextEntered) //Je�li co� wpisywane
			{
				Uint32 unicode = wyd.text.unicode; //Kod wci�ni�tego klawisza

				if (v_pytania[kolejne_pytanie].zaznaczenie_pytania) //Dla pytania
				{
					tekst = v_pytania[kolejne_pytanie].pytanie.getString();
					if (unicode == '\b' && tekst.size() > 0) //Je�li backspace
					{
						tekst.erase(tekst.size() - 1, 1);
						v_pytania[kolejne_pytanie].pytanie.setString(tekst);
						v_pytania[kolejne_pytanie].pytanie.setPosition(okno->getSize().x / 2 - v_pytania[kolejne_pytanie].pytanie.getGlobalBounds().width / 2, 163);
					}//if
					else if ((unicode >= 32 && unicode <= 126) && tekst.size() < maxTextLength)
					{
						tekst += static_cast<char>(unicode);
						v_pytania[kolejne_pytanie].pytanie.setString(tekst);
						v_pytania[kolejne_pytanie].pytanie.setPosition(okno->getSize().x / 2 - v_pytania[kolejne_pytanie].pytanie.getGlobalBounds().width / 2, 163);
					}//else if
				}//if
				else //Dla odpowiedzi
				{
					for (int i = 0; i < 4; i++)
					{
						tekst = v_pytania[kolejne_pytanie].odpowiedz[i].getString();
						if (v_pytania[kolejne_pytanie].zaznaczenie[i])
						{
							if (unicode == '\b' && tekst.size() > 0) //Je�li backspace
							{
								tekst.erase(tekst.size() - 1, 1);
								v_pytania[kolejne_pytanie].odpowiedz[i].setString(tekst);
								v_pytania[kolejne_pytanie].odpowiedz[i].setPosition(150, 250 + i * 60);
							}//if
							else if ((unicode >= 32 && unicode <= 126) && tekst.size() < maxTextLength)
							{
								tekst += static_cast<char>(unicode);
								v_pytania[kolejne_pytanie].odpowiedz[i].setString(tekst);
								v_pytania[kolejne_pytanie].odpowiedz[i].setPosition(150, 250 + i * 60);
							}//else if
						}//if
					}//for
				}//else
			}//if

			if (wyd.type == Event::Closed || wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Escape || wyjdz.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) //Zamni�cie okna przez "X" lub ESC lub przycisc wyjd�
				tworzenie = false;

			if (nastepne_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left && kolejne_pytanie < v_pytania.size() - 1) //Zmiana pytania w prawo
				kolejne_pytanie++;

			if (wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Right && kolejne_pytanie < v_pytania.size() - 1) //Zmiana pytania w prawo
				kolejne_pytanie++;

			if (poprzednie_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left && kolejne_pytanie > 0) //Zmiana pytania w lewo
				kolejne_pytanie--;

			if (wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Left && kolejne_pytanie > 0) //Zmiana pytania w lewo
				kolejne_pytanie--;

			if (dodaj_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				Dodaj_Pytanie();

			if (usun_pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				Usun_Pytanie();

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

			 //--------------------------------------------------Najechanie mysz� na odpowiedzi--------------------------------------------------
			if ((v_pytania[kolejne_pytanie].pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) && !v_pytania[kolejne_pytanie].zaznaczenie_pytania && !Zaznaczona_Odpowiedz())
				v_pytania[kolejne_pytanie].zaznaczenie_pytania = true;
			else if ((v_pytania[kolejne_pytanie].pytanie.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right) && v_pytania[kolejne_pytanie].zaznaczenie_pytania)
				v_pytania[kolejne_pytanie].zaznaczenie_pytania = false;

			for (int i = 0; i < 4; i++)
			{
				if ((v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left) && !v_pytania[kolejne_pytanie].zaznaczenie[i] && !v_pytania[kolejne_pytanie].zaznaczenie_pytania && !Zaznaczona_Odpowiedz())
					v_pytania[kolejne_pytanie].zaznaczenie[i] = true;
				else if ((v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right) && v_pytania[kolejne_pytanie].zaznaczenie[i])
					v_pytania[kolejne_pytanie].zaznaczenie[i] = false;
			}//for
			//------------------------------------------------------------------------------------------------------------------------------------------------------
		}//while
		//------------------------------------------------------------------------------------------------------------------------------------------------------

		//--------------------------------------------------Ustawienia zaznaczenia w quizie--------------------------------------------------
		//Pytanie
		if (v_pytania[kolejne_pytanie].pytanie.getGlobalBounds().contains(mysz) || v_pytania[kolejne_pytanie].zaznaczenie_pytania)
		{
			v_pytania[kolejne_pytanie].podkreslenie_pytania.setSize(Vector2f(v_pytania[kolejne_pytanie].pytanie.getGlobalBounds().width, 2));
			v_pytania[kolejne_pytanie].podkreslenie_pytania.setPosition(v_pytania[kolejne_pytanie].pytanie.getPosition().x, v_pytania[kolejne_pytanie].pytanie.getPosition().y + 50);
		}//if
		else v_pytania[kolejne_pytanie].podkreslenie_pytania.setSize(Vector2f(0, 0));

		//Odpowiedzi
		for (int i = 0; i < 4; i++)
		{
			if (v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().contains(mysz) || v_pytania[kolejne_pytanie].zaznaczenie[i])
			{
				v_pytania[kolejne_pytanie].podkreslenie[i].setSize(Vector2f(v_pytania[kolejne_pytanie].odpowiedz[i].getGlobalBounds().width, 2));
				v_pytania[kolejne_pytanie].podkreslenie[i].setPosition(v_pytania[kolejne_pytanie].odpowiedz[i].getPosition().x, v_pytania[kolejne_pytanie].odpowiedz[i].getPosition().y + 50);
			}//if
			else v_pytania[kolejne_pytanie].podkreslenie[i].setSize(Vector2f(0, 0));
		}//for
		 //------------------------------------------------------------------------------------------------------------------------------------------------------
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
		else if (zapisz.getGlobalBounds().contains(mysz))
		{
			zapisz.setOutlineThickness(1);
			zapisz.setOutlineColor(Color::Cyan);
		}//else if
		else if (wyjdz.getGlobalBounds().contains(mysz))
		{
			wyjdz.setOutlineThickness(1);
			wyjdz.setOutlineColor(Color::Cyan);
		}//else if
		else if (dodaj_pytanie.getGlobalBounds().contains(mysz))
		{
			dodaj_pytanie.setOutlineThickness(1);
			dodaj_pytanie.setOutlineColor(Color::Cyan);
		}//else if
		else if (usun_pytanie.getGlobalBounds().contains(mysz))
		{
			usun_pytanie.setOutlineThickness(1);
			usun_pytanie.setOutlineColor(Color::Cyan);
		}//else if
		else
		{
			nastepne_pytanie.setOutlineThickness(0);
			poprzednie_pytanie.setOutlineThickness(0);
			zapisz.setOutlineThickness(0);
			wyjdz.setOutlineThickness(0);
			usun_pytanie.setOutlineThickness(0);
			dodaj_pytanie.setOutlineThickness(0);
		}//else
		 //------------------------------------------------------------------------------------------------------------------------------------------------------

		ilosc_pytan_tekst.setString(L"Ilo�� pyta�: " + to_string(ilosc_pytan));

		okno->clear();
		okno->draw(*tlo);
		okno->draw(ilosc_pytan_tekst);
		okno->draw(v_pytania[kolejne_pytanie].pytanie);
		okno->draw(v_pytania[kolejne_pytanie].podkreslenie_pytania);
		for (int j = 0; j < 4; j++)
		{
			okno->draw(v_pytania[kolejne_pytanie].odpowiedz[j]);
			okno->draw(v_pytania[kolejne_pytanie].podkreslenie[j]);
		}//for
		okno->draw(nastepne_pytanie);
		okno->draw(poprzednie_pytanie);
		okno->draw(zapisz);
		okno->draw(wyjdz);
		okno->draw(dodaj_pytanie);
		okno->draw(usun_pytanie);
		okno->draw(*glosnik);
		okno->display();
	}//while
}

bool Stworz::Zaznaczona_Odpowiedz()
{
	for (int i = 0; i < 4; i++)
	{
		if (v_pytania[kolejne_pytanie].zaznaczenie[i])
		{
			return true;
			break;
		}//if
	}//for

	return false;
}

void Stworz::Dodaj_Pytanie()
{
	PYTANIA pytania;

	Ustawienia_tekstu(pytania.pytanie, *czcionka, 45, 0, Color::Blue, L"Przyk�adowe pytanie");
	pytania.pytanie.setPosition(okno->getSize().x / 2 - pytania.pytanie.getGlobalBounds().width / 2, 163);

	for (int j = 0; j < 4; j++)
	{
		//Odpowiedzi
		Ustawienia_tekstu(pytania.odpowiedz[j], *czcionka, 45, 0, Color::Blue, L"Przyk�adowa odpowied�");
		pytania.odpowiedz[j].setPosition(150, 250 + j * 60);

		//Podkre�lenie
		pytania.podkreslenie[j].setSize(Vector2f(0, 0));
		pytania.podkreslenie[j].setFillColor(Color::Red);

		pytania.podkreslenie_pytania.setSize(Vector2f(0, 0));
		pytania.podkreslenie_pytania.setFillColor(Color::Red);

		//Zaznaczenie
		pytania.zaznaczenie[j] = false;
	}//for

	v_pytania.push_back(pytania);

	kolejne_pytanie = v_pytania.size() - 1;
	ilosc_pytan = v_pytania.size();
}

void Stworz::Usun_Pytanie()
{
	string tekst;
	v_pytania.erase(v_pytania.begin() + kolejne_pytanie);
	if (kolejne_pytanie >= v_pytania.size())
		kolejne_pytanie = v_pytania.size() - 1;

	ilosc_pytan = v_pytania.size();
}