using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FishEyeLens.Class
{
    public class WebCam
    {
        OpenCvSharp.VideoCapture capture;
        BackgroundWorker bw = new BackgroundWorker();
        Mat source = new Mat();
        
        public int idx = 0;
        bool IsWork = true;

        MainWindow MW = null;

        static double cx, cy, fx, fy, k1, k2, k3, p1, p2, skew_c = 0;

        public WebCam(int _index, MainWindow _mw)
        {
            this.idx = _index;
            MW = _mw;

            capture = new VideoCapture(idx);

            if (!capture.IsOpened())
            {
                System.Windows.MessageBox.Show("연결 실패");
                return;
            }

            fx = 214.885 * 2;
            fy = 214.909 * 2;
            cx = 318.885 * 2;
            cy = 238.485 * 2;

            k1 = -0.18894 * 1.25;
            k2 = 0.02724 * 1.25;
            k3 = 0 * 1.25;
            p1 = 0.000787 * 1.25;
            p2 = -0.003942 * 1.25;

            bw.DoWork += Bw_DoWork;

            bw.RunWorkerAsync();
        }

        static int px;
        static int py;

        private void Bw_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                Cv2.NamedWindow(idx.ToString(), WindowMode.AutoSize);
                capture.Read(source);

                Size size = new Size(source.Cols * 2, source.Rows * 2);

                Mat cvtSource = new Mat(size, MatType.CV_8UC3);
                Mat undistort = new Mat(size, MatType.CV_8UC3);

                while (IsWork)
                {
                    capture.Read(source);
                    
                    cvtImage(source, ref cvtSource);

                    for (int y = 0; y < cvtSource.Rows; y++)
                    {
                        for (int x = 0; x < cvtSource.Cols; x++)
                        {
                            px = x;
                            py = y;

                            DistortPixel();

                            if (px >= 0 && py >= 0 && px < cvtSource.Cols && py < cvtSource.Rows)
                            {
                                int pointerUndistort = (y * undistort.Cols * 3) + (x * 3);
                                int pointerCvtSource = (py * cvtSource.Cols * 3) + (px * 3);
                                unsafe
                                {
                                    undistort.DataPointer[pointerUndistort + 2] = cvtSource.DataPointer[pointerCvtSource + 2];
                                    undistort.DataPointer[pointerUndistort + 1] = cvtSource.DataPointer[pointerCvtSource + 1];
                                    undistort.DataPointer[pointerUndistort] = cvtSource.DataPointer[pointerCvtSource];
                                }
                            }
                        }
                    }

                    Cv2.ImShow(idx.ToString(), undistort);

                    int c = Cv2.WaitKey(10);

                    if (c != -1) { break; }
                }
            }
            catch (Exception ex)
            {
                string err = ex.Message + "\r\n" + ex.StackTrace;
                System.Windows.MessageBox.Show(err);
            }
        }

        private static void cvtImage(Mat source, ref Mat cvtSource)
        {
            for (int y = 0; y < source.Rows; y++)
            {
                for (int x = 0; x < source.Cols; x++)
                {
                    int pointerCvtSource = ((y + 240) * cvtSource.Cols * 3) + ((x + 320) * 3);
                    int pointerSource = (y * source.Cols * 3) + (x * 3);

                    unsafe
                    {
                        cvtSource.DataPointer[pointerCvtSource + 2] = source.DataPointer[pointerSource + 2];
                        cvtSource.DataPointer[pointerCvtSource + 1] = source.DataPointer[pointerSource + 1];
                        cvtSource.DataPointer[pointerCvtSource] = source.DataPointer[pointerSource];
                    }
                }
            }
        }

        static double xx = px;
        static double yy = py;

        private static void DistortPixel()
        {
            xx = px;
            yy = py;

            Normalize();
            Distort();
            Denormalize();

            px = (int)(xx + 0.5);
            py = (int)(yy + 0.5);
        }

        private static void Normalize()
        {
            double y_n = (yy - cy) / fy;
            double x_n = (xx - cx) / fx - skew_c * y_n;

            xx = x_n;
            yy = y_n;
        }

        private static void Distort()
        {
            double r2 = xx * xx + yy * yy;
            double radial_d = 1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2;
            double x_d = radial_d * xx + 2 * p1 * xx * yy + p2 * (r2 + 2 * xx * xx);
            double y_d = radial_d * yy + p1 * (r2 + 2 * yy * yy) + 2 * p2 * xx * yy;

            xx = x_d;
            yy = y_d;
        }

        private static void Denormalize()
        {
            double x_p = fx * (xx + skew_c * yy) + cx;
            double y_p = fy * yy + cy;

            xx = x_p;
            yy = y_p;
        }
    }
}
