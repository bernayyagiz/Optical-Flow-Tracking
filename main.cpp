#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp" //Nokta takibi için gerekli modül
#include <iostream>

using namespace std;
using namespace cv;

Mat img; 
Mat grimgYeni, grimgEski;

Point2f nokta;
bool tiklama = false;//Görüntüye nokta eklenip eklenmediðini göstermek için

vector<Point2f>eskiNokta, yeniNokta; 
vector<uchar> durum; //Görüntüde nokta olup olmadýðýný kontrol etmek için
vector<float> hata; //Hesaplama hatasý

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
	VideoCapture video(0); //Kamerayý açmak için
	if (!video.isOpened()) //Kameranýn açýlýp açýlmadýðýný kontrol etmek için
	{
		cout << "Kamera açýlmadý." << endl;
	}

	while (true) 
	{
		if (!tiklama)
		{
			video>>img;        //Video görüntüye çerçeveler þeklinde okunur.		
			flip(img, img, 1); //Çerçevenin yatay simetrisi alýnýr.

			//Ýþlemi hýzlandýrmak için görüntü grileþtirilir.
			cvtColor(img, grimgYeni, CV_BGR2GRAY);
		}

		grimgEski = grimgYeni.clone(); //Görüntü kopyalanýr. 
		//Kopyalanan görüntü için iþlem tekrarlanýr.
		video>>img;      

		flip(img, img, 1); 

		
		cvtColor(img, grimgYeni, CV_BGR2GRAY);

		
		if (tiklama) {
			//Týklanan nokta yeniden aranýr.
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