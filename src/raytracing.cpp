#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

#define WIDTH 1200
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_YELLOW 0xFFFFFF00
#define COLOR_RED 0xFFFF0000
#define COLOR_RAY 0XFFD43B
#define RAYS_NUMBER 500
#define RAY_THICKNESS 1

typedef struct Circle_t{
    double x;
    double y;
    double r;
}Circle;

typedef struct Ray_t{
    double x;
    double y;
    double angle;
}Ray;

void FillCircle(SDL_Surface *surface, Circle *circle, Uint32 color){
    double radius_squared = circle->r * circle->r;
    for(double x = circle->x - circle->r; x < circle->x + circle->r; x++){
        for(double y = circle->y - circle->r; y < circle->y + circle->r; y++){
            double distance_squared = (x - circle->x) * (x - circle->x) + (y - circle->y) * (y - circle->y);
            if(distance_squared <= radius_squared){
                SDL_Rect rect = {(int)x, (int)y, 1, 1};
                SDL_FillRect(surface, &rect, color);
            }
        }
    }
}

vector<Ray> generate_circle_rays(Circle *circle, int ray_count){
    vector<Ray> rays;
    for(int i = 0; i < ray_count; i++){
        double angle = 2 * M_PI * i / ray_count;
        double x = circle->x + circle->r * cos(angle);
        double y = circle->y + circle->r * sin(angle);
        Ray ray = (Ray){x, y, angle};
        rays.push_back(ray);
    }
    return rays;
}

void draw_ray(SDL_Surface *surface, Ray *ray, Uint32 color){
    double x = ray->x;
    double y = ray->y;
    double dx = cos(ray->angle);
    double dy = sin(ray->angle);
    int width = surface->w;
    int height = surface->h;
    while(x >= 0 && x < width && y >= 0 && y < height){
        SDL_Rect rect = {(int)x, (int)y, RAY_THICKNESS, RAY_THICKNESS};
        SDL_FillRect(surface, &rect, color);
        x += dx;
        y += dy;
    }
}

void draw_ray(SDL_Surface *surface, Ray *ray, Circle *circle, Uint32 color){
    double x = ray->x;
    double y = ray->y;
    double dx = cos(ray->angle);
    double dy = sin(ray->angle);
    int width = surface->w;
    int height = surface->h;
    bool object_hit = false;
    double radius_squared = circle->r * circle->r;
    while(x >= 0 && x < width && y >= 0 && y < height && !object_hit){
        SDL_Rect rect = {(int)x, (int)y, RAY_THICKNESS, RAY_THICKNESS};
        SDL_FillRect(surface, &rect, color);
        x += dx;
        y += dy;
        double distance_squared = (x - circle->x) * (x - circle->x) + (y - circle->y) * (y - circle->y);
        if(distance_squared <= radius_squared){
            object_hit = true;
        }
    }
}

void move_shadow_circle(Circle *circle){

}

int main(int argc, char *argv[]) {
    printf("Hello, Ray Tracing!\n");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    
    int simulation_running = 1;
    SDL_Event event;
    int obstacle_y_speed = 1;
    Circle circle = (Circle){200, 200, 40};
    Circle shadow_circle = (Circle){550, 300, 140};
    while(simulation_running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                simulation_running = 0;
            }
            if(event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK)){
                circle.x = event.motion.x;
                circle.y = event.motion.y;
            }
        }
        SDL_FillRect(surface, NULL, COLOR_BLACK);
        FillCircle(surface, &circle, COLOR_WHITE);
        vector<Ray> rays = generate_circle_rays(&circle, RAYS_NUMBER);
        for(int i = 0; i < rays.size(); i++){
            draw_ray(surface, &rays[i], &shadow_circle, COLOR_RAY);
        }
        if(shadow_circle.y - shadow_circle.r < 0 || shadow_circle.y  + shadow_circle.r> HEIGHT){
            obstacle_y_speed *= -1;
        }
        shadow_circle.y += obstacle_y_speed;
        FillCircle(surface, &shadow_circle, COLOR_WHITE);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }
    return 0;
}