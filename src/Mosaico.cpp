/*
 * Author:  Alfonso Breglia 
 * Date: 27/12/16 
 * Description: Programma che tessella un'immagine utilizzando 
 *              un insieme finito di pattern
*/


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <cmath>

#define VERSION 	"1.0"
#define DOT_PER_CM   160
#define DOT_PER_TILE (DOT_PER_CM/8)

/***** TRIM ****/
std::string trim(const std::string& str, const std::string& whitespace){
    const size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const size_t strEnd = str.find_last_not_of(whitespace);
    const size_t strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


bool get_valid_line(std::istream &ifile,std::string &line){
    do{
        std::getline(ifile, line);
        line = trim(line," \t"); // trim leading and trailing white spaces
    }while(ifile.good() && (line[0]=='#'  || line.size()==0));
    
    return ifile.good();
}


int load_gray_bmp(
    std::string filename, 
    std::vector<uint8_t> &image, 
    int &dimx, 
    int &dimy){

    std::string line,token;        
    std::ifstream file(filename.c_str());
    dimx = -1;
    dimy = -1;
    
    if(!file.bad()){
        
        {   //read the magic number
            get_valid_line(file,line);
   	        std::stringstream ss(line);
   	        ss >> token;
   	        if (token != "P2"){
   	            std::cout << "ERRORE NEL CARICAMENTO DEL FILE: " <<"FORMATO SBAGLIATO" << std::endl;
   	            return -1; //Formato Sbagliato
   	        }
   	        
        }
        
        {   //Leggi la dimensione
            get_valid_line(file,line);
   	        std::stringstream ss(line);
   	        ss >> dimx  >> dimy;
   	        if( dimx < 0 || dimy < 0 ){
   	            std::cout << "ERRORE NEL CARICAMENTO DEL FILE: " 
                          <<"DIMENSIONI " << dimx << " " << dimy <<  " NON COMPATIBILI" << std::endl;
   	            return -1; // errore
   	        }
        }

        {   //Leggi la scala
            get_valid_line(file,line);
            int scale = -1;
   	        std::stringstream ss(line);
   	        ss >> scale;
   	        if( scale<=0 or scale >255 ){
  	            std::cout << "ERRORE NEL CARICAMENTO DEL FILE: " <<"SCALA ERRATA" << std::endl;
   	            return -1; // errore
   	        }
        }
        
        { // Leggi i caratteri
            int col;
            image.resize(dimx*dimy);
            
            for(int i =0; i <dimx*dimy; i++){
                if(!file.eof()){
                    file >> col;
                    image[i] = col;
                }else{
      	            std::cout << "ERRORE NEL CARICAMENTO DEL FILE: " <<"PIXEL MANCANTI" << std::endl;
                    return -1;
                }
            }
        }
        
    }else{
        std::cout << "ERRORE NEL CARICAMENTO DEL FILE: " <<"ERRORE DI APERTURA" << std::endl;
        return -1; // Errore        
    }
    
    return 0;
}

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

float compute_score(
    int t_row, int t_col, 
    const std::vector<uint8_t> &image,
    const int dimx,
    const int dimy,  
    const std::vector<uint8_t> &pattern){

    float score = 0;
    for (int row = 0; row< DOT_PER_TILE; row++  ){
        for( int col =0 ; col < DOT_PER_TILE; col++){
            
            int g_col = col +  t_col*DOT_PER_TILE;
            int g_row = row +  t_row*DOT_PER_TILE;
            
            int image_idx = g_col+g_row*dimx;
            int pattern_idx = col+row*DOT_PER_TILE;
            const float img_point     = image[image_idx];
            const float pattern_point = pattern[pattern_idx]; 
            
            score += (img_point-pattern_point) *(img_point-pattern_point);                        
        }
        
    }
    return sqrtf(score);    
}


//differenza tra luminosità
float compute_score2(
    int t_row, int t_col, 
    const std::vector<uint8_t> &image,
    const int dimx,
    const int dimy,  
    const std::vector<uint8_t> &pattern){

	float lum_tile = 0;
	float lum_pattern = 0 ;
	
    float score = 0;
    for (int row = 0; row< DOT_PER_TILE; row++  ){
        for( int col =0 ; col < DOT_PER_TILE; col++){
            
            int g_col = col +  t_col*DOT_PER_TILE;
            int g_row = row +  t_row*DOT_PER_TILE;
            
            int image_idx = g_col+g_row*dimx;
            int pattern_idx = col+row*DOT_PER_TILE;
            const float img_point     = image[image_idx];
            const float pattern_point = pattern[pattern_idx]; 
            lum_tile		+=	img_point;
            lum_pattern		+=	pattern_point;

        }        
    }
    
    lum_tile/= (DOT_PER_TILE*DOT_PER_TILE);
    lum_pattern/= (DOT_PER_TILE*DOT_PER_TILE);

    score = (lum_tile-lum_pattern) *(lum_tile-lum_pattern);                        

    
    return sqrtf(score);    
}

void paste_pattern_to_image(
    int t_row, int t_col, 
    std::vector<uint8_t> &new_image,
    int dimx,
    int dimy, 
    const std::vector<uint8_t> &pattern){
    
    for (int row = 0; row< DOT_PER_TILE; row++  ){
        for( int col =0 ; col < DOT_PER_TILE; col++){
            
            int g_col = col +  t_col*DOT_PER_TILE;
            int g_row = row +  t_row*DOT_PER_TILE;
            
            int image_idx = g_col+g_row*dimx;
            int pattern_idx = col+row*DOT_PER_TILE;
            new_image[image_idx] = pattern[pattern_idx];            
        }        
    }
}


int main(int argc,  char **argv){	

    std::string filename = "test.pgm";
    int num_pattern      = 5;
	int num_class        = 14;

	
    std::cout << "GENERATORE DI MOSAICI" << std::endl;
    std::cout << "VERSIONE: " << VERSION << std::endl;
    std::cout << "CARICAMENTO IMMAGINE: " << filename << std::endl;
    
    
    std::vector<uint8_t> image;
    int dimx;
    int dimy;
    int result = load_gray_bmp(filename, image, dimx, dimy);
    
    if(result <0){
        std::cout << "ERRORE NEL CARICAMENTO DEL FILE: " << filename << std::endl;
    }
    
    
	std::cout << "CARICAMENTO DEI PATTERN" << std::endl;
    std::vector<std::vector<uint8_t> > pattern_list;

	for (int j = 0; j < num_class; j ++ ){
		for (int i = 0; i < num_pattern; i ++ ){
		    std::vector<uint8_t> pattern;
		    int pdimx = 0;
		    int pdimy = 0;
		    std::stringstream ss;
		    ss << "pattern_" << i <<"_class_"<<j << ".pgm";
	        load_gray_bmp(ss.str(), pattern, pdimx, pdimy);
	        
	        if(pdimx != DOT_PER_TILE  || pdimy != DOT_PER_TILE){
	            std::cout << "DIMENSIONE DEI PATTERN "  << ss.str() << " ERRATA"  <<  std::endl;
	            std::cout << "LA DIMENSIONE DEI PATTERN DEVE ESSERE: " << DOT_PER_TILE << "x" << DOT_PER_TILE << std::endl;
	            return -1;
	        }
	        
	        pattern_list.push_back(pattern);
		}
	}
	
	
    int xtiles =  dimx/DOT_PER_TILE;	
    int ytiles =  dimy/DOT_PER_TILE;	
    if(xtiles * ytiles * DOT_PER_TILE *DOT_PER_TILE < dimx*dimy){
        std::cout << "ERRORE: NUMERO DI TILES NON INTERO";
        return -1;
    }
	
    std::cout << "NUMERO DI TILES: " << xtiles  << "x" << ytiles << std::endl;
    
    std::cout << "CREAZIONE IMMAGINE DELLE TILES" << std::endl;
    std::vector<uint16_t> tiles_id(xtiles*ytiles);
    
    for(int t_row =0; t_row < ytiles; t_row++){
        for(int t_col = 0; t_col < xtiles; t_col++){
            //TROVA IL PATTERN CON LA DISTANZA MINIMA            
            uint16_t best_pattern_id =     0;
            float    best_score      = 10000000000.0f;
            
            for (uint16_t id = 0; id < pattern_list.size();  id++){
                float score = compute_score2(t_row, t_col, image, dimx, dimy,pattern_list[id]);
                //std::cout << "score:" << score << std::endl;
                if(score < best_score){
                    best_score = score;
                    best_pattern_id = id;
                }
            }
            tiles_id[t_col+ t_row* xtiles] = best_pattern_id;
            std::cout << best_pattern_id << std::endl;
        }
    }

    std::cout << "RICOSTRUZIONE DELL'IMMAGINE" << std::endl;
    std::vector<uint8_t> new_image(dimx*dimy);
    for(int t_row =0; t_row < ytiles; t_row++){
        for(int t_col = 0; t_col < xtiles; t_col++){
            paste_pattern_to_image(t_row, t_col, new_image, dimx, dimy, pattern_list[tiles_id[t_col+ t_row* xtiles]]);
        }
    }
    
    std::cout << "SALVATAGGIO DELL'IMMAGINE RICOTRUITA" << std::endl;
    
    save_gray_bmp("out.pgm", new_image,dimx, dimy ); 
    
    
    	
    return 0;
}
