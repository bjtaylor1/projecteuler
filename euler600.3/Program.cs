using System;
using System.Collections.Generic;
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
            int maxPerim = int.Parse(args[0]);
            maxSide = (maxPerim - 4) / 2;

            var sidePairs = MakeSidePairs();
            var sidePairsByVector = sidePairs.ToLookup(sp => 
            {
                var v = -(sp.GetVector(Math.PI * 4 / 3));
                return v;
            }

            );
            long hexagons = 0;
            var hexagonList = new List<Hexagon>();

            foreach(var sp1 in sidePairs)
            {
                foreach(var sp2 in sidePairs.Where(psp2 => psp2.PerimUsed <= maxPerim - sp1.PerimUsed - 2))
                {
                    var totalVector = sp1.GetVector(0) + sp2.GetVector(Math.PI * 2 / 3);
                    foreach (var sp3 in sidePairsByVector[totalVector])
                    {
                        if (sp1.PerimUsed + sp2.PerimUsed + sp3.PerimUsed <= maxPerim)
                        {
                            var hexagon = new Hexagon(sp1, sp2, sp3);
                            if (hexagon.IsFirst)
                            {
                                hexagonList.Add(hexagon);
                                hexagons++;
                            }
                        }
                    }
                }
            }

            Console.WriteLine($"There are {hexagons} hexagons.");
        }

        private static List<SidePair> MakeSidePairs()
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
                        sidePairs.Add(sidePair);
                    }
                }
            }
            return sidePairs;
        }
        
    }
}
