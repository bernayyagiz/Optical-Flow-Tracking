#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp" //Nokta takibi i�in gerekli mod�l
#include <iostream>

using namespace std;
using namespace cv;

Mat img; 
Mat grimgYeni, grimgEski;

Point2f nokta;
bool tiklama = false;//G�r�nt�ye nokta eklenip eklenmedi�ini g�stermek i�in

vector<Point2f>eskiNokta, yeniNokta; 
vector<uchar> durum; //G�r�nt�de nokta olup olmad���n� kontrol etmek i�in
vector<float> hata; //Hesaplama hatas�

void Fare(int event, int x, int y, int /*flags*/, void* /*param*/)
    {
        if (event == EVENT_LBUTTONDOWN)
        {
            nokta = Point2f((float)x, (float)y);
			eskiNokta.push_back(nokta);
            tiklama = true;
        }
    }

int main()
{
	VideoCapture video(0); //Kameray� a�mak i�in
	if (!video.isOpened()) //Kameran�n a��l�p a��lmad���n� kontrol etmek i�in
	{
		cout << "Kamera a��lmad�." << endl;
	}

	while (true) 
	{
		if (!tiklama)
		{
			video>>img;        //Video g�r�nt�ye �er�eveler �eklinde okunur.		
			flip(img, img, 1); //�er�evenin yatay simetrisi al�n�r.

			//��lemi h�zland�rmak i�in g�r�nt� grile�tirilir.
			cvtColor(img, grimgYeni, CV_BGR2GRAY);
		}

		grimgEski = grimgYeni.clone(); //G�r�nt� kopyalan�r. 
		//Kopyalanan g�r�nt� i�in i�lem tekrarlan�r.
		video>>img;      

		flip(img, img, 1); 

		
		cvtColor(img, grimgYeni, CV_BGR2GRAY);

		
		if (tiklama) {
			//T�klanan nokta yeniden aran�r.
			calcOpticalFlowPyrLK(grimgEski, grimgYeni, eskiNokta, yeniNokta, durum, hata);

			for (int i = 0; i < eskiNokta.size(); i++)
			{
				
				line(img, yeniNokta[i], eskiNokta[i], Scalar(0, 0, 255), 3, 8, 0);

				eskiNokta[i] = yeniNokta[i]; 
			}
		}

		imshow("Pencere", img);
		setMouseCallback("Pencere", Fare, NULL);

		if (waitKey(20) == 27) 
		{
			break;
		}
	}
	namedWindow("Pencere", CV_WINDOW_AUTOSIZE);
    return 0;
}