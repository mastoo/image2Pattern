//compile with --std=c++11


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <cmath>
#include <random>
#include <chrono>

#define SIZE_X 20
#define SIZE_Y 20

#define NREP  5


enum STRIPED_TYPE {HORIZONTAL = 0, VERTICAL = 1};

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

void generate_stripes(int dimx, int dimy, std::vector<uint8_t> &image, int pen_width, int pen_spacing, STRIPED_TYPE tr, unsigned int offset ){
	
	image.resize(dimx*dimy);
	
    int color = 0;
    
    for(int y = 0; y < dimy ; y++){
    	if((y+offset) % (pen_width+pen_spacing )< pen_width)
    		color = 0;
    	else
    	 	color= 255;
    	 	
        for(int x = 0; x < dimx ; x++){
        	if(tr == HORIZONTAL){
			
            	image[x+y*dimx] = color;            
            }else{
        		image[y+x*dimy] = color;            

			}
        }
    } 
}


void generate_point(int dimx, int dimy, std::vector<uint8_t> &image, unsigned num_points , std::default_random_engine &generator ){
	
  	std::uniform_int_distribution<int> xdist(0,dimx-1);
 	std::uniform_int_distribution<int> ydist(0,dimy-1);

	image.resize(dimx*dimy);
	
	for(int y = 0; y < dimy ; y++){
		for(int x = 0; x < dimx ; x++){	
			image[x+y*dimx] = 255;
		}
	}
	
    for(int i = 0; i < num_points; i++){
    	int x = xdist(generator);
    	int y = ydist(generator);
    	
    	image[x+y*dimx] = 0;            
	}
    

}

int main(){
	std::vector<uint8_t> image;
	// construct a trivial random generator engine from a time-based seed:
  	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator (seed);

  	std::uniform_int_distribution<int> distribution(0,40);

	//genera  pattern stipes orizzontali
	for (int i = 0; i < NREP; i++){
		
		std::stringstream ss;
		ss << "pattern_" << i<<"_";
		
		std::string prefix;		
		ss >> prefix ;
		
		generate_stripes(SIZE_X,SIZE_Y,image,8,0,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_0.pgm"),image,SIZE_X,SIZE_Y);
		
		generate_stripes(SIZE_X,SIZE_Y,image,4,1,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_1.pgm"),image,SIZE_X,SIZE_Y);
		generate_stripes(SIZE_X,SIZE_Y,image,4,2,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_2.pgm"),image,SIZE_X,SIZE_Y);	
		generate_stripes(SIZE_X,SIZE_Y,image,4,3,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_3.pgm"),image,SIZE_X,SIZE_Y);	
		generate_stripes(SIZE_X,SIZE_Y,image,4,4,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_4.pgm"),image,SIZE_X,SIZE_Y);	

		generate_stripes(SIZE_X,SIZE_Y,image,4,5,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_5.pgm"),image,SIZE_X,SIZE_Y);
		generate_stripes(SIZE_X,SIZE_Y,image,4,6,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_6.pgm"),image,SIZE_X,SIZE_Y);	
		generate_stripes(SIZE_X,SIZE_Y,image,4,7,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_7.pgm"),image,SIZE_X,SIZE_Y);	
		generate_stripes(SIZE_X,SIZE_Y,image,4,8,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_8.pgm"),image,SIZE_X,SIZE_Y);

		save_gray_bmp(prefix+std::string("class_9.pgm"),image,SIZE_X,SIZE_Y);	
		generate_stripes(SIZE_X,SIZE_Y,image,4,9,HORIZONTAL,distribution(generator));
		save_gray_bmp(prefix+std::string("class_10.pgm"),image,SIZE_X,SIZE_Y);		
		generate_stripes(SIZE_X,SIZE_Y,image,4,10,HORIZONTAL,distribution(generator));

		save_gray_bmp(prefix+std::string("class_11.pgm"),image,SIZE_X,SIZE_Y);		
		generate_stripes(SIZE_X,SIZE_Y,image,4,11,HORIZONTAL,distribution(generator));

		save_gray_bmp(prefix+std::string("class_12.pgm"),image,SIZE_X,SIZE_Y);		
		generate_stripes(SIZE_X,SIZE_Y,image,4,12,HORIZONTAL,distribution(generator));

		save_gray_bmp(prefix+std::string("class_13.pgm"),image,SIZE_X,SIZE_Y);		
		generate_stripes(SIZE_X,SIZE_Y,image,4,13,HORIZONTAL,distribution(generator));		
		
//		generate_stripes(SIZE_X,SIZE_Y,image,8,2,VERTICAL,distribution(generator));
//		save_gray_bmp(prefix+std::string("class_5.pgm"),image,SIZE_X,SIZE_Y);
//		generate_stripes(SIZE_X,SIZE_Y,image,8,4,VERTICAL,distribution(generator));
//		save_gray_bmp(prefix+std::string("class_6.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_stripes(SIZE_X,SIZE_Y,image,8,8,VERTICAL,distribution(generator));
//		save_gray_bmp(prefix+std::string("class_7.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_stripes(SIZE_X,SIZE_Y,image,8,16,VERTICAL,distribution(generator));
//		save_gray_bmp(prefix+std::string("class_8.pgm"),image,SIZE_X,SIZE_Y);	
		
		
//		generate_point(SIZE_X,SIZE_Y,image,800,generator);
//		save_gray_bmp(prefix+std::string("class_9.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_point(SIZE_X,SIZE_Y,image,600,generator);
//		save_gray_bmp(prefix+std::string("class_10.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_point(SIZE_X,SIZE_Y,image,400,generator);
//		save_gray_bmp(prefix+std::string("class_11.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_point(SIZE_X,SIZE_Y,image,200,generator);
//		save_gray_bmp(prefix+std::string("class_12.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_point(SIZE_X,SIZE_Y,image,100,generator);
//		save_gray_bmp(prefix+std::string("class_13.pgm"),image,SIZE_X,SIZE_Y);	
//		generate_point(SIZE_X,SIZE_Y,image,50,generator);
//		save_gray_bmp(prefix+std::string("class_14.pgm"),image,SIZE_X,SIZE_Y);	
//
//		generate_point(SIZE_X,SIZE_Y,image,20,generator);
//		save_gray_bmp(prefix+std::string("class_15.pgm"),image,SIZE_X,SIZE_Y);	
		
//		for(int k = 0; k<64; k++){
//			std::stringstream ss;
//			ss << "pattern_" << i<<"_class_"<<k<<".pgm";
//			std::string filename;
//			ss >> filename;
//			
//			generate_point(SIZE_X,SIZE_Y,image,k*16000/64,generator);
//			save_gray_bmp(filename,image,SIZE_X,SIZE_Y);
//		}
	}

	
	return 0;
}
