using System.Linq;
using System.Windows;
using System.Windows.Media.Media3D;

namespace euler579_findtriples
{
    public class Triple
    {
        public Triple(int[] sides, int square)
        {
            Sides = sides;
            Square = square;
            Dimensions = Sides.Count(s => s > 0);
            Vector = new Vector3D(Sides[0], Sides[1], Sides[2]);
        }

        public Vector3D Vector { get;  }

        public int[] Sides { get; }
        public int Square { get; }
        public int Dimensions { get; }

        public override string ToString()
        {
            return $"{string.Join(",", Sides.Select(s => s.ToString()))} => {Square} ({Dimensions}D)";
        }
    }
}