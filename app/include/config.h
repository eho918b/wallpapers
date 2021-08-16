#pragma once

typedef struct config_app_t	
{
	int width;
	int height;
} config_app_t; 

typedef struct config_game_t	
{
	const char* host;
	int port;
	int window_width;
	int window_height;
	int window_x;
	int window_y;
} config_game_t;

typedef struct config_t	
{
	config_app_t app;
	config_game_t game;
} config_t; 
