#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <cmath>


#define SIZE_X 40
#define SIZE_Y 40


void save_gray_bmp(
    std::string filename, 
    const std::vector<uint8_t> &image,
    const int dimx,
    const  int dimy){

    std::ofstream ofile(filename.c_str());
    
    ofile << "P2\n";
    ofile << dimx << " " << dimy << "\n";
    ofile << 255 << "\n";
    
    int i =0;
    for(int y = 0; y < dimy ; y++){
        for(int x = 0; x < dimx ; x++){
            ofile <<( (int)image[i] )<< "\n";
            i++;
        }
    }        
}

void generate_horizontal_stripes(int dimx, int dimy, std::vector<uint8_t> &image, int pen_width, int pen_spacing ){
	
	image.resize(dimx*dimy);
	
    int i =0;
    int color = 0;
    
    for(int y = 0; y < dimy ; y++){
    	if(y % (pen_width+pen_spacing )< pen_width)
    		color = 0;
    	else
    	 	color= 255;
    	 	
        for(int x = 0; x < dimx ; x++){
            image[i] = color;
            i++;
        }
    } 
}

int main(){
	std::vector<uint8_t> image;
	

	//genera  pattern stipes orizzontali
	generate_horizontal_stripes(SIZE_X,SIZE_Y,image,8,4);
	save_gray_bmp("pattern1.pgm",image,SIZE_X,SIZE_Y);
	generate_horizontal_stripes(SIZE_X,SIZE_Y,image,8,2);
	save_gray_bmp("pattern2.pgm",image,SIZE_X,SIZE_Y);	
	
	
	return 0;
}
