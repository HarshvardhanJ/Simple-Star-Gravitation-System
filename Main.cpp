#include <iostream>
#include <SFML/Graphics.hpp>


class GravitySource {

	sf::Vector2f pos;
	float mass;
	sf::CircleShape source;


public:
	GravitySource(float xpos, float ypos, float mass)
	{

		pos.x = xpos;
		pos.y = ypos;
		this->mass = mass;

		source.setPosition(pos);
		source.setFillColor(sf::Color::Red);
		source.setRadius(25);


	}

	void render(sf::RenderWindow& window) 
	{
		window.draw(source);
	}

	sf::Vector2f get_pos() 
	{
		return pos;
	}

	float get_mass() 
	{
		return mass;
	}

};


class Particle {

	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::CircleShape particle;


public:
	Particle(float xpos, float ypos, float xvel, float yvel)
	{

		pos.x = xpos;
		pos.y = ypos;
		vel.x = xvel;
		vel.y = yvel;

		particle.setPosition(pos);
		particle.setFillColor(sf::Color::Yellow);
		particle.setRadius(4);

	}

	sf::Vector2f get_pos() 
	{
		return pos;
	}

	void render(sf::RenderWindow& window) 
	{
		particle.setPosition(pos);
		window.draw(particle);
	}

	void update_physics_fixed(GravitySource& source)
	{
		float xdistance = source.get_pos().x - pos.x;
		float ydistance = source.get_pos().y - pos.y;

		float tdistance = sqrt(xdistance * xdistance + ydistance * ydistance);

		float inverse_distance = 1.f / tdistance;

		float xnormalized = inverse_distance * xdistance;
		float ynormalized = inverse_distance * ydistance;

		float inverse_sqaure_drop = inverse_distance * inverse_distance;

		float xacceleration = source.get_mass() * xnormalized * inverse_sqaure_drop;
		float yacceleration = source.get_mass() * ynormalized * inverse_sqaure_drop;

		vel.x += xacceleration;
		vel.y += yacceleration;

		pos.x += vel.x;
		pos.y += vel.y;

	}

	void update_physics_og(GravitySource &source)
	{
		float G = 6.67 * pow(10, -5);
		
		float xdistance = source.get_pos().x - pos.x;
		float ydistance = source.get_pos().y - pos.y;

		float tdistance = sqrt(xdistance * xdistance + ydistance * ydistance);

		float acc = G * source.get_mass() / pow(tdistance, 2);
		float angle = atan(ydistance / xdistance);
		std::cout << "angle: " << angle << std::endl;
		float acc_x = acc * cos(angle);
		
		float acc_y = acc * sin(angle);
		

		vel.x += acc_x;
		std::cout << "vel_x: " << vel.x << " ";
		vel.y += acc_y;
		std::cout << "vel_y" << vel.y << std::endl;
		pos.x += vel.x;
		pos.y += vel.y;

	}

};




int main() {

	// Window setup
	sf::RenderWindow window(sf::VideoMode(1600, 1000), "Gravity Maybe");
	window.setFramerateLimit(60);

	GravitySource source(800, 500, 7000);

	Particle particle(600, 700, 4, 0);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) window.close();


		}

		sf::VertexArray line(sf::LinesStrip, 2);
		line[0].position = source.get_pos() + sf::Vector2f(25, 25);
		line[0].color = sf::Color::Magenta;
		line[1].position = particle.get_pos() + sf::Vector2f(4, 4);
		line[1].color = sf::Color::Cyan;

		window.clear();
		particle.update_physics_fixed(source);
		source.render(window);
		particle.render(window);
		window.draw(line);
		window.display();

	}

}