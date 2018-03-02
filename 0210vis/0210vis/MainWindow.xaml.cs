using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
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

            base.OnRender(drawingContext);
            
            c.Children.Clear();
            var yAxis = new Line();
            Color color = Colors.Black;
            const int stroke = 2;
            AddLine(yAxis, color, stroke);
        }

        private void AddLine(Line yAxis, Color color, int stroke)
        {
            yAxis.Stroke = new SolidColorBrush(color);
            yAxis.StrokeThickness = stroke;
            yAxis.X2 = yAxis.X1 = XPos(0);
            yAxis.Y1 = YPos(-ymax);
            yAxis.Y2 = YPos(ymax);
            c.Children.Add(yAxis);
        }

        private double XPos(int x)
        {
            double size = Math.Min(ActualHeight, ActualWidth);
            double center = Width / 2;
            double pos = center + (x * size / 2);
            return pos;
        }

        private double YPos(int y)
        {
            double size = Math.Min(Height, Width);
            double center = Height / 2;
            double pos = center - (y * size / 2);
            return pos;
        }
    }
}
