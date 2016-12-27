#include "Menu.h"

Menu::Menu()
{
	wybor_menu = NIC;

	okno.create(VideoMode(1024, 768), "Quiz", Style::Close); //Stworzenie okna
	okno.setFramerateLimit(60); //Ustawienie FPS

	if (!czcionka.loadFromFile("data\\fonts\\Nerwus.ttf"))
		MessageBox(NULL, "Brak czcionki Nerwus.ttf!", "B³¹d", MB_OK);

	if (!tlo_tekstura_menu.loadFromFile("data\\imgs\\background_menu.png"))
		MessageBox(NULL, "Brak grafiki background.png!", "B³¹d", MB_OK);
	else tlo_menu.setTexture(tlo_tekstura_menu);

	if (!tlo_tekstura_pomoc.loadFromFile("data\\imgs\\background_help.jpg"))
		MessageBox(NULL, "Brak grafiki background_help.jpg!", "B³¹d", MB_OK);
	else tlo_pomoc.setTexture(tlo_tekstura_pomoc);

	if (!glosnik_tekstura.loadFromFile("data\\imgs\\Speaker.png"))
		MessageBox(NULL, "Brak grafiki Speaker.png!", "B³¹d", MB_OK);
	else glosnik.setTexture(glosnik_tekstura);

	if (!glosnik_wyciszony_tekstura.loadFromFile("data\\imgs\\Mute_Speaker.png"))
		MessageBox(NULL, "Brak grafiki Mute_Speaker.png!", "B³¹d", MB_OK);

	if (!muzyka.openFromFile("data\\musics\\Podington_Bear_-_60s_Quiz_Show.ogg"))
		MessageBox(NULL, "Brak muzyki Podington_Bear_-_60s_Quiz_Show.ogg!", "B³¹d", MB_OK);
	else muzyka.play();

	muzyka.setLoop(true);

	wybor_menu = MENU;
}

Menu::~Menu()
{
	//destruktor
}

void Menu::Menu_Glowne()
{
	while (wybor_menu != NIC)
	{
		switch (wybor_menu)
		{
		case WYBOR_MENU::MENU:
		{
			menu();
			break;
		}//case
		case WYBOR_MENU::QUIZ:
		{
			rozwiaz();
			wybor_menu = MENU;
			break;
		}//case
		case WYBOR_MENU::TWORZENIE:
		{
			stworz();
			break;
		}//case
		case WYBOR_MENU::POMOC:
		{
			Jak_Grac();
			break;
		}//case
		}//switch
	}//while
}

void Menu::menu()
{
	const int ile = 4; //Iloœæ wyborów w menu
	String str[] = { L"Rozwi¹¿ Quiz", L"Edytuj Quiz", L"Pomoc", L"Zakoñcz" };
	Text tekst[ile]; //Zawiera str[], ale jest tylko do wyœwietlenia i manipulacji wyœwietlanym tekstem

	for (int i = 0; i < ile; i++)
	{
		tekst[i].setFont(czcionka);
		tekst[i].setCharacterSize(50);
		tekst[i].setString(str[i]);
		tekst[i].setPosition(okno.getSize().x / 2 - tekst[i].getGlobalBounds().width / 2, 220 + i * 80);
		tekst[i].setFillColor(Color::Blue);
	}//for

	glosnik.setPosition(750, 100);

	while (wybor_menu == MENU)
	{
		Event wyd;
		Vector2f mysz(Mouse::getPosition(okno));

		while (okno.pollEvent(wyd))
		{
			if (wyd.type == Event::Closed || tekst[ile - 1].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				wybor_menu = NIC;

			if (tekst[0].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				wybor_menu = QUIZ;

			if (tekst[1].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				wybor_menu = TWORZENIE;

			if (tekst[2].getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
				wybor_menu = POMOC;

			if (glosnik.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
			{
				muzyka.pause();
				glosnik.setTexture(glosnik_wyciszony_tekstura);
			}//if

			if (glosnik.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right)
			{
				muzyka.play();
				glosnik.setTexture(glosnik_tekstura);
			}//if
		}//while

		for (int i = 0; i < ile; i++)
		{
			if (tekst[i].getGlobalBounds().contains(mysz))
			{
				tekst[i].setOutlineThickness(1);
				tekst[i].setOutlineColor(Color::Cyan);
			}//if
			else
			{
				tekst[i].setOutlineThickness(0);
				tekst[i].setOutlineColor(Color::Blue);
			}//else
		}//for

		okno.clear();
		okno.draw(tlo_menu);
		for (int i = 0; i < ile; i++)
		{
			okno.draw(tekst[i]);
		}//for
		okno.draw(glosnik);
		okno.display();
	}//while
}

void Menu::rozwiaz()
{
	Quiz quiz(okno, czcionka, tlo_pomoc, muzyka, glosnik, glosnik_tekstura, glosnik_wyciszony_tekstura);

	quiz.Rozpocznij_Quiz();

	wybor_menu = MENU;
}

void Menu::Jak_Grac()
{
	const int ile = 3; //ZMIEÑ PRZY ZMIANIE STRINGA "POZOSTALE_STR"!
	Text wroc, tytul;
	Text pozostale[ile];
	String pozostale_str[] = {
		L"Punkty - Za ka¿d¹ poprawn¹ odpowiedŸ +1",
		L"Czas - 30 sekund na jedno pytanie",
		L"* Mo¿na równie¿ u¿ywaæ menu"
	};

	//--------------------------------------------------Ustawienia tekstu--------------------------------------------------
	Ustawienia_tekstu(wroc, czcionka, 35, 0, Color::Blue, L"Wróæ");
	wroc.setPosition(20, okno.getSize().y - wroc.getGlobalBounds().height - 20);

	Ustawienia_tekstu(tytul, czcionka, 45, 0, Color::Blue, L"POMOC");
	tytul.setPosition(okno.getSize().x / 2 - tytul.getGlobalBounds().width / 2, 30);

	for (int i = 0; i < ile; i++)
	{
		Ustawienia_tekstu(pozostale[i], czcionka, 20, 0, Color::Blue, pozostale_str[i]);
		pozostale[i].setPosition(550, 400 + i * 50);
	}//for
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	Zaladuj_Obrazki(); //Za³adowanie tekstur do quizu
	glosnik.setPosition(okno.getSize().x - glosnik.getGlobalBounds().width - 10, 10);

	while (wybor_menu == POMOC)
	{
		Event wyd;
		Vector2f mysz(Mouse::getPosition(okno));

		while (okno.pollEvent(wyd))
		{
			if (wyd.type == Event::Closed)
				wybor_menu = NIC;

			if (wroc.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left || wyd.type == Event::KeyReleased && wyd.key.code == Keyboard::Escape)
				wybor_menu = MENU;

			if (glosnik.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Left)
			{
				muzyka.pause();
				glosnik.setTexture(glosnik_wyciszony_tekstura);
			}//if

			if (glosnik.getGlobalBounds().contains(mysz) && wyd.type == Event::MouseButtonReleased && wyd.key.code == Mouse::Right)
			{
				muzyka.play();
				glosnik.setTexture(glosnik_tekstura);
			}//if
		}//while

		if (wroc.getGlobalBounds().contains(mysz))
		{
			wroc.setOutlineThickness(1);
			wroc.setOutlineColor(Color::Cyan);
		}//if
		else
		{
			wroc.setOutlineThickness(0);
		}//else

		okno.clear();
		okno.draw(tlo_pomoc);
		okno.draw(wroc);
		for (int i = 0; i < ile_obrazkow; i++)
		{
			okno.draw(v_pomoc[i].sprajt);
			okno.draw(v_pomoc[i].tekst);
		}//for
		for (int i = 0; i < ile; i++)
		{
			okno.draw(pozostale[i]);
		}//for
		okno.draw(tytul);
		okno.draw(glosnik);
		okno.display();
	}//while
}

void Menu::Zaladuj_Obrazki()
{
	string str[] = { "Arrow_Left", "Arrow_Right", "Backspace", "Enter", "Esc", "Mouse_Left", "Mouse_Right" };
	String do_strzalek[] = {
		L"Poprzednie pytanie*",
		L"Nastêpne pytanie*",
		L"Usuñ liczbê",
		L"ZatwierdŸ liczbê",
		L"Wyjœcie z quizu*",
		L"Zaznaczenie",
		L"Odznaczenie"
	};

	for (int i = 0; i < ile_obrazkow; i++)
	{
		if (teksturki[i].loadFromFile("data\\imgs\\" + str[i] + ".png"))
		{
			pomoc_tekst.sprajt.setTexture(teksturki[i]);
			Ustawienia_tekstu(pomoc_tekst.tekst, czcionka, 30, 0, Color::Blue, do_strzalek[i]);

			if (i >= 5)
				pomoc_tekst.sprajt.setPosition(550, 100 + ((i - 5) * 100));
			else pomoc_tekst.sprajt.setPosition(20, 100 + i * 100);

			pomoc_tekst.tekst.setPosition(pomoc_tekst.sprajt.getPosition().x + pomoc_tekst.sprajt.getGlobalBounds().width + 10, pomoc_tekst.sprajt.getPosition().y + 70 - pomoc_tekst.tekst.getGlobalBounds().height);

			v_pomoc.push_back(pomoc_tekst);
		}//if
		else MessageBox(NULL, "Problem z wczytaniem grafiki!", "B³¹d", MB_OK);
	}//for
}

void Menu::stworz()
{
	Stworz tworzenie(okno, czcionka, tlo_pomoc, glosnik, glosnik_tekstura, glosnik_wyciszony_tekstura, muzyka);

	tworzenie.Tworzenie();

	wybor_menu = MENU;
}