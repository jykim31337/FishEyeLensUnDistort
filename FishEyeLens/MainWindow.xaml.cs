using FishEyeLens.Class;
using System.Collections;
using System.Windows;

namespace FishEyeLens
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        int nWebCamCnt = 0;
        ArrayList arrWebCam = new ArrayList();

        public MainWindow()
        {
            InitializeComponent();

            InitControl();
        }

        public void InitControl()
        {
            btnStart.Click += BtnStart_Click;
        }

        private void BtnStart_Click(object sender, RoutedEventArgs e)
        {
            WebCam webCam = new WebCam(nWebCamCnt, this);

            nWebCamCnt++;

            arrWebCam.Add(webCam);
        }
    }
}
