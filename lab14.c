#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void GameLife(int** life, int hei, int wid){
    int N;
    int** tmp = (int**)malloc(hei * sizeof(int*));
    int i;
    for(i = 0; i < hei; i++)
        tmp[i] = (int*)malloc(wid * sizeof(int));
    int x, y;
    for(y = 0; y < hei; y++)
        for(x = 0; x < wid; x++)
            tmp[y][x] = life[y][x];

    for(y = 0; y < hei; y++)
	{
        for(x = 0; x < wid; x++)
		{
		if (y == 0)
    	{

    		if (x == 0) N = life[1][0] + life[0][1] + life[1][1];
    		if (x == wid - 1) N = life[0][wid - 2] + life[1][wid - 2] + life[0][wid];
    		else N = life[0][x - 1] + life[0][x + 1] + life[1][x - 1] + life[1][x] + life[1][x + 1];

		}
		else if (y == hei - 1)
		{
    		if (x == 0) N = life[hei -1][0] + life[hei -1][1] + life[hei - 2][0];
    		if (x == wid - 1) N = life[hei -1][wid - 2] + life[hei - 2][wid - 2] + life[hei - 2][wid - 1];
    		else N = life[hei - 1][x -1] + life[hei - 1][x + 1] + life[hei -2][x] + life[hei - 2][x - 1] + life[hei -2][x + 1];

		}
		else
        	N = life[y + 1][x - 1] + life[y + 1][x] + life[y + 1][x + 1] + life[y][x - 1] + life[y][x + 1] + life[y - 1][x - 1] + life[y - 1][x] + life[y - 1][x + 1];
            if(life[y][x] == 1)
			{
                if(N == 2) tmp[y][x] = life[y][x];
                if(N == 3) tmp[y][x] = life[y][x];
                if(N > 3) tmp[y][x] = 0;
                if(N < 2) tmp[y][x] = 0;
            }
            else
			{
                if(N == 3) tmp[y][x] = 1;
            }
            N = 0;
        }
    }
    int inf = 0;

    for(y = 0; y < hei; y++)
        for(x = 0; x < wid; x++)
		{
            if(life[y][x] == tmp[y][x])
                inf++;
            life[y][x] = tmp[y][x];
        }
    if(inf == hei * wid)
        exit(0);
}

struct BMP{
    int Width;
    int Height;
    int Size;
};

int main(int argc, char* argv[])
{
    struct BMP Image;
    unsigned char header[54];
    int i, j, l, m;
    int maxiter , dumpfreq = 1;
    char* dirname;
    FILE* file;



	for(i = 0; i < argc; i++)
	{
        if(!strcmp("--input", argv[i]))
		{
            file = fopen(argv[1 + 1], "rb");
        }
        if(!strcmp("--output", argv[i]))
		{
            dirname = argv[i + 1];
        }
        if(!strcmp("--max_iter", argv[i]))
		{
            maxiter = strtol(argv[i + 1], 0, 10);
        }
        if(!strcmp("--dump_freq", argv[i]))
		{
            dumpfreq = strtol(argv[i + 1], 0, 10);
        }
    }


    fread(header, 1, 54, file);
    Image.Width = header[21] * 256 * 256 * 256 + header[20] * 256 * 256 + header[19] * 256 + header[18];
    Image.Height = header[25] * 256 * 256 * 256 + header[24] * 256 * 256 + header[23] * 256 + header[22];
    Image.Size = header[5] * 256 * 256 * 256 + header[4] * 256 * 256 + header[3] * 256 + header[2];
    unsigned char *imagebyte = malloc(sizeof(unsigned char) * (Image.Size - 54));
    fread(imagebyte, 1, Image.Size, file);

    int** img = (int**)malloc(Image.Height * sizeof(int*));
    for(i = 0; i < Image.Height; i++)
        img[i] = (int*)malloc(Image.Width * sizeof(int));



	int padding;
    padding = -(Image.Width % 4);
    for(i = Image.Height - 1; i >= 0; i--)
	{
        padding += (Image.Width % 4);
        for(j = 0; j < Image.Width; j++)
		{
            if(imagebyte[padding] == 255)
                img[i][j] = 0;
            else
                img[i][j] = 1;
            padding += 3;
        }
    }

	for (l = 0; l < maxiter; l++)
	{
        GameLife(img, Image.Height, Image.Width);
        if(l % dumpfreq == 0)
		{
            char* filename = (char*)malloc(l * sizeof(char));;
            char* way = (char*)malloc(100 * sizeof(char));
            strcpy(way, dirname);
            strcat(strcat(way, "\\"), strcat(itoa(l, filename, 10), ".bmp"));

            FILE* life = fopen(way, "w");
            fwrite(header, 1, 54, life);
            m = 0;
			// MAKE BITMAP
            for (i = Image.Height - 1; i >= 0; i--)
			{
                for (j = 0; j < Image.Width; j++)
				{
                    for (padding = 0; padding < 3; padding++)
					{
                        if (img[i][j] == 1)
                            imagebyte[m] = 0;
                        else
                            imagebyte[m] = 255;
                        m++;
                    }
                }
                while (m % 4 != 0) {
                    imagebyte[m] = 0;
                    m++;
                }
            }
 			fwrite(imagebyte, 1, Image.Size, life);
            fclose(life);
            free(filename);
            free(way);
        }

	}
	for(i = 0; i < Image.Height; i++)
	{
		free(img[i]);
	}
	free(img);
	free(imagebyte);
	return 0;
}

