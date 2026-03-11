#include "HUD.h"
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <format>
#include "Converter.h"
#include "GameEngine.h"

HUD::HUD(sf::Font titleFont, sf::Font textFont) :
	m_TitleFont(titleFont),
	m_MainFont(textFont),
	m_TitleScreen(m_TitleFont),
	m_TextBoxSizeX(m_MainFont),
	m_TextBoxSizeY(m_MainFont),
	m_TextBoxRotation(m_MainFont),
	m_TextBoxSelectedMaterial(m_MainFont),
	m_TextCurrentLoadedLevel(m_MainFont),
	m_TextPendingChanges(m_MainFont),
	m_TextControls(m_MainFont),
	m_TitleControls(m_MainFont),
	m_TitleBoxProperties(m_MainFont),
	m_TextScore(m_MainFont),
	m_TextNumberOfTries(m_MainFont)
{	
	m_TitleScreen.setCharacterSize(60);
	
	m_TextScore.setCharacterSize(30);
	m_TextNumberOfTries.setCharacterSize(30);

	m_TextBoxSizeX.setCharacterSize(20);
	m_TextBoxSizeY.setCharacterSize(20);
	m_TextBoxRotation.setCharacterSize(20);
	m_TextBoxSelectedMaterial.setCharacterSize(20);
	m_TitleBoxProperties.setCharacterSize(30);

	m_TextCurrentLoadedLevel.setCharacterSize(50);
	
	m_TextPendingChanges.setCharacterSize(20);
	m_TextPendingChanges.setFillColor(sf::Color(255, 107, 107));

	m_TextControls.setCharacterSize(20);
	m_TitleControls.setCharacterSize(30);
}

void HUD::drawGameHUD(sf::RenderTarget& target, sf::View& hudView, std::string currentLoadedLevel, int currentNumberOfTries)
{
	m_TitleScreen.setString(currentLoadedLevel);
	m_TitleScreen.setPosition(
		{
			hudView.getCenter().x - m_TitleScreen.getLocalBounds().size.x / 2.0f,
			40
		});

	m_TextScore.setString(std::format("Score: {}", GameEngine::Score));
	m_TextScore.setPosition({
		hudView.getCenter().x - hudView.getSize().x / 2.0f,
		m_TextScore.getLocalBounds().size.y + m_PADDING + 40
		});

	target.draw(m_TitleScreen);
	target.draw(m_TextScore);
	target.draw(m_TextNumberOfTries);
}

void HUD::drawEditorModeHUD(
	sf::RenderTarget& target, 
	sf::View& hudView, 
	sf::View& gameview,
	std::string boxSelectedMaterial, 
	float rotationRadians,
	sf::FloatRect boxRect,
	std::string currentLoadedLevel, 
	bool hasPendingChanges)
{
	sf::Vector2f viewCenter = hudView.getCenter();
	sf::Vector2f viewSize = hudView.getSize();

	m_TitleScreen.setString("Editor de level");
	m_TitleScreen.setPosition(
		{
			viewCenter.x - m_TitleScreen.getLocalBounds().size.x / 2.0f,
			40
		});

	m_TextCurrentLoadedLevel.setString(currentLoadedLevel);
	m_TextCurrentLoadedLevel.setPosition(
		{
			viewCenter.x + viewSize.x/2.0f - m_TextCurrentLoadedLevel.getLocalBounds().size.x - m_PADDING,
			40
		});

	if (hasPendingChanges)
	{
		m_TextPendingChanges.setFillColor(sf::Color(255, 107, 107));
	}
	else
	{
		m_TextPendingChanges.setFillColor(sf::Color(255, 255, 255));
	}
	m_TextPendingChanges.setString(hasPendingChanges ? "Existem alteracoes pendentes" : "Mudancas salvas");
	m_TextPendingChanges.setPosition(
		{
			viewCenter.x + viewSize.x / 2.0f - m_TextPendingChanges.getLocalBounds().size.x - m_PADDING,
			40 + m_TextCurrentLoadedLevel.getLocalBounds().size.y + m_TextPendingChanges.getLocalBounds().size.y
		}
	);

	m_TextControls.setString(
	R"(
Trocar Level: 1...5
Limpar Level: Delete
Excluir Caixa: click direito
Mover Caixa: WASD
Aumentar Caixa: setinhas
Mover Caixa devagar: Shift + WASD
Aumentar Caixa devagar: Shift + Setinhas
Trocar material: N
Girar Caixa: R
Criar Caixa: Enter
Ativar/Desativar simulacao: Tab
Salvar level: Ctrl + S
Iniciar Jogo: "
		)"
	);
	m_TextControls.setPosition(
		{
			viewCenter.x - viewSize.x / 2.0f + m_PADDING,
			viewCenter.y - m_TextControls.getLocalBounds().size.y / 2.0f
		}
	);

	m_TitleControls.setString("Controles:");
	m_TitleControls.setPosition(
		{
			viewCenter.x - viewSize.x / 2.0f,
			viewCenter.y - m_TitleControls.getLocalBounds().size.y - m_TextControls.getLocalBounds().size.y / 2.0f
		}
	);

	m_TitleBoxProperties.setString("Propriedades da caixa:");
	m_TitleBoxProperties.setPosition({
		viewCenter.x - viewSize.x / 2.0f,
		m_TitleBoxProperties.getLocalBounds().size.y + m_PADDING
		});

	m_TextBoxSelectedMaterial.setString(std::format("Material: {}", boxSelectedMaterial));
	m_TextBoxSelectedMaterial.setPosition({
		viewCenter.x - viewSize.x / 2.0f + m_PADDING,
		m_TextBoxSelectedMaterial.getLocalBounds().size.y +
			m_TitleBoxProperties.getLocalBounds().size.y * 2
		});

	m_TextBoxRotation.setString(std::format("Rotacao (Graus): {:.0f}", sf::radians(rotationRadians).asDegrees()));
	m_TextBoxRotation.setPosition({
		viewCenter.x - viewSize.x / 2.0f + m_PADDING,
		m_TextBoxRotation.getLocalBounds().size.y + m_TextBoxSelectedMaterial.getPosition().y
		});

	m_TextBoxSizeX.setString(std::format("Largura (m): {:.2g}", converter::pixelsToMeters(boxRect.size.x)));
	m_TextBoxSizeX.setPosition({
		viewCenter.x - viewSize.x / 2.0f + m_PADDING,
		m_TextBoxSizeX.getLocalBounds().size.y + m_TextBoxRotation.getPosition().y
		});

	m_TextBoxSizeY.setString(std::format("Altura (m): {:.2g}", converter::pixelsToMeters(boxRect.size.y)));
	m_TextBoxSizeY.setPosition({
		viewCenter.x - viewSize.x / 2.0f + m_PADDING,
		m_TextBoxSizeY.getLocalBounds().size.y + m_TextBoxSizeX.getPosition().y
		});

	target.draw(m_TitleScreen);
	target.draw(m_TextCurrentLoadedLevel);
	target.draw(m_TextPendingChanges);
	target.draw(m_TextBoxRotation);
	target.draw(m_TextBoxSelectedMaterial);
	target.draw(m_TextBoxSizeX);
	target.draw(m_TextBoxSizeY);
	target.draw(m_TextControls);	
	target.draw(m_TitleControls);
	target.draw(m_TitleBoxProperties);
}
