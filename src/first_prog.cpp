// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <SDL2/SDL_image.h>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1920-1000;
const int SCREEN_HEIGHT = 1080-500;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 10000;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos);

// Frees media and shuts down SDL
void close(SDL_Window** window);

Point camera_position(5, 5, 5);

double cam_yaw = 1.5;
double cam_pitch = 0.0;
double cam_dist = 10.0;
const double step = 0.2;
const double zoom_step = 0.5;
const double rotate_step=0.1;

/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow( "TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    std::cerr << "Unable to initialize OpenGL!"  << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}


bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 100.0);


    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);


    // Lighting basic configuration and activation
    const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cerr << "Error initializing OpenGL!  " << gluErrorString( error ) << std::endl;
        success = false;
    }

    return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t)
{
    // Update the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        formlist[i]->update(delta_t);
        i++;
    }
}

void render(Form *formlist[MAX_FORMS_NUMBER])
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera position and parameters
    // Set the camera position and parameters
    double cam_x = camera_position.x + cam_dist * cos(cam_pitch) * cos(cam_yaw);
    double cam_y = camera_position.y + cam_dist * sin(cam_pitch);
    double cam_z = camera_position.z + cam_dist * cos(cam_pitch) * sin(cam_yaw);
    gluLookAt(cam_x, cam_y, cam_z, camera_position.x, camera_position.y, camera_position.z,0.0,1.0,0.0);
    // Isometric view
    glRotated(-45, 0, 1, 0);
    glRotated(30, 1, 0, -1);

    // X, Y and Z axis
    glPushMatrix(); // Preserve the camera viewing point for further forms
    // Render the coordinates system
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(1, 0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 1, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 1);
    }
    glEnd();
    glPopMatrix(); // Restore the camera viewing point for next object

    // Render the list of forms
    unsigned short i = 0;
    while (formlist[i] != NULL)
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int createTextureFromImage(const char *filename, GLuint *textureID)
{
    SDL_Surface *imgSurface = IMG_Load(filename);
    if (imgSurface == NULL)
    {
        std::cerr << "Failed to load texture image: " << filename << std::endl;
        return -1;
    }
    else
    {
        // Work out what format to tell glTexImage2D to use...
        int mode;
        if (imgSurface->format->BytesPerPixel == 3) // RGB 24bit
        {
            mode = GL_RGB;
        }
        else if (imgSurface->format->BytesPerPixel == 4) // RGBA 32bit
        {
            mode = GL_RGBA;
        }
        else
        {
            SDL_FreeSurface(imgSurface);
            std::cerr << "Unable to detect the image color format of: " << filename << std::endl;
            return -2;
        }
        // create one texture name
        glGenTextures(1, textureID);

        // tell opengl to use the generated texture name
        glBindTexture(GL_TEXTURE_2D, *textureID);

        // this reads from the sdl imgSurface and puts it into an openGL texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, imgSurface->w, imgSurface->h, 0, mode, GL_UNSIGNED_BYTE, imgSurface->pixels);

        // these affect how this texture is drawn later on...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // clean up
        SDL_FreeSurface(imgSurface);
        return 0;
    }
}

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;


    // Start up SDL and create window
    if( !init(&gWindow, &gContext))
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time, elapsed_time;
        Uint32 previous_time2, elapsed_time2;

        // Event handler
        SDL_Event event;

        // Camera position
        Point cam_direction(0.0, 1.0, 0.0);

        // Set the camera position and parameters
        double cam_x = camera_position.x + cam_dist * cos(cam_pitch) * cos(cam_yaw);
        double cam_y = camera_position.y + cam_dist * sin(cam_pitch);
        double cam_z = camera_position.z + cam_dist * cos(cam_pitch) * sin(cam_yaw);
        gluLookAt(cam_x, cam_y, cam_z, camera_position.x, camera_position.y, camera_position.z, 0.0, 5.0, 0.0);

        // Textures creation //////////////////////////////////////////////////////////
        GLuint textureid_1, textureid_2, textureid_3;
        createTextureFromImage("resources/images/domino2.jpg", &textureid_1);
        createTextureFromImage("resources/images/sol.jpg", &textureid_2);
        createTextureFromImage("resources/images/professeur.jpg", &textureid_3);
        // Textures ready to be enabled (with private member " texture_id" of each form)

        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }

        int createTextureFromImage(const char *filename, GLuint *textureid_1);

        // Ground
        Cube_face *pGround = new Cube_face(Vector(-1,0,0), Vector(0,0,1), Point(-4, 0, 0), -10, -100, WHITE);
        pGround->setTexture(textureid_2);
        forms_list[number_of_forms] = pGround;

        // Cube face for background
        Cube_face *pBackground = NULL;
        pBackground = new Cube_face(Vector(0,0,1), Vector(0,1,0), Point(-5, 0, -10), -10, 10, WHITE);
        pBackground->setTexture(textureid_3);
        forms_list[++number_of_forms] = pBackground;

        std::vector<Domino> allDominoes;


        for (int i = 0; i < 30; i++)
        {
            Domino *pDomino = new Domino(Vector(1,0,0), Vector(0,1,0), Vector(0,0,1), Point(1, 5, 10), 1, 2, 0.4, WHITE);
            pDomino->setTexture(textureid_1);
            pDomino->setPosition(Point(1, 0, -1.5 * (i + 1)));
            pDomino->setVelocity(Vector(0, 0, 0));
            pDomino->setAngularVelocity(Vector(0, 0, 0));
            forms_list[++number_of_forms] = pDomino;
            // allDominoes.push_back(ppDomino);
            // pDomino->setAllDominoes(allDominoes);
        }

        //extra Domino
        // Domino *pDomino = new Domino(Vector(1,0,0), Vector(0,1,0), Vector(0,0,1), Point(1, 5, 10), 1, 2, 0.4, PURPLE);
        // pDomino->setPosition(Point(1, 0, -1.5 * 10));
        // forms_list[++number_of_forms] = pDomino;
        // allDominoes.push_back(*pDomino);

        // pDomino->setAngularVelocity(Vector(-50, 0, 0));



        // pDomino->setAllDominoes(allDominoes);


        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            if (forms_list[i] != NULL)
            {
                number_of_forms++;
            }
        }

        // Timer for animation
        Uint32 nextDominoTime = 300; // 5 seconds
        int nextDominoIndex = 2;

        // Get first "current time"
        previous_time = SDL_GetTicks();
        // While application is running
        while(!quit)
        {
            // Handle events on queue
            while(SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch(event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // Handle key pressed with current mouse position
                    SDL_GetMouseState( &x, &y );

                    switch(key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_d:
                        camera_position.x += step * sin(cam_yaw);
                        camera_position.z -= step * cos(cam_yaw);
                        break;
                    case SDLK_q:
                        camera_position.x -= step * sin(cam_yaw);
                        camera_position.z += step * cos(cam_yaw);
                        break;
                    case SDLK_z:
                        camera_position.x -= step * cos(cam_yaw);
                        camera_position.z -= step * sin(cam_yaw);
                        break;
                    case SDLK_s:
                        camera_position.x += step * cos(cam_yaw);
                        camera_position.z += step * sin(cam_yaw);
                        break;
                    case SDLK_SPACE:
                        camera_position.y += step;
                        break;
                    case SDLK_LSHIFT:
                        camera_position.y -= step;
                        break;

                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time = current_time - previous_time;
            elapsed_time2 = current_time - previous_time2;
            if (elapsed_time > ANIM_DELAY)
            {
                if (elapsed_time2 > nextDominoTime && nextDominoIndex < 32)
                {
                    Domino *pDomino = dynamic_cast<Domino*>(forms_list[nextDominoIndex]);
                    pDomino->setAngularVelocity(Vector(-90, 0, 0));
                    nextDominoIndex++;

                    previous_time2 = current_time;
                }
                previous_time = current_time;
                update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
                

            }

            // Render the scene
            render(forms_list);

            // Update window screen
            SDL_GL_SwapWindow(gWindow);
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
