#include<iostream>
#include "gwindow.h"
#include "gobjects.h"
#include "gbufferedimage.h"

using namespace std;

//The variable specifies the color threshold for image binarization
const int THRESHOLD_COLOR = 12566463;//color HEX:  #b7b7b7

/*
 * The function of the original image binarization
 *@param - A dynamic array is placed where the result of binarization of the original image.
 *@param - The height of the source image.
 *@param - The width of the source image.
 *@param - The object of the original image.
 * */
void binarizationImage(int **p,int imgHeight,int imgWidth,GBufferedImage* img){
    string colorName = "";
    int rgb = 0;
    for(int y = 0;y < imgHeight;y++){
        for(int x = 0;x < imgWidth;x++){
            rgb = img->getRGB(x, y);
            colorName = convertRGBToColor(rgb);
            if(rgb > THRESHOLD_COLOR){
                p[y][x] = 0;
            }
            else {
                p[y][x] = 1;
            }
        }

    }
}

//A recursive function is found filling the set with the binary plane
/*@param - Starting coordinate y.
 *@param - Starting coordinate x.
 *@param - The new value of the found item.
 *@param - The old value of the found item.
 *@param - The height of the source image.
 *@param - The width of the source image.
 * */
void fill(int y,int x,int newValue,int oldValue,int **p,int imgHeight,int imgWidth){
    if(x >= 0 && x < imgWidth && y >= 0 && y < imgHeight && p[y][x] == oldValue && p[y][x] != newValue){
        p[y][x] = newValue;
        fill(y,x+1,newValue,oldValue,p,imgHeight,imgWidth);
        fill(y,x-1,newValue,oldValue,p,imgHeight,imgWidth);
        fill(y-1,x,newValue,oldValue,p,imgHeight,imgWidth);
        fill(y+1,x,newValue,oldValue,p,imgHeight,imgWidth);
    }


}

// The function counting the number of of silhouettes of something on the binary plane.
/*
 * @param The name of the original image file (name.jpg).
 * */
void findingNumberSilhouettesInImage(string nameImage){
    //Creating an object - the image and placing it in the source file.
    GImage* image;
    image = new GImage(nameImage);
    //Create Object window.
    GWindow gw(image->getWidth(),image->getHeight());
    gw.add(image);
    //Creating a buffer for reading the pixel values of the image.
    GBufferedImage* img = new GBufferedImage(0,0,image->getWidth(),image->getHeight());
    img->load(nameImage);
    // Parameters image width and height.
    int imgHeight = (int)img->getHeight();
    int imgWidth = (int)img->getWidth();

    //Create a dynamic array to record the binary image
    int **p = new int *[imgHeight];
    for(int i(0);i<imgHeight;i++){
        p[i] = new int[imgWidth];
    }
    //Call function binarization
    binarizationImage(p,imgHeight,imgWidth,img);

    //The variable for counting the number found silhouettes.
    int cur = 0;
    // @param - The new value of the item
    int newValue = 1;
    // @param - The old value of the item
    int oldValue = 1;

    //Starting the search cycle of certain elements
    //in the dynamic array and replacing them with new ones.
    for(int i = 1;i < imgHeight;i++){
        for(int j = 1;j < imgWidth;j++){
            if(p[i][j] == 1){
                newValue++;
                cur++;
                fill(i,j,newValue,oldValue,p,imgHeight,imgWidth);
            }
        }
    }
    //result output.
    cout<<cur<<endl;
    //Cleaning heap.
    for(int i(0);i<imgHeight;i++){
        delete[]p[i];
    }
    delete []p;
    delete image;
    delete img;
}




int main() {
    string str = "";
    while(true){
        cout<<"If you want to stop program enter -stop-"<<endl;
        cout<<"Enter the name of the source file/n(test1.jpg;test2.jpg;test3.jpg;test4.jpg;test5.jpg): ";
        if(cin>>str){
            if(str == "stop"){
                return -1;
            }
            else{
            findingNumberSilhouettesInImage(str);
            }
        }

    }

    return 0;
}



























