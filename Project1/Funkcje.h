#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <fstream>
#include <Windows.h>

using namespace sf;
using namespace std;

static void Ustawienia_tekstu(Text &tekst, Font &czcionka, int rozmiar, int grubosc_zew_koloru, Color kolor, String zawartosc) //Ustawienie podstawowych wartoœci tekstu
{
	tekst.setFont(czcionka);
	tekst.setCharacterSize(rozmiar);
	tekst.setOutlineThickness(grubosc_zew_koloru);
	tekst.setFillColor(kolor);
	tekst.setString(zawartosc);
}