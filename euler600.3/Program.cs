using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler600
{
    class Program
    {
        private static int maxSide;

        static void Main(string[] args)
        {
            double xp = Math.Cos(Math.PI / 3);
            double yp = Math.Sin(Math.PI / 3);
            int N = int.Parse(args[0]);
            int sMax = (N - 4) / 2;
            int totalHexagons = 0;
            for (int s0 = 1; s0 <= sMax; s0++)
            {
                for(int s1 = s0; s1 <= sMax - s0; s1++)
                {
                    for(int s2 = 1; s2 <= (N/2) - s0 - s1; s2++)
                    {
                        for(int s3 = 1; s3 <= sMax && s3 < N - s0 - s1 - s2; s3++)
                        {
                            for(int s4 = 1; s4 <= sMax && s4 < N - s0 - s1 - s2 - s3; s4++)
                            {
                                double x = s0 + (s1 * xp) - (s2 * xp) - s3 - (s4 * xp);
                                double y = (s1 * yp) + (s2 * yp) - (s4 * yp);
                                if(Math.Abs(x/y + xp /yp) < 1e-10)
                                {
                                    double s5d = Math.Sqrt(x * x + y * y);
                                    int s5 = (int)Math.Round(s5d);
                                    if(Math.Abs(s5 - s5d) < 1e-10 && s0 + s1 + s2 + s3 + s4 + s5 <= N)
                                    {
                                        var hexagon = new Hexagon(s0, s1, s2, s3, s4, s5);
                                        if(hexagon.IsFirst())
                                        {
                                            FoundHexagon(hexagon, ref totalHexagons);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Console.WriteLine($"There are {totalHexagons} hexagons.");
        }

        static void FoundHexagon(Hexagon hexagon, ref int totalHexagons)
        {
            if(hexagon.IsValid() && hexagon.IsFirst())
            {
                totalHexagons++;
                Debug.WriteLine($"Found {hexagon}");
            }

        }

        static void EnumerateHexagons(Hexagon initialHexagon, int dir, int maxPerim, ref int totalHexagons)
        {
            for(Hexagon hexagon = initialHexagon.Grow(dir,1); hexagon.TotalPerim() <= maxPerim; hexagon = hexagon.Grow(dir, 1))
            {
                FoundHexagon(hexagon, ref totalHexagons);
                for(int newDir = 0; newDir < 3; newDir++)
                {
                    EnumerateHexagons(hexagon, newDir, maxPerim, ref totalHexagons);
                }
            }
        }

        private static IEnumerable<SidePair> EnumerateSidePairs()
        {
            var sidePairs = new List<SidePair>();
            double maxXOrY = maxSide + Math.Sin(Math.PI / 3);
            for (int s0 = 1; s0 <= maxSide; s0++)
            {
                for (int s1 = 1; s1 <= maxSide; s1++)
                {
                    SidePair sidePair = new SidePair(s0, s1);
                    Vector firstVector = sidePair.GetVector(0);
                    if (firstVector.X <= maxXOrY && firstVector.Y <= maxXOrY)
                    {
                        yield return sidePair;
                    }
                }
            }
        }
        
    }
}
