#pragma once
#include <cstdint>

class Color
{
public:
    // Constructor de la clase Color
    Color(uint8_t red, uint8_t green, uint8_t blue);

    // Función para imprimir los componentes del color
    void print();

    // Funciones para obtener los valores de los componentes del color
    uint8_t getRed() const;
    uint8_t getGreen() const;
    uint8_t getBlue() const;

private:
    // Variables miembro para almacenar los componentes del color
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
};
