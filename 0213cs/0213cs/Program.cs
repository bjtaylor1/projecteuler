using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0213cs
{
    class Program
    {
        ConcurrentDictionary<((int X, int Y) s, int f, int i), double> pcache = new ConcurrentDictionary<((int X, int Y) s, int f, int i), double>();
        double P((int X, int Y) s, int f, int i) => pcache.GetOrAdd((s, f, i), P);
        double P(((int X, int Y) s, int f, int i) p)
        {

        }
        

        static void Main(string[] args)
        {
        }
    }
}
