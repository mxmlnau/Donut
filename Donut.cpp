#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <vector>



std::string GetBackpaces(const int length)
{
    std::string backspaces = "";
    for (int i = 0; i < length; i++)
    {
        backspaces += "\b \b";
        //std::cout << "\b \b" << std::flush;
    }
    std::cout << backspaces << std::flush;
    return backspaces;
}


void PrintWithTick(std::string string_to_print, const int tick_time)
{
    std::cout << string_to_print << std::flush;
    Sleep(tick_time);
    std::cout << GetBackpaces(string_to_print.length()) << std::flush;
}

void RenderFrame()
{

}

void render_test()
{
    std::string print_sequence[4] = { "-", "\\", "|", "/" };
    while (true)
    {
        const int tick_time = 100;
        for (int i = 0; i < 1000; i++) {
            PrintWithTick(print_sequence[i % 4], tick_time);
        }
    }
}

std::vector<float> GetTorusXYZ(float theta, float phi, float radius_tube, float radius_torus)
{
    // radius_tube: Respresents the radius of the crosscut of the tube
    // radius_torus: Represents the radius from the center of the tube 
    // to the center of the torus.
    std::vector<float> coordinates(3);
    float x = (radius_torus + radius_tube * cos(theta)) * cos(phi);
    float y = (radius_torus + radius_tube * cos(theta)) * sin(phi);
    float z = radius_tube * sin(theta);
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
    return coordinates;
}


std::vector<std::vector<std::vector<float>>> GetCompleteTorus(const float radius_tube, const float radius_torus, const int phi_steps, const int theta_steps)
{
    const float phi_spacing = M_PI*2/phi_steps;
    const float theta_spacing = M_PI*2 / theta_steps;
    const int dimensions = 3;
    std::vector<std::vector<std::vector<float>>> complete_torus(phi_steps, std::vector<std::vector<float>>(theta_steps, std::vector<float>(dimensions)));
    for (int phi_n = 0; phi_n < phi_steps; phi_n++)
    {
        const float sin_phi = sin(phi_n * phi_spacing);
        const float cos_phi = cos(phi_n * phi_spacing);
        for (int theta_n = 0; theta_n < theta_steps; theta_n++)
        {
            const float sin_theta = sin(theta_n * theta_spacing);
            const float cos_theta = cos(theta_n * theta_spacing);

      }
    }
 
    
    return complete_torus;
}

std::vector<std::vector<float>> Torus2dProjection()
{
    // Takes in a 3D torus and projects it on a 2D surface (a 2D vector).
    const int row_count = 5;
    const int column_count = 3;

    std::vector<std::vector<float>> projection(row_count, std::vector<float>(column_count, 1));
    return projection;
}

int main()
{
    const float radius_tube = 1;
    const float radius_torus = 2;
    const float torus_distance = 5;
    const float screen_width = 100;
    const float torus_displacement = 3 / 8;
    const float viewing_distance = screen_width * torus_distance * torus_displacement * 1 / (radius_tube + radius_torus);

    std::string shading_chars = ".,-~:;=!*#$@";
    render_test();
    return 0;
}



// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
