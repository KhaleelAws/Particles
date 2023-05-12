//made by Venkata G. Ande and Aws Khaleel

#include "Engine.h"

Engine::Engine()
{
    m_Window.create(VideoMode::getDesktopMode(), "Particles");
}

void Engine::run()
{
    // Timing
    Clock clock;

    // Construct a local Particle to be used for the unit tests
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete. Starting engine..." << endl;

    while(m_Window.isOpen())
    {
        // Restart the clock and save elapsed time
        Time dt = clock.restart();

        // Make a fraction from the delta time
        float dtAsSeconds = dt.asSeconds();

        // Call each private function in turn
        input();
        update(dtAsSeconds);
        draw();
    }
}

void Engine::input()
{
    // Declare an Event object
    Event event;
    // Polls the window events and loops until there are no more events
    while(m_Window.pollEvent(event))
    {
        // Check if the event is a window close event or if the escape key is pressed
        if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
        {
            // Closes the window
            m_Window.close();
        }
        // If the event is a left mouse button press event
        else if(event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            // Get the mouse click position and create 5 particles with random number of points between 25 and 50
            for(int i = 0; i < 5; i++)
            {
                int numPoints = rand() % 26 + 25;
                Particle p(m_Window, numPoints, Mouse::getPosition());
                m_particles.push_back(p);
            }
        }
    }
    
}

void Engine::update(float dtAsSeconds)
{
    // Update particles
    for (auto it = m_particles.begin(); it != m_particles.end(); ) // using iterator-based for-loop
    {
        if (it->getTTL() > 0.0) // check if particle's TTL is still valid
        {
            it->update(dtAsSeconds); // call update on that particle
            it++; // increment the iterator
        }
        else
        {
            // erase the element that the iterator points to, and get the next element
            it = m_particles.erase(it);
        }
    }
}

void Engine::draw()
{
    // Clear everything from the last frame
    m_Window.clear();

    // Draw particles
    for (long unsigned int i = 0; i < m_particles.size(); i++)
    {
        m_Window.draw(m_particles[i]);
    }

    // Show everything we just drew
    m_Window.display();
}
