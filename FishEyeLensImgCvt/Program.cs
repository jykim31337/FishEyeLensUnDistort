#define FULLSIZE

using OpenCvSharp;

namespace FishEyeLensImgCvt
{
    class Program
    {
        static double cx, cy, fx, fy, k1, k2, k3, p1, p2, skew_c = 0;
        static void Main(string[] args)
        {
#if FULLSIZE
            fx = 214.885 * 2;
            fy = 214.909 * 2;
            cx = 318.885 * 2;
            cy = 238.485 * 2;

            k1 = -0.18894 * 1.25;
            k2 = 0.02724 * 1.25;
            k3 = 0 * 1.5;
            p1 = 0.000787 * 1.25;
            p2 = -0.003942 * 1.25;
#else
            fx = 214.885;
            fy = 214.909;
            cx = 318.885;
            cy = 238.485;

            k1 = -0.18894;
            k2 = 0.02724;
            k3 = 0;
            p1 = 0.000787;
            p2 = -0.003942;
#endif

            string image = @"D:\CBACKUP\images\image6.png";

            Mat source = new Mat(image);

#if FULLSIZE
                Size size = new Size(source.Cols * 2, source.Rows * 2);
#else
            Size size = new Size(source.Cols, source.Rows);
#endif

            Mat cvtSource = new Mat(size, MatType.CV_8UC3);
            cvtImage(source, ref cvtSource);

            Mat undistort = new Mat(size, MatType.CV_8UC3);

            var colorIndexerCvtSource = cvtSource.GetGenericIndexer<Vec3b>();
            var colorIndexerUndistort = undistort.GetGenericIndexer<Vec3b>();

            for (int y = 0; y < cvtSource.Rows; y++)
            {
                for (int x = 0; x < cvtSource.Cols; x++)
                {
                    int px = x;
                    int py = y;

                    DistortPixel(ref px, ref py);

                    if (px >= 0 && py >= 0 && px < cvtSource.Cols && py < cvtSource.Rows)
                    {
                        colorIndexerUndistort[y, x] = colorIndexerCvtSource[py, px];
                    }
                    else
                    {
                        colorIndexerUndistort[y, x] = new Scalar(0, 0, 0).ToVec3b();
                    }
                }
            }

            Cv2.NamedWindow("result", WindowMode.AutoSize);

            Cv2.ImShow("result", undistort);

            int c = Cv2.WaitKey();
        }

        private static void cvtImage(Mat source, ref Mat cvtSource)
        {
#if FULLSIZE
            var colorIndexerSource = source.GetGenericIndexer<Vec3b>();
            var colorIndexerCvtSource = cvtSource.GetGenericIndexer<Vec3b>();

            for (int y = 0; y < source.Rows; y++)
            {
                for(int x = 0; x < source.Cols; x++)
                {
                    colorIndexerCvtSource[y + source.Rows / 2, x + source.Cols / 2] = colorIndexerSource[y, x];
                }
            }
#else
            cvtSource = source.Clone();
#endif
        }

        private static void DistortPixel(ref int px, ref int py)
        {
            double x = px;
            double y = py;

            Normalize(ref x, ref y);
            Distort(ref x, ref y);
            Denormalize(ref x, ref y);

            px = (int)(x + 0.5);
            py = (int)(y + 0.5);
        }

        private static void Normalize(ref double x, ref double y)
        {
            double y_n = (y - cy) / fy;
            double x_n = (x - cx) / fx - skew_c * y_n;

            x = x_n;
            y = y_n;
        }

        private static void Distort(ref double x, ref double y)
        {
            double r2 = x * x + y * y;
            double radial_d = 1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2;
            double x_d = radial_d * x + 2 * p1 * x * y + p2 * (r2 + 2 * x * x);
            double y_d = radial_d * y + p1 * (r2 + 2 * y * y) + 2 * p2 * x * y;

            x = x_d;
            y = y_d;
        }

        private static void Denormalize(ref double x, ref double y)
        {
            double x_p = fx * (x + skew_c * y) + cx;
            double y_p = fy * y + cy;

            x = x_p;
            y = y_p;
        }
    }
}
