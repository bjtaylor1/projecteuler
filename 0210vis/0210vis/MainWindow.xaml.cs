using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;

namespace _0210vis
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

        }

        int max = 4;
        protected override void OnRender(DrawingContext drawingContext)
        {
            c.Children.Clear();
            for (int x = -max; x <= max; x++)
            {
                AddLine(Brushes.LightGray, 1, x, -max, x, max);
            }
            for (int y = -max; y <= max; y++)
            {
                AddLine(Brushes.LightGray, 1, -max, y, max, y);
            }
            AddLine(Brushes.Black, 2, 0, -max, 0, max); //x axis
            AddLine(Brushes.Black, 2, -max, 0, max, 0); //y axis


            AddLine(Brushes.LightBlue, 1, -max, 0, 0, max);
            AddLine(Brushes.LightBlue, 1, 0, max, max, 0);
            AddLine(Brushes.LightBlue, 1, max, 0, 0, -max);
            AddLine(Brushes.LightBlue, 1, 0, -max, -max, 0);

            AddPoint(Brushes.Red, 0, 0, 0.5);
            AddPoint(Brushes.Red, max / 4, max / 4, 0.5);

            for(int x = -max; x <= max; x++)
            {
                for(int y = x+1; y <= max; y++)
                {
                    if (Math.Abs(x) + Math.Abs(y) <= max)
                    {
                        if ((GetAngle(x, y, 0, 0, max / 4, max / 4) > Math.PI / 2)
                            || (GetAngle(0, 0, x, y, max / 4, max / 4) > Math.PI / 2)
                            || (GetAngle(max / 4, max / 4, 0, 0, x, y) > Math.PI / 2))
                        {
                            AddPoint(Brushes.Orange, x, y, 0.25);
                        }
                    }
                }
            }
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
            double lineX = size / (2 * max);
            double lineY = size / (2 * max);
            double shapeSize = Math.Min(lineX, lineY)*boldnessComparedToGridWidth;
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

        private double XPos(int x)
        {
            double size = Math.Min(c.ActualHeight, c.ActualWidth) - 50;
            double center = c.ActualWidth / 2;
            double pos = center + ((x * size) / (2 * max));
            return pos;
        }

        private double YPos(int y)
        {
            double size = Math.Min(c.ActualHeight, c.ActualWidth) - 50;
            double center = c.ActualHeight / 2;
            double pos = center - ((y * size) / (2 * max));
            return pos;
        }
    }
}
