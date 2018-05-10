
#include <SFML/Graphics.hpp>
#include <time.h>
#include <cmath>
#include <iostream>

using namespace sf;

float euclidean_dist(int, int, int, int);
Vector2f velocity(Vector2f position);
Vector2f randomPosition(void);

const int W = 1200;
const int H = 800;
const int R = 1;
int kernel_radius = 5;
const int speed = 10.0;
const int draw_delay = 10;
int clearings = 1;
float mini_batch_size = 1;
Vector2f kernel( W/2.0, H/2.0);


float euclidean_dist(int x1, int x2, int y1, int y2)
{
    return (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

Vector2f velocity(Vector2f position)
{
    float a = position.x, b = position.y;
    float v_x = speed*(kernel.x-a)/euclidean_dist(a, b, kernel.x, kernel.y);
    float v_y = speed*(kernel.y-b)/euclidean_dist(a, b, kernel.x, kernel.y);
    return {v_x, v_y};
}

Vector2f randomPosition( void )
{
    Vector2f random_position;
    random_position.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360));
    random_position.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/W*2));
    Vector2f radial_random;
    radial_random.x = W/2 + random_position.y*cos(random_position.x);
    radial_random.y = H/2 + random_position.y*sin(random_position.x);
    return radial_random;
}
int main( void )
{
    srand(time(0));
    printf("Enter number of particles: ");
    long n;
    scanf("%ld", &n);
    RenderWindow app(VideoMode(W, H), "~Particles~");
    app.setFramerateLimit(20);
    clearings = n;
    mini_batch_size = n/120;
    kernel_radius = (100/(sqrt(n)+1)<5?5:100/(sqrt(n)+1));

    CircleShape kernel_circle;
    kernel_circle.setRadius(kernel_radius/10);
    kernel_circle.setFillColor(Color(255,255,255));
    kernel_circle.setPosition(kernel.x, kernel.y);

    std::vector <CircleShape> particles;

    for(int i = 0 ; i < n ; i++ )
    {
        CircleShape temp_particle;
        temp_particle.setRadius(R);
        temp_particle.setFillColor(Color(rand()%256,rand()%256,rand()%256));
        temp_particle.setPosition(randomPosition());
        particles.push_back(temp_particle);
    }

    while( app.isOpen() )
    {
        Event event;
        while( app.pollEvent(event) )
        {
            if( event.type == Event::Closed )
            app.close();
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                Vector2i click = (Mouse::getPosition(app));
                kernel.x = click.x;
                kernel.y = click.y;
                kernel_circle.setPosition(kernel.x, kernel.y);
            }
        }
        int near_count = 0;
        for(int i = 0 ; i < n ; i++ )
        {
            Vector2f curr_pos = particles[i].getPosition();
            float kernel_distance = euclidean_dist(curr_pos.x, kernel.x, curr_pos.y, kernel.y);
            if( kernel_distance < kernel_radius )
                particles[i].setPosition(randomPosition());
            else
            {
                Vector2f pos = curr_pos + velocity(curr_pos);
                particles[i].setPosition(pos);
            };
        }
        app.clear();
        app.draw(kernel_circle);
        if( n > 500 )for(int i = 0 ; i < mini_batch_size*clearings && i < n ; i++ ) app.draw(particles[i]);
        else for( int i = 0 ; i < n ; i++ ) app.draw(particles[i]);
        app.display();
        clearings++;
    }

    return 0;
}
