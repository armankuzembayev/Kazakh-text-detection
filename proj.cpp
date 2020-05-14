#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/text/ocr.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <iostream>
#include <fstream> 

using namespace cv;
using namespace std;

//sort by contour Area
bool compareContourAreas (vector<Point> contour1, vector<Point> contour2){
    double i = fabs(contourArea(Mat(contour1)));
    double j = fabs(contourArea(Mat(contour2)));
    return (i < j);
}

//using Tesseract OCR
void textDetection(Mat image){
    string outText;
 
    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    ocr->Init("tessdata", "kaz", tesseract::OEM_LSTM_ONLY);
 
    // Set Page segmentation mode to PSM_AUTO (3)
    ocr->SetPageSegMode(tesseract::PSM_AUTO);
   
    // Set image data
    ocr->SetImage(image.data, image.cols, image.rows, 3, image.step);
 
    // Running OCR on out preprocessed image
    outText = string(ocr->GetUTF8Text());

    // printint translated text
    cout << outText << endl; 
   
    //saving translated text to file
    ofstream outfile ("kaz_text.txt");

    outfile << outText << endl;	

    outfile.close();

    ocr->End();
}

int main(int argc, char** argv){
    Mat image;
    //here you can test your image
    image = imread("ex.jpg", CV_LOAD_IMAGE_COLOR); 
    if (!image.data){
        printf("No image data \n");
        return -1;
    }

    Size size(900,1200);
    resize(image, image, size);
    namedWindow("input", CV_WINDOW_AUTOSIZE);
    imshow("input", image);
    Mat gray, cannyContours, blurred;
    cvtColor(image, gray, COLOR_BGR2GRAY); //Convert to gray scale
    GaussianBlur(gray, blurred, Size(5,5), 1.5); //Gaussian blur
    Canny(blurred, cannyContours, 100, 200); //Canny edge detector
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(cannyContours, contours, hierarchy, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE); //Contours 

    // sort contours
    sort(contours.begin(), contours.end(), compareContourAreas);
    //find 4 corners
    vector<Point2f> v;
    for(unsigned int i = 0; i < contours.size(); i++){
	double p = arcLength(contours[i], true);
	approxPolyDP(contours[i], v, 0.02*p, true);
    }

    vector<Point2f> dst;
    dst.push_back(Point2f(900, 0));
    dst.push_back(Point2f(0, 0));
    dst.push_back(Point2f(0, 1200));
    dst.push_back(Point2f(900, 1200));
    //perspective transform
    Mat m = getPerspectiveTransform(v, dst);
    Mat output;
    warpPerspective(image, output, m, size);
	
    namedWindow("image", CV_WINDOW_AUTOSIZE);
    imshow("image", output);

    textDetection(output);

    waitKey(0);
    return 0;
}

