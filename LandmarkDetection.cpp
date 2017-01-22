#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;


int maintss()
{
    try
    {
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        image_window win;

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("E:\\shape_predictor_68_face_landmarks.dat") >> pose_model;

        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;
            cap >> temp;
            cv_image<bgr_pixel> cimg(temp);

            // Detect faces 
            std::vector<rectangle> faces = detector(cimg);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i){

				/*full_object_detection shape = pose_model(cimg, faces[i]);
                cout << "number of parts: "<< shape.num_parts() << endl;
                cout << "pixel position of first part:  " << shape.part(0) << endl;
                cout << "pixel position of second part: " << shape.part(1) << endl;*/

                shapes.push_back(pose_model(cimg, faces[i]));
			}

            win.clear_overlay();
            win.set_image(cimg);
			win.add_overlay(faces, rgb_pixel(255,0,0)); // facial detection shape
            //win.add_overlay(render_face_detections(shapes)); // landmark shape
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