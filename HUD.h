#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Rect.hpp>

class HUD
{
private:	
	const int PADDING = 10;

	sf::Font m_MainFont;
	sf::Font m_TitleFont;

	sf::Text m_TitleScreen;

	// Game Mode
	sf::Text m_TextScore;

	// Editor Mode
	sf::Text m_TitleControls;
	sf::Text m_TitleBoxProperties;

	sf::Text m_TextBoxSizeX;
	sf::Text m_TextBoxSizeY;
	sf::Text m_TextBoxRotation;
	sf::Text m_TextBoxSelectedMaterial;
	sf::Text m_TextCurrentLoadedLevel;
	sf::Text m_TextPendingChanges;
	sf::Text m_TextControls;

public:
	HUD(sf::Font titleFont, sf::Font textFont);

	void drawGameHUD(sf::RenderTarget& target, sf::View& hudView, std::string currentLoadedLevel);

	void drawEditorModeHUD(
		sf::RenderTarget& target,
		sf::View& hudView,
		sf::View& gameview,
		std::string boxSelectedMaterial, 
		float rotationRadians,
		sf::FloatRect boxRect,
		std::string currentLoadedLevel, 
		bool hasPendingChanges
	);
};

