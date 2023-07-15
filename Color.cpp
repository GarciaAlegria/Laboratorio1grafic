#include <iostream>
#include "Color.h"

// Definición del constructor de la clase Color
Color::Color(uint8_t red, uint8_t green, uint8_t blue)
    : m_red(red), m_green(green), m_blue(blue)
{
}

// Definición de la función print de la clase Color
void Color::print()
{
    std::cout << "r: " << static_cast<int>(m_red) // Imprime el valor del componente rojo del objeto
              << ", g: " << static_cast<int>(m_green) // Imprime el valor del componente verde del objeto
              << ", b: " << static_cast<int>(m_blue) << std::endl; // Imprime el valor del componente azul del objeto y un salto de línea
}

// Definición de la función getRed de la clase Color
uint8_t Color::getRed() const
{
    return m_red; // Devuelve el valor del componente rojo del objeto
}

// Definición de la función getGreen de la clase Color
uint8_t Color::getGreen() const
{
    return m_green; // Devuelve el valor del componente verde del objeto
}

// Definición de la función getBlue de la clase Color
uint8_t Color::getBlue() const
{
    return m_blue; // Devuelve el valor del componente azul del objeto
}