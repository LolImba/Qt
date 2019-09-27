#include "filterAlgorithms.h"

#include <QPixmap>
#include <QRgb>
#include <QtMath>

FilterAlgorithms::FilterAlgorithms()
{
}

QImage& FilterAlgorithms::setUpBlurAlgorithm(QImage &img)
{
    int width = img.width();
    int height = img.height();
    createKernel(Settings::kernelRadius);
    int radius = Settings::kernelRadius * 2 + 1;
    //QImage image = img.toImage();
    auto imageCopy = QImage(img);
   // QRgb **pixels = new QRgb*[height];
  //  QVector<QRgb*> lines(height);
    pixels = new QRgb*[height];
    for (int i = 0; i < height; i++) {
        pixels[i] = reinterpret_cast<QRgb *>(imageCopy.scanLine(i));
    }
    for (int i = Settings::kernelRadius; i < height - Settings::kernelRadius; i++) {
      //  QRgb* line = reinterpret_cast<QRgb *>(imageCopy.scanLine(i));

       // auto pixels = image.scanLine(0);
        for (int j = Settings::kernelRadius; j < width - Settings::kernelRadius; j++) {
//            QRgb color = line[j];
//            QRgb leftPixel = line[j - 1];
//            QRgb rightPixel = line[j + 1];
          //  QRgb color = setBlurPixelColor(line[j], line[j - 1], line[j + 1]);
            img.setPixelColor(j, i, getBlurPixelColor(radius, i, j, true));
            img.setPixelColor(j, i, getBlurPixelColor(radius, i, j, false));
//            pixels[i][j] = line[j];
//            QRgb color = imageCopy.pixel(i,j);
//            QRgb leftPixel = imageCopy.pixel(i - 1, j);
//            QRgb rightPixel = imageCopy.pixel(i + 1, j);
//            color = setBlurPixelColor(color, leftPixel, rightPixel);
//            image.setPixelColor(i, j, color);
        }
    }
//    imageCopy = QImage(image);
//    for (int i = 1; i < height - 1; i++) {
//        for (int j = 0; j < width; j++) {
//            QRgb color = imageCopy.pixel(i,j);
//            QRgb UpPixel = imageCopy.pixel(i - 1, j);
//            QRgb DownPixel = imageCopy.pixel(i + 1, j);
//            color = setBlurPixelColor(color, UpPixel, DownPixel);
//            image.setPixelColor(i, j, color);
//        }
//    }
    return img;
}

QImage &FilterAlgorithms::setUpSobelAlgorithm(QImage &img)
{
    int width = img.width();
    int height = img.height();
    auto imageCopy = QImage(img);
    pixels = new QRgb*[height];
    for (int i = 0; i < height; i++) {
        pixels[i] = reinterpret_cast<QRgb *>(imageCopy.scanLine(i));
    }
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int redGx = qRed(pixels[i + 1][j - 1]) + qRed(pixels[i + 1][j])* 2 + qRed(pixels[i + 1][j + 1]) -
                    qRed(pixels[i - 1][j - 1]) - qRed(pixels[i - 1][j])*2 - qRed(pixels[i - 1][j + 1]);
            int blueGx = qBlue(pixels[i + 1][j - 1]) + qBlue(pixels[i + 1][j])* 2 + qBlue(pixels[i + 1][j + 1]) -
                    qBlue(pixels[i - 1][j - 1]) - qBlue(pixels[i - 1][j])*2 - qBlue(pixels[i - 1][j + 1]);
            int greenGx = qGreen(pixels[i + 1][j - 1]) + qGreen(pixels[i + 1][j])* 2 + qGreen(pixels[i + 1][j + 1]) -
                    qGreen(pixels[i - 1][j - 1]) - qGreen(pixels[i - 1][j])*2 - qGreen(pixels[i - 1][j + 1]);

            int redGy = qRed(pixels[i - 1][j + 1]) + qRed(pixels[i][j + 1])* 2 + qRed(pixels[i + 1][j + 1]) -
                    qRed(pixels[i - 1][j - 1]) - qRed(pixels[i][j - 1])* 2 - qRed(pixels[i + 1][j - 1]);
            int blueGy = qBlue(pixels[i - 1][j + 1]) + qBlue(pixels[i][j + 1])* 2 + qBlue(pixels[i + 1][j + 1]) -
                    qBlue(pixels[i - 1][j - 1]) - qBlue(pixels[i][j - 1])* 2 - qBlue(pixels[i + 1][j - 1]);
            int greenGy = qGreen(pixels[i - 1][j + 1]) + qGreen(pixels[i][j + 1])* 2 + qGreen(pixels[i + 1][j + 1]) -
                    qGreen(pixels[i - 1][j - 1]) - qGreen(pixels[i][j - 1])* 2 - qGreen(pixels[i + 1][j - 1]);

            img.setPixelColor(j, i, qRgb(sqrt(pow(redGx, 2) + pow(redGy, 2)), sqrt(pow(greenGx, 2) + pow(greenGy, 2)), sqrt(pow(blueGx, 2) + pow(blueGy, 2))));
           // img.setPixelColor(j, i, getBlurPixelColor(radius, i, j, true));
            //img.setPixelColor(j, i, getBlurPixelColor(radius, i, j, false));
        }
    }
    return img;
}

void FilterAlgorithms::createKernel(int r)
{
    switch (r) {
    case 1: {
        //double temp[] = {0.1, 0.8, 0.1};
        kernel = new double[3];
        kernel[0] = 0.1;
        kernel[1] = 0.8;
        kernel[2] = 0.1;
        break;
    }
        //return temp;
    case 2:{
        //double temp[] = {0.08, 0.08, 0.68, 0.08, 0.08};
        kernel = new double[5];
        kernel[0] = 0.08;
        kernel[1] = 0.08;
        kernel[2] = 0.68;
        kernel[3] = 0.08;
        kernel[4] = 0.08;
        break;
    }
    case 3:{
        //double temp[] = {0.07, 0.07, 0.07, 0.58, 0.07, 0.07, 0.07};
        kernel = new double[7];
        kernel[0] = 0.09;
        kernel[1] = 0.09;
        kernel[2] = 0.09;
        kernel[3] = 0.46;
        kernel[4] = 0.09;
        kernel[5] = 0.09;
        kernel[6] = 0.09;
        break;
//    case 10:{
//        kernel = new double[21];
//        kernel[0] = 0.03;
//        kernel[1] = 0.03;
//        kernel[2] = 0.03;
//        kernel[3] = 0.03;
//        kernel[4] = 0.03;
//        kernel[5] = 0.03;
//        kernel[6] = 0.04;
//        kernel[7] = 0.04;
//        kernel[8] = 0.06;
//        kernel[9] = 0.08;
//        kernel[10] = 0.2;
//        kernel[11] = 0.08;
//        kernel[12] = 0.06;
//        kernel[13] = 0.04;
//        kernel[14] = 0.04;
//        kernel[15] = 0.03;
//        kernel[16] = 0.03;
//        kernel[17] = 0.03;
//        kernel[18] = 0.03;
//        kernel[19] = 0.03;
//        kernel[20] = 0.03;
//        break;
//        }
    }
    default:{
        double temp[] = {0.1, 0.8, 0.1};
        kernel = temp;
        break;
    }
    }
}

QRgb FilterAlgorithms::getBlurPixelColor(int kernelRadius, int pixelI, int pixelJ, bool lineMode)
{
    int red = 0;
    int blue = 0;
    int green = 0;
    int indent = Settings::kernelRadius * (-1);
    if(lineMode){
        for (int i = 0; i < kernelRadius; i++) {
            red += (double)qRed(pixels[pixelI][pixelJ + indent]);
            //red += (double)qRed(pixels[pixelI][pixelJ + indent])*kernel[i];
            blue += (double)qBlue(pixels[pixelI][pixelJ + indent])*kernel[i];
            green += (double)qGreen(pixels[pixelI][pixelJ + indent])*kernel[i];
            indent++;
        }
        return qRgb(red, green, blue);
    }
    for (int i = 0; i < kernelRadius; i++) {
        red += (double)qRed(pixels[pixelI + indent][pixelJ])*kernel[i];
        blue += (double)qBlue(pixels[pixelI + indent][pixelJ])*kernel[i];
        green += (double)qGreen(pixels[pixelI + indent][pixelJ])*kernel[i];
        indent++;
    }
    return qRgb(red, green, blue);
//    for(auto& blurRatio : kernel)
//    {
//        red += (double)qRed(color)*blurRatio;
//        blue += (double)qBlue(color)*blurRatio;
//        green += (double)qGreen(color)*blurRatio;
//    }
//    int red = (double)qRed(color)* blur3[1] + (double)qRed(leftPixel) * blur3[0] + (double)qRed(rightPixel) * blur3[2];
//    int blue = (double)qBlue(color)* blur3[1] + (double)qBlue(leftPixel) * blur3[0] + (double)qBlue(rightPixel) * blur3[2];
//    int green = (double)qGreen(color)* blur3[1] + (double)qGreen(leftPixel) * blur3[0] + (double)qGreen(rightPixel) * blur3[2];
  //  QRgb rgb = qRgb(red, green, blue);
}

