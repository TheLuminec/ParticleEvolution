#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "SystemConstraints.h"
#include "Force.h"
#include "Particle.h"

class StaticParticle : public Particle {
private:
    sf::Vector2f staticPos;
public:
    StaticParticle(float posX, float posY, float radius, float mass, std::vector<float> &forceSensitivity)
        : Particle{posX, posY, radius, mass, forceSensitivity } 
    {
        setForceSensitivity(0, 10.f);
		//setForceSensitivity(2, -1.f);
        //setCollisonEnabled(false);

        staticPos.x = posX;
		staticPos.y = posY;
    }

    void positonUpdate() {
        setPos(staticPos);
    }
};
class UnStickingForce : public Force {
public:
    UnStickingForce(uint8_t id, float forceMultiplier) :
        Force(id, forceMultiplier, 1.0f) {}

    void applyForces(PhysicsModel* m1, PhysicsModel* m2) override {
        if (&m1 != &m2)
        {
            sf::Vector2f distXY = m1->getPos() - m2->getPos();
            float dist = std::hypot(distXY.x, distXY.y);

            if ((m1->getMinDist() + m2->getMinDist()) * 1.3f < dist)
                return;

            float inverseDist = 1.0f / clip(dist, m1->getMinDist() + m2->getMinDist(), dist);
            distXY *= inverseDist;
            float inverseSquareDist = inverseDist * inverseDist;

            float force = getForceMultiplier() * m1->getMass() * m2->getMass() * inverseSquareDist;
            sf::Vector2f forceXY = sf::Vector2f(distXY.x * force, distXY.y * force);

            m1->changeForce(forceXY);
            m2->changeForce(-forceXY);

        }
    }
};

class UniformForce : public Force {
private:
    sf::Vector2f dir;
public:
    UniformForce(uint8_t id, float forceMultiplier, sf::Vector2f dir) :
        Force(id, forceMultiplier), dir(dir) {}

    ~UniformForce() {
        delete &dir;
    }

    void applyForces(PhysicsModel* m1, PhysicsModel* o) {
        float force = getForceMultiplier() * m1->getForceSensitivity(getId()) * m1->getMass();
		sf::Vector2f forceXY = dir * force;

        m1->changeForce(forceXY);
    }
};

struct SelectiveForce {//For forces and particles that only react to base particles
    Force* force;
    std::vector<PhysicsModel*> objects;
};

std::vector<Particle*> Repopulate(std::vector<Particle*> living, int population) {
	std::vector <Particle*> newParticles;

    for (int i = 0; i < population; i++) {
        newParticles.push_back(living[std::rand() % living.size()]->createChild());
    }

	return newParticles;
}

std::vector<Particle*> ConditonSelectAround(std::vector<Particle*> *particles, std::vector<PhysicsModel*> *selectors, float minDist) {
	std::vector<Particle*> inArea;

    for (PhysicsModel* a : *selectors) {
        for (Particle* p : *particles) {
            sf::Vector2f distXY = p->getPos() - a->getPos();
            float dist = std::hypot(distXY.x, distXY.y);

            if (dist <= minDist) {
				inArea.push_back(p);
            }

        }
    }

    return inArea;
}

std::vector<Particle*> ConditonSelectLeft(std::vector<Particle*>* particles, float dist) {
    std::vector<Particle*> inArea;

    for (Particle* p : *particles) {
        if (p->getPos().x <= dist) {
            inArea.push_back(p);
        }

    }

    return inArea;
}

int main()
{
    std::srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Particle Evolution");
    window.setFramerateLimit(60);

    UnStickingForce unStickingForce(-1, 9.0f);
    
    std::vector<SelectiveForce> selectiveForces = {
        SelectiveForce{//Static Object
            new Force(2, -5.0f),
            std::vector<PhysicsModel*>()
        },
        SelectiveForce{//Uniform y force
            new UniformForce(3, 0.1f, sf::Vector2f(0.0f, 1.0f)),
			std::vector<PhysicsModel*>()
        },
        SelectiveForce{//Uniform x force
            new UniformForce(4, 0.1f, sf::Vector2f(1.0f, 0.0f)),
			std::vector<PhysicsModel*>()
        }
    };

    std::vector<Force*> forces = { //universal forces
        &unStickingForce,
        new Force(0, -3.0f),//gravity
		new Force(1, -5.0f),//Charge
        //new Force(2, -5.0f, 20),//Strong Nuclear
    };

    std::vector<float> forceMap = {
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };

    std::vector<Particle*> particles;

    int particleNum = 100;

    for (int i = 0; i < particleNum; i++)
    {
        particles.push_back(new Particle(std::rand() % WIDTH, std::rand() % HEIGHT, 5, 30, forceMap));
    }

    for (int i = 0; i < 5; i++)
    {
        selectiveForces[0].objects.push_back(new StaticParticle(std::rand() % WIDTH, std::rand() % HEIGHT, 15, 100, forceMap));
    }


    while (window.isOpen())
    {
        for (int g = 0; g < 120; g++) {

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) window.close();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
            }
            window.clear();

            for (int n = 0; n < 20; n++) {//Calculation Loop

                for (int i = 0; i < particles.size(); i++)
                {
                    for (int j = i + 1; j < particles.size(); j++)
                    {
                        for (Force* f : forces)
                        {
                            f->applyForces(particles[i], particles[j]);
                        }
                    }

                    for (SelectiveForce sf : selectiveForces)
                    {
                        for (PhysicsModel* o : sf.objects)
                        {
                            sf.force->applyForces(particles[i], o);
                        }
                    }
                }

                for (Particle* p : particles)
                {
                    p->checkCollison(particles);
                    p->forward();

                    p->positonUpdate();

                }

            }

            for (Particle* p : particles)
            {
                p->render(window);
            }

            for (SelectiveForce sf : selectiveForces)
            {
                for (PhysicsModel* o : sf.objects)
                {
                    o->render(window);
                }
            }

            window.display();
        }
        //std::vector<Particle*> living = ConditonSelectAround(&particles, &selectiveForces[0].objects, 50);
        std::vector<Particle*> living = ConditonSelectLeft(&particles, WIDTH * 0.2f);
        if (living.size() > 0)
            particles = Repopulate(living, particleNum);
        else
            particles = Repopulate(particles, particleNum);
        std::cout << "New Generation: " << generationNumber << std::endl;
        generationNumber++;
    }

    return 0;
}