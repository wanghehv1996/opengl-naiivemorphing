#include "tgaload.h"

bool LoadUncompressedTGA(Texture * texture, std::string filename, FILE * file)//加载未压缩文件
{																			
	GLubyte	header[6];
	GLuint		bpp;
	GLuint		size;
	GLuint		type;
	GLuint		Height;
	GLuint		Width;

	if (fread(header, sizeof(header), 1, file) == 0)//6
	{
		std::cout<<"error:read tga header " << std::endl;
		if (file != NULL){
			fclose(file);	
		}
		return false;
	}

	texture->width = header[1] * 256 + header[0];	//(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];//(highbyte*256+lowbyte)
	texture->bpp = header[4];//bit per pixel
	Width = texture->width;
	Height = texture->height;

	if ((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp != 32)))
	{
		std::cout<< "Invalid texture information"<<std::endl;
		if (file != NULL)	{
			fclose(file);	
		}
		return false;
	}

	//根据bpp确定类型
	if (texture->bpp == 24)
		texture->type = GL_RGB;	
	else
		texture->type = GL_RGBA;

	//计算大小并赋值
	size = (texture->bpp / 8 * Width * Height);
	texture->data = (GLubyte *)malloc(size);
	if (fread(texture->data, 1, size, file) != size){
		std::cout<< "Could not read image data"<<std::endl;
		if (texture->data){
			free(texture->data);
		}
		fclose(file);	
		return false;
	}
	fclose(file);	
	return true;
}

bool LoadCompressedTGA(Texture * texture, std::string filename, FILE * file)		// Load COMPRESSED TGAs
{
	GLubyte	header[6];
	GLuint		bpp;
	GLuint		size;
	GLuint		type;
	GLuint		Height;
	GLuint		Width;
	if (fread(header, sizeof(header), 1, file) == 0)	{
		std::cout<< "Could not read info header"<<std::endl;	
		if (file != NULL){
			fclose(file);
		}
		return false;
	}

	texture->width = header[1] * 256 + header[0];
	texture->height = header[3] * 256 + header[2];
	texture->bpp = header[4];
	Width = texture->width;
	Height = texture->height;

	if ((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp != 32)))
	{
		std::cout << "Invalid texture information" << std::endl;
		if (file != NULL)	{
			fclose(file);
		}
		return false;
	}

	//根据bpp确定类型
	if (texture->bpp == 24)
		texture->type = GL_RGB;
	else
		texture->type = GL_RGBA;

	bpp = texture->bpp / 8;
	size = (bpp * Width * Height);
	texture->data = (GLubyte *)malloc(size);

	GLuint pixelcount = Height * Width;
	GLuint currentpixel = 0;
	GLuint currentbyte = 0;
	GLubyte * colorbuffer = (GLubyte *)malloc(bpp);

	do {
		GLubyte chunkheader = 0;
		//readheader
		if (fread(&chunkheader, sizeof(GLubyte), 1, file) == 0)
		{
			std::cout<< "Could not read RLE header"<<std::endl;	
			if (file != NULL)	
				fclose(file);
			if (texture->data)
				free(texture->data);
			return false;
		}
		//128表示不重复 第一位0
		if (chunkheader < 128)	{
			chunkheader++;
			//读颜色值
			for (short counter = 0; counter < chunkheader; counter++){
				if (fread(colorbuffer, 1, bpp, file) != bpp) // Try to read 1 pixel
				{
					std::cout<< "Could not read image data"<<std::endl;		// IF we cant, display an error
					if (file)													// See if file is open
						fclose(file);													// If so, close file
					if (colorbuffer)												// See if colorbuffer has data in it
						free(colorbuffer);												// If so, delete it
					if (texture->data)										// See if there is stored Image data
						free(texture->data);										// If so, delete it too

					return false;														// Return failed
				}
				// write to memory
				texture->data[currentbyte] = colorbuffer[2];
				texture->data[currentbyte + 1] = colorbuffer[1];
				texture->data[currentbyte + 2] = colorbuffer[0];
				//RGBA
				if (bpp == 4)
					texture->data[currentbyte + 3] = colorbuffer[3];
				//++
				currentbyte += bpp;
				currentpixel++;

				if (currentpixel > pixelcount)	{
					std::cout<< "Too many pixels read"<<std::endl;
					if (file != NULL)	
						fclose(file);
					if (colorbuffer != NULL)
						free(colorbuffer);
					if (texture->data != NULL)
						free(texture->data);
					return false;
				}
			}
		}
		// > 128 第一位为1 表示重复了 去掉第一位1先
		else
		{
			chunkheader -= 127;
			//读颜色
			if (fread(colorbuffer, 1, bpp, file) != bpp)			{
				std::cout<< "Could not read from file"<<std::endl;
				if (file)
					fclose(file);
				if (colorbuffer)
					free(colorbuffer);
				if (texture->data)
					free(texture->data);
				return false;
			}

			//按重复赋值
			for (short counter = 0; counter < chunkheader; counter++){
				texture->data[currentbyte] = colorbuffer[2];
				texture->data[currentbyte + 1] = colorbuffer[1];
				texture->data[currentbyte + 2] = colorbuffer[0];
				if (bpp == 4)
					texture->data[currentbyte + 3] = colorbuffer[3];
				currentbyte += bpp;	
				currentpixel++;
				if (currentpixel > pixelcount)	{
					std::cout<< "Too many pixels read"<<std::endl;
					if (file != NULL)	
						fclose(file);
					if (colorbuffer != NULL)
						free(colorbuffer);
					if (texture->data != NULL)
						free(texture->data);
					return false;
				}
			}
		}
	}
	while (currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(file);																		// Close the file
	return true;																		// return success
}