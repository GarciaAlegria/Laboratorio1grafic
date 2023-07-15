#pragma once
#include <vector>
#include <fstream> // Incluye la biblioteca <fstream>
#include "Vertex.h"
#include "Color.h"

class Framebuffer
{
public:
    Framebuffer(int width, int height, const Color &clearColor); // Constructor de la clase Framebuffer

    Color &getPixel(int x, int y); // Devuelve una referencia al color del píxel en la posición especificada
    void point(const Vertex &vertex, const Color &currentColor); // Dibuja un solo píxel en la posición especificada
    void clear(const Color &clearColor); // Borra el búfer de fotogramas y lo rellena con el color de fondo especificado
    void renderBuffer(); // Renderiza el búfer de fotogramas en la pantalla
    void line(const Vertex &p0, const Vertex &p1, const Color &currentColor); // Dibuja una línea entre dos vértices
    void polygon(const std::vector<Vertex> &vertices, const Color &currentColor); // Dibuja un polígono a partir de una lista de vértices
    void fillPolygon(const std::vector<Vertex> &vertices, const Color &currentColor); // Rellena un polígono a partir de una lista de vértices

private:
    int m_width; // La anchura del búfer de fotogramas
    int m_height; // La altura del búfer de fotogramas
    std::vector<Color> m_pixels; // Los colores de los píxeles del búfer de fotogramas

    bool isValidCoordinate(int x, int y); // Verifica si la coordenada especificada es válida en el búfer de fotogramas
    bool isInsidePolygon(int x, int y, const std::vector<Vertex> &vertices); // Verifica si la coordenada especificada está dentro del polígono especificado
    void writeBmpHeader(std::ofstream &file); // Escribe el encabezado BMP en un archivo de salida especificado
};
