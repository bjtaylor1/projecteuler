using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0213cs
{
    public class C
    {
        public const int N = 30;
    }
    class Program
    {
        static Pos[] allFleas;
        double Or(IEnumerable<double> inputs)
        {
            return 1 - inputs.Aggregate((p, d) => p * (1 - d));
        }
        ConcurrentDictionary<Key, double> pcache = new ConcurrentDictionary<Key, double>();
        double P(Key k) => pcache.GetOrAdd(k, CalcP);
        double CalcP(Key k)
        {
            if(k.I == 0)
            {
                return Equals(k.Flea, k.Square) ? 1 : 0;
            }
            var pAdjPreviously = 
        }
        

        static void Main(string[] args)
        {
            allFleas = Enumerable.Range(0, C.N).SelectMany(y => Enumerable.Range(0, C.N).Select(x => new Pos(x, y))).ToArray();
        }
    }

    public class Key
    {
        public Pos Square { get; } //the current square
        public Pos Flea { get; } //the flea from this original position
        public int I { get; } //the iteration
    }

    public struct Pos
    {
        public int X { get; }
        public int Y { get; }
        public int Index { get; }

        public Pos(int x, int y)
        {
            X = x;
            Y = y;
            Index = y * C.N + x;
        }

        public override string ToString()
        {
            return $"{X}, {Y}";
        }

        public override bool Equals(object obj)
        {
            return obj is Pos pos &&
                   Index == pos.Index;
        }

        public override int GetHashCode()
        {
            return -2134847229 + Index.GetHashCode();
        }
    }
}
