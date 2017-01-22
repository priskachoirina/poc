#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <iomanip>
#include <sstream>

using namespace dlib;
using namespace std;

void saveImage(cv::Mat temp_jaw ,cv::Mat temp_eye ,cv::Mat temp_mouth , int frame){
	string name1 = "Frame_"+to_string(frame)+"_jaw.png";
	string name2 = "Frame_"+to_string(frame)+"_eye.png";
	string name3 = "Frame_"+to_string(frame)+"_mouth.png";
					
	cv::imwrite("img\\" + name1,temp_jaw);
	cv::imwrite("img\\" + name2,temp_eye);
	cv::imwrite("img\\" + name3,temp_mouth);
}

void savePixel(cv::Mat img, string filename){
	ofstream outputFile;

	outputFile.open(filename);

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			float R = img.at<cv::Vec3b>(r,c)[0];
			float G = img.at<cv::Vec3b>(r,c)[1];
			float B = img.at<cv::Vec3b>(r,c)[2];

			float Gray = (R+G+B)/3;

			outputFile << "[" << Gray << "]" << ",";
		}
		outputFile << endl;
	}

	outputFile.close();		
}

int main(int argc, char** argv)
{
	cout << argc << endl;
	if (argc != 2)
    {
		
        cout << "Untuk menjalankan program ini: " << endl;
		cout << "poc [lokasi video] " << endl;
		cout << "contoh : poc E:\PENTING\Kerja\Face-BuRosi\Baru\Data\CASME2\CASME2_video\CASME2_compressed\sub01\EP19_03f.avi " << endl;
		cout << "Nama folder lokasi video tidak boleh ada spasi misal : [Face Burosi]"<<endl;
        cout << "Jika kolom video tidak diisi otomatis akan memanggil video yg berada di folder video/EP01_01.mp4" << endl;
        return 1;
    }else{
		try
		{
			string file_video = (argv[1] == "" ? "video\\EP01_01.mp4" : argv[1]);

			cv::VideoCapture cap(file_video);
			if (!cap.isOpened())
			{
				cerr << "Unable to connect to camera" << endl;
				return 1;
			}

			image_window win;

			frontal_face_detector detector = get_frontal_face_detector();
			shape_predictor pose_model;
			deserialize("landmark\\68.dat") >> pose_model;

			int no =0 ; 
			while(!win.is_closed())
			{
				// Grab a frame
				cv::Mat temp;
				cap >> temp;

				if(!temp.empty()){
					cv_image<bgr_pixel> cimg(temp);

					// Detect faces 
					std::vector<rectangle> faces = detector(cimg);
					// Find the pose of each face.
					std::vector<full_object_detection> shapes;

					for (unsigned long i = 0; i < faces.size(); ++i){

						full_object_detection shape = pose_model(cimg, faces[i]);

						int sum = 13;
						dlib::rectangle jaw(shape.part(17).x(),shape.part(24).y() - sum,shape.part(26).x(),shape.part(26).y() + sum);
						dlib::rectangle eye(shape.part(36).x() -sum ,shape.part(37).y() - sum,shape.part(45).x() + sum,shape.part(46).y() + sum);
						dlib::rectangle mouth(shape.part(48).x() -sum ,shape.part(50).y() - sum,shape.part(54).x() + sum,shape.part(57).y());

						cv::Rect jaws(jaw.left(), jaw.top(),(jaw.right() - jaw.left()),(jaw.bottom() - jaw.top()));
						cv::Rect eyes(eye.left(), eye.top(),(eye.right() - eye.left()),(eye.bottom() - eye.top()));
						cv::Rect mouths(mouth.left(), mouth.top(),(mouth.right() - mouth.left()),(mouth.bottom() - mouth.top()));

						cv::Mat temp_jaw = temp.clone();
						cv::Mat temp_eye = temp.clone();
						cv::Mat temp_mouth = temp.clone();

						temp_jaw	= temp_jaw(jaws);
						temp_eye	= temp_eye(eyes);
						temp_mouth	= temp_mouth(mouths);
					
						savePixel(temp_jaw, "CSV\\jaw_"+ to_string(no) +".csv");
						savePixel(temp_jaw, "CSV\\eye_"+ to_string(no) +".csv");
						savePixel(temp_jaw, "CSV\\mouth_"+ to_string(no) +".csv");

						saveImage(temp_jaw , temp_eye , temp_mouth , no);
					
						cv::rectangle(temp,jaws, cv::Scalar( 100, 155, 25 ), 1, 1 );
						cv::rectangle(temp,eyes, cv::Scalar( 100, 155, 25 ), 1, 1 );
						cv::rectangle(temp,mouths, cv::Scalar( 100, 155, 25 ), 1, 1 );

						shapes.push_back(pose_model(cimg, faces[i]));
					}

					win.clear_overlay();
					win.set_image(cimg);
					//win.add_overlay(faces, rgb_pixel(255,0,0)); // facial detection shape
					//win.add_overlay(render_face_detections(shapes)); // landmark shape
					no++;
				}
			}
		}
		catch(serialization_error& e)
		{
			cout << endl << e.what() << endl;
		}
		catch(exception& e)
		{
			cout << e.what() << endl;
		}
	}
}

