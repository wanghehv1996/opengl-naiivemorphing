#ifndef __TGALOAD_H__
#define __TGALOAD_H__
#pragma once
#pragma comment(lib, "Opengl32.lib")	

#include <windows.h>							
#include <stdio.h>								
#include <gl\gl.h>								
#include<iostream>

typedef	struct
{
	GLubyte * data;
	GLuint	width;
	GLuint	height;
	GLuint	type;
	GLuint	bpp;
	GLuint	texID;
} Texture;



bool LoadUncompressedTGA(Texture *, std::string, FILE *);	// Load an Uncompressed file
bool LoadCompressedTGA(Texture *, std::string, FILE *);		// Load a Compressed file
bool LoadTGA(Texture * texture, std::string filename);

#endif


