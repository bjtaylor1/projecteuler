using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler600
{
    public struct Hexagon
    {
        public Hexagon Grow(int dir, int amount)
        {
            if (dir == 0)
                return new Hexagon(S0 , S1, S2+ amount, S3, S4, S5 + amount);
            else if (dir == 1)
                return new Hexagon(S0, S1 + amount, S2, S3, S4 + amount, S5);
            else if (dir == 2)
                return new Hexagon(S0 + amount, S1, S2, S3 + amount, S4, S5);
            else
                throw new ArgumentException("Invalid dir", nameof(dir));
        }

        public int S0, S1, S2, S3, S4, S5;

        public Hexagon(int s0, int s1, int s2, int s3, int s4, int s5) : this(new SidePair(s0, s1), new SidePair(s2, s3), new SidePair(s4, s5)) { }

        public Hexagon(SidePair sp0, SidePair sp1, SidePair sp2)
        {
            S0 = sp0.S0;
            S1 = sp0.S1;
            S2 = sp1.S0;
            S3 = sp1.S1;
            S4 = sp2.S0;
            S5 = sp2.S1;
        }

        public bool IsValid()
        {
            return S0 > 0 && S1 > 0 && S2 > 0 && S3 > 0 && S4 > 0 && S5 > 0;
        }

        public bool IsFirst()
        {
            var hexWeights = new List<HexWeight>
            {
                new HexWeight(S0, S1, S2, S3, S4, S5),
                new HexWeight(S1, S2, S3, S4, S5, S0),
                new HexWeight(S2, S3, S4, S5, S0, S1),
                new HexWeight(S3, S4, S5, S0, S1, S2),
                new HexWeight(S4, S5, S0, S1, S2, S3),
                new HexWeight(S5, S0, S1, S2, S3, S4),
                new HexWeight(S5, S4, S3, S2, S1, S0),
                new HexWeight(S4, S3, S2, S1, S0, S5),
                new HexWeight(S3, S2, S1, S0, S5, S4),
                new HexWeight(S2, S1, S0, S5, S4, S3),
                new HexWeight(S1, S0, S5, S4, S3, S2),
                new HexWeight(S0, S5, S4, S3, S2, S1)
            };
            hexWeights.Sort();
            var hexWeight = hexWeights.First();
            return
                   S0 == hexWeight.S0 &&
                   S1 == hexWeight.S1 &&
                   S2 == hexWeight.S2 &&
                   S3 == hexWeight.S3 &&
                   S4 == hexWeight.S4 &&
                   S5 == hexWeight.S5;
        }

        public override string ToString()
        {
            return $"{S0}  {S1}  {S2}  {S3}  {S4}  {S5}";
        }

        public int TotalPerim()
        {
            return S0 + S1 + S2 + S3 + S4 + S5;
        }
    }

    public struct HexWeight : IComparable<HexWeight>
    {
        public int S0, S1, S2, S3, S4, S5;
        public HexWeight(int s0, int s1, int s2, int s3, int s4, int s5)
        {
            S0 = s0;
            S1 = s1;
            S2 = s2;
            S3 = s3;
            S4 = s4;
            S5 = s5;
        }

        public int CompareTo(HexWeight other)
        {
            int cs0 = S0.CompareTo(other.S0); if (cs0 != 0) return cs0;
            int cs1 = S1.CompareTo(other.S1); if (cs1 != 0) return cs1;
            int cs2 = S2.CompareTo(other.S2); if (cs2 != 0) return cs2;
            int cs3 = S3.CompareTo(other.S3); if (cs3 != 0) return cs3;
            int cs4 = S4.CompareTo(other.S4); if (cs4 != 0) return cs4;
            int cs5 = S5.CompareTo(other.S5); if (cs5 != 0) return cs5;
            return 0;

        }
    }
}
