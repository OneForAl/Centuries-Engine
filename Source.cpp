#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <limits>
using namespace std;

// CONSTANTS
int BLACK[4] = { 0,0,0,0 };
int WHITE[4] = { 255,255,255,0 };
int screen_dimensions[2] = { 800,800 };
int screen_pos[2] = { 20,20 };

// this function will be deleted
// only for testing.
bool game_logic(SDL_Event e) {
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		//User presses a key
		else if (e.type == SDL_KEYDOWN)
		{
			//Select surfaces based on key press
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				// up key is pressed
				break;

			case SDLK_DOWN:
				// down key is pressed
				break;

			case SDLK_LEFT:
				// left key is pressed
				break;

			case SDLK_RIGHT:
				// right key is pressed
				break;

			default:
				// default setting
				break;
			}
		}
	}
}

void setup() {

}

// real thing starts here!

bool init(bool loop(SDL_Event), int screen_dimensions[2], int screen_pos[2]) {
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		 throw "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Testing", screen_pos[0], screen_pos[1],
			screen_dimensions[0], screen_dimensions[1], SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			throw "Window could not be created! SDL_Error: %s\n", SDL_GetError() ;
		}
		else
		{
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				throw "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError();
			}
			else {
				//Get window surface
				screenSurface = SDL_GetWindowSurface(window);
				//Create renderer for window
				SDL_Renderer* gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL)
				{
					throw "Renderer could not be created! SDL Error: %s\n", SDL_GetError();
				}
				else {
					//Update the surface
					SDL_UpdateWindowSurface(window);

					// create object
					engine Template_engine(screen_dimensions, window, screenSurface, gRenderer, screen_pos);
					setup();
					game_loop(Template_engine, loop);


					//Deallocate surface
					Template_engine.deallocate_surfaces(Template_engine.SURFACES);

					bool success = close(window);
					if (success) {
						return true;
					}
					else {
						throw "An problem occurred in closing";
					}
				}
			}
		}
	}
	
}

bool close(SDL_Window* window) {
	try{
		//Destroy window
		SDL_DestroyWindow(window);

		//Quit SDL subsystems
		SDL_Quit();
		return true;
	}
	catch (...) {
		return false;
	}
}

class texture {
	int index{ 0 };
	string PATH;
	texture(string path){
		
	}
};

class image {
	string type = "";
	int surface_index{ 0 };
	SDL_Surface* DEST{ NULL };
	string path{ "" };
	int null[2] = { 0, 0 };
	int Dimensions[4] = { 0,0,0,0 };
	engine OBJECT{ null, NULL, NULL, NULL, null };
	image(engine obj, string path, SDL_Surface* destination, int dim[4]) {
		//load image
		image::DEST = destination;
		image::path = path;
		image::OBJECT = obj;
		image::type = path.substr(path.size()-4, 3);
		//The final optimized image
		SDL_Surface* optimizedSurface = NULL;

		//Load image at specified path
		SDL_Surface* loadedSurface;
		if (image::type == "bmp" or image::type == "BMP") {
			loadedSurface = SDL_LoadBMP(path.c_str());
		}
		else {
			loadedSurface = IMG_Load(path.c_str());
		}
		if (loadedSurface == NULL)
		{
			throw "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError();
		}
		else
		{
			//Convert surface to screen format
			optimizedSurface = SDL_ConvertSurface(loadedSurface, DEST->format, 0);
			if (optimizedSurface == NULL)
			{
				throw "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError();
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);
		}
		SDL_Rect stretchRect;
		stretchRect.x = dim[0];
		stretchRect.y = dim[1];
		stretchRect.h = dim[2];
		stretchRect.w = dim[3];

		image::surface_index = OBJECT.SURFACES.size();
		OBJECT.SURFACES.push_back(loadedSurface);
		SDL_BlitScaled(loadedSurface, NULL, DEST, &stretchRect);
	}
};

class button{
public:
	int pos[2] = { 0,0 };
	int pos2[2] = { 0,0 };
	int l{ 0 };
	int b{ 0 };
	int VCOL[4] = { 0,0,0,0 };
	int Border{0};
	int Bordercol[4] = { 0,0,0,0 };
	button(engine obj, int dimensions[4], int col[4], string text, int txtcol[4], 
		int border, int bordercol[4], int vcol[4]) {
		engine boss = obj;
		if (dimensions[0] < dimensions[2]) {
			button::pos[0] = dimensions[0];
			button::pos2[0] = dimensions[3];
		}
		else if (dimensions[0] > dimensions[2]) {
			button::pos2[0] = dimensions[0];
			button::pos[0] = dimensions[3];
		}
		else {
			throw "BUTTON ERROR:x values are equal!";
		}

		if (dimensions[1] < dimensions[3]) {
			button::pos[1] = dimensions[1];
			button::pos2[1] = dimensions[3];
		}
		else if (dimensions[1] > dimensions[3]) {
			button::pos2[1] = dimensions[1];
			button::pos[1] = dimensions[3];
		}
		else {
			throw "BUTTON ERROR:x values are equal!";
		}
		l = pos2[0] - pos[0];
		b = pos2[1] - pos[1];
	}
};

class engine {
	/*This is a game engine created by Ankit Satpati.
	* It is designed to be improved and be as fast as made possible.
	I hope that you like it and please feel free to
	give feedbacks or improve this code.

	Happy Coding!
	*/

public:
	SDL_Window* SCREEN = NULL;
	SDL_Surface* SURFACE = NULL;
	SDL_Renderer* RENDERER = NULL;
	int SCREEN_POS[2] = { 0,0 };
	int SCREEN_DIM[2] = { 0,0 };
	vector<SDL_Surface*> SURFACES; //surfaces used for freeing 
	vector<SDL_Texture*> TEXTURES;
	int draw_col[4] = { 0,0,0,0 };

	engine(int screen_dim[2], SDL_Window* screen, SDL_Surface* surface, SDL_Renderer* renderer, int screen_pos[2]) {
		engine::RENDERER = renderer;
		engine::SCREEN = screen;
		engine::SURFACE = surface;
		engine::SCREEN_POS[0] = screen_pos[0];
		engine::SCREEN_POS[1] = screen_pos[1];
		engine::SCREEN_DIM[0] = screen_dim[0];
		engine::SCREEN_DIM[1] = screen_dim[1];

	}
	void change_back_col(int col[4]){
		SDL_FillRect(engine::SURFACE, NULL, SDL_MapRGBA(engine::SURFACE->format, col[0], col[1], col[2], col[3]));
	}
	button init_rbutton(engine obj, int dimensions[4], int col[4], string text, int txtcol[4],
		int border, int bordercol[4], int vcol[4]);
	bool rect_button(button button,int mouse[2], bool mouse_hold);
	bool polygonal_button(vector<int> y_coords, vector<vector<int>> x_dims, int col[4], int center[2], string text,
		int mouse[2], bool mouse_hold, int txtcol[4], int border, int bordercol[4], int vcol[4], bool init);
	SDL_Surface* load_image(string file, int pos[2], int size[2]) {

	}
	void set_renderer_col(int col[4]){
		engine::draw_col[0] = col[0];
		engine::draw_col[1] = col[1];
		engine::draw_col[2] = col[2];
		engine::draw_col[3] = col[3];
		SDL_SetRenderDrawColor(engine::RENDERER, col[0], col[1], col[2], col[3]);
	}
	void deallocate_surface(vector<SDL_Surface*> surfaces, int at) {
		SDL_FreeSurface(surfaces.at(at));
		surfaces.at(at) = NULL;
	}
	void deallocate_surfaces(vector<SDL_Surface*> surfaces) {
		for (int i = 0; i <= surfaces.size(); i++) {
			SDL_FreeSurface(surfaces.at(i));
			surfaces.at(i) = NULL;
		}
	}
};

button engine::init_rbutton(engine obj, int dimensions[4], int col[4], string text, int txtcol[4] = BLACK,
	int border = 0, int bordercol[4] = BLACK, int vcol[4] = BLACK) {
	button b(obj, dimensions,col,text,txtcol,border,bordercol,vcol);
	return b;
}

void game_loop(engine obj, bool loop(SDL_Event)) {
	//continue in the game_loop
	bool cont{true};
	//Event handler
	SDL_Event e;
	while(cont){
		cont = loop(e);
		// update window
		SDL_UpdateWindowSurface(obj.SCREEN);
	}
}

int main(int argc, char* args[]) {
	
	bool init_msg = init(game_logic, screen_dimensions, screen_pos);
	
	if (init_msg) {
		return 0;
	}
}