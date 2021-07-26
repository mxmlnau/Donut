#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <vector>
#include <numeric>
#include <chrono>

class Donut {
    public :
        const float radius_tube = 2.0;
        const float radius_torus = 3.0;
        const float torus_distance = 5.0;
        const float screen_width = 40.0;
        const float phi_steps = 80.0;
        const float theta_steps = 40.0;
        const float phi_spacing = M_PI * 2.0 / phi_steps;
        const float theta_spacing = M_PI * 2.0 / theta_steps;
        const int dimensions = 3;
        const float viewing_distance = screen_width * torus_distance * 3.0 / (8.0 * (radius_tube + radius_torus));
        std::vector<float> light_direction{ 1.0, 1.0, -1.0};
        std::vector<std::vector<std::vector<float>>> torus;
        std::vector<std::vector<float>> torus_center;
        std::vector<std::vector<float>> torus_luminance;
        const std::string shading_chars = ".,-~:;=!*#$@";
        float max_luminance = 0;


    std::vector<float> RotateOnY(std::vector<float> coordinates, float angle)
    {
        std::vector<float> rotated_coords(3);
        float sin_angle = sin(angle);
        float cos_angle = cos(angle);
        float x = coordinates[0];
        float y = coordinates[1];
        float z = coordinates[2];
        rotated_coords[0] = x * cos_angle + z * sin_angle;
        rotated_coords[1] = y;
        rotated_coords[2] = x * (-sin_angle) + z * cos_angle;
        return rotated_coords;
    }


    std::vector<float> RotateOnX(std::vector<float> coordinates, float angle)
    {
        std::vector<float> rotated_coords(3);
        float sin_angle = sin(angle);
        float cos_angle = cos(angle);
        float x = coordinates[0];
        float y = coordinates[1];
        float z = coordinates[2];
        rotated_coords[0] = x;
        rotated_coords[1] = y * cos_angle + z * (-sin_angle);
        rotated_coords[2] = y * sin_angle + z * cos_angle;
        return rotated_coords;
    }


    std::vector<float> RotateOnXandY(std::vector<float> coordinates, float angle)
    {
        std::vector<float> x_rotation = RotateOnX(coordinates, angle);
        std::vector<float> rotated_coords = RotateOnY(coordinates, angle + M_PI / 4);
        return rotated_coords;
    }

    void CreateTorus()
    {
        torus.clear();
        torus_luminance.clear();
        max_luminance = 0;
        float x, y, z, x_middle, y_middle, z_middle;



        for (int phi_n = 0; phi_n < phi_steps; phi_n++)
        {
            std::vector<std::vector<float>> theta_vector;
            std::vector<float> theta_luminance_vector;


            const float sin_phi = sin(phi_n * phi_spacing);
            const float cos_phi = cos(phi_n * phi_spacing);
            float luminance;

            x_middle = radius_torus * cos_phi;
            y_middle = radius_torus * sin_phi;
            z_middle = 0.0;
            torus_center.push_back(std::vector<float>{x_middle, y_middle, z_middle});



            for (int theta_n = 0; theta_n < theta_steps; theta_n++)
            {
                const float sin_theta = sin(theta_n * theta_spacing);
                const float cos_theta = cos(theta_n * theta_spacing);
                x = cos_phi * (radius_tube * cos_theta + radius_torus);
                y = sin_phi * (radius_tube * cos_theta + radius_torus);
                z = radius_tube * sin_theta;



                std::vector<float> coordinates = {x, y, z};
                std::vector<float> middle_coordinates = {x_middle, y_middle, z_middle};

                theta_vector.push_back(coordinates);


                std::vector<float> normals;
                for (int i = 0; i < coordinates.size(); i++)
                {
                    normals.push_back(coordinates[i] - middle_coordinates[i]);
                }

                luminance = normals[0] * light_direction[0] + normals[1] * light_direction[1] + normals[2] * light_direction[2];
                theta_luminance_vector.push_back(luminance);
                if (luminance > max_luminance) {
                    max_luminance = luminance;
                }


            }
            torus.push_back(theta_vector);
            torus_luminance.push_back(theta_luminance_vector);
        }
    }

    void UpdateTorus(const float spin_angle)
    {

        float sin_spin_angle = sin(spin_angle);
        float x, y, z, x_middle, y_middle, z_middle;
        double luminance;
        std::vector<float> coordinates, coordinates_middle;

        for (int phi_n = 0; phi_n < phi_steps; phi_n++)
        {
            x_middle = torus_center[phi_n][0];
            y_middle = torus_center[phi_n][1];
            z_middle = torus_center[phi_n][2];
            
            coordinates_middle = RotateOnX(std::vector<float>{x_middle, y_middle, z_middle}, spin_angle);
            torus_center[phi_n] = coordinates_middle;



            for (int theta_n = 0; theta_n < theta_steps; theta_n++)
            {
                x = torus[phi_n][theta_n][0];
                y = torus[phi_n][theta_n][1];
                z = torus[phi_n][theta_n][2];

             /*   coordinates = RotateOnX(RotateOnXandY(std::vector<float>{x, y, z}, spin_angle), spin_angle);
                coordinates_middle = RotateOnX(RotateOnXandY(std::vector<float>{x_middle, y_middle, z_middle}, spin_angle), spin_angle);*/
                coordinates = RotateOnX(std::vector<float>{x, y, z}, spin_angle);
                
                torus[phi_n][theta_n] = coordinates;


                std::vector<double> normals;
                for (int i = 0; i < coordinates.size(); i++)
                {
                    normals.push_back(coordinates[i] - coordinates_middle[i]);
                }

                luminance = normals[0] * light_direction[0] + normals[1] * light_direction[1] + normals[2] * light_direction[2];
                torus_luminance[phi_n][theta_n] = luminance;
                if (luminance > max_luminance) {
                    max_luminance = luminance;
                }
            }
        }
    }

    void UpdateTorusOld(const float spin_angle)
    {
        torus.clear();
        torus_luminance.clear();
        max_luminance = 0;
        std::vector<std::vector<float>> torus_normal_ring;
        float x, y, z, x_middle, y_middle, z_middle;
        float sin_spin_angle = sin(spin_angle);



        for (int phi_n = 0; phi_n < phi_steps; phi_n++)
        {
            std::vector<std::vector<float>> theta_vector;
            std::vector<float> theta_luminance_vector;


            const float sin_phi = sin(phi_n * phi_spacing);
            const float cos_phi = cos(phi_n * phi_spacing);
            float luminance;

            x_middle = radius_torus * cos_phi;
            y_middle = radius_torus * sin_phi;
            z_middle = 0.0;
            //torus_normal_ring.push_back(RotateOnXandY(x_middle, y_middle, z_middle, spin_angle));



            for (int theta_n = 0; theta_n < theta_steps; theta_n++)
            {
                const float sin_theta = sin(theta_n * theta_spacing);
                const float cos_theta = cos(theta_n * theta_spacing);
                x = cos_phi * (radius_tube * cos_theta + radius_torus);
                y = sin_phi * (radius_tube * cos_theta + radius_torus);
                z = radius_tube * sin_theta;

                std::vector<float> coordinates = RotateOnX(RotateOnXandY(std::vector<float>{x, y, z}, spin_angle), spin_angle);
                std::vector<float> middle_coordinates = RotateOnX(RotateOnXandY(std::vector<float>{x_middle, y_middle, z_middle}, spin_angle), spin_angle);

                //std::vector<float> coordinates = RotateOnX(x, y, z, spin_angle);
                //std::vector<float> middle_coordinates = RotateOnX(x_middle, y_middle, z_middle, spin_angle);

                theta_vector.push_back(coordinates);


                std::vector<float> normals;
                for (int i = 0; i < coordinates.size(); i++)
                {
                    normals.push_back(coordinates[i] - middle_coordinates[i]);
                }

                luminance = normals[0]*light_direction[0] + normals[1]*light_direction[1] + normals[2]*light_direction[2];
                theta_luminance_vector.push_back(luminance);
                if (luminance > max_luminance) {
                    max_luminance = luminance;
                }


            }
            torus.push_back(theta_vector);
            torus_luminance.push_back(theta_luminance_vector);
        }
    }


    

    bool IsWithinProjectionFrame(const int x, const int y)
    {
        const int screen_width_half = int(screen_width / 2.0);
        return x > -screen_width_half && x < screen_width_half&& y > -screen_width_half && y < screen_width_half;
    }


    std::vector<std::vector<float>> GetProjection()
    {
        std::vector<std::vector<float>> projection(screen_width, std::vector<float>(screen_width, 0));
        // Takes the 3D torus and projects it on a flat 2D surface.
        const int row_count = 5;
        const int column_count = 3;
        double x, y, z, z_viewer;
        float x_proj, y_proj;
        std::vector<std::vector<float>> z_buffer(screen_width, std::vector<float>(screen_width, 0));

        for (int phi_n = 0; phi_n < torus.size(); phi_n++)
        {
            for (int theta_n = 0; theta_n < torus[0].size(); theta_n++)
            {
                x = torus[phi_n][theta_n][0];
                y = torus[phi_n][theta_n][1];
                z = torus[phi_n][theta_n][2];
                z_viewer = z + viewing_distance;
                x_proj = viewing_distance * x / torus_distance;
                y_proj = viewing_distance * y / torus_distance;
                if (IsWithinProjectionFrame(x_proj, y_proj))
                {
                    int row = x_proj + screen_width / 2;
                    int col = y_proj + screen_width / 2;
                    float luminance = (torus_luminance[phi_n][theta_n] / max_luminance);

                    //double distance = x * x + y * y + z_viewer * z_viewer;
                    double distance = z_viewer;


                    if (1 / distance > z_buffer[row][col])
                    {
                        projection[row][col] = luminance;
                        //projection[row][col] = std::fabsf(luminance);
                        z_buffer[row][col] = 1 / distance;
                    }
                }
            }
        }
        return projection;
    }


    void RenderFrame()
    {
        std::vector<std::vector<float>> projection = GetProjection();
        std::string string_render = "";
        for (int row = 0; row < projection.size(); row++)
        {
            for (int col = 0; col < projection[0].size(); col++)
            {
                if (projection[row][col] > 0)
                {
                    string_render += '*';
                }
                else
                {
                    string_render += ' ';
                }
                string_render += ' ';
            }
            string_render += '\n';
        }
        std::cout << "\033[2J\033[1;1H" << std::flush;
        std::cout << string_render << std::flush;
    }


    void RenderFrameShading()
    {
        std::vector<std::vector<float>> projection = GetProjection();
        std::string string_render = "";
        for (int row = 0; row < projection.size(); row++)
        {
            for (int col = 0; col < projection[0].size(); col++)
            {
                if (projection[row][col] > 0)
                {
                    string_render += shading_chars[int(projection[row][col]*12)];
                }
                else
                {
                    string_render += ' ';
                }
                string_render += ' ';
            }
            string_render += '\n';
        }
        ClearScreen();
        std::cout << string_render << std::flush;
    }

    void ClearScreen()
    {
        std::cout << "\033[2J\033[1;1H" << std::flush;
    }


    

    void RenderTorus()
    {
        const float spin_angle_steps = 150.0;
        const float spin_angle = M_PI / spin_angle_steps;

        CreateTorus();

        while (true)
        {
            auto torus_time = std::chrono::steady_clock::now();
            UpdateTorus(spin_angle);
            auto projection_time = std::chrono::steady_clock::now();
            GetProjection();
            auto render_time = std::chrono::steady_clock::now();
            RenderFrameShading();
            auto end_time = std::chrono::steady_clock::now();
            //spin_angle += spin_angle_step;
            std::cout
                << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - torus_time).count()
                << " "
                << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - projection_time).count()
                << " "
                << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - render_time).count()
                << std::flush;



        }
    }

};


int main()
{
while (true)
{
    Donut donut;
    donut.RenderTorus();
}

return 0;
}
