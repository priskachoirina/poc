#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"

using namespace dlib;
using namespace std;
using namespace cv;
// ----------------------------------------------------------------------------------------

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN )
    {
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
}

int mainssss(int argc, char** argv)
{  
    try
    {
        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }
		cout << argv[0];
		cout <<"\n";
		cout << argv[1] << endl;
     
        frontal_face_detector detector = get_frontal_face_detector();
    
        shape_predictor sp;
        deserialize(argv[1]) >> sp;

        
        // Loop over all the images provided on the command line.
        for (int i = 2; i < argc; ++i)
        {
            cout << "processing image " << argv[i] << endl;
            array2d<rgb_pixel> img;
            load_image(img, argv[i]);
            // Make the image larger so we can detect small faces.
            pyramid_up(img);
			
            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces in the image.
            std::vector<dlib::rectangle> dets = detector(img);
            cout << "Number of faces detected: " << dets.size() << endl;
			
            // Now we will go ask the shape_predictor to tell us the pose of
            // each face we detected.

			// min height : 375 Landmark 20
			// max height : 917 landmark 9

			// min width : 494  Landmark 1
			// max width : 679 landmark 9

			//rectangle (494, 375, (679-494),(917-375));
			dlib::rectangle roi;
			/*roi.set_left(320);
			roi.set_right(957);
			roi.set_top(359);
			roi.set_bottom(917);*/
			
			cout<< "width : " << (679-494) << endl;
			cout<< "height : " << (917-375) << endl;
            std::vector<full_object_detection> shapes;
            for (unsigned long j = 0; j < dets.size(); ++j)
			{				
                full_object_detection shape = sp(img, dets[j]);
				//dlib::draw_line(img,point(roi.left(),roi.top()),point(roi.right(),roi.top()),dlib:: rgb_pixel(255,255, 0)); //atas
				//dlib::draw_line(img,point(roi.left(),roi.bottom()),point(roi.right(),roi.bottom()),dlib:: rgb_pixel(255,255, 0)); //bawah

				//dlib::draw_line(img,point(roi.left(),roi.top()),point(roi.left(),roi.bottom()),dlib:: rgb_pixel(255,255, 0)); //kiri
				//dlib::draw_line(img,point(roi.right(),roi.top()),point(roi.right(),roi.bottom()),dlib:: rgb_pixel(255,255, 0)); //kanan

				roi.set_left(shape.part(0).x());
				roi.set_right(shape.part(16).x());
				roi.set_top(shape.part(25).y());
				roi.set_bottom(shape.part(8).y());

				dlib::draw_rectangle(img, roi, dlib::rgb_pixel(255, 0, 0), 1);
				

				//dlib::draw_rectangle(img,roi, dlib::rgb_pixel(255, 0, 0), 1);

				/*Show Landmark By Pixel 
				const full_object_detection& d = shapes[0];
                for (unsigned long k = 0; k < shape.num_parts(); ++k)
                {
					point p1(shape.part(k).x(),shape.part(k).y());
					point p2(957,433);
					if(k==1){
						dlib::draw_line(img, p1 ,p2, dlib:: rgb_pixel(255,255, 0));
					}else{
						dlib::draw_line(img, p1 ,p1, dlib:: rgb_pixel(255,255, 255));
					}
					
					//dlib::draw_line(img,p1,p2,dlib::rgb_pixel(255, 245, 0));
                }*/

				
				
				//long hori = ((dets[j].left()+dets[j].right())/2) + 20; // as x
				//long vert = ((dets[j].top()+dets[j].bottom())/2); // as y
				//
				//cout << "number of parts: "<< shape.num_parts() << endl;
				//cout << "width : " << to_string(dets[j].width()) << endl;
				//cout << "height : " << to_string(dets[j].height()) << endl;
				//cout << "left : " << to_string(dets[j].left()) << endl;
				//cout << "right : " << to_string(dets[j].right()) << endl;
				//cout << "bottom : " << to_string(dets[j].bottom()) << endl;
				//cout << "top : " << to_string(dets[j].top()) << endl;

				//dlib::draw_rectangle(img, dets[j], dlib::rgb_pixel(255, 0, 0), 1);
				//dlib::draw_line(img, point(hori,dets[j].top()) ,point(hori,dets[j].bottom()) ,  dlib::rgb_pixel(255, 245, 0));
				//dlib::draw_line(img, point(dets[j].left(),vert) ,point(dets[j].right(),vert) ,  dlib::rgb_pixel(255, 245, 0));
				
                shapes.push_back(shape);

				/* save point in file */
				/*std::stringstream points_filename;
                std::ofstream ofs;
                
                if ( j == 0 )
                {
                    points_filename <<  "E:/PENTING/Kerja/Face-BuRosi/Baru/Data/CASME2/CroppedFrame/sub01/0.txt";
                }else
                {
                    points_filename << "E:/PENTING/Kerja/Face-BuRosi/Baru/Data/CASME2/CroppedFrame/sub01/0_"  << j << ".txt";
                }
                
                ofs.open(points_filename.str().c_str());
                const full_object_detection& d = shapes[0];
                for (unsigned long k = 0; k < shape.num_parts(); ++k)
                {
                    ofs << shape.part(k).x() << " " << shape.part(k).y() << endl;
                    
                }
                ofs.close();*/
				/* end of save point in file */

            }

            // Now let's view our face poses on the screen.
			image_window win(img,"Image Landmark");
            win.clear_overlay();
            win.set_image(img);
			
            win.add_overlay(render_face_detections(shapes));
			//win.add_overlay(dets, rgb_pixel(255,0,0));

			point p;
			while (win.get_next_double_click(p))
			{
				cout << "User double clicked on pixel:" << p << endl;
			}

            /*dlib::array<array2d<rgb_pixel> > face_chips;
            extract_image_chips(img, get_face_chip_details(shapes), face_chips);
            win_faces.set_image(tile_images(face_chips));*/
			
            cout << "Hit enter to process the next image..." << endl;
            cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------------

