using System;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace _0210vis
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        class SaveCanvasCommand : ICommand
        {
            private readonly MainWindow mainWindow;

            public SaveCanvasCommand(MainWindow mainWindow)
            {
                this.mainWindow = mainWindow;
            }

            public event EventHandler CanExecuteChanged;

            public bool CanExecute(object parameter)
            {
                return true;
            }

            public void Execute(object parameter)
            {
                Rect rect = new Rect(mainWindow.c.RenderSize);
                RenderTargetBitmap rtb = new RenderTargetBitmap((int)rect.Right,
                  (int)rect.Bottom, 96d, 96d, System.Windows.Media.PixelFormats.Default);
                rtb.Render(mainWindow.c);
                //endcode as PNG
                BitmapEncoder encoder = new JpegBitmapEncoder();
                encoder.Frames.Add(BitmapFrame.Create(rtb));

                //save to file stream
                using (var fileStream = File.OpenWrite("output.jpg"))
                {
                    encoder.Save(fileStream);
                }
            }
        }

        public MainWindow()
        {
            SaveCommand = new SaveCanvasCommand(this);
            DataContext = this;
            InitializeComponent();
        }

        public int Max
        {
            get { return (int)GetValue(maxProperty); }
            set { SetValue(maxProperty, value); }
        }

        public static readonly DependencyProperty maxProperty =
            DependencyProperty.Register("Max", typeof(int), typeof(MainWindow), new PropertyMetadata(4));




        public string Message
        {
            get { return (string)GetValue(MessageProperty); }
            set { SetValue(MessageProperty, value); }
        }

        public ICommand SaveCommand { get; }

        public static readonly DependencyProperty MessageProperty =
            DependencyProperty.Register("Message", typeof(string), typeof(MainWindow), new PropertyMetadata(string.Empty));



        protected override void OnPropertyChanged(DependencyPropertyChangedEventArgs e)
        {
            base.OnPropertyChanged(e);
            if (e.Property == maxProperty)
            {
                this.InvalidateVisual();
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            c.Children.Clear();
            //for (int x = -Max; x <= Max; x++)
            //{
            //    AddLine(Brushes.LightGray, 1, x, -Max, x, Max);
            //}
            //for (int y = -Max; y <= Max; y++)
            //{
            //    AddLine(Brushes.LightGray, 1, -Max, y, Max, y);
            //}
            AddLine(Brushes.Black, 2, 0, -Max, 0, Max); //x axis
            AddLine(Brushes.Black, 2, -Max, 0, Max, 0); //y axis

            AddLine(Brushes.LightBlue, 1, -Max, 0, 0, Max);
            AddLine(Brushes.LightBlue, 1, 0, Max, Max, 0);
            AddLine(Brushes.LightBlue, 1, Max, 0, 0, -Max);
            AddLine(Brushes.LightBlue, 1, 0, -Max, -Max, 0);

            AddPoint(Brushes.Red, 0, 0, 0.5);
            AddPoint(Brushes.Red, Max / 4, Max / 4, 0.5);
            int numPoints = 0;
            for (int x = -Max; x <= Max; x++)
            {
                for (int y = x + 1; y <= Max; y++)
                {
                    if (Math.Abs(x) + Math.Abs(y) <= Max)
                    {
                        if ((GetAngle(x, y, 0, 0, Max / 4, Max / 4) > Math.PI / 2)
                            || (GetAngle(0, 0, x, y, Max / 4, Max / 4) > Math.PI / 2)
                            || (GetAngle(Max / 4, Max / 4, 0, 0, x, y) > Math.PI / 2))
                        {
                            numPoints++;
                            AddPoint(Brushes.Orange, x, y, 0.25);
                        }
                    }
                }
            }
            Message = $"{numPoints} x 2 = {numPoints * 2}";
        }

        double GetAngle(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y)
        {
            double p12sq = Math.Pow(p1x - p2x, 2) + Math.Pow(p1y - p2y, 2);
            double p13sq = Math.Pow(p1x - p3x, 2) + Math.Pow(p1y - p3y, 2);
            double p23sq = Math.Pow(p2x - p3x, 2) + Math.Pow(p2y - p3y, 2);

            double p12 = Math.Sqrt(p12sq);
            double p13 = Math.Sqrt(p13sq);

            double angle = Math.Acos((p12sq + p13sq - p23sq) / (2 * p12 * p13));
            return angle;
        }

        private void AddPoint(Brush fill, int x, int y, double boldnessComparedToGridWidth)
        {
            double size = Math.Min(ActualHeight, ActualWidth);
            double line = size / (2 * Max);
            double shapeSize = line * boldnessComparedToGridWidth;
            var ellipse = new Ellipse();
            ellipse.Height = shapeSize;
            ellipse.Width = shapeSize;
            ellipse.Fill = fill;
            Canvas.SetLeft(ellipse, XPos(x) - shapeSize / 2);
            Canvas.SetTop(ellipse, YPos(y) - shapeSize / 2);
            c.Children.Add(ellipse);
        }

        private void AddLine(Brush stroke, int strokeThickness, int x1, int y1, int x2, int y2)
        {
            var line = new Line();
            line.Stroke = stroke;
            line.StrokeThickness = strokeThickness;
            line.X1 = XPos(x1);
            line.X2 = XPos(x2);
            line.Y1 = YPos(y1);
            line.Y2 = YPos(y2);
            c.Children.Add(line);
        }

        private double XPos(double x)
        {
            double size = Math.Min(c.ActualHeight, c.ActualWidth) - 50;
            double center = c.ActualWidth / 2;
            double pos = center + ((x * size) / (2 * Max));
            return pos;
        }

        private double YPos(double y)
        {
            double size = Math.Min(c.ActualHeight, c.ActualWidth) - 50;
            double center = c.ActualHeight / 2;
            double pos = center - ((y * size) / (2 * Max));
            return pos;
        }
    }
}
