#include <dlib/opencv.h>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp" 
#include <dlib/image_io.h>

#include "iostream"
#include <string>
#include <direct.h>
#include <windows.h>
#include <fstream>


using namespace std;
using namespace dlib;
using namespace cv;
/*string name2 = "mask_2_" + to_string(c) + ".png";
						cv::imwrite("img\\" + name2, imggray2);*/

//cv::Rect detectAndDisplay( Mat frame );
Mat detectAndDisplay( Mat frame );

CascadeClassifier face_cascade;

int maindsdsd(void)
{
	string face_cascade_name = "haarcascade_frontalface_alt.xml";
	
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	//Membagi step frame pada video
	cv::VideoCapture cap ("EP02_01f.avi");

	if (!cap.isOpened())
    {
        cerr << "Unable to connect to camera" << endl;
        return 1;
	}else{
		cout << CV_CAP_PROP_FRAME_COUNT << endl ;
		cout << CV_CAP_PROP_FPS << endl ;
			
		float nmax = (float)CV_CAP_PROP_FRAME_COUNT / (float)CV_CAP_PROP_FPS;

		//cout<< nmax << endl;

		cv::Mat frame1, frame2, img1 , img2 , imgs1 , imgs2 ,imggray1,imggray2;
		cv::Rect roi1, roi2;
		
		int c=1;
			

		while (1)
		{
			cap >> frame1;
			frame2 = cap.get(c+1);

			if(!frame1.empty() || !frame2.empty()){
						
				//MEMBACA PER FRAME
				img1 = frame1.clone();
				img2 = frame2.clone();			

				//img1 = detectAndDisplay(img1);
				//cv::resize(img1, img1,Size(240,240),0.5, 0.5, INTER_LINEAR);

				if(!img2.empty()){
					img2 = detectAndDisplay(img2);
					cv::resize(img2, imgs2,Size(240,240),0.5, 0.5, INTER_LINEAR);
				}

				string name2 = "mask_2_" + to_string(c) + ".png";
				cv::imwrite("img\\" + name2, img1);
			
				/*ofstream outputFile;
				outputFile.open("CSV//img1.txt");*/

				//// Iterate through pixels.
				//for (int r = 0; r < img1.rows; r++)
				//{
				//	for (int c = 0; c < img1.cols; c++)
				//	{
				//		float pixel = img1.at<uchar>(r,c) ;
				//		pixel = pixel / 255;

				//		outputFile << pixel << '\t';
				//	}
				//	outputFile << endl;
				//}

				//outputFile.close();					
			}else{
				break;
			}
				
			c++;
				
			if (cvWaitKey(1)>= 0) break;
		}				
	}
}

Mat detectAndDisplay( Mat frame )
{
	std::vector<Rect> faces;
	Mat frame_gray;
   
	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
   
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	for( size_t i = 0; i < faces.size(); i++ ){ 
		return frame_gray( faces[i] );
	}
}