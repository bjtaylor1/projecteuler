using System.Windows.Media.Media3D;

namespace euler579
{
    class PlanePair
    {
        public Plane Plane1 { get; }
        public Plane Plane2 { get; }

        public PlanePair(Plane plane1, Plane plane2)
        {
            Plane1 = plane1;
            Plane2 = plane2;
        }

        public bool IsBetweenOrOn(VectorInt point)
        {
            var side1 = Plane1.GetSide(point);
            var side2 = Plane2.GetSide(point);
            var isBetweenOrOn = side1 == 0 || side2 == 0 || side1 != side2;
            return isBetweenOrOn;
        }
    }
}