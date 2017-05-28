using System;
using System.Windows;

namespace euler600
{
    public struct SidePair
    {
        public int S0, S1;
        public int PerimUsed;
        public SidePair(int s0, int s1)
        {
            S0 = s0;
            S1 = s1;
            PerimUsed = s0 + s1;
            double x = s0 + s1 * Math.Cos(Math.PI / 3);
            double y = s1 * Math.Sin(Math.PI / 3);
        }

        public Vector GetVector(double angle)
        {
            var x = S0 * Math.Cos(angle) + S1 * Math.Cos(angle + Math.PI /3);
            var y = S0 * Math.Sin(angle) + S1 * Math.Sin(angle + Math.PI / 3);
            var v = new Vector(x, y);
            return v;
        }

        public override string ToString()
        {
            return $"{S0}, {S1}";
        }
    }
}
