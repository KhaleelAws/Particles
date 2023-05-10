//made by Venkata G. Ande and Aws Khaleel

#include "Particle.h"

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints)
{
    // Initialize member variables
    m_ttl = TTL; // The particle's time-to-live is set to a constant value
    m_numPoints = numPoints; // The number of points that make up the particle's shape
    m_radiansPerSec = ((float)rand() / RAND_MAX * PI); // The particle's rotation speed
    m_cartesianPlane.setCenter(0,0); // Set the center of the Cartesian plane
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y); // Set the size of the Cartesian plane
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane); // Map the mouse click position to Cartesian coordinates
    m_vx = (rand() % 401 + 100); // The particle's x velocity
    m_vy = (rand() % 401 + 100); // The particle's y velocity
    m_color1 = Color::White; // The first color used to draw the particle
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256); // The second color used to draw the particle
    // Generate random vertices for the particle's shape
    float temp = ((float)rand()) / RAND_MAX * PI / 2.0;
    float theta = temp;
    float dTheta = 2.0 * PI / (numPoints - 1);
    for (int j = 0; j < numPoints; j++) {
        float r = (rand() % 61 + 20); // The radius of the vertex
        float dx = r * cos(theta); // The x coordinate of the vertex
        float dy = r * sin(theta); // The y coordinate of the vertex
        theta += dTheta; // Increment the angle for the next vertex
        m_A(0, j) = m_centerCoordinate.x + dx; // Set the x coordinate of the vertex
        m_A(1, j) = m_centerCoordinate.y + dy; // Set the y coordinate of the vertex
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    // Create a VertexArray object that will be used to draw the particle
    VertexArray lines(TriangleFan, m_numPoints + 1);
    Vector2f pos;
    Vector2f center = Vector2f(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));
    // Set the first vertex of the VertexArray to be the center of the particle
    lines[0].position = center;
    lines[0].color = m_color1;
    // For each vertex of the particle (excluding the center), set its position and color in the VertexArray
    for (int j = 1; j <= m_numPoints; j++) {
        // Get the position of the j vertex of the particle
        pos.x = m_A(0,j-1);
        pos.y = m_A(1,j-1);
        // Convert the position from world coordinates to pixel coordinates
        Vector2f pixelPos = Vector2f(target.mapCoordsToPixel(pos, m_cartesianPlane));
        // Set the jth vertex of the VertexArray to be at the converted position and have the color m_color2
        lines[j].position = pixelPos;
        lines[j].color = m_color2;
    }
    // Draw the VertexArray onto the RenderTarget
    target.draw(lines);
}

void Particle::update(float dt)
{
    // Decrease the particle's time-to-live by the elapsed time
    m_ttl -= dt;
    // Rotate the particle based on the elapsed time and its rotation speed
    rotate(dt * m_radiansPerSec);
    // Scale the particle by a constant scale factor
    scale(SCALE);
    // Calculate the particle's horizontal and vertical displacement based on its velocity and elapsed time
    float dx;
    dx = m_vx * dt;
    float dy;
    m_vy -= G * dt; // Apply gravity to the vertical velocity
    dy = m_vy * dt;
    // Translate the particle by its calculated displacement
    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift)
{
    // Create a translation matrix with the given shifts.
    Matrix T = TranslationMatrix(xShift, yShift, m_A.getCols());

    // Apply the translation matrix to the particle's matrix.
    m_A = T + m_A;

    // Update the center coordinate of the particle.
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

void Particle::rotate(double theta)
{
    // Store the center coordinate in a temporary variable
    Vector2f temp = m_centerCoordinate;

    // Shift the particle to the origin before rotating it
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    // Construct a RotationMatrix R with the specified angle of rotation theta
    Matrix R = RotationMatrix(theta);

    // Multiply the rotation matrix by m_A
    m_A = R * m_A;

    // Shift the particle back to its original center
    translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
    // Store the value of m_centerCoordinate in a Vector2f temp
    Vector2f temp = m_centerCoordinate;
    
    // Call translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    // This will shift our particle's center, wherever it is, back to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    
    // Construct a ScalingMatrix S with the specified scaling multiplier c
    Matrix S = ScalingMatrix(c);
    
    // Multiply it by m_A as m_A = S * m_A
    m_A = S * m_A;
    
    // Shift our particle back to its original center
    translate(temp.x, temp.y);
}



bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = PI / 4.0;
    RotationMatrix r(PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}
