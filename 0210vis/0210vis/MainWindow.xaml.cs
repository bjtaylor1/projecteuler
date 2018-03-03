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

        int xmax = 4;
        int ymax = 4;
        protected override void OnRender(DrawingContext drawingContext)
        {
            c.Children.Clear();
            for (int x = -xmax; x <= xmax; x++)
            {
                AddLine(Brushes.LightGray, 1, x, -ymax, x, ymax);
            }
            for (int y = -ymax; y <= ymax; y++)
            {
                AddLine(Brushes.LightGray, 1, -xmax, y, xmax, y);
            }
            AddLine(Brushes.Black, 2, 0, -ymax, 0, ymax); //x axis
            AddLine(Brushes.Black, 2, -xmax, 0, xmax, 0); //y axis


            AddLine(Brushes.LightBlue, 1, -xmax, 0, 0, ymax);
            AddLine(Brushes.LightBlue, 1, 0, ymax, xmax, 0);
            AddLine(Brushes.LightBlue, 1, xmax, 0, 0, -ymax);
            AddLine(Brushes.LightBlue, 1, 0, -ymax, -xmax, 0);

            AddPoint(Brushes.Red, 0, 0);
            AddPoint(Brushes.Red, xmax / 4, ymax / 4);
        }

        private void AddPoint(Brush fill, int x, int y)
        {
            double size = Math.Min(ActualHeight, ActualWidth);
            double lineX = size / (2 * xmax);
            double lineY = size / (2 * ymax);
            double shapeSize = Math.Min(lineX, lineY)/2;
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
            double size = Math.Min(c.ActualHeight, c.ActualWidth);
            double center = c.ActualWidth / 2;
            double pos = center + ((x * size) / (2 * xmax));
            return pos;
        }

        private double YPos(int y)
        {
            double size = Math.Min(c.ActualHeight, c.ActualWidth);
            double center = c.ActualHeight / 2;
            double pos = center - ((y * size) / (2 * ymax));
            return pos;
        }
    }
}
