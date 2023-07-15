#include "Framebuffer.h"
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <fstream>

// Constructor de la clase Framebuffer
Framebuffer::Framebuffer(int width, int height, const Color &clearColor)
    : m_width(width), m_height(height), m_pixels(width * height, clearColor)
{
}

// Devuelve una referencia al color del píxel en la posición especificada
Color &Framebuffer::getPixel(int x, int y)
{
    return m_pixels[y * m_width + x];
}

// Dibuja un solo píxel en la posición especificada
void Framebuffer::point(const Vertex &vertex, const Color &currentColor)
{
    if (isValidCoordinate(vertex.x, vertex.y))
    {
        int index = vertex.y * m_width + vertex.x;
        m_pixels[index] = currentColor;
    }
}

// Borra el búfer de fotogramas y lo rellena con el color de fondo especificado
void Framebuffer::clear(const Color &clearColor)
{
    std::fill(m_pixels.begin(), m_pixels.end(), clearColor);
}

// Renderiza el búfer de fotogramas en la pantalla
void Framebuffer::renderBuffer()
{
    std::ofstream file("out.bmp", std::ios::binary);

    // Escribir el encabezado BMP
    writeBmpHeader(file);

    // Escribir los datos de los píxeles
    for (const auto &color : m_pixels)
    {
        // Escribir los componentes de color en orden BGR
        file.put(color.getBlue());
        file.put(color.getGreen());
        file.put(color.getRed());
    }

    file.close();
}

// Dibuja una línea entre dos vértices
void Framebuffer::line(const Vertex &p0, const Vertex &p1, const Color &currentColor)
{
    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (x0 != x1 || y0 != y1)
    {
        Vertex current{x0, y0};
        point(current, currentColor);

        int err2 = 2 * err;
        if (err2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// Dibuja un polígono a partir de una lista de vértices
void Framebuffer::polygon(const std::vector<Vertex> &vertices, const Color &currentColor)
{
    for (int i = 0; i < vertices.size() - 1; i++)
    {
        line(vertices[i], vertices[i + 1], currentColor);
    }
    line(vertices[vertices.size() - 1], vertices[0], currentColor);
}

// Rellena un polígono a partir de una lista de vértices
void Framebuffer::fillPolygon(const std::vector<Vertex> &vertices, const Color &fillColor)
{
    // Encontrar el cuadro delimitador del polígono
    int minX = m_width;
    int maxX = 0;
    int minY = m_height;
    int maxY = 0;

    for (const auto &vertex : vertices)
    {
        minX = std::min(minX, vertex.x);
        maxX = std::max(maxX, vertex.x);
        minY = std::min(minY, vertex.y);
        maxY = std::max(maxY, vertex.y);
    }

    // Iterar sobre cada píxel dentro del cuadro delimitador
    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            // Verificar si el píxel actual está dentro del polígono
            if (isInsidePolygon(x, y, vertices))
            {
                point({x, y}, fillColor);
            }
        }
    }
}

// Verifica si la coordenada especificada está dentro del polígono especificado
bool Framebuffer::isInsidePolygon(int x, int y, const std::vector<Vertex> &vertices)
{
    int numVertices = vertices.size();
    bool inside = false;

    for (int i = 0, j = numVertices - 1; i < numVertices; j = i++)
    {
        if (((vertices[i].y > y) != (vertices[j].y > y)) &&
            (x < (vertices[j].x - vertices[i].x) * (y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x))
        {
            inside = !inside;
        }
    }

    return inside;
}

// Verifica si la coordenada especificada es válida (dentro de los límites del búfer de fotogramas)
bool Framebuffer::isValidCoordinate(int x, int y)
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

// Escribe el encabezado BMP en el archivo especificado
void Framebuffer::writeBmpHeader(std::ofstream &file)
{
    // Estructura del encabezado BMP
#pragma pack(push, 1) // Asegura que la estructura esté empaquetada de forma ajustada
    struct BitmapHeader
    {
        uint16_t type;
        uint32_t size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset;
        uint32_t headerSize;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bitDepth;
        uint32_t compression;
        uint32_t imageSize;
        int32_t xPixelsPerMeter;
        int32_t yPixelsPerMeter;
        uint32_t totalColors;
        uint32_t importantColors;
    };
#pragma pack(pop)

    BitmapHeader header;
    std::memset(&header, 0, sizeof(BitmapHeader));

    header.type = 0x4D42;                                     
    header.size = sizeof(BitmapHeader) + m_pixels.size() * 3; 
    header.offset = sizeof(BitmapHeader);
    header.headerSize = sizeof(BitmapHeader) - 14; // se resta el tamaño de los campos comunes
    header.width = m_width;
    header.height = m_height;
    header.planes = 1;
    header.bitDepth = 24;
    header.compression = 0;
    header.imageSize = m_width * m_height * 3; 
    header.xPixelsPerMeter = 2835;             
    header.yPixelsPerMeter = 2835;             
    header.totalColors = 0;
    header.importantColors = 0;

    file.write(reinterpret_cast<const char *>(&header), sizeof(BitmapHeader));
}