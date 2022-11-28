// openCV.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
// openCVTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <string.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"
struct ans {
    int x, y;
};
using namespace cv;
using namespace std;

#define EPSILON 1E-5
int WIDTH = 100;
int HEIGHT = 240;
int RANK_WIDTH = 100;
int RANK_HEIGHT = 115;
int SUIT_WIDTH = 100;
int SUIT_HEIGHT = 105;

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}
int matchRank(Mat rnk) {
    int qualities[13];
    int maxindex = 0;
    Mat img;
    Mat dst;
    Mat rnk_c;
    cvtColor(rnk, rnk_c, COLOR_BGR2GRAY);
    string file[] = {"1.jpg","2.jpg","3.jpg","4.jpg","5.jpg","6.jpg","7.jpg","8.jpg","9.jpg","10.jpg","11.jpg","12.jpg","0.jpg" };
    threshold(rnk_c, rnk, 97, 255, 1);

    img = imread(file[0]);
    
    vector<Mat> vec;

    for (int i = 0; i < 13; i++) {
        int match_quality = 0;
        img = imread(file[i]);
        resize(img, img, rnk.size(), 0, 0, cv::INTER_NEAREST);

        cvtColor(img, img, COLOR_BGR2GRAY);
        //imshow("Img" + to_string(i), img);
       


        absdiff(rnk, img, dst);

        imshow("dst" + to_string(i), dst);
        for (int y = 0; y < dst.rows; y++)
        {
            for (int x = 0; x < dst.cols; x++)
            {
                if (dst.at<uchar>(y, x) == 0) {
                    match_quality++;
                }
            }
        }
        qualities[i] = match_quality;
        //cout << "White Pixels in AbsDiff_Output: " << match_quality << "\n";
         // imshow("Train Images", img);
         // imshow("Diff", dst);
          //waitKey(1);
    }
    int max = qualities[0];
    maxindex = 0;
    for (int j = 0; j < 13; j++)
    {
       // cout << to_string(j) + " Qualitie: " << qualities[j] << " ";
        if (qualities[j] > max)
        {
            max = qualities[j];
            maxindex = j;
        }
    }
    maxindex++;
    //cout << "Minimum White Pixel on Position: " << maxindex;
    return maxindex;
}
int matchSuit(Mat suit) {
    int qualities[4];
    int maxindex = 0;
    Mat img;
    Mat dst;
    Mat suit_c;
    string file[] = { "Hearts.jpg","Diamonds.jpg","Clubs.jpg","Spades.jpg" };
  
    cvtColor(suit, suit_c, COLOR_BGR2GRAY);
    vector<Mat> vec;
    threshold(suit_c, suit, 97, 255, 1);
    imshow("Something", suit);
    for (int i = 0; i < 4; i++) {
        int match_quality = 0;
        img = imread(file[i]);
        resize(img, img, suit.size(), 0, 0, cv::INTER_NEAREST);

        cvtColor(img, img, COLOR_BGR2GRAY);
        //imshow("Img" + to_string(i), img);
       // resize(suit, suit, Size(SUIT_HEIGHT, SUIT_WIDTH), 0, 0, cv::INTER_NEAREST);


        absdiff(suit, img, dst);

       // imshow("dst" + to_string(i), dst);
        for (int y = 0; y < dst.rows; y++)
        {
            for (int x = 0; x < dst.cols; x++)
            {
                if (dst.at<uchar>(y, x) == 0) {
                    
                    match_quality++;
                }
               
            }
            cout << endl;
        }
        
        qualities[i] = match_quality;
       //cout << "White Pixels in AbsDiff_Output: " << match_quality << "\n";
        // imshow("Train Images", img);
         //imshow("Diff"+to_string(i), dst);
         //waitKey(1);
    }
    int max = qualities[0];
    maxindex = 0;
    for (int j = 0; j < 4; j++)
    {
        cout << to_string(j)+ " Qualitie: " << qualities[j] << " ";
        if (qualities[j] > max)
        {
            max = qualities[j];
            maxindex = j;
        }
    }

    //cout << "Minimum White Pixel on Position: " << maxindex;
    return maxindex;
}
int findRANK(Mat r) {
    /*vector<vector<Point> > r_cnts;
    vector<vector<Point> > s_cnts;

    Rect r_boundbox;
    Rect s_boundbox;

    Mat r_copy = r.clone();
    Mat s_copy = s.clone();
    Mat pyr0, timg0, gray0(s.size(), CV_8U),gray;
    pyrDown(s, pyr0, Size(s.cols / 2, s.rows / 2));
    pyrUp(pyr0, timg0, s.size());
    for (int c = 0; c < 3; c++) {
        int ch[] = { c, 0 };
        mixChannels(&timg0, 1, &gray0, 1, ch, 1);
        Canny(gray0, gray, 0, 50, 5);
    }
    Mat pyr1, timg1, gray10(r.size(), CV_8U), gray1;
    pyrDown(r, pyr1, Size(r.cols / 2, r.rows / 2));
    pyrUp(pyr1, timg1, r.size());
    for (int c = 0; c < 3; c++) {
        int ch[] = { c, 0 };
        mixChannels(&timg1, 1, &gray10, 1, ch, 1);
        Canny(gray10, gray1, 0, 50, 5);
    }
    findContours(gray, s_cnts, RETR_TREE, CHAIN_APPROX_SIMPLE);
    sort(s_cnts.begin(), s_cnts.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
        return contourArea(c1, false) < contourArea(c2, false);
        });
    cout << "Hello\n";
    s_boundbox = boundingRect(s_cnts[s_cnts.size() - 1]);
    cout << "Hello\n";
    findContours(gray1, r_cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    cout << "Hello\n";
    sort(r_cnts.begin(), r_cnts.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
        return contourArea(c1, false) < contourArea(c2, false);
        });
    cout << r_cnts.size() << endl;
    r_boundbox = boundingRect(r_cnts[r_cnts.size() - 1]);
   
    Mat rr, ss;
    //imshow("Window", s_cnts);
    rr = r(r_boundbox);
    ss = s(s_boundbox);

    resize(rr, rr, Size(RANK_HEIGHT, RANK_WIDTH));
    resize(ss, ss, Size(SUIT_HEIGHT, SUIT_WIDTH));
    imshow("SUITROI1", ss);
    imshow("SUITROI2", rr);
    int Ss;
    int Rr;
    Ss=matchSuit(ss);
    Rr = matchRank(rr);
    cout << "SS: "<<Ss << endl;
    return { Ss,Rr};*/
    Mat canny_output;
    Mat src=r;
 
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    blur(src_gray, src_gray, Size(3, 3));

    Canny(src_gray, canny_output, 100, 100 * 2);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());
    double area = 0;
    int idx=0;
    vector<Point> approx;

    for (int i = 0; i < contours_poly.size(); i++) {
        approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
        if (area < fabs(contourArea(approx))) {
            idx = i;
            area = contourArea(approx);
        }
    }
    cout << "IDX: " << idx << endl;
    approxPolyDP(contours[idx], approx, arcLength(contours[idx], true) * 0.02, true);
    Rect rect = boundingRect(approx);
    Point pt1, pt2;
    pt1.x = rect.x;
    pt1.y = rect.y;
    pt2.x = rect.x + rect.width;
    pt2.y = rect.y + rect.height;
    Mat r_bnd= Mat::zeros(Size(pt2.x - pt1.x, pt2.y - pt1.y), CV_8UC3);
    cout << "IDX: " << idx << endl;
    cout << pt2.y << endl;
    for (int i = pt1.y; i < pt2.y; i++) {
        for (int j = pt1.x; j < pt2.x; j++) {
   
            Vec3b bgrPix = r.at<Vec3b>(i, j);
            r_bnd.at<Vec3b>(i - pt1.y, j - pt1.x) = bgrPix;
        }
    }
    imshow("RBND", r_bnd);
    int Rr = matchRank(r_bnd);
    return Rr;
}
int findSuit(Mat s) {
    Mat canny_output;
    Mat src = s;

    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    Canny(src_gray, canny_output, 100, 100 * 2);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());
    double area = 0;
    int idx = 0;
    vector<Point> approx;

    for (int i = 0; i < contours_poly.size(); i++) {
        approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
        if (area < fabs(contourArea(approx))) {
            idx = i;
            area = contourArea(approx);
        }
    }
    approxPolyDP(contours[idx], approx, arcLength(contours[idx], true) * 0.02, true);
    Rect rect = boundingRect(approx);
    Point pt1, pt2;
    pt1.x = rect.x;
    pt1.y = rect.y;
    pt2.x = rect.x + rect.width;
    pt2.y = rect.y + rect.height;
    Mat r_bnd = Mat::zeros(Size(pt2.x - pt1.x, pt2.y - pt1.y), CV_8UC3);
    for (int i = pt1.y; i < pt2.y; i++) {
        for (int j = pt1.x; j < pt2.x; j++) {

            Vec3b bgrPix = s.at<Vec3b>(i, j);
            r_bnd.at<Vec3b>(i - pt1.y, j - pt1.x) = bgrPix;
        }
    }
    imshow("SBND", r_bnd);
    int Ss = matchSuit(r_bnd);
    return Ss;
}
string putTextString2(int mm)
{
    string Result;
    if (mm == 0)
    {
        Result = "hearts";
    }
    else	if (mm == 1)
    {
        Result = "diamonds";
    }
    else	if (mm == 2)
    {
        Result = "clovers";
    }
    else	if (mm == 3)
    {
        Result = "spades";
    }

    return Result;
}
string putTextString1(int mm)
{
    string Result;
    if (mm == 13)
    {
        Result = "10";
    }
    else	if (mm == 1)
    {
        Result = "Ace";
    }
    else	if (mm == 2)
    {
        Result = "2";
    }
    else	if (mm == 3)
    {
        Result = "3";
    }
    else	if (mm == 3)
    {
        Result = "3";
    }
    else	if (mm == 4)
    {
        Result = "4";
    }
    else	if (mm == 4)
    {
        Result = "4";
    }
    else	if (mm == 5)
    {
        Result = "5";
    }
    else	if (mm == 6)
    {
        Result = "6";
    }
    else	if (mm == 7)
    {
        Result = "7";
    }
    else	if (mm == 8)
    {
        Result = "8";
    }
    else	if (mm == 9)
    {
        Result = "9";
    }
    else	if (mm == 10)
    {
        Result = "Jack";
    }
    else	if (mm == 11)
    {
        Result = "Queen";
    }
    else	if (mm == 12)
    {
        Result = "King";
    }
    return Result;
}
static vector<Mat> findSquares(const Mat& image, vector<vector<Point> >& squares)
{
    vector<Mat> v;
    Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    pyrDown(image, pyr, Size(image.cols / 2, image.rows / 2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;
    for (int c = 0; c < 3; c++)
    {
        int ch[] = { c, 0 };
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        for (int l = 0; l < 1; l++)
        {

            if (l == 0)
            {

                Canny(gray0, gray, 0, 50, 5);

                //dilate(gray, gray, Mat(), Point(-1, -1));
            }
            else
            {

                gray = gray0 >= (l + 1) * 255 / 2;
            }

            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            vector<Point> approx;
            for (size_t i = 0; i < contours.size(); i++)
            {

                approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);

                if (approx.size() == 4 &&
                    fabs(contourArea(approx)) > 100000 &&
                    isContourConvex(approx))
                {
                    double maxCosine = 0;
                    for (int j = 2; j < 5; j++)
                    {
                        double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    if (maxCosine < 0.3) {
                        squares.push_back(approx);
                        for (int i = 0; i < approx.size(); i++) {
                            cout << approx[i] << " ";
                        }
                        cout << endl;
                        Mat card;
                        std::vector<cv::Point2f> quad_pts;
                        int min_x = 1000000;
                        int min_y = 1000000;
                        int max_x = 0;
                        int max_y = 0;
                        for (int i = 0; i < 4; i++) {
                                min_x = MIN(fabs(approx[i].x), min_x);
                                min_y = MIN(fabs(approx[i].y), min_y);
                                max_x = MAX(fabs(approx[i].x), max_x);
                                max_y = MAX(fabs(approx[i].y), max_y);
                            
                        }
                        int width = max_x - min_x;
                        int height = max_y - min_y;

                        Mat quad = cv::Mat::zeros((width), (height), CV_8UC3);
                    
                        Mat(approx).convertTo(approx, CV_32S);
                        Mat img = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
                        for (int i = min_y; i < max_y; i++) {
                            for (int j = min_x; j < max_x; j++) {
                              
                                Vec3b bgrPix = image.at<Vec3b>(i, j);
                                img.at<Vec3b>(i-min_y, j-min_x) = bgrPix;
                            }
                        }
                        cout << endl;
                        v.push_back(img);
                        
                        //Mat transmtx=getPerspectiveTransform(approx,quad_pts);
                        
                        //warpPerspective(image, quad, transmtx, quad.size());
                       
                    }
                }
            }
        }
    }
    return v;
}

void getCardRank(Mat img) {
    Mat rnk = cv::Mat::zeros((RANK_HEIGHT), (RANK_WIDTH), CV_8UC3);
    Mat suit = cv::Mat::zeros((SUIT_HEIGHT), (SUIT_WIDTH), CV_8UC3);
    cout << "Size: " << img.size() << endl;
    for (int i = 0; i < RANK_HEIGHT; i++) {
        for (int j = 0; j < RANK_WIDTH; j++) {
            Vec3b bgrPix = img.at<Vec3b>(i, j);
            rnk.at<Vec3b>(i, j) = bgrPix;
        }
    }
    for (int i = HEIGHT/2; i < HEIGHT / 2+SUIT_HEIGHT; i++) {
        for (int j = 0; j < SUIT_WIDTH; j++) {
            Vec3b bgrPix = img.at<Vec3b>(i, j);

            suit.at<Vec3b>(i - HEIGHT/2, j) = bgrPix;
        }
    }
    
    imshow("Rank", rnk);
    imshow("SUIT", suit);
    enum rank {
        Hearts = 0,
        Diamonds = 1,
        Clovers = 2,
        Spades = 3
    };
  
    int a = findRANK(rnk);
    int b = findSuit(suit);
    string Suit;
    Suit=putTextString2(b);
    string Rank;
    Rank = putTextString1(a);
    putText(img, Suit, Point(30, 150), 2, 0.8, Scalar(155, 155, 0), 2, 8);
    putText(img, Rank, Point(180, 50), 2, 0.8, Scalar(155, 155, 0), 2, 8);
    imshow("AAAAA", img);
}


    

int main()
{
	Mat img = imread("test3.jpg");
    vector<vector<Point>> vec;
    vector<Mat> v;
    v=findSquares(img, vec);
    imshow("Selected contours", img);
   
    polylines(img, vec, true, Scalar(0, 255, 0), 3, LINE_AA);
    putText(img, to_string(vec.size()), cv::Point(50, 50), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 0), 2, false);
	//imshow("Selected contours", img);
    if (!v.empty()) {

        getCardRank(v[0]);
    }
	waitKey(0);
}
