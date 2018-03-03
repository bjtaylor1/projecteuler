using System;
using System.Diagnostics;
using System.Windows;
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

        int xmax = 24;
        int ymax = 24;
        protected override void OnRender(DrawingContext drawingContext)
        {
            Debug.WriteLine($"Rendering with size {ActualWidth}");
            c.Children.Clear();
            for (int x = -xmax; x <= xmax; x++)
            {
                AddLine(Colors.LightGray, 1, x, x, -ymax, ymax);
            }
            for (int y = -ymax; y <= ymax; y++)
            {
                AddLine(Colors.LightGray, 1, -xmax, xmax, y, y);
            }
            AddLine(Colors.Black, 2, 0, 0, -ymax, ymax); //x axis
            AddLine(Colors.Black, 2, -xmax, xmax, 0, 0); //y axis

        }

        private void AddLine(Color color, int stroke, int x1, int x2, int y1, int y2)
        {
            var line = new Line();
            line.Stroke = new SolidColorBrush(color);
            line.StrokeThickness = stroke;
            line.X1 = XPos(x1);
            line.X2 = XPos(x2);
            line.Y1 = YPos(y1);
            line.Y2 = YPos(y2);
            Debug.WriteLine($"Adding line from {line.X1},{line.Y1} to {line.X2},{line.Y2}");
            c.Children.Add(line);
        }

        private double XPos(int x)
        {
            double size = Math.Min(ActualHeight, ActualWidth);
            double center = ActualWidth / 2;
            double pos = center + ((x * size) / (2 * xmax));
            return pos;
        }

        private double YPos(int y)
        {
            double size = Math.Min(ActualHeight, ActualWidth);
            double center = ActualHeight / 2;
            double pos = center - ((y * size) / (2 * ymax));
            return pos;
        }
    }
}
