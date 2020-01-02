#include "common.h"
#include "cmath"
#include "OpenSimplexNoise.h"

bool Init();
void CleanUp();
void Run();
void Noise();
void Biomes();
double ScaleNum(double n, double minN, double maxN, double min, double max);

SDL_Window *window;
SDL_GLContext glContext;
SDL_Surface *gScreenSurface = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Rect pos;

int screenWidth = 500;
int screenHeight = 500;
int xpos = 0;
int ypos = 0;
double resolution = 1;
double featureSize = 100;
OpenSimplexNoise *noise1 = nullptr;
OpenSimplexNoise *noise2 = nullptr;

vector<vector<int>> mountainColor;
vector<vector<int>> forestColor;
vector<vector<int>> beachColor;
vector<vector<int>> oceanColor;
vector<vector<int>> pColor;

bool Init()
{
    if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,   
        SDL_WINDOW_OPENGL);

    //Check that the window was succesfully created
    if (window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return false;
    }
    else{
        gScreenSurface = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Log("Window Successful Generated");
    }
    //Map OpenGL Context to Window
    glContext = SDL_GL_CreateContext(window);

    return true;
}

int main()
{
    //Error Checking/Initialisation
    if (!Init())
    {
        printf("Failed to Initialize");
        return -1;
    }

    // Clear buffer with black background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Run();

    CleanUp();
    return 0;
}

void CleanUp()
{
    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Run()
{
    bool gameLoop = true;
    srand(time(NULL));
    long rand1 = rand() * (RAND_MAX + 1) + rand();
    long rand2 = rand() * (RAND_MAX + 1) + rand();
    noise1 = new OpenSimplexNoise{rand1};
    noise2 = new OpenSimplexNoise{rand2};
    Biomes();
    while (gameLoop)
    {   
        Noise();
        SDL_RenderPresent(renderer);
        SDL_Event event;
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameLoop = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_q:
                        featureSize++;
                        break;
                    case SDLK_e:
                        featureSize--;
                        break;
                    case SDLK_w:
                        ypos-=5;
                        break;
                    case SDLK_a:
                        xpos-=5;
                        break;
                    case SDLK_s:
                        ypos+=5;
                        break;
                    case SDLK_d:
                        xpos+=5;
                        break;
                    case SDLK_SPACE:
                        rand1 = rand() * (RAND_MAX + 1) + rand();
                        rand2 = rand() * (RAND_MAX + 1) + rand();
                        noise1 = new OpenSimplexNoise{rand1};
                        noise2 = new OpenSimplexNoise{rand2};
                    case SDLK_ESCAPE:
                        gameLoop = false;
                        break;
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                    default:
                        break;
                }
            }
        }
    }
}

void Noise(){
    for(int x = xpos; x < screenWidth / resolution + xpos; x++){
        for(int y = ypos; y < screenHeight / resolution + ypos; y++){
            double value = (*noise1).eval(x/featureSize, y/featureSize);
            double variation = (*noise2).eval(x/(featureSize/10), y/(featureSize/10));
            value = ScaleNum(value, -1, 1, 0, 1);
            variation = ScaleNum(variation, -1, 1, -.1, .1);
            value = value + variation;

            if(variation > 1)
                variation = 1;
            else if(variation < 0)
                variation = 0;
            int color;
            if(value <= .1){
                color = ScaleNum(value, 0, .1, 0, mountainColor.size() - 1);
                pos.x = x - xpos;
                pos.y = y - ypos;
                pos.w = resolution;
                pos.h = resolution;
                SDL_SetRenderDrawColor(renderer, mountainColor[color][0], mountainColor[color][1], mountainColor[color][2], 255);
                SDL_RenderFillRect(renderer, &pos);
            }
            else if(value <= .3){
                color = ScaleNum(value, .1, .3, 0, forestColor.size() - 1);
                pos.x = x - xpos;
                pos.y = y - ypos;
                pos.w = resolution;
                pos.h = resolution;
                SDL_SetRenderDrawColor(renderer, forestColor[color][0], forestColor[color][1], forestColor[color][2], 255);
                SDL_RenderFillRect(renderer, &pos);
            }
            else if(value <= .35){
                color = ScaleNum(value, .3, .35, 0, beachColor.size() - 1);
                pos.x = x - xpos;
                pos.y = y - ypos;
                pos.w = resolution;
                pos.h = resolution;
                SDL_SetRenderDrawColor(renderer, beachColor[color][0], beachColor[color][1], beachColor[color][2], 255);
                SDL_RenderFillRect(renderer, &pos);
            }
            else if(value <= 1.0){
                color = ScaleNum(value, .35, 1, 0, oceanColor.size() - 1);
                pos.x = x - xpos;
                pos.y = y - ypos;
                pos.w = resolution;
                pos.h = resolution;
                SDL_SetRenderDrawColor(renderer, oceanColor[color][0], oceanColor[color][1], oceanColor[color][2], 255);
                SDL_RenderFillRect(renderer, &pos);
            }
        }
    }
}

void Biomes(){
    for(int i = 255; i > 204; i--){
        mountainColor.push_back({i,i,i});
    }
    for(int i = 10; i < 61; i++){
        forestColor.push_back({i,i + 145,i - 10});
    }
    for(int i = 205; i < 256; i++){
        beachColor.push_back({i+205,i+184,i-205});
    }
    for(int i = 100; i > 35; i--){
        oceanColor.push_back({i,i+57,i+155});
    }
}

double ScaleNum(double n, double minN, double maxN, double min, double max){
    return (((n - minN) / (maxN - minN)) * (max - min)) + min;
}