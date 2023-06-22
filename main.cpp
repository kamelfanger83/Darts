//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
//#include <GL/gl3w.h>

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <cassert>

#define all(x) x.begin(), x.end()

using namespace std;

// Vertex Shader source code
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

// Fragment Shader source code
const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 ourColor;

    void main()
    {
        FragColor = ourColor;
    }
)";

unsigned int shaderProgram, colorLocation;

// Function to compile and link shaders
bool compileShaders(unsigned int &shaderProgram) {
    // Create vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }

    // Create fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }

    // Create shader program and link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
        return false;
    }

    // Delete shaders as they're linked into the program and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Get color location
    colorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    return true;
}

const float l = 0.05, w = 0.01, c_s = 0.03;
const float angstep = 0.05, speed = 0.02;
const int ndarts = 100;

const int s = 640;
const int sub = 50;
const float sgr = 2.0f / sub;
const float r = 0.1, flee_r = 0.05;
const float other_coef = 0.1, cursor_coef = 0, flee_coef = 0.2, checkpoint_coef = 0.3;

const float alpha = 0.5;

pair<int, int> gcoords(float x, float y) {
    return {(x + 1) / 2 * sub, (y + 1) / 2 * sub};
}

struct Dart {
    float x, y, ang;
    pair<int, int> ingrid;
};

struct Checkpoint {
    float x, y, coef;
};

vector<Dart> darts;
list<int> grid[sub][sub];
vector<Checkpoint> checkpoints;
int positives = 0, negatives = 0;

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        // Left mouse button was clicked
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos = xpos / (s/2) - 1;
        ypos = 1 - ypos / (s/2);
        if (button == GLFW_MOUSE_BUTTON_LEFT){
            positives++;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            negatives++;
        }
        checkpoints.push_back({(float)xpos, (float)ypos, checkpoint_coef * (button == GLFW_MOUSE_BUTTON_LEFT ? 1.0f : -1.0f)});
    }
}

void drawDarts() {
    float *vertices = new float[darts.size() * 3 * 3];

    for (int i = 0; i < darts.size(); ++i) {
        auto dart = darts[i];
        vector<pair<float, float>> corners = {
                {dart.x,                                         dart.y},
                {dart.x + l * cos(dart.ang) - w * sin(dart.ang), dart.y + l * sin(dart.ang) + w * cos(dart.ang)},
                {dart.x + l * cos(dart.ang) + w * sin(dart.ang), dart.y + l * sin(dart.ang) - w * cos(dart.ang)},
        };
        for (int j = 0; j < 3; ++j) {
            vertices[i * 9 + j * 3 + 0] = corners[j].first;
            vertices[i * 9 + j * 3 + 1] = corners[j].second;
            vertices[i * 9 + j * 3 + 2] = 0;
        }
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, darts.size() * 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    delete[] vertices;

    // Specify the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform4f(colorLocation, 1.0, 0.5, 0.2, 1.0);
    glDrawArrays(GL_TRIANGLES, 0, darts.size() * 3);
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

#define CIRCLETRIANGLE 10

void drawCheckpoints() { // draw squares as checkpoints (green for positive coef, red for negative)
    float *greenvertices = new float[positives * 3 * 3 * CIRCLETRIANGLE];
    float *redvertices = new float[negatives * 3 * 3 * CIRCLETRIANGLE];

    int g = 0, r = 0;

    for (int i = 0; i < checkpoints.size(); ++i) {
        auto checkpoint = checkpoints[i];
        for (int triangle = 0; triangle < CIRCLETRIANGLE; triangle++){
            float angle = triangle * 2 * M_PI / CIRCLETRIANGLE;
            float nextangle = (triangle + 1) * 2 * M_PI / CIRCLETRIANGLE;
            vector<pair<float, float>> corners {
                    {checkpoint.x + c_s * cos(angle), checkpoint.y + c_s * sin(angle)},
                    {checkpoint.x + c_s * cos(nextangle), checkpoint.y + c_s * sin(nextangle)},
                    {checkpoint.x, checkpoint.y}
            };
            auto toedit = checkpoint.coef > 0 ? greenvertices : redvertices;
            for (int j = 0; j < 3; ++j) {
                toedit[(checkpoint.coef > 0 ? g : r) * 9 * CIRCLETRIANGLE + triangle * 9 + j * 3 + 0] = corners[j].first;
                toedit[(checkpoint.coef > 0 ? g : r) * 9 * CIRCLETRIANGLE + triangle * 9 + j * 3 + 1] = corners[j].second;
                toedit[(checkpoint.coef > 0 ? g : r) * 9 * CIRCLETRIANGLE + triangle * 9 + j * 3 + 2] = 0;
            }
        }
        if (checkpoint.coef > 0) {
            g++;
        } else {
            r++;
        }
    }

    unsigned int greenVAO, greenVBO, redVAO, redVBO;
    glGenVertexArrays(1, &greenVAO);
    glGenBuffers(1, &greenVBO);

    glBindVertexArray(greenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, greenVBO);
    glBufferData(GL_ARRAY_BUFFER, positives * 3 * 3 * CIRCLETRIANGLE * sizeof(float), greenvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    delete [] greenvertices;

    glGenVertexArrays(1, &redVAO);
    glGenBuffers(1, &redVBO);

    glBindVertexArray(redVAO);
    glBindBuffer(GL_ARRAY_BUFFER, redVBO);
    glBufferData(GL_ARRAY_BUFFER, negatives * 3 * 3 * CIRCLETRIANGLE * sizeof(float), redvertices, GL_STATIC_DRAW);

    // Set up the vertex attributes for the red checkpoints
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    delete [] redvertices;

    glBindVertexArray(greenVAO);
    glUniform4f(colorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, positives * 3 * CIRCLETRIANGLE);

    glBindVertexArray(redVAO); // Bind the red VAO
    glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, negatives * 3 * CIRCLETRIANGLE);

    // Cleanup
    glDeleteVertexArrays(1, &greenVAO);
    glDeleteBuffers(1, &greenVBO);
    glDeleteVertexArrays(1, &redVAO);
    glDeleteBuffers(1, &redVBO);
}

#define PI 3.1415926f

int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(s, s, "Darts", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    // Compile and link shaders
    if (!compileShaders(shaderProgram)) {
        glfwTerminate();
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.4f, 0.3f, 0.4f, 0.5f);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    for (int i = 0; i < ndarts; ++i) {
        darts.push_back({(float) rand() / RAND_MAX * 2 - 1, (float) rand() / RAND_MAX * 2 - 1,
                         (float) rand() / RAND_MAX * 2 * PI});
        darts.back().ingrid = gcoords(darts.back().x, darts.back().y);
        grid[darts.back().ingrid.first][darts.back().ingrid.second].push_back(darts.size() - 1);
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        xpos -= s / 2;
        xpos /= s / 2;
        ypos -= s / 2;
        ypos /= s / 2;
        ypos = -ypos;


        for (int i = 0; i < darts.size(); ++i) {
            pair<float, float> wantg{xpos - darts[i].x, ypos - darts[i].y};
            float l = sqrt(wantg.first * wantg.first + wantg.second * wantg.second);
            l = max(l, 0.001f);
            wantg.first *= cursor_coef / l;
            wantg.second *= cursor_coef / l;

            for (auto checkpoint : checkpoints){
                pair<float, float> vecto {checkpoint.x - darts[i].x, checkpoint.y - darts[i].y};
                float l = sqrt(vecto.first * vecto.first + vecto.second * vecto.second);
                l = max(l, 0.001f);
                wantg.first += vecto.first * checkpoint.coef / l / l;
                wantg.second += vecto.second * checkpoint.coef / l / l;
            }

            if (other_coef > 0 || flee_coef > 0) {
                for (int y = darts[i].ingrid.second - (r / sgr); y < darts[i].ingrid.second + 1 + (r / sgr); ++y) {
                    for (int x = darts[i].ingrid.first - (r / sgr); x < darts[i].ingrid.first + 1 + (r / sgr); ++x) {
                        if (x < 0 || y < 0 || x >= sub || y >= sub) continue;
                        int cx = x, cy = y;
                        if (abs((cx + 1) * sgr - 1 - darts[i].x) < abs(cx * sgr - 1 - darts[i].x)) cx = cx + 1;
                        if (abs((cy + 1) * sgr - 1 - darts[i].y) < abs(cy * sgr - 1 - darts[i].y)) cy = cy + 1;
                        float dx = cx * sgr - 1 - darts[i].x;
                        float dy = cy * sgr - 1 - darts[i].y;
                        float ds = dx * dx + dy * dy;
                        if (ds < r * r || (x == darts[i].ingrid.first && y == darts[i].ingrid.second)) {
                            for (int j: grid[x][y]) {
                                if (i == j) continue;
                                float dx = darts[j].x - darts[i].x;
                                float dy = darts[j].y - darts[i].y;
                                float ds = dx * dx + dy * dy;
                                if (ds < flee_r * flee_r && flee_coef > 0) {
                                    pair<float, float> veca{darts[i].x - darts[j].x, darts[i].y - darts[j].y};
                                    float ls = sqrt(veca.first * veca.first + veca.second * veca.second);
                                    ls = max(ls, 0.001f);
                                    wantg.first += veca.first * flee_coef / ls;
                                    wantg.second += veca.second * flee_coef / ls;
                                } else if (ds < r * r) {
                                    wantg.first += cos(darts[j].ang + PI) * other_coef;
                                    wantg.second += sin(darts[j].ang + PI) * other_coef;
                                }
                            }
                        }
                    }
                }
            }

            pair<float, float> posa{-sin(PI + darts[i].ang), cos(PI + darts[i].ang)};
            float dotp = wantg.first * posa.first + wantg.second * posa.second;
            if (dotp > 0) darts[i].ang += angstep;
            else darts[i].ang -= angstep;
            if (darts[i].ang > 2 * PI) darts[i].ang -= 2 * PI;
            if (darts[i].ang <= 0) darts[i].ang += 2 * PI;
            darts[i].x -= cos(darts[i].ang) * speed;
            darts[i].y -= sin(darts[i].ang) * speed;

            if (darts[i].x < -1) darts[i].x += 2;
            if (darts[i].y < -1) darts[i].y += 2;
            if (darts[i].x >= 1) darts[i].x -= 2;
            if (darts[i].y >= 1) darts[i].y -= 2;

            auto ncoords = gcoords(darts[i].x, darts[i].y);
            if (ncoords != darts[i].ingrid) {
                auto it = find(all(grid[darts[i].ingrid.first][darts[i].ingrid.second]), i);
                assert(it != grid[darts[i].ingrid.first][darts[i].ingrid.second].end());
                grid[darts[i].ingrid.first][darts[i].ingrid.second].erase(it);
                grid[ncoords.first][ncoords.second].push_back(i);
                darts[i].ingrid = ncoords;
            }
        }

        /* Render here */

        glUniform4f(colorLocation, 0.4f, 0.3f, 0.4f, alpha);
        glBegin(GL_QUADS);
        glVertex2f(-1, -1);
        glVertex2f(-1, 1);
        glVertex2f(1, 1);
        glVertex2f(1, -1);
        glEnd();

        drawDarts();
        if (checkpoints.size() > 0) drawCheckpoints();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
